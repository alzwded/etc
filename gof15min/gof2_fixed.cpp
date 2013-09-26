#include <cstdio>
#include <set>
#include <utility>
#include <algorithm>

struct SparseMatrix {
    std::set<std::pair<int, int> > _alive;
    int minx, miny, maxx, maxy;
    bool operator()(int x, int y) {
        if(_alive.find(std::make_pair(x, y)) != _alive.end()) {
            return true;
        } else {
            return false;
        }
    }
    void set(int x, int y) {
        _alive.insert(std::make_pair(x, y));
        minx = std::min(minx, x);
        miny = std::min(miny, y);
        maxx = std::max(maxx, x);
        maxy = std::max(maxy, y);
    }
    void reset(int x, int y) {
        auto i = _alive.find(std::make_pair(x, y));
        if(i != _alive.end()) {
            _alive.erase(i);
        }
    }
    void clear() {
        _alive.clear();
        minx = miny = 32768;
        maxx = maxy = -32768;
    }
} a, b;

void evolve(int g)
{
    SparseMatrix& m = (g % 2) ? a : b;
    SparseMatrix& n = (g % 2) ? b : a;
    n.clear();
    
    for(int i = m.minx; i <= m.maxx; ++i) {
        for(int j = m.miny; j <= m.maxy; ++j) {
            if(m(i, j)) {
                int c = 0;
                if(m(i - 1, j)) c++;
                if(m(i + 1, j)) c++;
                if(m(i, j - 1)) c++;
                if(m(i, j + 1)) c++;
                if(c < 2 || c == 4) {
                    continue;
                } else {
                    n.set(i, j);
                }
            } else {
                int c = 0;
                if(m(i - 1, j)) c++;
                if(m(i + 1, j)) c++;
                if(m(i, j - 1)) c++;
                if(m(i, j + 1)) c++;
                if(c == 3) {
                    n.set(i, j);
                } else {
                    continue;
                }
            }
        }
    }
}

void print(int g)
{
    SparseMatrix& m = (g % 2) ? a : b;
    for(int i = m.minx; i <= m.maxx; ++i) {
        for(int j = m.miny; j <= m.maxy; ++j) {
            if(m(i, j)) printf("o");
            else printf(" ");
        }
        printf("\n");
    }
    fflush(stdout);
}

int main()
{
    a.clear();
    b.clear();
    while(!feof(stdin)) {
        int x, y;
        if(2 == fscanf(stdin, "%d %d", &x, &y))
            a.set(x, y);
    }
    int generation = 0;
    while(1) {
        printf("generation %d\n", generation++);
        evolve(generation);
        print(generation);
    }
}
