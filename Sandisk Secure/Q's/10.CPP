#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <stdio.h>

#define PI 3.14159

// Structure for a point
struct Point {
    int x, y;
};

// Rotate a point about origin
struct Point rotateAboutOrigin(struct Point p, float angle) {
    float rad = angle * PI / 180.0;
    struct Point result;
    result.x = (int)(p.x * cos(rad) - p.y * sin(rad));
    result.y = (int)(p.x * sin(rad) + p.y * cos(rad));
    return result;
}

// Rotate a point about a fixed point
struct Point rotateAboutFixed(struct Point p, struct Point fixed, float angle) {
    struct Point translated;
    translated.x = p.x - fixed.x;
    translated.y = p.y - fixed.y;

    struct Point rotated = rotateAboutOrigin(translated, angle);

    rotated.x += fixed.x;
    rotated.y += fixed.y;

    return rotated;
}

// Convert to graphics coordinates (centered screen)
struct Point toGraphics(struct Point p, int cx, int cy) {
    struct Point gp;
    gp.x = cx + p.x;
    gp.y = cy - p.y;
    return gp;
}

// Draw triangle
void drawTriangle(struct Point p1, struct Point p2, struct Point p3, int color, int cx, int cy) {
    struct Point g1 = toGraphics(p1, cx, cy);
    struct Point g2 = toGraphics(p2, cx, cy);
    struct Point g3 = toGraphics(p3, cx, cy);

    setcolor(color);
    line(g1.x, g1.y, g2.x, g2.y);
    line(g2.x, g2.y, g3.x, g3.y);
    line(g3.x, g3.y, g1.x, g1.y);
}

// Draw coordinate axes and quadrants
void drawAxes(int cx, int cy) {
    setcolor(WHITE);
    line(0, cy, getmaxx(), cy); // X-axis
    line(cx, 0, cx, getmaxy()); // Y-axis

    setcolor(YELLOW);
    outtextxy(cx + 10, 10, "Quadrant I");
    outtextxy(10, 10, "Quadrant II");
    outtextxy(10, cy + 10, "Quadrant III");
    outtextxy(cx + 10, cy + 10, "Quadrant IV");

    setcolor(RED);
    circle(cx, cy, 3);
    outtextxy(cx + 5, cy + 5, "Origin");
}

void main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");

    int cx = getmaxx() / 2;
    int cy = getmaxy() / 2;

    struct Point triangle[3] = {{50, 50}, {20, 10}, {80, 10}};
    struct Point fixed = {100, 80};
    float angle = 0;
    char ch;

    drawAxes(cx, cy);
    setcolor(GREEN);
    outtextxy(10, getmaxy() - 40, "Original Triangle - Green");
    drawTriangle(triangle[0], triangle[1], triangle[2], GREEN, cx, cy);

    do {
        ch = getch();
        if (ch == 'r') {
            angle += 30;
            struct Point r1 = rotateAboutOrigin(triangle[0], angle);
            struct Point r2 = rotateAboutOrigin(triangle[1], angle);
            struct Point r3 = rotateAboutOrigin(triangle[2], angle);

            cleardevice();
            drawAxes(cx, cy);
            drawTriangle(triangle[0], triangle[1], triangle[2], GREEN, cx, cy);
            drawTriangle(r1, r2, r3, BLUE, cx, cy);

            setcolor(WHITE);
            outtextxy(10, getmaxy() - 20, "Rotated about Origin - Blue");
        }
        else if (ch == 'f') {
            angle += 30;
            struct Point r1 = rotateAboutFixed(triangle[0], fixed, angle);
            struct Point r2 = rotateAboutFixed(triangle[1], fixed, angle);
            struct Point r3 = rotateAboutFixed(triangle[2], fixed, angle);

            cleardevice();
            drawAxes(cx, cy);
            drawTriangle(triangle[0], triangle[1], triangle[2], GREEN, cx, cy);
            drawTriangle(r1, r2, r3, RED, cx, cy);

            // Mark fixed point
            struct Point fg = toGraphics(fixed, cx, cy);
            setcolor(YELLOW);
            circle(fg.x, fg.y, 3);
            outtextxy(fg.x + 5, fg.y + 5, "Fixed Pt");

            setcolor(WHITE);
            outtextxy(10, getmaxy() - 20, "Rotated about Fixed Point - Red");
        }
        else if (ch == 'c') {
            cleardevice();
            angle = 0;
            drawAxes(cx, cy);
            drawTriangle(triangle[0], triangle[1], triangle[2], GREEN, cx, cy);
            outtextxy(10, getmaxy() - 40, "Cleared. Original Triangle - Green");
        }
    } while (ch != 'q');

    closegraph();
}
