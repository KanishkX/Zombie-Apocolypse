#ifndef _Organism_H
#define _Organism_H
#include <vector>
#include <iostream>

using namespace std;

class City;

class Organism {
public:
    char symbol;
    int recruitCounter;

    Organism(char symbol);

    virtual void move(vector<vector<Organism*>> &grid, int x, int y);
    virtual void recruit(vector<vector<Organism*>> &grid, int x, int y);
};

#endif
