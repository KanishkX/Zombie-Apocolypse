#ifndef _Zombie_H
#define _Zombie_H

#include "Organism.h"

class Zombie : public Organism {
public:
    int breedCounter;
    bool ateHuman;
    Zombie();
    void move(vector<vector<Organism*>> &grid, int x, int y)  override;
    void recruit(std::vector<std::vector<Organism*>> &grid, int x, int y)  override;
private:

    bool findHuman(vector<vector<Organism*>> &grid, int x, int y, int &newX, int &newY);
    void moveRandomly( vector<vector<Organism*>> &grid, int x, int y);
};

#endif
