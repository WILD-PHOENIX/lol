#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include <stdlib.h>

#define MAX_POINTS 10   // Maximum number of vertices for the polygon

// Function to initialize graphics mode
void initGraphics() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");
    cleardevice();
}

// Function to draw a polygon given its vertices
void drawPolygon(int x[], int y[], int n) {
    int i;
    
    // Set drawing color to white
    setcolor(WHITE);
    
    // Draw lines connecting consecutive vertices
    for(i = 0; i < n; i++) {
        line(x[i], y[i], x[(i+1)%n], y[(i+1)%n]);
    }
}

// Function to draw interface layout
void drawInterface() {
    // Draw a dividing line between text area and drawing area
    setcolor(DARKGRAY);
    line(200, 0, 200, getmaxy());
    
    // Label areas
    setcolor(WHITE);
    outtextxy(10, 10, "FLOOD FILL ALGORITHM");
    outtextxy(250, 10, "DRAWING AREA");
}

// Iterative Flood Fill using a stack (to avoid stack overflow for large areas)
void floodFill(int x, int y, int fillColor, int borderColor) {
    // Simple coordinate stack
    #define STACK_SIZE 10000
    int stack_x[STACK_SIZE];
    int stack_y[STACK_SIZE];
    int stack_pointer = 0;
    
    // Push the starting pixel onto the stack
    stack_x[stack_pointer] = x;
    stack_y[stack_pointer] = y;
    stack_pointer++;
    
    while(stack_pointer > 0) {
        // Pop a pixel from the stack
        stack_pointer--;
        x = stack_x[stack_pointer];
        y = stack_y[stack_pointer];
        
        // If pixel is within valid range
        if(x >= 0 && x < getmaxx() && y >= 0 && y < getmaxy()) {
            int current = getpixel(x, y);
            
            // If current pixel is not the border color and not already filled
            if(current != borderColor && current != fillColor) {
                // Fill the current pixel
                putpixel(x, y, fillColor);
                
                // Push the four adjacent pixels onto the stack
                if(stack_pointer < STACK_SIZE - 4) {
                    stack_x[stack_pointer] = x+1;
                    stack_y[stack_pointer] = y;
                    stack_pointer++;
                    
                    stack_x[stack_pointer] = x-1;
                    stack_y[stack_pointer] = y;
                    stack_pointer++;
                    
                    stack_x[stack_pointer] = x;
                    stack_y[stack_pointer] = y+1;
                    stack_pointer++;
                    
                    stack_x[stack_pointer] = x;
                    stack_y[stack_pointer] = y-1;
                    stack_pointer++;
                }
            }
        }
    }
}

// Function to get user input for vertices
int getVertices(int x[], int y[]) {
    int n, i;
    char str[80];
    
    // Draw interface
    drawInterface();
    
    // Display instructions in the text area
    setcolor(WHITE);
    outtextxy(10, 40, "Enter number of vertices (3-10):");
    
    // Get number of vertices
    gotoxy(10, 4); // Position cursor for text input
    scanf("%d", &n);
    
    // Validate input
    if(n < 3 || n > MAX_POINTS) {
        outtextxy(10, 70, "Invalid number! Press any key...");
        getch();
        return 0;
    }
    
    // Clear screen and redraw interface
    cleardevice();
    drawInterface();
    
    sprintf(str, "Enter coordinates for %d vertices:", n);
    outtextxy(10, 40, str);
    outtextxy(10, 60, "Format: x y (e.g., 300 150)");
    outtextxy(10, 80, "Keep x > 200 to stay in drawing area");
    
    // Get coordinates for each vertex
    for(i = 0; i < n; i++) {
        sprintf(str, "Vertex %d:", i+1);
        outtextxy(10, 100 + i*20, str);
        gotoxy(20, 7 + i); // Position cursor for text input
        scanf("%d %d", &x[i], &y[i]);
        
        // Ensure vertex is in drawing area
        if(x[i] < 200) x[i] = 200 + (x[i] % 200);
        
        // Draw point to show vertex
        setcolor(YELLOW);
        circle(x[i], y[i], 3);
        
        // Connect to previous vertex if not the first one
        if(i > 0) {
            setcolor(WHITE);
            line(x[i-1], y[i-1], x[i], y[i]);
        }
    }
    
    // Connect the last vertex to the first vertex
    setcolor(WHITE);
    line(x[n-1], y[n-1], x[0], y[0]);
    
    return n;
}

// Function to get interior point from user
void getInteriorPoint(int *x, int *y) {
    char str[80];
    
    // Ask for seed point
    setcolor(WHITE);
    outtextxy(10, 280, "Enter a point inside the polygon:");
    outtextxy(10, 300, "Format: x y (e.g., 300 150)");
    
    gotoxy(10, 20); // Position cursor for text input
    scanf("%d %d", x, y);
    
    // Ensure point is in drawing area
    if(*x < 200) *x = 200 + (*x % 200);
    
    // Mark the seed point
    setcolor(LIGHTCYAN);
    circle(*x, *y, 5);
}

// Function to display vertex information
void displayVertexInfo(int x[], int y[], int n) {
    int i;
    char str[80];
    
    setcolor(LIGHTGRAY);
    for(i = 0; i < n; i++) {
        sprintf(str, "Vertex %d: (%d, %d)", i+1, x[i], y[i]);
        outtextxy(10, 100 + i*15, str);
    }
}

int main() {
    int xpoints[MAX_POINTS];  // Array to store x-coordinates
    int ypoints[MAX_POINTS];  // Array to store y-coordinates
    int n = 0;                // Number of vertices
    int seedX, seedY;         // Seed point for flood fill
    
    // Initialize graphics
    initGraphics();
    
    // Get vertices from user
    n = getVertices(xpoints, ypoints);
    
    if(n > 0) {
        // Get seed point for flood fill
        getInteriorPoint(&seedX, &seedY);
        
        // Provide instructions for filling
        setcolor(WHITE);
        outtextxy(10, 320, "Press any key to start flood fill...");
        getch();
        
        // Apply flood fill algorithm
        floodFill(seedX, seedY, RED, WHITE);
        
        // Display completion message
        setcolor(WHITE);
        outtextxy(10, 340, "Polygon filled! Press any key to exit...");
    }
    
    // Wait for key press
    getch();
    closegraph();
    return 0;
}

/*******************************************************************************
 * PROGRAM EXPLANATION
 *******************************************************************************
 *
 * This program implements the Flood Fill algorithm to fill a convex polygon
 * with a specified color. The user defines the polygon by entering the vertex
 * coordinates and specifies a seed point inside the polygon.
 *
 * WHAT IS THE FLOOD FILL ALGORITHM?
 * ---------------------------------
 * Flood Fill is a filling algorithm that determines the area connected to a given
 * node in a multi-dimensional array. It's similar to the "bucket fill" tool in
 * painting programs. Starting from a seed point inside a boundary, it fills all
 * adjacent pixels of the same color until it reaches the boundary.
 *
 * HOW THE ALGORITHM WORKS:
 * ------------------------
 * 1. Start with a seed point inside the polygon
 * 2. Check if the current pixel is not the boundary color and not already filled
 * 3. If true, fill the current pixel
 * 4. Add the four adjacent pixels (up, down, left, right) to the stack
 * 5. Continue processing pixels from the stack until the stack is empty
 *
 * This implementation uses an iterative approach with a stack data structure to
 * avoid the stack overflow issues that can occur with recursive implementations
 * for large areas.
 *
 * USER INTERFACE:
 * --------------
 * The program divides the screen into two areas:
 * - Left side: Text area for instructions and input
 * - Right side: Drawing area for the polygon and fill
 *
 * This separation ensures that the text and drawing don't overlap, making the
 * program more user-friendly.
 *
 * USER INTERACTION:
 * ----------------
 * 1. The user enters the number of vertices (3-10)
 * 2. For each vertex, the user enters x and y coordinates
 * 3. The program draws the polygon as the vertices are entered
 * 4. The user enters a seed point inside the polygon
 * 5. The program fills the polygon using the flood fill algorithm
 *
 * ALGORITHM DETAILS:
 * -----------------
 * This program uses a 4-connected flood fill algorithm, meaning it only fills
 * in the four cardinal directions (up, down, left, right) from each pixel.
 * The algorithm uses a stack to keep track of pixels to be processed, which
 * avoids the stack overflow issues of recursive implementations.
 *
 * The key steps in the flood fill function are:
 * 1. Initialize a stack with the seed point
 * 2. While the stack is not empty:
 *    a. Pop a pixel from the stack
 *    b. If the pixel is not the boundary color and not already filled:
 *       i. Fill the pixel
 *       ii. Push the four adjacent pixels onto the stack
 *
 * APPLICATIONS IN COMPUTER GRAPHICS:
 * ---------------------------------
 * Flood Fill algorithms have many applications:
 *
 * 1. Paint programs (bucket fill tool)
 * 2. Game development (filling areas, map generation)
 * 3. Image processing (segmentation, region coloring)
 * 4. Maze solving
 * 5. Connected component labeling
 *
 * This program provides a practical demonstration of the Flood Fill algorithm,
 * showing how it can be used to fill a user-defined polygon in computer graphics.
 ******************************************************************************/
