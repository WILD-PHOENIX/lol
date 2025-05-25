#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <dos.h>
#include <math.h>

#define MAX_POINTS 20

// Structure to store x,y coordinates
struct Point {
    int x, y;
};

// Global variables
struct Point inVertices[MAX_POINTS];   // Input polygon vertices
struct Point outVertices[MAX_POINTS];  // Output polygon vertices
int inVertexCount = 0;                 // Number of input vertices
int outVertexCount = 0;                // Number of output vertices

// Clipping window coordinates
int xmin = 100, ymin = 100, xmax = 400, ymax = 300;

// Function to draw a polygon
void drawPolygon(struct Point vertices[], int vertexCount, int color) {
    int i;
    setcolor(color);
    
    for(i = 0; i < vertexCount; i++) {
        line(vertices[i].x, vertices[i].y, 
             vertices[(i+1)%vertexCount].x, vertices[(i+1)%vertexCount].y);
    }
}

// Function to draw clipping window
void drawClippingWindow() {
    setcolor(WHITE);
    rectangle(xmin, ymin, xmax, ymax);
}

// Function to input polygon vertices
void inputPolygon() {
    int i;
    
    clrscr();
    printf("\nEnter number of vertices (3-10): ");
    scanf("%d", &inVertexCount);
    
    if(inVertexCount < 3 || inVertexCount > 10) {
        printf("Invalid number! Setting to 3 vertices.\n");
        inVertexCount = 3;
    }
    
    printf("\nEnter coordinates for each vertex:\n");
    printf("(Keep x between 50-500 and y between 50-400)\n\n");
    
    for(i = 0; i < inVertexCount; i++) {
        printf("Vertex %d:\n", i+1);
        printf("X coordinate: ");
        scanf("%d", &inVertices[i].x);  // Fixed: use %d instead of %f
        printf("Y coordinate: ");
        scanf("%d", &inVertices[i].y);  // Fixed: use %d instead of %f
    }
}

// Returns true if point P is on left side of line from A to B
long isInside(struct Point A, struct Point B, struct Point P) {
    long dx = B.x - A.x;
    long dy = B.y - A.y;
    long dpx = P.x - A.x;
    long dpy = P.y - A.y;
    return (dx * dpy - dy * dpx);
}

// Returns intersection point of line from A to B with edge
struct Point getIntersection(struct Point A, struct Point B, struct Point C, struct Point D) {
    struct Point intersection;
    float slope1, slope2, c1, c2;
    float x, y;
    
    // If either line is vertical
    if(B.x == A.x) {
        x = A.x;
        if(D.x != C.x) {
            slope2 = (float)(D.y - C.y) / (D.x - C.x);
            c2 = C.y - slope2 * C.x;
            y = slope2 * x + c2;
        } else {
            y = A.y; // Both lines vertical, use arbitrary point
        }
    }
    else if(D.x == C.x) {
        x = C.x;
        slope1 = (float)(B.y - A.y) / (B.x - A.x);
        c1 = A.y - slope1 * A.x;
        y = slope1 * x + c1;
    }
    else {
        slope1 = (float)(B.y - A.y) / (B.x - A.x);
        slope2 = (float)(D.y - C.y) / (D.x - C.x);
        c1 = A.y - slope1 * A.x;
        c2 = C.y - slope2 * C.x;
        
        if(slope1 != slope2) {
            x = (c2 - c1) / (slope1 - slope2);
            y = slope1 * x + c1;
        } else {
            // Lines are parallel
            x = A.x;
            y = A.y;
        }
    }
    
    intersection.x = (int)(x + 0.5);  // Round to nearest integer
    intersection.y = (int)(y + 0.5);
    return intersection;
}

// Clip polygon against one edge
void clipAgainstLine(struct Point A, struct Point B) {
    struct Point newVertices[MAX_POINTS];
    int newCount = 0;
    int i;
    struct Point current, next;
    long currentSide, nextSide;
    
    if(outVertexCount == 0) return;  // No vertices to clip
    
    for(i = 0; i < outVertexCount; i++) {
        current = outVertices[i];
        next = outVertices[(i + 1) % outVertexCount];
        currentSide = isInside(A, B, current);
        nextSide = isInside(A, B, next);
        
        // If current point is inside
        if(currentSide >= 0) {
            newVertices[newCount++] = current;
            
            // If next point is outside
            if(nextSide < 0) {
                newVertices[newCount++] = getIntersection(current, next, A, B);
            }
        }
        // If current is outside and next is inside
        else if(nextSide >= 0) {
            newVertices[newCount++] = getIntersection(current, next, A, B);
        }
    }
    
    // Update output polygon
    outVertexCount = newCount;
    for(i = 0; i < newCount; i++) {
        outVertices[i] = newVertices[i];
    }
}

// Main clipping function
void sutherlandHodgmanClip() {
    int i;
    struct Point edges[4][2];
    
    // Copy input vertices to output array
    outVertexCount = inVertexCount;
    for(i = 0; i < inVertexCount; i++) {
        outVertices[i] = inVertices[i];
    }
    
    // Create clipping window edges (clockwise order)
    edges[0][0].x = xmin; edges[0][0].y = ymin;
    edges[0][1].x = xmax; edges[0][1].y = ymin;  // Top edge
    
    edges[1][0].x = xmax; edges[1][0].y = ymin;
    edges[1][1].x = xmax; edges[1][1].y = ymax;  // Right edge
    
    edges[2][0].x = xmax; edges[2][0].y = ymax;
    edges[2][1].x = xmin; edges[2][1].y = ymax;  // Bottom edge
    
    edges[3][0].x = xmin; edges[3][0].y = ymax;
    edges[3][1].x = xmin; edges[3][1].y = ymin;  // Left edge
    
    // Show original polygon
    cleardevice();
    drawClippingWindow();
    drawPolygon(inVertices, inVertexCount, RED);
    outtextxy(10, 10, "Press any key to start clipping...");
    getch();
    
    // Clip against each edge
    for(i = 0; i < 4; i++) {
        clipAgainstLine(edges[i][0], edges[i][1]);
        
        // Show intermediate result
        cleardevice();
        drawClippingWindow();
        drawPolygon(inVertices, inVertexCount, RED);
        if(outVertexCount > 0) {
            drawPolygon(outVertices, outVertexCount, GREEN);
        }
        
        // Display which edge we're clipping against
        char message[50];
        char* edgeNames[] = {"Top", "Right", "Bottom", "Left"};
        sprintf(message, "Clipping against %s edge", edgeNames[i]);
        outtextxy(10, 10, message);
        
        delay(1000);
    }
}

int main() {
    int gd = DETECT, gm;
    
    // Try different BGI paths
    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");  // Try current directory first
    
    if(graphresult() != grOk) {
        initgraph(&gd, &gm, "C:\\TC\\BGI");  // Try common Turbo C path
    }
    
    if(graphresult() != grOk) {
        initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");  // Original path
    }
    
    if(graphresult() != grOk) {
        printf("Graphics initialization failed!\n");
        printf("Make sure BGI files are in the correct directory.\n");
        printf("Press any key to continue...");
        getch();
        return 1;
    }
    
    // Input polygon vertices
    inputPolygon();
    
    // Clear screen and draw initial state
    cleardevice();
    drawClippingWindow();
    drawPolygon(inVertices, inVertexCount, RED);
    
    // Display message
    outtextxy(10, 10, "Original polygon in RED, Clipping window in WHITE");
    outtextxy(10, 30, "Press any key to start clipping...");
    getch();
    
    // Perform clipping
    sutherlandHodgmanClip();
    
    // Display final result
    cleardevice();
    drawClippingWindow();
    drawPolygon(inVertices, inVertexCount, RED);
    if(outVertexCount > 0) {
        drawPolygon(outVertices, outVertexCount, GREEN);
        outtextxy(10, 450, "Clipping complete! Original: RED, Clipped: GREEN");
    } else {
        outtextxy(10, 450, "Polygon completely clipped (outside window)");
    }
    outtextxy(10, 470, "Press any key to exit...");
    
    getch();
    closegraph();
    return 0;
}

/*
PROGRAM EXPLANATION:

1. Algorithm Overview:
   Sutherland-Hodgman polygon clipping algorithm clips a polygon against a
   rectangular window by processing each edge of the window sequentially.

2. Key Components:

   a) Data Structures:
      - Point structure: Stores x,y coordinates as floats
      - inVertices[]: Stores input polygon vertices
      - outVertices[]: Stores clipped polygon vertices
      - Clipping window defined by xmin, ymin, xmax, ymax

   b) Main Functions:

      sutherlandHodgmanClip():
      - Main clipping function
      - Clips polygon against each window edge
      - Updates output after each edge clip
      
      clipAgainstLine():
      - Clips polygon against a single line
      - Processes each vertex pair
      - Adds intersection points when needed
      
      isInside():
      - Determines if point is inside (left of) a line
      - Uses cross product method
      
      getIntersection():
      - Calculates intersection of two lines
      - Handles vertical lines specially

3. Algorithm Steps:

   a) For each edge of clipping window:
      - Process each vertex pair of polygon
      - For each vertex:
        * If inside edge: Keep vertex
        * If crossing edge: Add intersection point
        * If outside edge: Skip vertex
      - Update polygon for next edge

   b) Inside/Outside Test:
      - Uses cross product to determine point position
      - Positive: Inside (left of line)
      - Negative: Outside (right of line)
      - Zero: On the line

4. Program Flow:
   1. Initialize graphics
   2. Input polygon vertices
   3. Draw original polygon (RED)
   4. Clip against each window edge
   5. Show clipped polygon (GREEN)

5. How to Use:
   1. Run program
   2. Enter number of vertices (3-10)
   3. Enter coordinates for each vertex
   4. Watch clipping process
   5. Press key to exit

6. Key Features:
   - Visual feedback during clipping
   - Handles all polygon types
   - Shows both original and clipped polygons
   - Step-by-step visualization

7. Advantages:
   - Works for both convex and concave polygons
   - Handles all special cases
   - Efficient algorithm
   - Easy to understand visualization

8. Limitations:
   - Fixed clipping window
   - No support for curves
   - Limited to 2D polygons

9. Possible Enhancements:
   - Add movable clipping window
   - Support different window shapes
   - Add multiple polygon support
   - Add color options
*/
