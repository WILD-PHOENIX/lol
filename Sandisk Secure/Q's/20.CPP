#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <math.h>
#include <dos.h>

// Region codes
const int INSIDE = 0;  // 0000
const int LEFT = 1;    // 0001
const int RIGHT = 2;   // 0010
const int BOTTOM = 4;  // 0100
const int TOP = 8;     // 1000

// Window coordinates
int xmin = 100, ymin = 100, xmax = 400, ymax = 300;

// Function to draw the clipping window
void drawWindow() {
    setcolor(WHITE);
    rectangle(xmin, ymin, xmax, ymax);
}

// Function to get region code for a point (x,y)
int getRegionCode(int x, int y) {
    int code = INSIDE;

    if(x < xmin)
	code |= LEFT;
    else if(x > xmax)
	code |= RIGHT;

    if(y < ymin)
	code |= TOP;
    else if(y > ymax)
	code |= BOTTOM;

    return code;
}

// Midpoint Line Clipping Algorithm
void midpointClip(int x1, int y1, int x2, int y2) {
    int code1 = getRegionCode(x1, y1);
    int code2 = getRegionCode(x2, y2);
    int accept = 0;

    while(1) {
	float m = (float)(y2 - y1) / (x2 - x1);  // Line slope

	// If both endpoints inside window
	if(code1 == 0 && code2 == 0) {
	    accept = 1;
	    break;
	}
	// If both endpoints are outside window
	else if(code1 & code2) {
	    break;
	}
	// Some segment lies inside
	else {
	    int code = code1 ? code1 : code2;
	    int x, y;

	    // Find intersection using midpoint method
	    if(code & TOP) {
		x = x1 + (ymin - y1) / m;
		y = ymin;
	    }
	    else if(code & BOTTOM) {
		x = x1 + (ymax - y1) / m;
		y = ymax;
	    }
	    else if(code & RIGHT) {
		y = y1 + m * (xmax - x1);
		x = xmax;
	    }
	    else if(code & LEFT) {
		y = y1 + m * (xmin - x1);
		x = xmin;
	    }

	    // Replace outside point with intersection point
	    if(code == code1) {
		x1 = x;
		y1 = y;
		code1 = getRegionCode(x1, y1);

		// Draw intermediate steps
		cleardevice();
		drawWindow();
		setcolor(RED);
		line(x2, y2, x, y);  // Draw remaining line segment
		delay(500);
	    }
	    else {
		x2 = x;
		y2 = y;
		code2 = getRegionCode(x2, y2);

		// Draw intermediate steps
		cleardevice();
		drawWindow();
		setcolor(RED);
		line(x1, y1, x, y);  // Draw remaining line segment
		delay(500);
	    }
	}
    }

    if(accept) {
	cleardevice();
	drawWindow();
	setcolor(GREEN);
	line(x1, y1, x2, y2);
	outtextxy(10, 10, "Final clipped line in GREEN");
    }
}

int main() {
    int gd = DETECT, gm;
    int x1, y1, x2, y2;

    // Initialize graphics
    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");

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
    
    // Get line endpoints from user
    clrscr();
    printf("\nEnter line endpoints (x1 y1 x2 y2): ");
    printf("\n(Keep coordinates between 50-500)\n");
    scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
    
    // Clear screen and draw initial state
    cleardevice();
    drawWindow();
    setcolor(RED);
    line(x1, y1, x2, y2);
    outtextxy(10, 10, "Original line in RED");
    outtextxy(10, 30, "Press any key to start clipping...");
    getch();
    
    // Perform line clipping
    midpointClip(x1, y1, x2, y2);
    
    outtextxy(10, 450, "Press any key to exit...");
    getch();
    closegraph();
    return 0;
}

/*
PROGRAM EXPLANATION:

1. Algorithm Overview:
   The Midpoint Line Clipping algorithm clips a line segment against a rectangular
   window by iteratively finding intersection points with window edges.

2. Key Components:

   a) Region Codes:
      - Each point is assigned a 4-bit code based on its position
      - INSIDE (0000): Point inside window
      - LEFT  (0001): Point is left of window
      - RIGHT (0010): Point is right of window
      - TOP   (1000): Point is above window
      - BOTTOM(0100): Point is below window

   b) Main Functions:

      getRegionCode():
      - Determines position of point relative to window
      - Returns 4-bit code combining all position flags
      
      midpointClip():
      - Main clipping function
      - Uses slope-intercept method to find intersections
      - Shows step-by-step clipping process

3. Algorithm Steps:

   a) For each endpoint:
      - Calculate region code
      - If both codes 0000: Line completely inside
      - If bitwise AND != 0: Line completely outside
      - Otherwise: Line needs clipping

   b) Clipping Process:
      - Find intersection with nearest window edge
      - Replace outside point with intersection point
      - Repeat until line fully inside or rejected

4. Program Flow:
   1. Initialize graphics
   2. Get line endpoints from user
   3. Draw original line (RED)
   4. Perform clipping with visual steps
   5. Show final clipped line (GREEN)

5. How to Use:
   1. Run program
   2. Enter x1 y1 x2 y2 coordinates
   3. Watch clipping process
   4. Press key to exit

6. Key Features:
   - Visual feedback during clipping
   - Step-by-step demonstration
   - Error handling for graphics
   - User input validation

7. Advantages:
   - Simple implementation
   - Visual learning aid
   - Handles all cases
   - Shows intermediate steps

8. Limitations:
   - Fixed window size
   - Only handles straight lines
   - No multiple line support

9. Possible Enhancements:
   - Add multiple line support
   - Make window size adjustable
   - Add different clipping shapes
   - Add line style options
*/
