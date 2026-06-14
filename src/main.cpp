#include <raylib.h>
#include <iostream>
#include <cmath>
#include <vector>
#include "objclass.hpp"
#include <map>
using namespace std;
    
Color backgroundColor = BLACK;
int screenWidth = 3740;
int screenHeight = 2060;
int scale = 70;
int timescale=10000;
int massscale=100000;
map<int, Color> MassColors = {
    {0, LIGHTGRAY},
    {10, GRAY},
    {25, DARKGRAY},
    {50, BLUE},
    {100, GREEN},
    {250, YELLOW},
    {500, ORANGE},
    {1000, RED},
    {2500, PINK},
    {5000, VIOLET},
    {10000, MAROON},
};

vector<Object> objects;
int objCount=0;
bool movingObject = false;
Object* movingObjPtr = nullptr;
Vector2 mousePos;
Vector2 prevMousePos;
Vector2 newobjCenter;
double newobjMass;

double distance(double x1, double y1, double x2, double y2){
    return sqrt(pow(x2-x1,2)+pow(y2-y1,2));
}
Color getColor(double mass){
    for (auto& pair : MassColors) {
        if (mass <= pair.first) {
            return pair.second;
        }
    }
    return WHITE;
}

vector<double> qformula(double a, double b, double c){
    double discriminant = pow(b, 2) - 4 * a * c;
    if (discriminant < 0) {
        return {};
    }
    double sqrtDiscriminant = sqrt(discriminant);
    double root1 = (-b + sqrtDiscriminant) / (2 * a);
    double root2 = (-b - sqrtDiscriminant) / (2 * a);
    return {root1, root2};
}

vector<double> forceDistr(double x1, double y1, double x2, double y2, double mass1, double mass2)
{
    double dy = y2 - y1;double dx = x2 - x1;
    double distance = scale*sqrt(pow(dx,2) + pow(dy,2)) +1;
    double totalforce = massscale*(mass1 * mass2) / (pow(x2-x1,2)+pow(y2-y1,2)) / scale / scale;
    double forceX = dx * totalforce / distance ;
    double forceY = dy * totalforce / distance ;
    vector<double> result;
    result.push_back(forceX);
    result.push_back(forceY);
    return result;
}
vector<double> collision(Object& obj1, Object& obj2){
    obj1.setMove(obj1.velocityX/-2, obj1.velocityY/-2);
    obj2.setMove(obj1.velocityX/2, obj1.velocityY/2);

    double v1 = sqrt(pow(obj1.velocityX,2)+pow(obj1.velocityY,2));
    double v2 = sqrt(pow(obj2.velocityX,2)+pow(obj2.velocityY,2));

    if (v1<=v2){
        return {};
    }
    double p1 = obj1.mass * v1;
    double p2 = obj2.mass * v2;
    double px = obj1.mass * obj1.velocityX + obj2.mass * obj2.velocityX;
    double py = obj1.mass * obj1.velocityY + obj2.mass * obj2.velocityY;
    double c1 = sqrt(pow(px,2)+pow(py,2));
    double c2 = (obj1.mass * v1 *v1) + (obj2.mass * v2 * v2);
    double dm = obj2.mass / obj1.mass;

    if (dm>1){
        dm=1/dm;
        double newv1 = qformula(-1-obj1.mass/obj2.mass, -2 * (c1/obj2.mass), c2/obj1.mass - (c1*c1)/(obj1.mass)/(obj2.mass))[1];
        double newv2 = (c1/obj2.mass) + (obj1.mass/obj2.mass) * newv1;
        obj2.velocityX = obj1.velocityX * newv2 / v1;
        obj2.velocityY = obj1.velocityY * newv2 / v1;
        obj1.velocityX = - obj1.velocityX * newv1 / v1;
        obj1.velocityY = - obj1.velocityY * newv1 / v1;

    } else {
        double newv1 = qformula(-1-obj1.mass/obj2.mass, 2 * (c1/obj2.mass), c2/obj1.mass - (c1*c1)/(obj1.mass)/(obj2.mass))[0];
        double newv2 = (c1/obj2.mass) - (obj1.mass/obj2.mass) * newv1;
        obj2.velocityX = obj1.velocityX * newv2 / v1;
        obj2.velocityY = obj1.velocityY * newv2 / v1;
        obj1.velocityX = obj1.velocityX * newv1 / v1;
        obj1.velocityY = obj1.velocityY * newv1 / v1;
    }
    return {};
}

int main() 
{
    InitWindow(screenWidth, screenHeight, "GOFRY I ŚMIETANA");
    ToggleFullscreen();
    SetTargetFPS(60);
    
    objects.push_back(Object(400.0f, 300.0f, 50, 30));
    objects.push_back(Object(500.0f, 100.0f, 5000, 20));
    objCount+=2;

    while (!WindowShouldClose())
    {
        // Input
        mousePos = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            // is object clicked
            for (int i = 0; i < objCount; i++) {
                if (distance(objects[i].x, objects[i].y, mousePos.x, mousePos.y) < objects[i].radius) {
                    movingObject = true;
                    movingObjPtr = &objects[i];
                    break;
                }
            }

            if (!movingObject){
                // creation start
                newobjCenter = mousePos;
                newobjMass = 0;
            }
        }
        if (movingObject && movingObjPtr != nullptr){
            double velx = (mousePos.x - prevMousePos.x) / GetFrameTime()/ timescale;
            double vely = (mousePos.y - prevMousePos.y) / GetFrameTime()/ timescale;
            movingObjPtr->velocityX = velx;
            movingObjPtr->velocityY = vely;
            movingObjPtr->x = mousePos.x;
            movingObjPtr->y = mousePos.y;

        }
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !movingObject){
            newobjMass += 10*massscale*GetFrameTime();
            double radius = distance(newobjCenter.x, newobjCenter.y, mousePos.x, mousePos.y);
            DrawCircle(newobjCenter.x, newobjCenter.y, radius, GRAY);
        }
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
            if (!movingObject){
                double radius = distance(newobjCenter.x, newobjCenter.y, mousePos.x, mousePos.y);
                objects.push_back(Object(newobjCenter.x, newobjCenter.y, newobjMass, radius));
                objCount++;
            }
            movingObject = false;
            movingObjPtr = nullptr;
        }

        prevMousePos = mousePos;



        // Update
        for (int i=0; i<objCount;i++){
            
            // Force
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
        

        // Drawing
        BeginDrawing();
            ClearBackground(backgroundColor);
            // Draw
            for (int i=0; i<objCount;i++) objects[i].draw();

        EndDrawing();
    }
    
    CloseWindow();
}