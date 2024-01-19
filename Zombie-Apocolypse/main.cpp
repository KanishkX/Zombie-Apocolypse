#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <windows.h>


using namespace std;

struct Coordinates {
    int x;
    int y;
    Coordinates(int x, int y) : x(x), y(y) {}
};

void Col(int);
class Organism {
public:
    char symbol;
    int recruitCounter;

    Organism(char symbol) : symbol(symbol), recruitCounter(0) {}

    virtual void move(std::vector<std::vector<Organism*>> &grid, int x, int y) = 0;
    virtual void recruit(std::vector<std::vector<Organism*>> &grid, int x, int y) = 0;
};

class Human : public Organism {
public:
    Human() : Organism('H') {}

    void move(vector<vector<Organism*>> &grid, int x, int y) override {

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

    void recruit(vector<vector<Organism*>> &grid, int x, int y) override{
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


};

class Zombie : public Organism {
public:
    int breedCounter;
    bool ateHuman;
    Zombie() : Organism('Z') { breedCounter = 0; ateHuman = true;}

    void move(vector<vector<Organism*>> &grid, int x, int y) override {

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

    void recruit(std::vector<std::vector<Organism*>> &grid, int x, int y) override {
        // Convert a randomly selected adjacent human into a zombie
        int newX, newY;
        if(findHuman(grid,x,y,newX,newY)){
            // Replace Human with a new zombie
            grid[newX][newY] = new Zombie;
        }


    }
private:


    bool findHuman(vector<vector<Organism*>> &grid, int x, int y, int &newX, int &newY) {
        int numRows = (int)grid.size();
        int numCols = (numRows > 0) ? grid[0].size() : 0;

        // Check adjacent cells for a human
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

        //no human is found, return false
        return false;
    }

    void moveRandomly( vector<vector<Organism*>> &grid, int x, int y) {
        // Move to a random adjacent (including diagonal) empty cell
        vector<pair<int, int>> possibleMoves;
        // Loop through adjacent cells
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                int newX = x + i;
                int newY = y + j;

                //new position is within the grid bounds and is empty
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
class Grid {
public:
    int size;
    vector<vector<Organism*>> cells;
    vector<Coordinates> HumansavedCoordinates;
    vector<Coordinates> ZombiesavedCoordinates;

    Grid(int size) : size(size), cells(size, vector<Organism*>(size, nullptr)) {}

    void printGrid(){
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (cells[i][j] == nullptr) {
                    cout << '-';
                }else if(typeid(*cells[i][j]) == typeid(Human)){
                    Col(2);
                    cout << cells[i][j]->symbol;
                    Coordinates cord = {i,j};
                    HumansavedCoordinates.push_back(cord);
                    Col(3);
                }else if(typeid(*cells[i][j]) == typeid(Zombie)){
                    Col(12);
                    cout << cells[i][j]->symbol;
                    Coordinates cord = {i,j};
                    ZombiesavedCoordinates.push_back(cord);
                    Col(3);
                }
                cout << ' ';
            }
            cout << '\n';
        }
        cout << endl;
        //Loop through the structure
        for (const auto& coord : HumansavedCoordinates) {
            cells[coord.x][coord.y]->move(cells, coord.x, coord.y);
        }
        for (const auto& coord :  ZombiesavedCoordinates) {
            cells[coord.x][coord.y]->move(cells, coord.x, coord.y);
        }
        //Clearing the structure  for new X and Y points
        ZombiesavedCoordinates.clear();
        HumansavedCoordinates.clear();
    };
    void initialize(){
        // 100 Zombies divided equally in smaller grid
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 20; ++j) {
                if (j % 2 == 0) {
                    cells[i][j] = new Human;
                }
            }
        }

        // Creating 5 zombies at specific locations
        cells[10][10] = new Zombie;
        cells[0][0] = new Zombie;
        cells[0][19] = new Zombie;
        cells[19][19] = new Zombie;
        cells[19][0] = new Zombie;
    }
};
int main()
{
    const int gridSize = 20;
    Grid grid(gridSize);
    grid.initialize();
    while(true)//always true endless loop
    {
        grid.printGrid();
        cout << "tick" << endl;
        this_thread::sleep_for(0.5s);
    }

}
void Col(int c)
{
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, c);
}