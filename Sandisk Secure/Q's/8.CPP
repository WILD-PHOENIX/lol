#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>

#define VERTICES 5
#define FILL_COLOR GREEN
#define MAX_INTERSECTIONS 10

// Structure for points
struct Point {
    int x, y;
};

// Structure for edges
struct Edge {
    int ymin;    // Minimum y-coordinate of edge
    int ymax;    // Maximum y-coordinate of edge
    float xofymin;  // X-coordinate at ymin
    float slopeinv;  // Inverse slope (dx/dy)
};

// Global variables
struct Point vertices[VERTICES];  // Array to store polygon vertices
struct Edge edges[VERTICES];     // Array to store edges
int edgeCount = 0;              // Number of edges

// Function to draw the polygon
void drawPolygon() {
    int i;
    setcolor(WHITE);
    for(i = 0; i < VERTICES; i++) {
        line(vertices[i].x, vertices[i].y,
             vertices[(i+1)%VERTICES].x, vertices[(i+1)%VERTICES].y);
    }
}

// Function to initialize edge table
void initEdgeTable() {
    int i;
    edgeCount = 0;
    
    for(i = 0; i < VERTICES; i++) {
        int next = (i + 1) % VERTICES;
        
        // Skip horizontal edges
        if(vertices[i].y != vertices[next].y) {
            // Ensure ymin is always the lower endpoint
            if(vertices[i].y < vertices[next].y) {
                edges[edgeCount].ymin = vertices[i].y;
                edges[edgeCount].ymax = vertices[next].y;
                edges[edgeCount].xofymin = vertices[i].x;
                edges[edgeCount].slopeinv = (float)(vertices[next].x - vertices[i].x) /
                                          (vertices[next].y - vertices[i].y);
            } else {
                edges[edgeCount].ymin = vertices[next].y;
                edges[edgeCount].ymax = vertices[i].y;
                edges[edgeCount].xofymin = vertices[next].x;
                edges[edgeCount].slopeinv = (float)(vertices[i].x - vertices[next].x) /
                                          (vertices[i].y - vertices[next].y);
            }
            edgeCount++;
        }
    }
}

// Function to sort edges by ymin and x-coordinate
void sortEdges() {
    int i, j;
    struct Edge temp;
    
    for(i = 0; i < edgeCount-1; i++) {
        for(j = 0; j < edgeCount-i-1; j++) {
            if(edges[j].ymin > edges[j+1].ymin ||
               (edges[j].ymin == edges[j+1].ymin &&
                edges[j].xofymin > edges[j+1].xofymin)) {
                temp = edges[j];
                edges[j] = edges[j+1];
                edges[j+1] = temp;
            }
        }
    }
}

// Function to handle concave polygon filling
void scanLineFill() {
    int ymin = edges[0].ymin;
    int ymax = edges[0].ymax;
    int i, y;
    float *xints;  // Dynamic array for intersections
    
    // Find overall ymin and ymax
    for(i = 1; i < edgeCount; i++) {
        if(edges[i].ymin < ymin) ymin = edges[i].ymin;
        if(edges[i].ymax > ymax) ymax = edges[i].ymax;
    }
    
    // Allocate memory for intersections
    xints = (float *)malloc(sizeof(float) * MAX_INTERSECTIONS);
    
    // Process each scan line
    for(y = ymin; y <= ymax; y++) {
        int count = 0;  // Number of intersections
        
        // Find intersections with all edges
        for(i = 0; i < edgeCount; i++) {
            if(y >= edges[i].ymin && y < edges[i].ymax) {
                xints[count++] = edges[i].xofymin +
                                (y - edges[i].ymin) * edges[i].slopeinv;
            }
        }
        
        // Sort x-intersections
        for(i = 0; i < count-1; i++) {
            int j;
            for(j = 0; j < count-i-1; j++) {
                if(xints[j] > xints[j+1]) {
                    float temp = xints[j];
                    xints[j] = xints[j+1];
                    xints[j+1] = temp;
                }
            }
        }
        
        // Fill between pairs of intersections
        setcolor(FILL_COLOR);
        for(i = 0; i < count-1; i += 2) {
            line(xints[i], y, xints[i+1], y);
            delay(5);  // Show filling process
        }
    }
    
    // Free allocated memory
    free(xints);
}

// Function to check if polygon is properly oriented
void checkOrientation() {
    int area = 0;
    int i;
    
    // Calculate signed area
    for(i = 0; i < VERTICES; i++) {
        int next = (i + 1) % VERTICES;
        area += (vertices[i].x * vertices[next].y -
                vertices[next].x * vertices[i].y);
    }
    
    // If area is negative, reverse vertex order
    if(area < 0) {
        for(i = 0; i < VERTICES/2; i++) {
            struct Point temp = vertices[i];
            vertices[i] = vertices[VERTICES-1-i];
            vertices[VERTICES-1-i] = temp;
        }
    }
}

int main() {
    int gd = DETECT, gm;
    int i;
    
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
    
    // Get polygon vertices from user
    clrscr();
    printf("\nEnter coordinates for 5 vertices:\n");
    printf("(Keep coordinates between 50-500)\n");
    printf("For concave polygon, try making one vertex point inward\n\n");
    
    for(i = 0; i < VERTICES; i++) {
        printf("Vertex %d:\n", i+1);
        printf("X coordinate: ");
        scanf("%d", &vertices[i].x);
        printf("Y coordinate: ");
        scanf("%d", &vertices[i].y);
    }
    
    // Check and fix polygon orientation
    checkOrientation();
    
    // Clear screen and draw initial polygon
    cleardevice();
    drawPolygon();
    
    // Display message
    outtextxy(10, 10, "Original polygon in WHITE");
    outtextxy(10, 30, "Press any key to start scan line fill...");
    getch();
    
    // Initialize edge table and sort edges
    initEdgeTable();
    sortEdges();
    
    // Perform scan line fill
    scanLineFill();
    
    // Display completion message
    outtextxy(10, 450, "Scan line fill complete! Press any key to exit...");
    getch();
    
    closegraph();
    return 0;
}

/*
PROGRAM EXPLANATION:

1. Algorithm Overview:
   This program implements the Scan Line fill algorithm specifically optimized
   for concave polygons, handling multiple intersections per scan line.

2. Key Components:

   a) Data Structures:
      - Point structure: Stores vertex coordinates
      - Edge structure: Enhanced edge information:
        * ymin, ymax: Edge y-range
        * xofymin: X at minimum y
        * slopeinv: Inverse slope for intersection calculation

   b) Main Functions:

      checkOrientation():
      - Ensures polygon vertices are in correct order
      - Calculates signed area to determine orientation
      - Reverses vertices if needed
      
      initEdgeTable():
      - Creates edge table with enhanced information
      - Handles edge cases for concave shapes
      - Properly initializes slope calculations
      
      scanLineFill():
      - Main filling algorithm
      - Handles multiple intersections
      - Dynamic memory for intersection storage
      - Proper sorting and pairing

3. Algorithm Steps:

   a) Preprocessing:
      1. Check and fix polygon orientation
      2. Create edge table
      3. Sort edges by ymin and x-coordinate

   b) Filling Process:
      1. Find active edges for each scan line
      2. Calculate all intersections
      3. Sort intersections
      4. Fill between pairs properly

4. Key Improvements for Concave Polygons:

   a) Orientation Check:
      - Ensures consistent filling
      - Prevents filling errors at concave vertices

   b) Enhanced Edge Processing:
      - Better handling of multiple intersections
      - Proper sorting of intersections
      - Dynamic intersection storage

   c) Robust Filling:
      - Handles complex concave shapes
      - Proper pairing of intersections
      - Visual feedback during filling

5. How to Use:
   1. Run program
   2. Enter 5 vertices:
      - Make one vertex point inward for concave shape
      - Keep coordinates between 50-500
   3. Watch filling process
   4. Press key to exit

6. Key Features:
   - Handles concave polygons
   - Orientation correction
   - Dynamic memory management
   - Visual feedback

7. Advantages:
   - More robust than simple scan line
   - Handles complex shapes
   - Memory efficient
   - User friendly

8. Limitations:
   - Fixed vertex count (5)
   - Single fill color
   - No pattern support
   - Fixed scan line spacing

9. Possible Enhancements:
   - Variable vertex count
   - Multiple fill patterns
   - Animation speed control
   - Interactive vertex editing
*/
