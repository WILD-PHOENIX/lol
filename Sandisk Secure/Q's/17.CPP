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

// Function to draw a rectangle given its coordinates
void drawRectangle(int points[][2], int color, char* label) {
    int i;
    int midX = getmaxx() / 2;
    int midY = getmaxy() / 2;
    int centerX = 0, centerY = 0;
    
    setcolor(color);
    
    // Draw the four sides of the rectangle
    for(i = 0; i < 4; i++) {
        int x1 = midX + points[i][0] * 10;     // Scale for better visibility
        int y1 = midY - points[i][1] * 10;     // Invert Y for screen coordinates
        int x2 = midX + points[(i+1)%4][0] * 10;
        int y2 = midY - points[(i+1)%4][1] * 10;
        line(x1, y1, x2, y2);
        
        // Sum coordinates for label placement
        centerX += x1;
        centerY += y1;
    }
    
    // Place label near the center of the rectangle
    if(label != NULL) {
        outtextxy(centerX/4, centerY/4, label);
    }
}

// Function to perform X-shearing
void xShear(int original[][2], int result[][2], int n, float shx) {
    int i;
    for(i = 0; i < n; i++) {
        result[i][0] = original[i][0] + shx * original[i][1];
        result[i][1] = original[i][1];
    }
}

// Function to perform Y-shearing
void yShear(int original[][2], int result[][2], int n, float shy) {
    int i;
    for(i = 0; i < n; i++) {
        result[i][0] = original[i][0];
        result[i][1] = original[i][1] + shy * original[i][0];
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
    outtextxy(10, 10, "2D TRANSFORMATIONS: SHEARING AND ROTATION");
    outtextxy(10, 30, "GREEN: Original Rectangle");
    outtextxy(10, 45, "RED: After X-Shearing (shx = 0.5)");
    outtextxy(10, 60, "BLUE: After Y-Shearing (shy = 0.5)");
    outtextxy(10, 75, "YELLOW: After Rotation (45 degrees)");
    outtextxy(10, getmaxy() - 20, "Press any key to exit...");
}

int main() {
    // Original rectangle coordinates
    int original[4][2] = {
        {2, 2},    // Point A (top-left)
        {8, 2},    // Point B (top-right)
        {8, 6},    // Point C (bottom-right)
        {2, 6}     // Point D (bottom-left)
    };
    
    // Arrays to store transformed coordinates
    int xSheared[4][2];
    int ySheared[4][2];
    int rotated[4][2];
    
    // Initialize graphics
    initGraphics();
    
    // Draw coordinate axes and quadrants
    drawQuadrants();
    
    // Display information
    displayInfo();
    
    // Draw original rectangle
    drawRectangle(original, GREEN, "Orig");
    
    // Perform X-shearing (shx = 0.5)
    xShear(original, xSheared, 4, 0.5);
    drawRectangle(xSheared, RED, "X-Shear");
    
    // Perform Y-shearing (shy = 0.5)
    yShear(original, ySheared, 4, 0.5);
    drawRectangle(ySheared, BLUE, "Y-Shear");
    
    // Perform rotation (45 degrees around origin)
    rotate(original, rotated, 4, 45);
    drawRectangle(rotated, YELLOW, "Rotated");
    
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
 * 1. Shearing - distorting an object along one axis
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
 * We use a simple rectangle with four points:
 * - Point A (2, 2)   - top-left
 * - Point B (8, 2)   - top-right
 * - Point C (8, 6)   - bottom-right
 * - Point D (2, 6)   - bottom-left
 * The original rectangle is drawn in GREEN in the first quadrant.
 *
 * TRANSFORMATIONS APPLIED:
 *
 * 1. X-SHEARING:
 *    - The rectangle is sheared along the X-axis with a shear factor of 0.5
 *    - The formula used is: (x', y') = (x + shx * y, y)
 *    - Where shx is the shear factor in the X direction
 *    - This slants the rectangle to the right, with the top edge moving more than
 *      the bottom edge
 *    - The X-sheared rectangle is shown in RED
 *
 * 2. Y-SHEARING:
 *    - The rectangle is sheared along the Y-axis with a shear factor of 0.5
 *    - The formula used is: (x', y') = (x, y + shy * x)
 *    - Where shy is the shear factor in the Y direction
 *    - This slants the rectangle upward, with the right edge moving more than
 *      the left edge
 *    - The Y-sheared rectangle is shown in BLUE
 *
 * 3. ROTATION:
 *    - The original rectangle is rotated by 45 degrees around the origin
 *    - The rotation formulas used are:
 *      x' = x*cos(θ) - y*sin(θ)
 *      y' = x*sin(θ) + y*cos(θ)
 *    - Where θ is the angle of rotation in radians
 *    - The rotated rectangle is shown in YELLOW
 *
 * MATHEMATICAL BACKGROUND:
 * 
 * Shearing can be represented as matrix multiplications:
 *
 * X-shear:
 * [x']   [1  shx] [x]
 * [y'] = [0   1 ] [y]
 *
 * Y-shear:
 * [x']   [1   0 ] [x]
 * [y'] = [shy 1 ] [y]
 *
 * Rotation in 2D can be represented as a matrix multiplication:
 * [x']   [cos(θ) -sin(θ)] [x]
 * [y'] = [sin(θ)  cos(θ)] [y]
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
 * 1. Shearing is used for:
 *    - Creating perspective effects
 *    - Simulating 3D objects in 2D
 *    - Special visual effects like "italicizing" text
 *    - Deforming objects for animations
 *
 * 2. Rotation is used for:
 *    - Character and object orientation
 *    - Camera movement
 *    - Creating special effects like spinning objects
 *    - Aiming mechanics in games
 *
 * This program provides a visual demonstration of how shearing and rotation
 * transformations affect objects in 2D space, which is a fundamental concept
 * in computer graphics and game development.
 ******************************************************************************/
