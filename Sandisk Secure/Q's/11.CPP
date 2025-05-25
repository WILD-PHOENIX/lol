#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include <math.h>

// Function to set the graphics mode
void initGraphics() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");
    cleardevice();
}

// Function to draw coordinate axes
void drawAxes() {
    int midX = getmaxx() / 2;
    int midY = getmaxy() / 2;
    
    setcolor(WHITE);
    
    // Draw X-axis
    line(0, midY, getmaxx(), midY);
    
    // Draw Y-axis
    line(midX, 0, midX, getmaxy());
    
    // Draw quadrant labels
    setcolor(YELLOW);
    outtextxy(midX + 10, midY + 10, "Q4");
    outtextxy(midX - 30, midY + 10, "Q3");
    outtextxy(midX - 30, midY - 30, "Q2");
    outtextxy(midX + 10, midY - 30, "Q1");
    
    // Draw X=Y line
    setcolor(LIGHTCYAN);
    line(0, getmaxy(), getmaxx(), 0);
    outtextxy(midX + 50, midY - 50, "Y=X");
}

// Function to plot a point
void plotPoint(int x, int y, int color) {
    int midX = getmaxx() / 2;
    int midY = getmaxy() / 2;
    
    putpixel(midX + x, midY - y, color);
}

// Function to draw a polygon
void drawPolygon(int points[][2], int n, int color) {
    int i;
    int midX = getmaxx() / 2;
    int midY = getmaxy() / 2;
    
    setcolor(color);
    
    for(i = 0; i < n; i++) {
        int x1 = midX + points[i][0];
        int y1 = midY - points[i][1];
        int x2 = midX + points[(i+1)%n][0];
        int y2 = midY - points[(i+1)%n][1];
        line(x1, y1, x2, y2);
    }
}

// Function to perform reflection about origin
void reflectOrigin(int original[][2], int reflected[][2], int n) {
    int i;
    
    for(i = 0; i < n; i++) {
        reflected[i][0] = -original[i][0];
        reflected[i][1] = -original[i][1];
    }
}

// Function to perform reflection about Y=X
void reflectYX(int original[][2], int reflected[][2], int n) {
    int i;
    
    for(i = 0; i < n; i++) {
        reflected[i][0] = original[i][1];
        reflected[i][1] = original[i][0];
    }
}

int main() {
    int i;
    
    // Original polygon coordinates
    int original[4][2] = {
        {50, 50},
        {100, 50},
        {100, 100},
        {50, 100}
    };
    
    // Arrays to store reflected coordinates
    int reflectedOrigin[4][2];
    int reflectedYX[4][2];
    
    // Initialize graphics
    initGraphics();
    
    // Draw coordinate axes
    drawAxes();
    
    // Draw original polygon
    drawPolygon(original, 4, GREEN);
    outtextxy(getmaxx()/2 + 75, getmaxy()/2 - 75, "Original");
    
    // Perform reflection about origin
    reflectOrigin(original, reflectedOrigin, 4);
    drawPolygon(reflectedOrigin, 4, RED);
    outtextxy(getmaxx()/2 - 120, getmaxy()/2 + 75, "Reflection about Origin");
    
    // Perform reflection about Y=X
    reflectYX(original, reflectedYX, 4);
    drawPolygon(reflectedYX, 4, BLUE);
    outtextxy(getmaxx()/2 - 75, getmaxy()/2 - 120, "Reflection about Y=X");
    
    // Display instructions
    setcolor(WHITE);
    outtextxy(10, 10, "Press any key to exit...");
    
    getch();
    closegraph();
    return 0;
}
