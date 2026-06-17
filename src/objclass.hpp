#pragma once
#include <raylib.h>
extern int scale;
extern int timescale;
extern int massscale;

class Object
{
// Private
    int screenX = GetScreenWidth();
    int screenY = GetScreenHeight();
public:
    double x;
    double y;
    double mass;
    double velocityX=0.0;
    double velocityY=0.0;
    int radius;
    Color objColor;

    Object(double x, double y, double mass, int radius, Color objColor=WHITE, double velocityX=0, double velocityY=0);

    void incrementVelocity(double forceX, double forceY);

    void move();

    void setMove(double velx, double vely);

    void draw();
};

