/*
Copyright (c) 2015, Vlad Mesco
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// This file is used as a proving grounds for Delay::Resample from
// over at github.com/alzwded/JakMuseV3/jakmuse

// compile with:
// g++ -fopenmp -O3 -msse -msse2 -msse3 -mssse3 -ffast-math --std=gnu++11 ds.cpp -lgomp
// ...of which
// -msse* do nothing (the compiler cannot parallelize this code, so no difference in performance; not its fault)
// -O3 vs -O0 results in a 3x speed boost
// -fopenmp results in an insignificant speed boost every now and then

#define _USE_MATH_CONSTANTS
#include <cstdio>
#include <tuple>
#include <vector>
#include <deque>
#include <cmath>
#include <algorithm>
#if 0
# define PRINT_RESAMPLE(F, ...)  printf(F, __VA_ARGS__)
#else
# define PRINT_RESAMPLE(F, ...)
#endif

#include <omp.h>

//#define PERFTEST
#ifdef PERFTEST
int iteration = 0;

void h(int)
{
    printf("was at: %d\n", iteration);
    exit(0);
}

void omptest()
{
    #pragma omp parallel
    {
        fprintf(stderr, "%d\n", omp_get_thread_num());
    }
}
# undef printf
# define printf nop
int nop(const char* s, ...) __attribute__((format(printf, 1, 2)));
int nop(const char* s, ...) {}

#endif


enum class ResetKind
{
    REST,
    NOTE
};

enum class InterpolationMethod {
    COSINE,
    CUBIC
};

std::deque<std::tuple<double, bool, ResetKind>> buffer_;

void Resample(int64_t newSize, InterpolationMethod method)
{
    auto oldSize = buffer_.size();
    std::vector<decltype(buffer_)::value_type> newBuffer(newSize, std::make_tuple(0.0, false, ResetKind::REST));
    std::vector<double> weights(newSize, 0.0);
    std::vector<decltype(buffer_)::value_type> buffer(buffer_.begin(), buffer_.end());

    if(newSize < oldSize) {
        int64_t i1 = 0;
        double w = (double)oldSize / (double)newSize;
        int64_t i2 = (int64_t)ceil(w);
        auto log_i2 = log(i2 + 1.0);

        #pragma omp parallel for
        for(int64_t j = 0; j < newSize; ++j) {
            PRINT_RESAMPLE("j=%d\n", j);
            double i0 = ((double)j / (double)(newSize-1) * (double)(oldSize-1)); // map this
            int64_t i1 = (int64_t)round(i0);
            double thing = 0.0;
            double weight = 0.0;
            bool reset = false;
            ResetKind rk = ResetKind::REST;
            for(int64_t k = i1 - i2; k <= i1 + i2; ++k) {
                if(k < 0 || k >= buffer_.size()) continue;

                double x = i2 - fabs(i0 - k) + 1.0;
                double we = log(x) / log_i2;
                if(we < 0.8) {
                    we = (0.2 + we) * (0.2 + we) - 0.2;
                } else {
                    we = 0.95 * we + 0.05;
                }

                PRINT_RESAMPLE("    k=%d we=%lf w=%lf\n", k, we, w);
                PRINT_RESAMPLE("      > x=%lf i2=%lf i0=%lf\n", x, (double)i2, i0);

                thing += we * std::get<0>(buffer[k]);
                weight += we;
                reset |= std::get<1>(buffer[k]);
                rk = std::get<2>(buffer[k]) == ResetKind::NOTE ? ResetKind::NOTE : rk;
                //printf("      %lf*%lf\n", thing, weight);
            }
            newBuffer[j] = std::make_tuple(thing, reset, rk);
            weights[j] = weight;
        }

        for(auto&& it = newBuffer.begin(); it != newBuffer.end(); ++it) {
            if(weights[it - newBuffer.begin()] > 0.0)
                std::get<0>(*it) /= weights[it - newBuffer.begin()];
        }

        buffer_.assign(newBuffer.begin(), newBuffer.end());
    } else if(newSize > oldSize) {
#if 0
        // extend limits with a couple of linear samples
        int64_t numExtend = std::max(ceil(oldSize / 3.0), sqrt(oldSize));
        double preD = std::get<0>(buffer[0]);;
        double preD2 = std::get<0>(buffer.back());
        std::deque<decltype(buffer)::value_type> pre;
        std::deque<decltype(buffer)::value_type> post;
        for(int64_t i = 0; i < numExtend; ++i) {
            double d = std::get<0>(buffer[i + 1]) - std::get<0>(buffer[i]);
            preD -= d;
            pre.push_front(std::make_tuple(
                    preD,
                    false,
                    ResetKind::REST));
            d = std::get<0>(buffer[buffer.size() - 1 - i]) - std::get<0>(buffer[buffer.size() - 1 - i - 1]);
            preD2 += d;
            post.push_back(std::make_tuple(
                    preD2,
                    false,
                    ResetKind::REST));
        }
        for(auto&& i : pre) printf("* %lf\n", std::get<0>(i));
        decltype(buffer) b2;
        b2.insert(b2.end(), pre.begin(), pre.end());
        b2.insert(b2.end(), buffer.begin(), buffer.end());
        b2.insert(b2.end(), post.begin(), post.end());
        double w = (double)oldSize / (double)newSize;
        // polynomial interpolation
        for(int64_t idx = 0; idx < newSize; ++idx) {
            double x = (double)idx * w;
            double x0 = std::get<0>(buffer_.front());

            double y1 = 0.0;
            for(int64_t i = 0; i < oldSize + 2 * numExtend; ++i) {
                double y2 = 1.0;
                for(int64_t j = 0; j < oldSize + numExtend; ++j) {
                    if(j == i) continue;
                    y2 *= (x - (double)j) / ((double)i - (double)j);
                }
                y1 += std::get<0>(buffer[i]) * y2;
            }

            std::get<0>(newBuffer[idx]) = y1;
        }

        int64_t last = 0;
        for(int64_t i = 0; i < oldSize; ++i) {
            int64_t next = round(i/w);
            for(; last <= next && last < newSize; ++last) {
                std::get<1>(newBuffer[last]) = std::get<1>(buffer_[numExtend + i]);
                std::get<2>(newBuffer[last]) = std::get<2>(buffer_[numExtend + i]);
            }
        }
#else
        double w = (double)(oldSize-1) / (double)(newSize-1);
        // cubic (feat. cosine) interpolation
        //for(int64_t idx = 0; idx < newSize; ++idx) {
        #pragma omp parallel for
        for(int idx = 0; idx < newSize; ++idx) {
            double x = (double)idx * w;

            double i1 = floor(x), i2 = ceil(x);
            if(fabs(i1 - i2) < 1.0e-15) {
                std::get<0>(newBuffer[idx]) = std::get<0>(buffer[(int)x]);
                //PRINT_RESAMPLE("overlap\n");
                continue;
            }

            if((method == InterpolationMethod::COSINE) || i1 <= 1 || i2 >= buffer.size() - 2) {
                double u = (x - i1) * M_PI;
                double t = (1.0 - cos(u)) / 2.0;
                double y0 = std::get<0>(buffer[(int)i1]);
                double y1 = std::get<0>(buffer[(int)i2]);
                PRINT_RESAMPLE("idx=%d x=%lf u=%lf t=%lf y0=%lf y1=%lf\n",
                        idx, x, u, t, y0, y1);
                std::get<0>(newBuffer[idx]) = (1.0 - t) * y0 + t * y1;
            } else {
                double a[4];
                double y[4] = {
                    std::get<0>(buffer[(int)i1 - 1]),
                    std::get<0>(buffer[(int)i1 - 0]),
                    std::get<0>(buffer[(int)i1 + 1]),
                    std::get<0>(buffer[(int)i1 + 2]),
                };
                double u = (x - i1);
                double uu = u * u;
                a[0] = y[3] - y[2] - y[0] + y[1];
                a[1] = y[0] - y[1] - a[0];
                a[2] = y[2] - y[0];
                a[3] = y[1];
                double yy = a[0] * u * uu
                        + a[1] * uu
                        + a[2] * u
                        + a[3]
                        ;
                PRINT_RESAMPLE("cubic: idx=%d x=%lf u=%lf yy=%lf\n",
                        idx, x, u, yy);
                std::get<0>(newBuffer[idx]) = yy;
            }
        }

        int64_t last = 0;
        for(int64_t i = 0; i < oldSize; ++i) {
            int64_t next = round(i/w);
            for(; last <= next && last < newSize; ++last) {
                std::get<1>(newBuffer[last]) = std::get<1>(buffer_[i]);
                std::get<2>(newBuffer[last]) = std::get<2>(buffer_[i]);
            }
        }
#endif

        buffer_.assign(newBuffer.begin(), newBuffer.end());
    } else {
        //NOP
        return;
    }
}

template<typename T>
void assign(T first, T last, int newSize)
{
    printf("%s\n", std::string(62, '=').c_str());
    printf("%d -> %d\n", last - first, newSize);
    printf("%s\n", std::string(62, '-').c_str());
    decltype(buffer_) newStuff;
    for(auto i = first; i != last; ++i) {
        newStuff.push_back(std::make_tuple(*i, false, ResetKind::NOTE));
    }
    buffer_.assign(newStuff.begin(), newStuff.end());
    for(auto&& e : buffer_) {
        printf("%f\n", std::get<0>(e));
    }
    printf("\n");
    if(newSize <= last - first) {
        Resample(newSize, InterpolationMethod::COSINE);
        for(auto&& e : buffer_) {
            printf("%f\n", std::get<0>(e));
        }
        printf("\n");
    } else {
        printf("cosine\n");
        auto bbuf = buffer_;
        Resample(newSize, InterpolationMethod::COSINE);
        for(auto&& e : buffer_) {
            printf("%f\n", std::get<0>(e));
        }
        printf("\n");
        printf("cubic\n");
        buffer_ = bbuf;
        Resample(newSize, InterpolationMethod::CUBIC);
        for(auto&& e : buffer_) {
            printf("%f\n", std::get<0>(e));
        }
    }
}

template<typename T>
void assign(std::initializer_list<T> stuff, int newSize)
{
    assign(stuff.begin(), stuff.end(), newSize);
}

template<typename T>
void assign(T const& stuff, int newSize)
{
    assign(stuff.begin(), stuff.end(), newSize);
}

int main()
{
    assign<double>({0.0, 0.2, 0.4, 0.6, 0.8, 1.0}, 3);
    assign<double>({0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35,
                    0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75,
                    0.8, 0.85, 0.9, 0.95, 1.0},
                    12);
    assign<double>({0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35,
                    0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75,
                    0.8, 0.85, 0.9, 0.95, 1.0},
                    8);
    assign<double>({0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35,
                    0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75,
                    0.8, 0.85, 0.9, 0.95, 1.0},
                    16);

std::initializer_list<double> sin21 = {
0.0,
0.309011695,
0.587776236,
0.809007168,
0.951049628,
1,
0.951066848,
0.809039922,
0.587821318,
0.309064692,
5.57254E-05,
-0.308958696,
-0.587731152,
-0.808974411,
-0.951032405,
-0.999999997,
-0.951084065,
-0.809072673,
-0.587866398,
-0.309117689,
-0.000111451};
    assign<double>(sin21, 16);
    assign<double>(sin21, 10);
    assign<double>(sin21, 8);

    printf("%s\n", std::string(62, '#').c_str());
    auto split = std::string((62 - 11) / 2, '#');
    printf("%s UPSCALING %s\n", split.c_str(), split.c_str());
    printf("%s\n", std::string(62, '#').c_str());

    assign<double>(sin21, 22);
    assign<double>(sin21, 32);
    assign<double>(sin21, 42);
    assign<double>(sin21, 84);

    printf("%s\n", std::string(62, '#').c_str());
    printf("%s\n", std::string(62, '#').c_str());

    assign<double>(sin21, 18);
    assign<double>(sin21, 24);

    printf("%s\n", std::string(62, '#').c_str());
    printf("%s\n", std::string(62, '#').c_str());

    assign<double>(sin21, 18);
    std::deque<double> mySecondaryBuffer;
    std::transform(buffer_.begin(), buffer_.end(),
        std::back_inserter(mySecondaryBuffer),
        [](decltype(buffer_)::const_reference e) -> double {
            return std::get<0>(e);
        });
    assign(mySecondaryBuffer, 24);

#ifdef PERFTEST
    /* WELL NOW
       based on performance runs, I can guarantee you that
       there's some completely different algorithm for handling
       variations in the delay amount. Using resampling is slower
       than real-time when only doing it for one delay unit (!)

       we'll need to regroup and retry with a non-resampling aproach

       oh, and neither cubic nor cosine interpolations are general enough anyway
    */
    //omptest();
    signal(SIGINT, h);
    for(int i = 0; i < 44100/20; ++i) {
        iteration = i;
        assign<double>(sin21, 400);
        std::deque<double> mySecondaryBuffer;
        std::transform(buffer_.begin(), buffer_.end(),
            std::back_inserter(mySecondaryBuffer),
            [](decltype(buffer_)::const_reference e) -> double {
                return std::get<0>(e);
            });
        assign(mySecondaryBuffer, 300);
    }
#endif
}
