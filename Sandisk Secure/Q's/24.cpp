#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <math.h>
#include <dos.h>

// Constants for animation
#define DELAY 50
#define RADIUS 30
#define STEP 5

// Function to implement Bresenham's line algorithm
void bresenhamLine(int x1, int y1, int x2, int y2, int color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int x = x1;
    int y = y1;
    int x_inc = (x2 > x1) ? 1 : -1;
    int y_inc = (y2 > y1) ? 1 : -1;
    
    setcolor(color);
    
    if(dx > dy) {
        int p = 2*dy - dx;
        for(int i = 0; i <= dx; i++) {
            putpixel(x, y, color);
            if(p >= 0) {
                y += y_inc;
                p += 2*(dy - dx);
            } else {
                p += 2*dy;
            }
            x += x_inc;
        }
    } else {
        int p = 2*dx - dy;
        for(int i = 0; i <= dy; i++) {
            putpixel(x, y, color);
            if(p >= 0) {
                x += x_inc;
                p += 2*(dx - dy);
            } else {
                p += 2*dx;
            }
            y += y_inc;
        }
    }
}

// Function to implement Midpoint circle algorithm
void midpointCircle(int xc, int yc, int r, int color) {
    int x = 0;
    int y = r;
    int p = 1 - r;
    
    setcolor(color);
    
    // Plot initial points in all octants
    while(x <= y) {
        putpixel(xc + x, yc + y, color);
        putpixel(xc + y, yc + x, color);
        putpixel(xc - y, yc + x, color);
        putpixel(xc - x, yc + y, color);
        putpixel(xc - x, yc - y, color);
        putpixel(xc - y, yc - x, color);
        putpixel(xc + y, yc - x, color);
        putpixel(xc + x, yc - y, color);
        
        if(p < 0) {
            p += 2*x + 3;
        } else {
            p += 2*(x - y) + 5;
            y--;
        }
        x++;
    }
}

// Function to animate bouncing ball
void bouncingBall() {
    int x = 100;
    int y = 100;
    int dx = 5;
    int dy = 5;
    int radius = RADIUS;
    int maxX = getmaxx() - radius;
    int maxY = getmaxy() - radius;
    int minX = radius;
    int minY = radius;
    
    while(!kbhit()) {
        // Clear previous ball
        setcolor(BLACK);
        midpointCircle(x, y, radius, BLACK);
        
        // Update position
        x += dx;
        y += dy;
        
        // Check boundaries and bounce
        if(x >= maxX || x <= minX) dx = -dx;
        if(y >= maxY || y <= minY) dy = -dy;
        
        // Draw new ball
        setcolor(WHITE);
        midpointCircle(x, y, radius, WHITE);
        
        delay(DELAY);
    }
}

// Function to animate rotating line
void rotatingLine() {
    int centerX = getmaxx() / 2;
    int centerY = getmaxy() / 2;
    int length = 100;
    float angle = 0;
    
    while(!kbhit()) {
        // Clear previous line
        setcolor(BLACK);
        bresenhamLine(centerX, centerY,
                     centerX + length*cos(angle),
                     centerY + length*sin(angle),
                     BLACK);
        
        // Update angle
        angle += 0.1;
        if(angle >= 2*3.14159) angle = 0;
        
        // Draw new line
        setcolor(WHITE);
        bresenhamLine(centerX, centerY,
                     centerX + length*cos(angle),
                     centerY + length*sin(angle),
                     WHITE);
        
        delay(DELAY);
    }
}

// Function to animate expanding/contracting circle
void pulsingCircle() {
    int centerX = getmaxx() / 2;
    int centerY = getmaxy() / 2;
    int radius = RADIUS;
    int dr = 2;
    int maxRadius = 100;
    int minRadius = 20;
    
    while(!kbhit()) {
        // Clear previous circle
        setcolor(BLACK);
        midpointCircle(centerX, centerY, radius, BLACK);
        
        // Update radius
        radius += dr;
        if(radius >= maxRadius || radius <= minRadius) dr = -dr;
        
        // Draw new circle
        setcolor(WHITE);
        midpointCircle(centerX, centerY, radius, WHITE);
        
        delay(DELAY);
    }
}

int main() {
    int gd = DETECT, gm;
    int choice;
    
    // Initialize graphics
    initgraph(&gd, &gm, "");
    
    if(graphresult() != grOk) {
        initgraph(&gd, &gm, "C:\\TC\\BGI");  // Try common Turbo C path
    }
    
    if(graphresult() != grOk) {
        initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");  // Try another common path
    }
    
    if(graphresult() != grOk) {
        printf("Graphics error! Make sure BGI files are available.\n");
        printf("Press any key to exit...");
        getch();
        return 1;
    }
    
    while(1) {
        cleardevice();
        // Display menu
        setcolor(WHITE);
        outtextxy(10, 10, "Animation Menu:");
        outtextxy(10, 30, "1. Bouncing Ball");
        outtextxy(10, 50, "2. Rotating Line");
        outtextxy(10, 70, "3. Pulsing Circle");
        outtextxy(10, 90, "4. Exit");
        outtextxy(10, 110, "Enter choice (1-4): ");
        
        choice = getch() - '0';
        cleardevice();
        
        switch(choice) {
            case 1:
                outtextxy(10, 10, "Bouncing Ball - Press any key to stop");
                bouncingBall();
                break;
                
            case 2:
                outtextxy(10, 10, "Rotating Line - Press any key to stop");
                rotatingLine();
                break;
                
            case 3:
                outtextxy(10, 10, "Pulsing Circle - Press any key to stop");
                pulsingCircle();
                break;
                
            case 4:
                closegraph();
                return 0;
        }
        
        getch();  // Clear the key press that stopped the animation
    }
    
    return 0;
}

/*
PROGRAM EXPLANATION:

1. Algorithm Overview:
   This program implements three different animations using Bresenham's line
   drawing algorithm and Midpoint circle drawing algorithm.

2. Key Components:

   a) Drawing Algorithms:
      
      bresenhamLine():
      - Implements Bresenham's line algorithm
      - Optimized for all slopes
      - Handles all directions
      
      midpointCircle():
      - Implements Midpoint circle algorithm
      - Uses symmetry of circle
      - Plots points in all octants

   b) Animations:

      bouncingBall():
      - Simulates a bouncing ball
      - Uses midpoint circle algorithm
      - Handles boundary collisions
      
      rotatingLine():
      - Creates rotating line effect
      - Uses Bresenham's line algorithm
      - Implements smooth rotation
      
      pulsingCircle():
      - Creates expanding/contracting circle
      - Uses midpoint circle algorithm
      - Smooth size transitions

3. Animation Techniques:

   a) Movement:
      - Clear previous shape
      - Update position/parameters
      - Draw new shape
      - Add delay for visibility

   b) Collision Detection:
      - Check boundaries
      - Reverse direction on collision
      - Maintain object within screen

4. Program Flow:
   1. Initialize graphics
   2. Display menu
   3. Based on choice:
      - Run selected animation
      - Continue until key press
   4. Return to menu

5. How to Use:
   1. Run program
   2. Select animation (1-4)
   3. Watch animation
   4. Press any key to stop
   5. Select another or exit

6. Key Features:
   - Multiple animations
   - Menu-driven interface
   - Smooth transitions
   - Efficient algorithms

7. Mathematical Concepts:
   - Line drawing algorithm
   - Circle drawing algorithm
   - Trigonometry for rotation
   - Collision physics

8. Limitations:
   - Fixed animation speed
   - Basic collision physics
   - No user parameters
   - Limited animation types

9. Possible Enhancements:
   - Add more animations
   - Add user controls for:
     * Speed
     * Size
     * Colors
   - Add complex paths
   - Add multiple objects
*/
