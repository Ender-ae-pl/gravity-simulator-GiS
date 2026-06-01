#include <raylib.h>
#include <iostream>
#include <cmath>
#include <vector>
#include "object.hpp"
using namespace std;


Color backgroundColor = WHITE;
int screenWidth = 800;
int screenHeight = 600;

vector<object> objects;


float forceDistr(float x1, float y1, float x2, float y2, float mass1, float mass2)
{
    float dy = y2 - y1;float dx = x2 - x1;
    float distance = sqrt(pow(dx,2) + pow(dy,2));
    float totalforce = (mass1 * mass2) / pow(distance, 2);
    float forceX = totalforce * (dx / distance);float forceY = totalforce * (dy / distance);
    return forceX, forceY;
}

int main() 
{
    InitWindow(screenWidth, screenHeight, "GOFRY I ŚMIETANA");
    SetTargetFPS(60);
    
    while (!WindowShouldClose())
    {
        // Update
        
        BeginDrawing();
            ClearBackground(backgroundColor);
            // Draw

        EndDrawing();
    }
    
    CloseWindow();
}