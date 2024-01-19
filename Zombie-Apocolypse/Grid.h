#ifndef _CITY_H
#define _CITY_H

#include <iostream>
#include <vector>
#include "Coordinates.h"
using namespace std;
class Organism;

class Grid {
public:
    int size;
    vector<vector<Organism*>> cells;
    vector<Coordinates> HumansavedCoordinates;
    vector<Coordinates> ZombiesavedCoordinates;

    Grid(int size);

    void printGrid();
    void initialize();
};

#endif

