#ifndef _Human_H
#define _Human_H

#include "Organism.h"
#include <vector>
using namespace std;
struct Coordinates {
    int x;
    int y;

    Coordinates(int x, int y) : x(x), y(y) {}
};
class Human : public Organism {
public:
    Human();
    void move(vector<vector<Organism*>> &grid, int x, int y) override;
    void recruit(vector<vector<Organism*>> &grid, int x, int y) override;
};


#endif
