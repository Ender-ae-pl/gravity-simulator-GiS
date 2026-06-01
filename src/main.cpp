#include <raylib.h>
#include <iostream>
#include <cmath>
#include <vector>
#include "objclass.hpp"
using namespace std;


Color backgroundColor = BLACK;
int screenWidth = 1600;
int screenHeight = 900;
int scale = 100;

vector<Object> objects;
int objCount=0;


vector<float> forceDistr(float x1, float y1, float x2, float y2, float mass1, float mass2)
{
    float dy = y2 - y1;float dx = x2 - x1;
    float distance = scale*sqrt(pow(dx,2) + pow(dy,2)) +1;
    float totalforce = 1000*(mass1 * mass2) / pow(distance, 2);
    float forceX = scale * dx * distance / totalforce;
    float forceY = scale * dy * distance / totalforce;
    vector<float> result;
    result.push_back(forceX);
    result.push_back(forceY);
    return result;
}


int main() 
{
    InitWindow(screenWidth, screenHeight, "GOFRY I ŚMIETANA");
    SetTargetFPS(60);
    
    objects.push_back(Object(400.0f, 300.0f, 30000, 30));
    objects.push_back(Object(500.0f, 100.0f, 500, 20, 10.0f, 0.0f));
    objCount+=2;

    while (!WindowShouldClose())
    {
        // Update
        for (int i=0; i<objCount;i++){
            int forceX=0, forceY=0;
            for (int j=0; j<objCount;j++){
                if (i==j){
                    continue;
                }
                vector<float> dforce = forceDistr(objects[i].x, objects[i].y, objects[j].x, objects[j].y, objects[i].mass, objects[j].mass);
                forceX += dforce[0];forceY += dforce[1];
            }

            objects[i].incrementVelocity(forceX, forceY);
            objects[i].move();
        }
        

        // Draw
        BeginDrawing();
            ClearBackground(backgroundColor);
            // Draw
            for (int i=0; i<objCount;i++){
                objects[i].draw();
            }

        EndDrawing();
    }
    
    CloseWindow();
}