#include <raylib.h>
#include <iostream>
#include <cmath>
#include <vector>
#include "objclass.hpp"
#include <map>
using namespace std;
    
Color backgroundColor = BLACK;
int screenWidth = 1920;
int screenHeight = 1080;
int scale = 200;
int timescale=10000;
int massscale=100000;
int fps = 60;

vector<Object> objects;
int objCount=0;
bool movingObject = false;
Object* movingObjPtr = nullptr;
Vector2 mousePos;
Vector2 prevMousePos;
Vector2 newobjCenter;
double newobjMass;
int color_r=0, color_g=0, color_b=20;
int color_change=0;

double distance(double x1, double y1, double x2, double y2){
    return sqrt(pow(x2-x1,2)+pow(y2-y1,2));
}

Color getColor(){
    if(color_change == 0) {
        color_b++;
        if(color_b == 255) color_change++;
    }
    else if(color_change == 1) {
        color_b--;
        color_g++;
        if(color_g == 255) color_change++;
    }
    else if(color_change == 2) {
        color_g--;
        color_r++;
        if(color_r == 255) color_change++;
    }
    return {(unsigned char)color_r, (unsigned char)color_g, (unsigned char)color_b, 255};
}

vector<double> qformula(double a, double b, double c){
    double discriminant = pow(b, 2) - 4 * a * c;
    if (discriminant < 0) {
        cout << "No real roots exist." << endl;
        cout << "a: " << a << ", b: " << b << ", c: " << c << endl;
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
    obj1.setMove(-obj1.velocityX, -obj1.velocityY);

    double v1 = sqrt(pow(obj1.velocityX,2)+pow(obj1.velocityY,2));
    double v2 = sqrt(pow(obj2.velocityX,2)+pow(obj2.velocityY,2));
    if (v1<=v2){
        return {};
    }
    cout << (v1+v2)*timescale*fps << endl;
    if ((v1+v2)*timescale*fps<1000 || distance(obj1.x, obj1.y, obj2.x, obj2.y) < (obj1.radius + obj2.radius)){
        cout << "Objects too slow to collide: " << v1 << " " << v2 << endl;
        if (obj1.mass > obj2.mass) {
            return {1,1};
        }
        else {
            return {1,2};
        }
    }
    double dx = obj2.x - obj1.x;
    double dy = obj2.y - obj1.y;
    double dist = sqrt(pow(dx,2)+pow(dy,2));
    double nx = dx / dist;
    double ny = dy / dist;
    double vr = (obj1.velocityX - obj2.velocityX) * nx + (obj1.velocityY - obj2.velocityY) * ny;
    double impulse = (2 * vr) / (1/obj1.mass + 1/obj2.mass);

    obj1.velocityX -= impulse * nx / obj1.mass;
    obj1.velocityY -= impulse * ny / obj1.mass;
    obj2.velocityX += impulse * nx / obj2.mass;
    obj2.velocityY += impulse * ny / obj2.mass;


    return {};
}

int main() 
{
    InitWindow(screenWidth, screenHeight, "GOFRY I ŚMIETANA");
    //ToggleFullscreen();
    SetTargetFPS(fps);

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
            DrawCircle(newobjCenter.x, newobjCenter.y, radius, getColor());
        }
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
            if (!movingObject){
                double radius = distance(newobjCenter.x, newobjCenter.y, mousePos.x, mousePos.y);
                objects.push_back(Object(newobjCenter.x, newobjCenter.y, newobjMass, radius, getColor()));
                color_r=0; color_g=0; color_b=20; color_change=0;
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
                    vector<double> event = collision(objects[i], objects[j]);

                    if (event.size()>1 && event[0]==1){
                    // merge
                    objCount--;
                    
                    if (event[1]==1){
                    objects[i].mass += objects[j].mass;
                    objects[i].radius = sqrt(objects[i].radius*objects[i].radius + objects[j].radius*objects[j].radius);
                    objects.erase(objects.begin() + j);}
                    else{
                    objects[j].mass += objects[i].mass;
                    objects[j].radius = sqrt(objects[j].radius*objects[j].radius + objects[i].radius*objects[i].radius);
                    objects.erase(objects.begin() + i);}
                    }
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