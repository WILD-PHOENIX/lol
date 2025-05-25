#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <dos.h>

// Function to draw a polygon given coordinates
void drawPolygon(int x[], int y[], int n, int color) {
    int i;
    setcolor(color);
    for(i = 0; i < n; i++) {
        int next = (i + 1) % n;
        line(x[i], y[i], x[next], y[next]);
    }
}

// Function to display coordinates
void displayCoordinates(int x[], int y[], int n, int startX, int startY, char* title) {
    int i;
    outtextxy(startX, startY, title);
    char temp[50];
    for(i = 0; i < n; i++) {
        sprintf(temp, "Point %c: (%d, %d)", 'A' + i, x[i], y[i]);
        outtextxy(startX, startY + 20 + (i * 15), temp);
    }
}

// Function to perform scaling
void scalePolygon(int x[], int y[], int n, float sx, float sy) {
    int i;
    for(i = 0; i < n; i++) {
        x[i] = (int)(x[i] * sx);
        y[i] = (int)(y[i] * sy);
    }
}

// Function to perform reflection through origin
void reflectThroughOrigin(int x[], int y[], int n) {
    int i;
    for(i = 0; i < n; i++) {
        x[i] = -x[i];
        y[i] = -y[i];
    }
}

// Function to perform translation
void translatePolygon(int x[], int y[], int n, int tx, int ty) {
    int i;
    for(i = 0; i < n; i++) {
        x[i] = x[i] + tx;
        y[i] = y[i] + ty;
    }
}

// Function to convert coordinates for screen display (origin at center)
void convertToScreen(int x[], int y[], int n, int centerX, int centerY) {
    int i;
    for(i = 0; i < n; i++) {
        x[i] = centerX + x[i] * 10;  // Scale up for better visibility
        y[i] = centerY - y[i] * 10;  // Flip Y axis (screen Y increases downward)
    }
}

int main() {
    int gd = DETECT, gm;
    int i;
    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");

    if(graphresult() != grOk) {
        printf("Graphics initialization failed!");
        return 1;
    }

    int centerX = getmaxx() / 2;
    int centerY = getmaxy() / 2;

    // Original polygon coordinates
    int origX[] = {7, 9, 9, 7};
    int origY[] = {3, 3, 5, 5};
    int n = 4;

    // Working copies for transformations
    int workX[4], workY[4];
    int dispX[4], dispY[4];  // For display coordinates

    // Step 1: Display original polygon
    cleardevice();
    setcolor(WHITE);
    outtextxy(10, 10, "POLYGON TRANSFORMATIONS");
    outtextxy(10, 30, "Step 1: Original Polygon");

    for(i = 0; i < n; i++) {
        workX[i] = origX[i];
        workY[i] = origY[i];
        dispX[i] = origX[i];
        dispY[i] = origY[i];
    }

    convertToScreen(dispX, dispY, n, centerX, centerY);
    drawPolygon(dispX, dispY, n, YELLOW);
    displayCoordinates(origX, origY, n, 10, 60, "Original Coordinates:");

    setcolor(DARKGRAY);
    line(0, centerY, getmaxx(), centerY);  // X-axis
    line(centerX, 0, centerX, getmaxy());  // Y-axis

    getch();

    // Step 2: Scaling (Sx = Sy = 2)
    cleardevice();
    setcolor(WHITE);
    outtextxy(10, 10, "Step 2: After Scaling (Sx = Sy = 2)");

    scalePolygon(workX, workY, n, 2.0, 2.0);

    for(i = 0; i < n; i++) {
        dispX[i] = workX[i];
        dispY[i] = workY[i];
    }

    convertToScreen(dispX, dispY, n, centerX, centerY);
    drawPolygon(dispX, dispY, n, GREEN);
    displayCoordinates(workX, workY, n, 10, 40, "After Scaling:");

    setcolor(DARKGRAY);
    line(0, centerY, getmaxx(), centerY);
    line(centerX, 0, centerX, getmaxy());

    getch();

    // Step 3: Reflection through origin
    cleardevice();
    setcolor(WHITE);
    outtextxy(10, 10, "Step 3: After Reflection through Origin");

    reflectThroughOrigin(workX, workY, n);

    for(i = 0; i < n; i++) {
        dispX[i] = workX[i];
        dispY[i] = workY[i];
    }

    convertToScreen(dispX, dispY, n, centerX, centerY);
    drawPolygon(dispX, dispY, n, CYAN);
    displayCoordinates(workX, workY, n, 10, 40, "After Reflection:");

    setcolor(DARKGRAY);
    line(0, centerY, getmaxx(), centerY);
    line(centerX, 0, centerX, getmaxy());

    getch();

    // Step 4: Translation (Tx = Ty = 1)
    cleardevice();
    setcolor(WHITE);
    outtextxy(10, 10, "Step 4: After Translation (Tx = Ty = 1)");

    translatePolygon(workX, workY, n, 1, 1);

    for(i = 0; i < n; i++) {
        dispX[i] = workX[i];
        dispY[i] = workY[i];
    }

    convertToScreen(dispX, dispY, n, centerX, centerY);
    drawPolygon(dispX, dispY, n, MAGENTA);
    displayCoordinates(workX, workY, n, 10, 40, "Final Coordinates:");

    setcolor(DARKGRAY);
    line(0, centerY, getmaxx(), centerY);
    line(centerX, 0, centerX, getmaxy());

    outtextxy(10, 200, "Press any key to see all transformations together...");
    getch();

    // Step 5: Show all transformations
    cleardevice();
    setcolor(WHITE);
    outtextxy(10, 10, "All Transformations Comparison");

    for(i = 0; i < n; i++) {
        dispX[i] = origX[i];
        dispY[i] = origY[i];
    }
    convertToScreen(dispX, dispY, n, centerX, centerY);
    drawPolygon(dispX, dispY, n, YELLOW);

    for(i = 0; i < n; i++) {
        dispX[i] = workX[i];
        dispY[i] = workY[i];
    }
    convertToScreen(dispX, dispY, n, centerX, centerY);
    drawPolygon(dispX, dispY, n, MAGENTA);

    setcolor(DARKGRAY);
    line(0, centerY, getmaxx(), centerY);
    line(centerX, 0, centerX, getmaxy());

    setcolor(YELLOW);
    outtextxy(10, 40, "Yellow: Original Polygon");
    setcolor(MAGENTA);
    outtextxy(10, 55, "Magenta: Final Transformed Polygon");

    setcolor(WHITE);
    outtextxy(10, 80, "Transformations Applied:");
    outtextxy(10, 95, "1. Scaling: Sx = Sy = 2");
    outtextxy(10, 110, "2. Reflection through origin");
    outtextxy(10, 125, "3. Translation: Tx = Ty = 1");

    outtextxy(10, 150, "Press any key to exit...");
    getch();

    closegraph();
    return 0;
}
