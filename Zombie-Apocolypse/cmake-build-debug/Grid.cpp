//
// Created by W0478410 on 2023-12-07.
//
#include "../Grid.h"
#include "../Organism.h"
#include "../Human.h"
#include "../Zombie.h"
#include "../Coordinates.h"
#include <windows.h>
void Col(int c)
{
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, c);
}
    Grid::Grid(int size) : size(size), cells(size, vector<Organism*>(size, nullptr)){};

    void  Grid::printGrid(){
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
        for (const auto& coord : HumansavedCoordinates) {
            cells[coord.x][coord.y]->move(cells, coord.x, coord.y);
        }
        for (const auto& coord :  ZombiesavedCoordinates) {
            cells[coord.x][coord.y]->move(cells, coord.x, coord.y);
        }
        ZombiesavedCoordinates.clear();
        HumansavedCoordinates.clear();
    };
    void Grid::initialize(){
        //Left
        cells[10][0] = new Human;
        //top
        cells[0][10] = new Human;
        //right
        cells[19][10] = new Human;
        //Bottom
        cells[10][19] = new Human;


        cells[0][0] = new Zombie;
        cells[0][19] = new Zombie;
        cells[19][19] = new Zombie;
        cells[19][0] = new Zombie;
    }
