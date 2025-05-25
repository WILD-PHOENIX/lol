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
    
    // Label axes
    outtextxy(getmaxx() - 10, midY + 10, "X");
    outtextxy(midX + 10, 10, "Y");
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

// Function to perform X-shear transformation
void xShear(int original[][2], int transformed[][2], int n, float shx) {
    int i;
    
    for(i = 0; i < n; i++) {
        transformed[i][0] = original[i][0] + shx * original[i][1];
        transformed[i][1] = original[i][1];
    }
}

// Function to perform Y-shear transformation
void yShear(int original[][2], int transformed[][2], int n, float shy) {
    int i;
    
    for(i = 0; i < n; i++) {
        transformed[i][0] = original[i][0];
        transformed[i][1] = original[i][1] + shy * original[i][0];
    }
}

void displayInfo() {
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(10, 10, "X-SHEAR AND Y-SHEAR TRANSFORMATIONS");
    outtextxy(10, 30, "GREEN: Original Shape");
    outtextxy(10, 50, "RED: X-Shear (shx = 0.5)");
    outtextxy(10, 70, "BLUE: Y-Shear (shy = 0.5)");
    outtextxy(10, getmaxy() - 30, "Press any key to exit...");
}

int main() {
    // Original rectangle coordinates
    int original[4][2] = {
        {50, 50},
        {150, 50},
        {150, 100},
        {50, 100}
    };
    
    // Arrays to store transformed coordinates
    int xSheared[4][2];
    int ySheared[4][2];
    
    float shx = 0.5; // X-shear factor
    float shy = 0.5; // Y-shear factor
    
    // Initialize graphics
    initGraphics();
    
    // Draw coordinate axes
    drawAxes();
    
    // Display information
    displayInfo();
    
    // Draw original polygon
    drawPolygon(original, 4, GREEN);
    
    // Perform X-shear transformation
    xShear(original, xSheared, 4, shx);
    drawPolygon(xSheared, 4, RED);
    
    // Perform Y-shear transformation
    yShear(original, ySheared, 4, shy);
    drawPolygon(ySheared, 4, BLUE);
    
    // Wait for key press
    getch();
    closegraph();
    return 0;
}
