#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <emscripten.h>

//number of circles
#define NUM_CIRCLES 500

// Circle Struct
struct Circle
{
    int x;     // x coordinate
    int y;     // y coordinate
    int r;     // radius
    int red;   // amount of red
    int green; // ammount of green
    int blue;  // ammount of blue
};

// Circle Struct
struct CircleAnimnationData
{
    int x;  // x coordinate
    int y;  // y coordinate
    int r;  // radius
    int xv; // x- axis velocity
    int yv; // y - axis velocity
    int xd; // x - axis direction (1 = forward)
    int yd; // y - axis direction (1 = forward)
};

//Circles Variable
struct Circle circles[NUM_CIRCLES];
struct CircleAnimnationData animationData[NUM_CIRCLES];

//Random number generator
int getRand(max)
{
    return (rand() % max);
}

// Init circle data and start render -JS

int main()
{
    // Seed random number generator
    srand(time(NULL));

    // Create circles
    for (int i = 0; i < NUM_CIRCLES; i++)
    {
        // Radius
        int radius = getRand(50);

        // Coordinates
        int x = getRand(1000) + radius;
        int y = getRand(1000) + radius;

        // Colors
        int red = getRand(255);
        int green = getRand(255);
        int blue = getRand(255);

        // Velocity
        int xv = getRand(10);
        int yv = getRand(10);

        // Fill animation data struct -i
        animationData[i].x = x;
        animationData[i].y = y;
        animationData[i].r = radius;
        animationData[i].xv = xv;
        animationData[i].yv = yv;
        animationData[i].xd = 1;
        animationData[i].yd = 1;

        // Fill circle struct -i
        circles[i].x = x;
        circles[i].y = y;
        circles[i].r = radius;
        circles[i].red = red;
        circles[i].green = green;
        circles[i].blue = blue;
    }

    // Start JS Rendering
    EM_ASM({ render($0, $1); }, NUM_CIRCLES * 6, 6);
}

// Return circles to JS

struct Circle *getCircles(int canvasWidth, int canvasHeight)
{
    //update circle data
    for (int i = 0; i < NUM_CIRCLES; i++)
    {
        // Collision Right - set x direction backwards
        if ((animationData[i].x + animationData[i].r) >= canvasWidth)
            animationData[i].xd = 0;

        // Collision Left - set x direction backwards
        if ((animationData[i].x - animationData[i].r) <= 0)
            animationData[i].xd = 1;

        // Collision Top - set y direction backwards
        if ((animationData[i].y - animationData[i].r) <= 0)
            animationData[i].yd = 1;

        // Collision Bottom - set y direction backwards
        if ((animationData[i].y + animationData[i].r) >= canvasHeight)
            animationData[i].yd = 0;

        //Move Circle in specified direction;
        if (animationData[i].xd == 1)
        {
            animationData[i].x += animationData[i].xv;
        }
        else
        {
            animationData[i].x -= animationData[i].xv;
        }
        if (animationData[i].yd == 1)
        {
            animationData[i].y += animationData[i].yv;
        }
        else
        {
            animationData[i].y -= animationData[i].yv;
        }

        //update matching circle
        circles[i].x = animationData[i].x;
        circles[i].y = animationData[i].y;
    }
    //return updated circles
    return circles;
}