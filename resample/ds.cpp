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

#define _USE_MATH_CONSTANTS
#include <cstdio>
#include <tuple>
#include <vector>
#include <deque>
#include <cmath>
#if 0
# define PRINT_RESAMPLE(F, ...)  printf(F, __VA_ARGS__)
#else
# define PRINT_RESAMPLE(F, ...)
#endif

enum class ResetKind
{
    REST,
    NOTE
};

std::deque<std::tuple<double, bool, ResetKind>> buffer_;

void Resample(int64_t newSize)
{
    auto oldSize = buffer_.size();
    std::vector<decltype(buffer_)::value_type> newBuffer(newSize, std::make_tuple(0.0, false, ResetKind::REST));
    std::vector<double> weights(newSize, 0.0);
    std::vector<decltype(buffer_)::value_type> buffer(buffer_.begin(), buffer_.end());

    if(newSize < oldSize) {
        int64_t i1 = 0;
        double w = (double)oldSize / (double)newSize;
        int64_t i2 = (int64_t)ceil(w);

        for(int64_t j = 0; j < newSize; ++j) {
            PRINT_RESAMPLE("j=%d\n", j);
            //int64_t i1 = (int64_t)round(j * w);
            double i0 = ((double)j / (double)(newSize-1) * (double)(oldSize-1)); // map this
            int64_t i1 = (int64_t)round(i0);
            double thing = 0.0;
            double weight = 0.0;
            bool reset = false;
            ResetKind rk = ResetKind::REST;
            for(int64_t k = i1 - i2; k <= i1 + i2; ++k) {
                if(k < 0 || k >= buffer_.size()) continue;
                //double x = fabs(i1 - k) / (double)i2;
                //x *= M_PI_2;
                //double we2 = 1.0 - atan2(i2 * M_PI, fabs(i1 - k));
                //double we = we2 * 0.99999 + 0.00001;

                double x = i2 - fabs(i0 - k) + 1.0;
                double we = log(x) / log(i2 + 1.0);
                if(we < 0.8) {
                    we = (0.2 + we) * (0.2 + we) - 0.2;
                }
                we = 0.95 * we + 0.05;

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
    } else {
        //NOP
        return;
    }
}

template<typename T>
void assign(std::initializer_list<double> stuff, int newSize)
{
    printf("%s\n", std::string(62, '=').c_str());
    printf("%d -> %d\n", stuff.size(), newSize);
    printf("%s\n", std::string(62, '-').c_str());
    decltype(buffer_) newStuff;
    for(auto&& i : stuff) {
        newStuff.push_back(std::make_tuple(i, false, ResetKind::NOTE));
    }
    buffer_.assign(newStuff.begin(), newStuff.end());
    for(auto&& e : buffer_) {
        printf("%f\n", std::get<0>(e));
    }
    printf("\n");
    Resample(newSize);
    for(auto&& e : buffer_) {
        printf("%f\n", std::get<0>(e));
    }
    printf("\n");
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
}
