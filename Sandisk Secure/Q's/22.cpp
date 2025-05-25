#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <math.h>
#include <dos.h>

#define PI 3.14159265
#define MAX_ITERATIONS 3

// Structure for points
struct Point {
    int x, y;
};

// Global variables
int currentIteration = 0;
int lineColor = WHITE;

// Function to draw a line
void drawLine(int x1, int y1, int x2, int y2) {
    setcolor(lineColor);
    line(x1, y1, x2, y2);
    delay(50);  // Show line drawing progression
}

// Function to calculate Koch curve points
void kochCurve(int x1, int y1, int x2, int y2, int iteration) {
    float angle = 60 * PI / 180;  // 60 degrees in radians
    
    // Calculate the length of line segment
    int dx = x2 - x1;
    int dy = y2 - y1;
    float length = sqrt(dx*dx + dy*dy);
    
    // Base case: if iteration is 0, draw the line
    if(iteration == 0) {
        drawLine(x1, y1, x2, y2);
        return;
    }
    
    // Calculate points that divide line into three parts
    float x3 = x1 + dx/3;
    float y3 = y1 + dy/3;
    float x4 = x1 + dx*2/3;
    float y4 = y1 + dy*2/3;
    
    // Calculate the peak point of the equilateral triangle
    float x5 = x3 + (x4-x3)*cos(angle) + (y4-y3)*sin(angle);
    float y5 = y3 - (x4-x3)*sin(angle) + (y4-y3)*cos(angle);
    
    // Recursive calls for each segment
    kochCurve(x1, y1, x3, y3, iteration-1);    // First segment
    kochCurve(x3, y3, x5, y5, iteration-1);    // Second segment
    kochCurve(x5, y5, x4, y4, iteration-1);    // Third segment
    kochCurve(x4, y4, x2, y2, iteration-1);    // Fourth segment
}

// Function to draw complete Koch curve for an iteration
void drawKochCurve(int iteration) {
    char msg[50];
    int startX = 100;
    int startY = 300;
    int endX = 500;
    int endY = 300;
    
    cleardevice();
    sprintf(msg, "Koch Curve - Iteration %d", iteration);
    outtextxy(10, 10, msg);
    
    // Change color for each iteration
    switch(iteration) {
        case 0: lineColor = WHITE; break;
        case 1: lineColor = YELLOW; break;
        case 2: lineColor = GREEN; break;
        case 3: lineColor = CYAN; break;
    }
    
    kochCurve(startX, startY, endX, endY, iteration);
}

int main() {
    int gd = DETECT, gm;
    int i;
    
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
    
    // Show initial message
    outtextxy(10, 10, "Koch Curve Generation");
    outtextxy(10, 30, "Press any key to start...");
    getch();
    
    // Draw Koch curve for each iteration
    for(i = 0; i <= MAX_ITERATIONS; i++) {
        drawKochCurve(i);
        
        // Show message for next iteration
        if(i < MAX_ITERATIONS) {
            outtextxy(10, 450, "Press any key for next iteration...");
        } else {
            outtextxy(10, 450, "Final iteration complete!");
        }
        getch();
    }
    
    outtextxy(10, 470, "Press any key to exit...");
    getch();
    closegraph();
    return 0;
}

/*
PROGRAM EXPLANATION:

1. Algorithm Overview:
   The Koch curve is a fractal curve that starts with a line segment and
   recursively replaces the middle third with two lines forming an equilateral
   triangle.

2. Key Components:

   a) Data Structure:
      - Point structure for coordinates
      - Global variables for iteration tracking
      - Different colors for each iteration

   b) Main Functions:

      kochCurve():
      - Recursive function implementing Koch curve generation
      - Takes start point, end point, and iteration count
      - Calculates new points and makes recursive calls
      
      drawKochCurve():
      - Wrapper function for each iteration
      - Handles screen clearing and color changes
      - Shows iteration progress

3. Algorithm Steps:

   a) For each line segment:
      1. Divide into three equal parts
      2. Replace middle part with two lines forming equilateral triangle
      3. Repeat for each new line segment
      4. Stop when iteration count reaches 0

   b) Color Scheme:
      - Iteration 0: WHITE  (base line)
      - Iteration 1: YELLOW
      - Iteration 2: GREEN
      - Iteration 3: CYAN

4. Program Flow:
   1. Initialize graphics
   2. Start with horizontal line
   3. For each iteration (0-3):
      - Calculate new points
      - Draw segments
      - Show progress
   4. Display final result

5. How to Use:
   1. Run program
   2. Press key to start
   3. Watch each iteration
   4. Press key between iterations
   5. Press key to exit

6. Key Features:
   - Visual feedback during generation
   - Color-coded iterations
   - Step-by-step demonstration
   - Delay for visualization

7. Mathematical Concepts:
   - Trigonometry for point calculation
   - Recursive pattern generation
   - Fractal geometry
   - Equilateral triangle properties

8. Limitations:
   - Maximum 3 iterations
   - Fixed starting line
   - No user input for parameters

9. Possible Enhancements:
   - Add user input for:
     * Starting line position
     * Number of iterations
     * Line colors
   - Add snowflake variation
   - Add animation control
   - Add multiple curve display
*/
