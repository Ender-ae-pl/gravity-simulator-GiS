#include <raylib.h>
#include <iostream>
#include "objclass.hpp"

Object::Object(double x, double y, double mass, int radius, Color objColor, double velocityX, double velocityY):x(x), y(y), mass(mass), radius(radius), objColor(objColor), velocityX(velocityX), velocityY(velocityY){};


void Object::incrementVelocity(double forceX, double forceY){
    double dt = GetFrameTime() * timescale;
    velocityX += dt * forceX / mass / massscale / scale;
    velocityY += dt * forceY / mass / massscale / scale;
};

void Object::move(int mode){
    double dt = GetFrameTime() * timescale; 
    x += dt * velocityX;
    y += dt * velocityY;
    if (y>screenY){
        y=0;
        if(mode == 1 || mode == 3) {
            velocityY=0;
            velocityX=0;
        }
    }
    if (y<0){
        y=screenY;
        if(mode == 1 || mode == 3) {
            velocityY=0;
            velocityX=0;
        }
    }
    if (x>screenX){
        x=0;
        if(mode == 1 || mode == 3) {
            velocityX=0;
            velocityY=0;
        }
    }
    if (x<0){
        x=screenX;
        if(mode == 1 || mode == 3) {
            velocityX=0;
            velocityY=0;
        }
    }
};

void Object::setMove(double velx, double vely){
    double dt = GetFrameTime() * timescale;
    x += dt * velx;
    y += dt * vely;
};

void Object::draw(){
    if (y > screenY || y < 0 || x > screenX || x < 0){
        return;
    }
    DrawCircle(x, y, radius, objColor);
};

