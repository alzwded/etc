/*
Copyright (C) 2015 Vlad Mesco

This file is part of VoxelVolumizer.

VoxelVolumizer is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

VoxelVolumizer is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with VoxelVolumizer.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MM.hxx"

#ifdef MM_TEST

#include <vector>
#include <iostream>
#include <cstdio>

#ifdef _MSC_VER
void ErrorExit(StringType_t message)
{
    _tprintf(_T("%s\n"), message);
    exit(2);
}
#else
void ErrorExit(StringType_t message)
{
    printf("%s\n", message);
    exit(2);
}
#endif

struct S1
{
    unsigned i1;
    char s[252];
};

int main(int argc, char* argv[])
{
    MM mm(FU_S("test.tmp"));

    std::vector<MM::Handle> v;
    int idx = 0;
    char s[][252] = {
        "hello",
        "world",
        "yo",
    };
    for(int j = 0; j < 80; ++j) {
        auto a = mm.New(sizeof(S1) * 256 * 1024);
        v.push_back(a);
        auto ptr = mm.Deref<S1>(a);
        for(int i = 0; i < 256 * 1024; ++i) {
            ptr[i].i1 = idx;
            strcpy(ptr[i].s, s[j % 3]);
            idx++;
        }
    }

    for(int i = 0; i < v.size(); ++i) {
        auto ptr = mm.Deref<S1>(v[i]);
        printf("@%016" PRIx64 ": ", v[i]);
        printf("%8X %12s %8X %s\n", ptr[0].i1, ptr[0].s, ptr[42].i1, ptr[42].s);
    }

    // test with allocations less than a page
    v.clear();
    MM mm2(FU_S("test2.tmp"));
    idx = 0;
    for(int j = 0; j < 10; ++j) {
        auto a = mm2.New(sizeof(S1) * (256 * 512 - 1));
        v.push_back(a);
        auto ptr = mm2.Deref<S1>(a);
        for(int i = 0; i < (256 * 512 - 1); ++i) {
            ptr[i].i1 = idx;
            strcpy(ptr[i].s, s[j % 3]);
            idx++;
        }
    }

    for(int i = 0; i < v.size(); ++i) {
        auto ptr = mm2.Deref<S1>(v[i]);
        printf("@%016" PRIx64 ": ", v[i]);
        printf("%8X %12s %8X %s\n", ptr[0].i1, ptr[0].s, ptr[42].i1, ptr[42].s);
    }
}
#endif
