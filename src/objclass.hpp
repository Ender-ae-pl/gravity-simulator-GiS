#pragma once
#include <raylib.h>
extern int scale;

class Object
{
private:
        int screenX = GetScreenWidth();
        int screenY = GetScreenHeight();
public:
    double x;
    double y;
    int mass;
    double velocityX=0;
    double velocityY=0;
    int radius;

    Object(double x, double y, int mass, int radius, double velocityX=0, double velocityY=0);

    void incrementVelocity(double forceX, double forceY);

    void move();

    void draw();
};

