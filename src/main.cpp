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
int timescale=1000;
int massscale=100000;

vector<Object> objects;
int objCount=0;


double distance(double x1, double y1, double x2, double y2){
    return sqrt(pow(x2-x1,2)+pow(y2-y1,2));
}

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
vector<double> collision(Object& obj1, Object& obj2){
    double dy = obj2.y - obj1.y;double dx = obj2.x - obj1.x;
    double distance = scale*sqrt(pow(dx,2) + pow(dy,2)) +1;
    double v1 = sqrt(pow(obj1.velocityX,2)+pow(obj1.velocityY,2));
    double v2 = sqrt(pow(obj2.velocityX,2)+pow(obj2.velocityY,2));
    
    double correction = -v1;
    obj1.velocityX *= -1;
    obj1.velocityY *= -1;
    cout << "pos1" << obj1.x << " " << obj1.y << endl;
    obj1.move();
    cout << "pos2" << obj1.x << " " << obj1.y << endl;
    obj1.velocityX = 0;
    obj1.velocityY = 0;

    return {};
}

int main() 
{
    InitWindow(screenWidth, screenHeight, "GOFRY I ŚMIETANA");
    SetTargetFPS(60);
    
    objects.push_back(Object(400.0f, 300.0f, 30000, 30));
    objects.push_back(Object(500.0f, 100.0f, 500, 20, 0.1, 0.0));
    objCount+=2;

    while (!WindowShouldClose())
    {
        // Input
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            Vector2 mousePos = GetMousePosition();
            objects.push_back(Object(mousePos.x, mousePos.y, 500, 20));
            objCount++;
        }


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
            // Collision
            for (int j=0; j<objCount;j++){
                if (i==j){
                    continue;
                }
                if (distance(objects[i].x, objects[i].y, objects[j].x, objects[j].y) < objects[i].radius + objects[j].radius){
                    collision(objects[i], objects[j]);
                }
            }
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