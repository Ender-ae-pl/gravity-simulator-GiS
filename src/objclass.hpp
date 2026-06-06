#pragma once
#include <raylib.h>
extern int scale;
extern int timescale;
extern int massscale;
class Object
{
private:
        int screenX = GetScreenWidth();
        int screenY = GetScreenHeight();
public:
    double x;
    double y;
    double mass;
    double velocityX=0;
    double velocityY=0;
    int radius;

    Object(double x, double y, double mass, int radius, double velocityX=0, double velocityY=0);

    void incrementVelocity(double forceX, double forceY);

    void move();

    void setMove(double velx, double vely);

    void draw();
};

