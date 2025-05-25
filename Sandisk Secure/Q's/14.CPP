#include <stdio.h>
#include <conio.h>
#include <graphics.h>

// Function to initialize graphics mode
void initGraphics() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");
    cleardevice();
}

// Function to draw coordinate axes and divide screen into four quadrants
void drawQuadrants() {
    int midX = getmaxx() / 2;
    int midY = getmaxy() / 2;
    
    // Draw the axes
    setcolor(WHITE);
    line(0, midY, getmaxx(), midY);    // X-axis
    line(midX, 0, midX, getmaxy());    // Y-axis
    
    // Label the axes
    outtextxy(getmaxx() - 20, midY + 10, "X");
    outtextxy(midX + 10, 10, "Y");
    
    // Label the quadrants
    setcolor(LIGHTGRAY);
    outtextxy(midX + 20, midY + 20, "Q4");
    outtextxy(midX - 50, midY + 20, "Q3");
    outtextxy(midX - 50, midY - 30, "Q2");
    outtextxy(midX + 20, midY - 30, "Q1");
}

// Function to draw a triangle given its coordinates
void drawTriangle(int points[][2], int color) {
    int i;
    int midX = getmaxx() / 2;
    int midY = getmaxy() / 2;
    
    setcolor(color);
    
    // Draw the three sides of the triangle
    for(i = 0; i < 3; i++) {
        int x1 = midX + points[i][0] * 10;     // Scale for better visibility
        int y1 = midY - points[i][1] * 10;     // Invert Y for screen coordinates
        int x2 = midX + points[(i+1)%3][0] * 10;
        int y2 = midY - points[(i+1)%3][1] * 10;
        line(x1, y1, x2, y2);
    }
}

// Function to scale a shape
void scale(int original[][2], int result[][2], int n, float sx, float sy) {
    int i;
    for(i = 0; i < n; i++) {
        result[i][0] = original[i][0] * sx;
        result[i][1] = original[i][1] * sy;
    }
}

// Function to translate a shape
void translate(int original[][2], int result[][2], int n, int tx, int ty) {
    int i;
    for(i = 0; i < n; i++) {
        result[i][0] = original[i][0] + tx;
        result[i][1] = original[i][1] + ty;
    }
}

// Function to display transformation information
void displayInfo(int x, int y) {
    setcolor(WHITE);
    outtextxy(x, y, "2D TRANSFORMATIONS");
    outtextxy(x, y + 15, "---------------");
    outtextxy(x, y + 30, "GREEN: Original Triangle");
    outtextxy(x, y + 45, "RED: After Scaling (2x in X, 1.5x in Y)");
    outtextxy(x, y + 60, "BLUE: After Translation (+5 in X, -5 in Y)");
    outtextxy(x, y + 75, "YELLOW: After Both Transformations");
}

int main() {
    // Original triangle coordinates
    int original[3][2] = {
        {5, 5},    // Point A
        {10, 5},   // Point B
        {7, 10}    // Point C
    };
    
    // Arrays to store transformed coordinates
    int scaled[3][2];
    int translated[3][2];
    int both[3][2];
    
    // Initialize graphics
    initGraphics();
    
    // Draw coordinate axes and quadrants
    drawQuadrants();
    
    // Display information
    displayInfo(10, 10);
    
    // Draw original triangle
    drawTriangle(original, GREEN);
    
    // Perform scaling (2x in X direction, 1.5x in Y direction)
    scale(original, scaled, 3, 2.0, 1.5);
    drawTriangle(scaled, RED);
    
    // Perform translation (+5 in X, -5 in Y)
    translate(original, translated, 3, 5, -5);
    drawTriangle(translated, BLUE);
    
    // Perform both transformations
    // First scale, then translate
    scale(original, both, 3, 2.0, 1.5);
    translate(both, both, 3, 5, -5);
    drawTriangle(both, YELLOW);
    
    // Display extra information
    setcolor(WHITE);
    outtextxy(10, getmaxy() - 20, "Press any key to exit...");
    
    // Wait for key press
    getch();
    closegraph();
    return 0;
}

/*******************************************************************************
 * PROGRAM EXPLANATION
 *******************************************************************************
 *
 * This program demonstrates two fundamental 2D transformations in computer graphics:
 * 1. Scaling - changing the size of an object
 * 2. Translation - moving an object to a new position
 *
 * THE COORDINATE SYSTEM:
 * The program divides the screen into four quadrants by drawing X and Y axes:
 * - Quadrant 1 (Q1): Top-right (positive X, positive Y)
 * - Quadrant 2 (Q2): Top-left (negative X, positive Y)
 * - Quadrant 3 (Q3): Bottom-left (negative X, negative Y)
 * - Quadrant 4 (Q4): Bottom-right (positive X, negative Y)
 *
 * This mimics the Cartesian coordinate system used in mathematics.
 *
 * THE SHAPE:
 * We use a simple triangle with three points:
 * - Point A (5, 5)
 * - Point B (10, 5)
 * - Point C (7, 10)
 * The original triangle is drawn in GREEN in the first quadrant.
 *
 * TRANSFORMATIONS APPLIED:
 *
 * 1. SCALING:
 *    - The triangle is scaled by a factor of 2.0 in the X direction 
 *      and 1.5 in the Y direction
 *    - The formula used is: (x', y') = (x * sx, y * sy)
 *    - Where sx and sy are the scaling factors
 *    - This makes the triangle wider and taller
 *    - The scaled triangle is shown in RED
 *
 * 2. TRANSLATION:
 *    - The original triangle is moved by +5 units in the X direction 
 *      and -5 units in the Y direction
 *    - The formula used is: (x', y') = (x + tx, y + ty)
 *    - Where tx and ty are the translation amounts
 *    - This shifts the triangle right and down
 *    - The translated triangle is shown in BLUE
 *
 * 3. COMBINED TRANSFORMATION:
 *    - The triangle is first scaled and then translated
 *    - This demonstrates how transformations can be combined
 *    - The resulting triangle is shown in YELLOW
 *
 * IMPLEMENTATION DETAILS:
 * - The program uses Borland Graphics Interface (BGI) for visualization
 * - Screen coordinates are different from Cartesian coordinates:
 *   * In screen coordinates, (0,0) is at the top-left corner
 *   * The Y-axis increases downward
 * - To correct this, the program:
 *   * Shifts the origin to the center of the screen
 *   * Scales the coordinates for better visibility (multiplies by 10)
 *   * Inverts the Y-axis (subtracts from midY instead of adding)
 *
 * This program provides a visual demonstration of how scaling and translation
 * transformations affect objects in 2D space, which is a fundamental concept
 * in computer graphics and game development.
 ******************************************************************************/
