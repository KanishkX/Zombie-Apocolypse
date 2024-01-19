//
// Created by W0478410 on 2023-12-07.
//
#include "../Organism.h"
#include "../Zombie.h"
#include "../Human.h"
    Zombie::Zombie() : Organism('Z') { breedCounter = 0; ateHuman = true;}

    void Zombie::move(vector<vector<Organism*>> &grid, int x, int y){

        int newX, newY;
        if (breedCounter >= 3 && ateHuman) {
            // Convert to a human (starvation)
            grid[x][y] = nullptr;
            grid[x][y] = new Human;
            delete this;
        }
            // Check for adjacent humans and move to eat them
        else if (findHuman(grid, x, y, newX, newY)) {
            // Move to the position of a human and eat them
            grid[newX][newY] = this;  // Move the zombie to the new position
            grid[x][y] = nullptr;     // Clear the old position
            ateHuman = false;
//            breedCounter = 0;         // Reset the breed counter
        } else {
            // No adjacent humans, move to an empty position
            moveRandomly(grid, x, y);
        }

        // Check and increment breed counter
        breedCounter++;
        if (breedCounter == 8) {
            recruit(grid, x, y);  // Breed if the counter reaches 8 or more
            breedCounter = 0;     // Reset the breed counter
        }

        // Check for starvation

    }

    void Zombie::recruit(std::vector<std::vector<Organism*>> &grid, int x, int y){
        // Convert a randomly selected adjacent human into a zombie
        int newX, newY;
        if(findHuman(grid,x,y,newX,newY)){
            // Replace Human with a new zombie
            grid[newX][newY] = new Zombie;
        }


    }

    bool Zombie::findHuman(vector<vector<Organism*>> &grid, int x, int y, int &newX, int &newY) {
        int numRows = (int)grid.size();
        int numCols = (numRows > 0) ? grid[0].size() : 0;

        // Check adjacent cells (including diagonal) for a human
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                int X = x + i;
                int Y = y + j;

                // Check if the new position is within the grid bounds
                if (X >= 0 && X < numRows && Y >= 0 && Y < numCols) {
                    // Check if the cell contains a Human (assuming Human is a derived class from Organism)
                    if (grid[X][Y] != nullptr && typeid(*grid[X][Y]) == typeid(Human)) {
                        // If a human is found, set newX and newY and return true
                        newX = X;
                        newY = Y;
                        return true;
                    }
                }
            }
        }

        // If no human is found, return false
        return false;
    }

    void Zombie::moveRandomly( vector<vector<Organism*>> &grid, int x, int y) {
        // Move to a random adjacent (including diagonal) empty cell
        vector<pair<int, int>> possibleMoves;
        // Loop through adjacent cells
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                int newX = x + i;
                int newY = y + j;

                // Check if the new position is within the grid bounds and is empty
                if (newX >= 0 && newX < grid.size() && newY >= 0 && newY < grid[newX].size() && grid[newX][newY] == nullptr) {
                    possibleMoves.emplace_back(newX, newY);
                }
            }
        }
        //Move randomly if there are valid empty cells
        if (!possibleMoves.empty()) {
            int randomIndex = rand() % possibleMoves.size();
            int newX = possibleMoves[randomIndex].first;
            int newY = possibleMoves[randomIndex].second;

            // Move to the randomly selected empty cell
            grid[newX][newY] = this;
            grid[x][y] = nullptr;

        }
    }

};
