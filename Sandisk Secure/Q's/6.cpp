#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <dos.h>

#define MAX_POINTS 10
#define FENCE_COLOR YELLOW
#define FILL_COLOR GREEN
#define BOUNDARY_COLOR WHITE

// Structure for points
struct Point {
    int x, y;
};

// Global variables
struct Point vertices[MAX_POINTS];  // Array to store polygon vertices
int vertexCount = 0;               // Number of vertices

// Function to draw a line
void drawLine(int x1, int y1, int x2, int y2, int color) {
    setcolor(color);
    line(x1, y1, x2, y2);
}

// Function to draw the polygon
void drawPolygon() {
    int i;
    for(i = 0; i < vertexCount; i++) {
        drawLine(vertices[i].x, vertices[i].y,
                vertices[(i+1)%vertexCount].x, vertices[(i+1)%vertexCount].y,
                BOUNDARY_COLOR);
    }
}

// Function to draw horizontal fence lines
void drawFenceLines(int startY, int endY) {
    int y;
    // Draw horizontal fence lines with spacing of 2 pixels
    for(y = startY; y <= endY; y += 2) {
        int intersectCount = 0;
        int intersections[MAX_POINTS];
        int i;
        
        // Find intersections with polygon edges
        for(i = 0; i < vertexCount; i++) {
            int x1 = vertices[i].x;
            int y1 = vertices[i].y;
            int x2 = vertices[(i+1)%vertexCount].x;
            int y2 = vertices[(i+1)%vertexCount].y;
            
            // Check if scan line intersects with edge
            if((y1 <= y && y2 > y) || (y2 <= y && y1 > y)) {
                // Calculate x-intersection using line equation
                int x = x1 + (y - y1) * (x2 - x1) / (y2 - y1);
                intersections[intersectCount++] = x;
            }
        }
        
        // Sort intersections (bubble sort)
        for(i = 0; i < intersectCount-1; i++) {
            int j;
            for(j = 0; j < intersectCount-i-1; j++) {
                if(intersections[j] > intersections[j+1]) {
                    int temp = intersections[j];
                    intersections[j] = intersections[j+1];
                    intersections[j+1] = temp;
                }
            }
        }
        
        // Draw fence lines between pairs of intersections
        for(i = 0; i < intersectCount-1; i += 2) {
            drawLine(intersections[i], y, intersections[i+1], y, FENCE_COLOR);
            delay(10);  // Add delay to show filling process
        }
    }
}

// Function to fill polygon using fence fill
void fenceFill() {
    int minY = vertices[0].y;
    int maxY = vertices[0].y;
    int i;
    
    // Find minimum and maximum y coordinates
    for(i = 1; i < vertexCount; i++) {
        if(vertices[i].y < minY) minY = vertices[i].y;
        if(vertices[i].y > maxY) maxY = vertices[i].y;
    }
    
    // Draw fence lines
    drawFenceLines(minY, maxY);
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
    
    // Get polygon vertices from user
    clrscr();
    printf("\nEnter number of vertices (3-10): ");
    scanf("%d", &vertexCount);
    
    if(vertexCount < 3 || vertexCount > 10) {
        printf("Invalid number of vertices! Setting to 3\n");
        vertexCount = 3;
    }
    
    printf("\nEnter coordinates for each vertex:\n");
    printf("(Keep coordinates between 50-500)\n\n");
    
    for(i = 0; i < vertexCount; i++) {
        printf("Vertex %d:\n", i+1);
        printf("X coordinate: ");
        scanf("%d", &vertices[i].x);
        printf("Y coordinate: ");
        scanf("%d", &vertices[i].y);
    }
    
    // Clear screen and draw initial polygon
    cleardevice();
    drawPolygon();
    
    // Display message
    outtextxy(10, 10, "Original polygon in WHITE");
    outtextxy(10, 30, "Press any key to start fence fill...");
    getch();
    
    // Perform fence fill
    fenceFill();
    
    // Display completion message
    outtextxy(10, 450, "Fence fill complete! Press any key to exit...");
    getch();
    
    closegraph();
    return 0;
}

/*
PROGRAM EXPLANATION:

1. Algorithm Overview:
   The fence fill algorithm fills a polygon by drawing horizontal lines (fences)
   between pairs of intersections with the polygon edges.

2. Key Components:

   a) Data Structure:
      - Point structure for vertices
      - Array to store polygon vertices
      - Array for intersection points

   b) Main Functions:

      drawFenceLines():
      - Core of fence fill algorithm
      - Finds intersections with edges
      - Sorts intersections
      - Draws horizontal lines
      
      fenceFill():
      - Main filling function
      - Finds y-range of polygon
      - Calls drawFenceLines()

3. Algorithm Steps:

   a) For each scan line (horizontal fence):
      1. Find intersections with all edges
      2. Sort intersections from left to right
      3. Draw lines between pairs of intersections

   b) Edge Intersection:
      - Check if scan line crosses edge
      - Calculate x-coordinate of intersection
      - Store in intersections array

4. Program Flow:
   1. Initialize graphics
   2. Get polygon vertices from user
   3. Draw original polygon
   4. Perform fence fill:
      - Calculate y range
      - Draw fence lines
   5. Show result

5. How to Use:
   1. Run program
   2. Enter number of vertices (3-10)
   3. Enter x,y coordinates for each vertex
   4. Watch filling process
   5. Press key to exit

6. Key Features:
   - Interactive vertex input
   - Visual filling process
   - Color coding:
     * WHITE: Polygon boundary
     * YELLOW: Fence lines
   - Delay for visualization

7. Advantages:
   - Simple implementation
   - Visual feedback
   - Works for convex polygons
   - No recursion needed

8. Limitations:
   - Only works for convex polygons
   - Fixed fence spacing
   - No pattern filling
   - Fixed colors

9. Possible Enhancements:
   - Add pattern support
   - Add color selection
   - Add fence spacing control
   - Add concave polygon support
*/
