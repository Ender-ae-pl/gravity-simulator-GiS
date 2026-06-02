#include <raylib.h>
#include <iostream>
#include <cmath>
#include <vector>
#include "objclass.hpp"
using namespace std;


Color backgroundColor = BLACK;
int screenWidth = 1600;
int screenHeight = 900;
int scale = 10;
int timescale=500;
int massscale=100000;

vector<Object> objects;
int objCount=0;


vector<double> forceDistr(double x1, double y1, double x2, double y2, double mass1, double mass2)
{
    double dy = y2 - y1;double dx = x2 - x1;
    double distance = scale*sqrt(pow(dx,2) + pow(dy,2)) +1;
    double totalforce = massscale*(mass1 * mass2) / pow(distance, 2);
    double forceX = dx * totalforce / distance ;
    double forceY = dy * totalforce / distance ;
    vector<double> result;
    result.push_back(forceX);
    result.push_back(forceY);
    return result;
}


int main() 
{
    InitWindow(screenWidth, screenHeight, "GOFRY I ŚMIETANA");
    SetTargetFPS(100);
    
    objects.push_back(Object(400.0f, 300.0f, 30000, 30));
    objects.push_back(Object(500.0f, 100.0f, 500, 20, 0.0, 0.0));
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
                vector<double> dforce = forceDistr(objects[i].x, objects[i].y, objects[j].x, objects[j].y, objects[i].mass, objects[j].mass);
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