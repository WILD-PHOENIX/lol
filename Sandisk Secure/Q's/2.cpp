#include <graphics.h>
#include <iostream.h>  // Changed from <iostream>
#include <conio.h>

// Function to plot 8 symmetrical points of the circle
void plotCirclePoints(int xc, int yc, int x, int y) {
    putpixel(xc + x, yc + y, WHITE);
    putpixel(xc - x, yc + y, WHITE);
    putpixel(xc + x, yc - y, WHITE);
    putpixel(xc - x, yc - y, WHITE);
    putpixel(xc + y, yc + x, WHITE);
    putpixel(xc - y, yc + x, WHITE);
    putpixel(xc + y, yc - x, WHITE);
    putpixel(xc - y, yc - x, WHITE);
}

// Bresenham's Circle Drawing Algorithm
void bresenhamCircle(int xc, int yc, int r) {
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    while (x <= y) {
        plotCirclePoints(xc, yc, x, y);
        if (d < 0) {
            d = d + 4 * x + 6;
        } else {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

// Midpoint Circle Drawing Algorithm
void midpointCircle(int xc, int yc, int r) {
    int x = 0;
    int y = r;
    int d = 1 - r;

    while (x <= y) {
        plotCirclePoints(xc, yc, x, y);
        if (d < 0) {
            d = d + 2 * x + 3;
        } else {
            d = d + 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

int main() {
    int gd = DETECT, gm;
    int errorcode;
    int choice;
    int xc, yc, r;

    // Initialize graphics with error checking
    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");  // Empty string for current directory
    
    errorcode = graphresult();
    if (errorcode != grOk) {
        cout << "Graphics error: " << grapherrormsg(errorcode) << endl;
        cout << "Make sure BGI files are in current directory or correct path" << endl;
        getch();
        return 1;
    }

    do {
        cleardevice();
        
        // Display menu on graphics screen
        setcolor(WHITE);
        outtextxy(10, 10, "Circle Drawing Algorithms Menu");
        outtextxy(10, 30, "1. Bresenham Circle Drawing");
        outtextxy(10, 50, "2. Midpoint Circle Drawing"); 
        outtextxy(10, 70, "3. Exit");
        outtextxy(10, 90, "Enter your choice (1-3): ");
        
        // Get choice from user
        choice = getch() - '0';  // Convert char to int
        
        if (choice == 1 || choice == 2) {
            cleardevice();
            outtextxy(10, 10, "Enter center X coordinate: ");
            cin >> xc;
            
            outtextxy(10, 30, "Enter center Y coordinate: ");
            cin >> yc;
            
            outtextxy(10, 50, "Enter radius: ");
            cin >> r;

            cleardevice();

            if (choice == 1) {
                bresenhamCircle(xc, yc, r);
                outtextxy(10, 10, "Bresenham Circle Drawn (Press any key)");
            } else {
                midpointCircle(xc, yc, r);
                outtextxy(10, 10, "Midpoint Circle Drawn (Press any key)");
            }
            getch();
            
        } else if (choice != 3) {
            cleardevice();
            outtextxy(10, 10, "Invalid choice! Press any key to continue");
            getch();
        }
        
    } while (choice != 3);

    closegraph();
    return 0;
}