#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include <math.h>

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
void drawTriangle(int points[][2], int color, char* label) {
    int i;
    int midX = getmaxx() / 2;
    int midY = getmaxy() / 2;
    int centerX = 0, centerY = 0;
    
    setcolor(color);
    
    // Draw the three sides of the triangle
    for(i = 0; i < 3; i++) {
        int x1 = midX + points[i][0] * 10;     // Scale for better visibility
        int y1 = midY - points[i][1] * 10;     // Invert Y for screen coordinates
        int x2 = midX + points[(i+1)%3][0] * 10;
        int y2 = midY - points[(i+1)%3][1] * 10;
        line(x1, y1, x2, y2);
        
        // Sum coordinates for label placement
        centerX += x1;
        centerY += y1;
    }
    
    // Place label near the center of the triangle
    if(label != NULL) {
        outtextxy(centerX/3, centerY/3, label);
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

// Function to rotate a shape around the origin
void rotate(int original[][2], int result[][2], int n, float angle_degrees) {
    int i;
    float angle_radians = angle_degrees * 3.14159 / 180.0;
    float cos_val = cos(angle_radians);
    float sin_val = sin(angle_radians);
    
    for(i = 0; i < n; i++) {
        int x = original[i][0];
        int y = original[i][1];
        
        // Apply rotation formula:
        // x' = x*cos(θ) - y*sin(θ)
        // y' = x*sin(θ) + y*cos(θ)
        result[i][0] = x * cos_val - y * sin_val;
        result[i][1] = x * sin_val + y * cos_val;
    }
}

// Function to display transformation information
void displayInfo() {
    setcolor(WHITE);
    outtextxy(10, 10, "2D TRANSFORMATIONS: TRANSLATION AND ROTATION");
    outtextxy(10, 30, "GREEN: Original Triangle");
    outtextxy(10, 45, "RED: After Translation (+5 in X, -5 in Y)");
    outtextxy(10, 60, "BLUE: After Rotation (45 degrees)");
    outtextxy(10, 75, "YELLOW: After Both Transformations");
    outtextxy(10, getmaxy() - 20, "Press any key to exit...");
}

int main() {
    // Original triangle coordinates
    int original[3][2] = {
        {5, 5},    // Point A
        {10, 5},   // Point B
        {7, 10}    // Point C
    };
    
    // Arrays to store transformed coordinates
    int translated[3][2];
    int rotated[3][2];
    int both[3][2];
    
    // Initialize graphics
    initGraphics();
    
    // Draw coordinate axes and quadrants
    drawQuadrants();
    
    // Display information
    displayInfo();
    
    // Draw original triangle
    drawTriangle(original, GREEN, "Orig");
    
    // Perform translation (+5 in X direction, -5 in Y direction)
    translate(original, translated, 3, 5, -5);
    drawTriangle(translated, RED, "Trans");
    
    // Perform rotation (45 degrees around origin)
    rotate(original, rotated, 3, 45);
    drawTriangle(rotated, BLUE, "Rot");
    
    // Perform both transformations
    // First translate, then rotate
    translate(original, both, 3, 5, -5);
    rotate(both, both, 3, 45);
    drawTriangle(both, YELLOW, "Both");
    
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
 * 1. Translation - moving an object to a new position
 * 2. Rotation - rotating an object around a point (in this case, the origin)
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
 * 1. TRANSLATION:
 *    - The triangle is moved by +5 units in the X direction and -5 units in the Y direction
 *    - The formula used is: (x', y') = (x + tx, y + ty)
 *    - Where tx and ty are the translation amounts
 *    - This shifts the triangle right and down
 *    - The translated triangle is shown in RED
 *
 * 2. ROTATION:
 *    - The original triangle is rotated by 45 degrees around the origin
 *    - The rotation formulas used are:
 *      x' = x*cos(θ) - y*sin(θ)
 *      y' = x*sin(θ) + y*cos(θ)
 *    - Where θ is the angle of rotation in radians
 *    - The rotated triangle is shown in BLUE
 *
 * 3. COMBINED TRANSFORMATION:
 *    - The triangle is first translated and then rotated
 *    - This demonstrates how transformations can be combined
 *    - Note that the order of transformations matters! 
 *      (Translating then rotating gives different results than rotating then translating)
 *    - The resulting triangle is shown in YELLOW
 *
 * MATHEMATICAL BACKGROUND:
 * 
 * Translation is a simple addition:
 * [x'] = [x + tx]
 * [y'] = [y + ty]
 *
 * Rotation in 2D can be represented as a matrix multiplication:
 * [x']   [cos(θ) -sin(θ)] [x]
 * [y'] = [sin(θ)  cos(θ)] [y]
 *
 * This is why we use the formulas:
 * x' = x*cos(θ) - y*sin(θ)
 * y' = x*sin(θ) + y*cos(θ)
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
 * - In Turbo C++, angles need to be converted from degrees to radians
 *   since the math functions work with radians
 *   (1 degree = π/180 radians)
 *
 * APPLICATIONS IN COMPUTER GRAPHICS AND GAMING:
 *
 * 1. Translation is used for:
 *    - Moving characters and objects in a game world
 *    - Scrolling backgrounds
 *    - Positioning UI elements
 *
 * 2. Rotation is used for:
 *    - Character and object orientation
 *    - Camera movement
 *    - Creating special effects like spinning objects
 *    - Aiming mechanics in games
 *
 * This program provides a visual demonstration of how translation and rotation
 * transformations affect objects in 2D space, which is a fundamental concept
 * in computer graphics and game development.
 ******************************************************************************/
