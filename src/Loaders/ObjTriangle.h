#pragma once
#include <vector>
struct ObjTriangle {
    int v[3], vt[3], vn[3];
    ObjTriangle(const std::vector<std::tuple<int, int, int>>& vs) {
        int i = 0;
        for (auto [a, b, c] : vs) {
            v[i] = a;
            vt[i] = b;
            vn[i] = c;
            i++;
        }
    }
    ObjTriangle(int a, int b, int c) {
        v[0] = a, v[1] = b, v[2] = c;
    }
};
