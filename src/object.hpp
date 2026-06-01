class object
{
private:

public:
    float x;
    float y;
    float mass;
    float velocityX=0;
    float velocityY=0;

    object(float x, float y, float mass);

    void incrementVelocity(float forceX, float forceY){
        velocityX += forceX / mass;
        velocityY += forceY / mass;
    };

    void move(){
        x += velocityX;
        y += velocityY;
    };

    void draw();
};

