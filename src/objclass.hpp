#pragma once
#include <raylib.h>
extern int scale;

class Object
{
private:
        int screenX = GetScreenWidth();
        int screenY = GetScreenHeight();
public:
    float x;
    float y;
    int mass;
    float velocityX=0;
    float velocityY=0;
    int radius;

    Object(float x, float y, int mass, int radius, float velocityX=0, float velocityY=0);

    void incrementVelocity(float forceX, float forceY);

    void move();

    void draw();
};

