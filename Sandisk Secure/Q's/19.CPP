#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_POINTS 20

struct Point {
    int x, y;
};

// Clipping window boundaries
int clipLeft = 100, clipTop = 100, clipRight = 300, clipBottom = 300;

void drawClippingWindow() {
    setcolor(RED);
    rectangle(clipLeft, clipTop, clipRight, clipBottom);
}

void drawPolygon(Point poly[], int n, int color) {
    setcolor(color);
    for (int i = 0; i < n; i++) {
        int next = (i + 1) % n;
        line(poly[i].x, poly[i].y, poly[next].x, poly[next].y);
    }
}

int clipPolygon(Point inPoly[], int inSize, Point outPoly[], int edge, char edgeType) {
    Point curr, prev, inter;
    int outCount = 0;

    for (int i = 0; i < inSize; i++) {
        curr = inPoly[i];
        prev = inPoly[(i - 1 + inSize) % inSize];

        int currIn = 0, prevIn = 0;

        switch (edgeType) {
            case 'L': currIn = curr.x >= edge; prevIn = prev.x >= edge; break;
            case 'R': currIn = curr.x <= edge; prevIn = prev.x <= edge; break;
            case 'T': currIn = curr.y >= edge; prevIn = prev.y >= edge; break;
            case 'B': currIn = curr.y <= edge; prevIn = prev.y <= edge; break;
        }

        if (prevIn && currIn) {
            outPoly[outCount++] = curr;
        } else if (prevIn && !currIn) {
            if (edgeType == 'L' || edgeType == 'R') {
                inter.x = edge;
                inter.y = prev.y + (curr.y - prev.y) * (edge - prev.x) / (curr.x - prev.x);
            } else {
                inter.y = edge;
                inter.x = prev.x + (curr.x - prev.x) * (edge - prev.y) / (curr.y - prev.y);
            }
            outPoly[outCount++] = inter;
        } else if (!prevIn && currIn) {
            if (edgeType == 'L' || edgeType == 'R') {
                inter.x = edge;
                inter.y = prev.y + (curr.y - prev.y) * (edge - prev.x) / (curr.x - prev.x);
            } else {
                inter.y = edge;
                inter.x = prev.x + (curr.x - prev.x) * (edge - prev.y) / (curr.y - prev.y);
            }
            outPoly[outCount++] = inter;
            outPoly[outCount++] = curr;
        }
    }

    return outCount;
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");

    Point inputPoly[MAX_POINTS], tempPoly1[MAX_POINTS], tempPoly2[MAX_POINTS];
    int n;

    printf("Enter number of vertices (max %d): ", MAX_POINTS);
    scanf("%d", &n);

    if (n > MAX_POINTS) {
        printf("Too many vertices!\n");
        closegraph();
        return 1;
    }

    for (int i = 0; i < n; i++) {
        printf("Enter vertex %d (x y): ", i + 1);
        scanf("%d %d", &inputPoly[i].x, &inputPoly[i].y);
    }

    cleardevice();
    drawClippingWindow();
    drawPolygon(inputPoly, n, WHITE);

    outtextxy(10, 10, (char *)"Original Polygon (WHITE)");
    outtextxy(10, 30, (char *)"Press any key to clip...");
    getch();

    int size1 = clipPolygon(inputPoly, n, tempPoly1, clipLeft, 'L');
    int size2 = clipPolygon(tempPoly1, size1, tempPoly2, clipRight, 'R');
    int size3 = clipPolygon(tempPoly2, size2, tempPoly1, clipTop, 'T');
    int finalSize = clipPolygon(tempPoly1, size3, tempPoly2, clipBottom, 'B');

    cleardevice();
    drawClippingWindow();

    if (finalSize > 0) {
        drawPolygon(tempPoly2, finalSize, GREEN);
	outtextxy(10, 10, (char *)"Clipped Polygon (GREEN)");
    } else {
        outtextxy(100, 200, (char *)"Polygon completely outside window!");
    }

    outtextxy(10, 400, (char *)"Press any key to exit...");
    getch();
    closegraph();
    return 0;
}