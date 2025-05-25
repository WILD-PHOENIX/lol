#include <graphics.h>
#include <conio.h>
#include <iostream.h>  // Changed from <iostream>
#include <math.h>
#include <stdlib.h>
#include <dos.h>       // Added for delay() function

// Function prototypes - Required for Turbo C++
void drawLineDDA(int x1, int y1, int x2, int y2);
void drawLineBresenham(int x1, int y1, int x2, int y2);

// DDA Line Drawing Algorithm
void drawLineDDA(int x1, int y1, int x2, int y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);

    float xInc = dx / steps;
    float yInc = dy / steps;

    float x = x1;
    float y = y1;

    putpixel((int)(x + 0.5), (int)(y + 0.5), WHITE);

    for (int i = 0; i < steps; i++) {
        x += xInc;
        y += yInc;
        putpixel((int)(x + 0.5), (int)(y + 0.5), WHITE);
        delay(10);
    }
}

// Bresenham's Line Drawing Algorithm
void drawLineBresenham(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;

    while (1) {
        putpixel(x1, y1, YELLOW);
        if (x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
        delay(10);
    }
}

int main() {
    int gd = DETECT, gm;

    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");  // Updated path for typical Turbo C++ installation

    int x1, y1, x2, y2;
    cout << "Enter coordinates for line (x1 y1 x2 y2): ";
    cin >> x1 >> y1 >> x2 >> y2;

    cleardevice();
    outtextxy(10, 10, "DDA Line (White)");
    drawLineDDA(x1, y1, x2, y2);

    delay(1000);

    outtextxy(10, 30, "Bresenham Line (Yellow)");
    drawLineBresenham(x1, y1 + 100, x2, y2 + 100); // Offset to show both lines

    getch();
    closegraph();
    return 0;
}