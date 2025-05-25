#include <graphics.h>
#include <iostream.h>
#include <conio.h>

void reflectAboutXAxis(int *x, int *y, int centerX, int centerY) {
    int relativeY = *y - centerY;
    relativeY = -relativeY;
    *y = centerY + relativeY;
}

void reflectAboutYAxis(int *x, int *y, int centerX, int centerY) {
    int relativeX = *x - centerX;
    relativeX = -relativeX;
    *x = centerX + relativeX;
}

void drawPolygon(int *x, int *y, int n, int color) {
    setcolor(color);
    int i;
    for (i = 0; i < n - 1; i++) {
        line(x[i], y[i], x[i + 1], y[i + 1]);
    }
    line(x[n - 1], y[n - 1], x[0], y[0]);
}

void copyCoordinates(int *srcX, int *srcY, int *destX, int *destY, int n) {
    int i;
    for (i = 0; i < n; i++) {
        destX[i] = srcX[i];
        destY[i] = srcY[i];
    }
}

void main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");

    int maxX = getmaxx();
    int maxY = getmaxy();
    int centerX = maxX / 2;
    int centerY = maxY / 2;

    setcolor(WHITE);
    setlinestyle(SOLID_LINE, 0, 2);
    line(0, centerY, maxX, centerY);
    line(centerX, 0, centerX, maxY);

    setcolor(YELLOW);
    outtextxy(centerX + 10, 20, "Q1 (+,+)");
    outtextxy(20, 20, "Q2 (-,+)");
    outtextxy(20, centerY + 20, "Q3 (-,-)");
    outtextxy(centerX + 10, centerY + 20, "Q4 (+,-)");
    outtextxy(maxX - 20, centerY + 10, "X");
    outtextxy(centerX + 10, 10, "Y");

    int n, i;
    cout << "Enter number of vertices of polygon: ";
    cin >> n;

    int originalX[20], originalY[20];
    int xReflectedX[20], xReflectedY[20];
    int yReflectedX[20], yReflectedY[20];
    int bothReflectedX[20], bothReflectedY[20];

    cout << "Enter vertices (x y):\n";
    for (i = 0; i < n; i++) {
        cout << "Vertex " << i + 1 << ": ";
        cin >> originalX[i] >> originalY[i];
    }

    copyCoordinates(originalX, originalY, xReflectedX, xReflectedY, n);
    copyCoordinates(originalX, originalY, yReflectedX, yReflectedY, n);
    copyCoordinates(originalX, originalY, bothReflectedX, bothReflectedY, n);

    for (i = 0; i < n; i++) {
        reflectAboutXAxis(&xReflectedX[i], &xReflectedY[i], centerX, centerY);
        reflectAboutYAxis(&yReflectedX[i], &yReflectedY[i], centerX, centerY);
        reflectAboutXAxis(&bothReflectedX[i], &bothReflectedY[i], centerX, centerY);
        reflectAboutYAxis(&bothReflectedX[i], &bothReflectedY[i], centerX, centerY);
    }

    drawPolygon(originalX, originalY, n, RED);
    drawPolygon(xReflectedX, xReflectedY, n, GREEN);
    drawPolygon(yReflectedX, yReflectedY, n, BLUE);
    drawPolygon(bothReflectedX, bothReflectedY, n, MAGENTA);

    setcolor(WHITE);
    outtextxy(10, maxY - 80, "Legend:");
    setcolor(RED);
    outtextxy(10, maxY - 65, "RED: Original");
    setcolor(GREEN);
    outtextxy(10, maxY - 50, "GREEN: X-axis reflection");
    setcolor(BLUE);
    outtextxy(10, maxY - 35, "BLUE: Y-axis reflection");
    setcolor(MAGENTA);
    outtextxy(10, maxY - 20, "MAGENTA: Both axes reflection");

    cout << "\nReflection completed!" << endl;
    cout << "Press any key to continue..." << endl;

    getch();
    closegraph();
}
