#ifndef _HYPERLOGLOG_H
#define _HYPERLOGLOG_H

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstdint>
#include <memory>

#include "constants.h"

template <typename T, int precision = 14>
struct HyperLogLog {
    private:
        static_assert(precision >= 4 && precision <= 20);
        // pass-by-value if `T` is trivially copyable else pass-by-reference
        using Type = std::conditional_t<std::is_trivially_copyable_v<T>, T, const T &>; 
        static constexpr std::hash<T> hasher{};

#ifdef STACK_ALLOCATE
        alignas(64) uint8_t counts[1 << precision] = {0};
#else
        uint8_t *counts;
#endif

        // systematic bias correction
        constexpr double alpha() const {
            switch (precision) {
                case 4:
                    return 0.673;
                case 5:
                    return 0.697;
                case 6:
                    return 0.709;
                default:
                    return 0.7213 / (1 + 1.079 / (1UL << precision));
            }
        }

        double bias(double biased_estimate) const {
            const auto raw_array = RAW_ARRAYS[precision - 4];
            const auto bias_array = BIAS_ARRAYS[precision - 4];
            auto first_index =
                std::upper_bound(raw_array.begin(), raw_array.end(), biased_estimate);

            if (first_index == raw_array.end()) {
                return 0;
            } else if (first_index == raw_array.begin()) {
                return bias_array[0];
            } else {
                int index = first_index - raw_array.begin();
                auto x1 = raw_array[index - 1];
                auto x2 = raw_array[index];
                auto y1 = bias_array[index - 1];
                auto y2 = bias_array[index];
                auto delta = (biased_estimate - x1) / (x2 - x1);
                return y1 + delta * (y2 - y1);
            }
        }

    public:
#ifndef STACK_ALLOCATE
        HyperLogLog() {
            const auto hll_size = (1 << precision);
            counts = new uint8_t[hll_size];
            std::fill(counts, counts + hll_size, 0x00);
        }

        ~HyperLogLog() {
            delete[] counts;
        }
#endif

        bool empty() const {
            const auto hll_size = (1 << precision);
            for (int i = 0; i < hll_size; i++) {
                if (counts[i] != 0x00) {
                    return false;
                }
            }
            return true;
        }

        size_t size() const {
            // only approx
            double harmonic_mean = 0;
            constexpr auto hll_size = (1 << precision);
            for (int i = 0; i < hll_size; i++) {
                harmonic_mean += std::pow(2, -counts[i]);
            }
            harmonic_mean = hll_size / harmonic_mean;
            double biased_estimate = alpha() * hll_size * harmonic_mean;
            return std::round(biased_estimate - bias(biased_estimate));
        }

        void operator+=(const HyperLogLog<T, precision> &hll) {
            for (int i = 0; i < (1 << precision); i++) {
                counts[i] = std::max(counts[i], hll.counts[i]);
            }
        }

        void insert(Type x) {
            auto h = hasher(x);
            uint8_t trailing_zeros = std::countr_zero(h);
            auto bin = h >> (64 - precision);
            if (counts[bin] < trailing_zeros + 1) {
                counts[bin] = trailing_zeros + 1;
            }
            if (trailing_zeros > 64 - precision) [[unlikely]] {
                counts[bin] = 65 - precision;
            }
        }
};

#endif
