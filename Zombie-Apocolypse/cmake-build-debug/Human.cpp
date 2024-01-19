//
// Created by W0478410 on 2023-12-07.
//
#include "../Human.h"
Human::Human() : Organism('H') {}

    void Human::move(vector<vector<Organism*>> &grid, int x, int y){

        // Generate a random direction: 0 for up, 1 for down, 2 for left, 3 for right
        int direction = rand() % 4;

        int newX = x, newY = y;

        switch (direction) {
            case 0: // Move up
                newX = (x > 0) ? x - 1 : x;
                break;
            case 1: // Move down
                newX = (x < grid.size() - 1) ? x + 1 : x;
                break;
            case 2: // Move left
                newY = (y > 0) ? y - 1 : y;
                break;
            case 3: // Move right
                newY = (y < grid[x].size() - 1) ? y + 1 : y;
                break;
        }

        // Check if the target cell is empty
        if (grid[newX][newY] == nullptr) {
            // Move the human to the new cell
            grid[newX][newY] = this;
            grid[x][y] = nullptr;
            recruitCounter++;
            recruit(grid, newX, newY);

        }


    }

    void Human::recruit(vector<vector<Organism*>> &grid, int x, int y){
        // Check if the human has survived for three time steps
        if (recruitCounter >= 3) {
            // Attempt to recruit a new human in an adjacent empty cell
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    int newX = x + i;
                    int newY = y + j;

                    // Check if the cell is within the grid and empty
                    if (newX >= 0 && newX < grid.size() && newY >= 0 && newY < grid[x].size() &&
                        grid[newX][newY] == nullptr) {
                        // Create a new human in the adjacent empty cell
                        grid[newX][newY] = this;
                        recruitCounter = 0;  // Reset the recruit counter
                        return;
                    }
                }
            }

            // If no empty cell is found, reset the recruit counter
            recruitCounter = 0;
        }
    }


