#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>
#include<QLayout>
using namespace std;
struct point{
    int x;
    int y;
};
enum{WALL = 1, GROUND = 0, CRATES = 2, PLAYER = 3, GOAL = 4,COIN =5,BOMB=6,POOP=7};
class Map
{
public:
    Map(int);
    ~Map();
    void setmapname(int);
    void setmapdata();
    void setwidth(int);
    void setheight(int);
    string name;
    int stage;
    int width;
    int height;
    int **mapdata;
    int **mapbackup;
    int cratenum;
    point start;
};

#endif // MAP_H
