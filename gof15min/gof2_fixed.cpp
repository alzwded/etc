#include <cstdio>
#include <set>
#include <utility>
#include <algorithm>
#include <climits>

// We need a sparse matrix to keep track of our board
// This implies we need a way to access the value (i,j) => operator()
// and a way to clean it => clear()
// We also need to keep track of our bounds since the board can grow in
// any direction => {min|max}[xy]
// Since the board contains only boolean values, the fastest way to
// implement it is with a set of pair<int, int>. If a pair is in the
// set, this means that the cell (i, j) is alive
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
        // update the bounds after a set
        minx = std::min(minx, x);
        miny = std::min(miny, y);
        maxx = std::max(maxx, x);
        maxy = std::max(maxy, y);
    }
    void clear() {
        _alive.clear();
        minx = miny = INT_MAX;
        maxx = maxy = INT_MIN;
    }
} a, b; // we need to boards, the current and the next generations

// evolve the next generation
void evolve(int g)
{
    // we need to flip boards, easiest way to do it is to keep track
    // of the generation parity
    SparseMatrix& m = (g % 2) ? a : b;
    SparseMatrix& n = (g % 2) ? b : a;
    // clear the next generation's board (we don't want residues)
    n.clear();
    
    // go through the board, the board knows its bounds
    for(int i = m.minx; i <= m.maxx; ++i) {
        for(int j = m.miny; j <= m.maxy; ++j) {
            // apply the game of life rules
            if(m(i, j)) {
                int c = 0;
                if(m(i - 1, j)) c++;
                if(m(i + 1, j)) c++;
                if(m(i, j - 1)) c++;
                if(m(i, j + 1)) c++;
                if(c >= 2 && c <= 3) {
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
                }
            }
        }
    }
}

// print the board
void print(int g)
{
    // again, we need the correct board, we keep track of the generation
    // number
    SparseMatrix& m = (g % 2) ? a : b;
    for(int i = m.minx; i <= m.maxx; ++i) {
        for(int j = m.miny; j <= m.maxy; ++j) {
            if(m(i, j)) printf("o");
            else printf(" ");
        }
        printf("\n");
    }
    // force flush the output since the only way to stop the program
    // is ^C
    fflush(stdout);
}

int main()
{
    // initialize our boards
    a.clear();
    b.clear();
    // read the coordinates of living cells
    while(!feof(stdin)) {
        int x, y;
        // make sure we've actually read something useful
        if(2 == fscanf(stdin, "%d %d", &x, &y))
            a.set(x, y);
        else
            break;
    }
    // keep track of the current generation
    // it's okay if it overflows, since INT_MAX is odd and INT_MIN is even
    int generation = 0;
    while(1) {
        printf("generation %d\n", generation++);
        evolve(generation);
        print(generation);
    }
    return 0;
}
