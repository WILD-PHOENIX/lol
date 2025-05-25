#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>
#include <time.h>

#define MAX_ITERATIONS 5
#define DISPLACEMENT 50   // Maximum random displacement

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
}

// Function to get random displacement
int getDisplacement(int iteration) {
    // Displacement decreases with each iteration
    return (rand() % DISPLACEMENT) / (iteration + 1);
}

// Function to draw fractal line
void fractalLine(int x1, int y1, int x2, int y2, int iteration) {
    int midX, midY, displacement;

    // Base case: if iteration is 0, draw the line
    if(iteration == 0) {
	drawLine(x1, y1, x2, y2);
	return;
    }

    // Calculate midpoint
    midX = (x1 + x2) / 2;
    midY = (y1 + y2) / 2;

    // Add random displacement perpendicular to line
    displacement = getDisplacement(iteration);

    // Calculate perpendicular direction
    float dx = x2 - x1;
    float dy = y2 - y1;
    float length = sqrt(dx*dx + dy*dy);

    if(length != 0) {
	// Normalize and rotate 90 degrees
	float nx = -dy / length;
	float ny = dx / length;

	// Apply displacement
	midX += nx * displacement;
	midY += ny * displacement;
    }

    // Recursive calls for both halves
    fractalLine(x1, y1, midX, midY, iteration - 1);
    fractalLine(midX, midY, x2, y2, iteration - 1);
}

// Function to draw complete fractal for an iteration
void drawFractal(int iteration) {
    char msg[50];
    int startX = 100;
    int startY = 300;
    int endX = 500;
    int endY = 300;

    cleardevice();
    sprintf(msg, "Fractal Line - Iteration %d", iteration);
    outtextxy(10, 10, msg);

    // Change color for each iteration
    lineColor = (iteration % 7) + 9;  // Cycle through bright colors

    fractalLine(startX, startY, endX, endY, iteration);
}

int main() {
    int gd = DETECT, gm;
    int i;

    // Initialize random seed
    randomize();

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
    
    // Show initial message
    outtextxy(10, 10, "Fractal Line Generator");
    outtextxy(10, 30, "Press any key to start...");
    getch();
    
    // Draw fractal for each iteration
    for(i = 0; i <= MAX_ITERATIONS; i++) {
        drawFractal(i);
        
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
   The fractal line generator creates a self-similar pattern by recursively
   displacing the midpoint of line segments in a perpendicular direction.

2. Key Components:

   a) Data Structure:
      - Point structure for coordinates
      - Global variables for iteration and color
      - Displacement control constant

   b) Main Functions:

      fractalLine():
      - Recursive function implementing fractal generation
      - Calculates midpoint and applies displacement
      - Makes recursive calls for sub-segments
      
      getDisplacement():
      - Calculates random displacement
      - Scales down with iteration level

3. Algorithm Steps:

   a) For each line segment:
      1. Find midpoint
      2. Calculate perpendicular direction
      3. Add random displacement
      4. Recursively process both halves
      5. Stop when iteration count reaches 0

   b) Displacement Logic:
      - Random value up to DISPLACEMENT
      - Scaled down with each iteration
      - Applied perpendicular to line
      - Creates natural-looking roughness

4. Program Flow:
   1. Initialize graphics and random seed
   2. Start with horizontal line
   3. For each iteration (0-10):
      - Calculate midpoints
      - Apply displacements
      - Draw segments
   4. Show final result

5. How to Use:
   1. Run program
   2. Press key to start
   3. Watch each iteration
   4. Press key between iterations
   5. Press key to exit

6. Key Features:
   - Up to 10 iterations
   - Random displacement
   - Color changes per iteration
   - Progressive complexity

7. Mathematical Concepts:
   - Midpoint calculation
   - Perpendicular vector computation
   - Random number scaling
   - Recursive subdivision

8. Limitations:
   - Fixed starting line
   - Predetermined displacement range
   - No user input for parameters

9. Possible Enhancements:
   - Add user input for:
     * Starting line position
     * Number of iterations
     * Displacement range
   - Add multiple line support
   - Add color customization
   - Add animation speed control
*/
