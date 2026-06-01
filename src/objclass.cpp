#include <raylib.h>
#include "objclass.hpp"

Object::Object(float x, float y, int mass, int radius, float velocityX, float velocityY):x(x), y(y), mass(mass), radius(radius), velocityX(velocityX), velocityY(velocityY){};


void Object::incrementVelocity(float forceX, float forceY){
    velocityX += forceX / (1000*mass) / scale;
    velocityY += forceY / (1000*mass) / scale;
};

void Object::move(){
    x += velocityX;
    y += velocityY;
    if (y>screenY || y<0){
        y=screenY-y;
        velocityY=0;
    }
    if (x>screenX || x<0){
        x=screenX-x;
        velocityX=0;
    }
};

void Object::draw(){
    if (y > screenY || y < 0 || x > screenX || x < 0){
        return;
    }
    DrawCircle(x, y, radius, GRAY);
};

