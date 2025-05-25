#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <dos.h>

#define VERTICES 5
#define FILL_COLOR GREEN

// Structure for points
struct Point {
    int x, y;
};

// Structure for edges
struct Edge {
    int ymin;    // Minimum y-coordinate of edge
    int ymax;    // Maximum y-coordinate of edge
    float xmin;  // X-coordinate corresponding to ymin
    float slopeinv;  // Inverse slope (1/m)
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
        
        // Calculate edge parameters
        if(vertices[i].y != vertices[next].y) {  // Skip horizontal edges
            edges[edgeCount].ymin = (vertices[i].y < vertices[next].y) ? 
                                   vertices[i].y : vertices[next].y;
            edges[edgeCount].ymax = (vertices[i].y > vertices[next].y) ? 
                                   vertices[i].y : vertices[next].y;
            
            if(vertices[i].y < vertices[next].y) {
                edges[edgeCount].xmin = vertices[i].x;
                edges[edgeCount].slopeinv = (float)(vertices[next].x - vertices[i].x) /
                                          (vertices[next].y - vertices[i].y);
            } else {
                edges[edgeCount].xmin = vertices[next].x;
                edges[edgeCount].slopeinv = (float)(vertices[i].x - vertices[next].x) /
                                          (vertices[i].y - vertices[next].y);
            }
            edgeCount++;
        }
    }
}

// Function to sort edges by ymin
void sortEdges() {
    int i, j;
    struct Edge temp;
    
    for(i = 0; i < edgeCount-1; i++) {
        for(j = 0; j < edgeCount-i-1; j++) {
            if(edges[j].ymin > edges[j+1].ymin) {
                temp = edges[j];
                edges[j] = edges[j+1];
                edges[j+1] = temp;
            }
        }
    }
}

// Function to fill polygon using scan line algorithm
void scanLineFill() {
    int ymin = edges[0].ymin;
    int ymax = edges[0].ymax;
    int i, y;
    
    // Find overall ymin and ymax
    for(i = 1; i < edgeCount; i++) {
        if(edges[i].ymin < ymin) ymin = edges[i].ymin;
        if(edges[i].ymax > ymax) ymax = edges[i].ymax;
    }
    
    // Process each scan line
    for(y = ymin; y <= ymax; y++) {
        int activeEdges = 0;
        float xints[VERTICES];  // Array to store x-intersections
        
        // Find intersections with all edges
        for(i = 0; i < edgeCount; i++) {
            if(y >= edges[i].ymin && y < edges[i].ymax) {
                xints[activeEdges++] = edges[i].xmin + 
                                     (y - edges[i].ymin) * edges[i].slopeinv;
            }
        }
        
        // Sort x-intersections
        for(i = 0; i < activeEdges-1; i++) {
            int j;
            for(j = 0; j < activeEdges-i-1; j++) {
                if(xints[j] > xints[j+1]) {
                    float temp = xints[j];
                    xints[j] = xints[j+1];
                    xints[j+1] = temp;
                }
            }
        }
        
        // Fill between pairs of intersections
        for(i = 0; i < activeEdges-1; i += 2) {
            setcolor(FILL_COLOR);
            line(xints[i], y, xints[i+1], y);
            delay(10);  // Show filling process
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
    printf("(Keep coordinates between 50-500)\n\n");
    
    for(i = 0; i < VERTICES; i++) {
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
   The scan line fill algorithm fills a polygon by processing one horizontal
   scan line at a time, finding intersections with edges, and filling between
   pairs of intersections.

2. Key Components:

   a) Data Structures:
      - Point structure: Stores vertex coordinates
      - Edge structure: Stores edge properties:
        * ymin: Minimum y-coordinate
        * ymax: Maximum y-coordinate
        * xmin: X-coordinate at ymin
        * slopeinv: Inverse slope (1/m)

   b) Main Functions:

      initEdgeTable():
      - Initializes edge table
      - Calculates edge parameters
      - Skips horizontal edges
      
      scanLineFill():
      - Main filling function
      - Processes each scan line
      - Finds and sorts intersections
      - Fills between intersections

3. Algorithm Steps:

   a) Initialization:
      1. Get polygon vertices
      2. Create edge table
      3. Sort edges by ymin

   b) For each scan line:
      1. Find active edges
      2. Calculate x-intersections
      3. Sort intersections
      4. Fill between pairs

4. Program Flow:
   1. Initialize graphics
   2. Get five vertices
   3. Draw original polygon
   4. Initialize edge table
   5. Perform scan line fill
   6. Show result

5. How to Use:
   1. Run program
   2. Enter x,y coordinates for 5 vertices
   3. Watch filling process
   4. Press key to exit

6. Key Features:
   - Fixed 5 vertices
   - Visual filling process
   - Edge sorting
   - Intersection calculation

7. Advantages:
   - Efficient algorithm
   - No recursion
   - Works for any polygon
   - Visual feedback

8. Limitations:
   - Fixed vertex count
   - Single fill color
   - No pattern support
   - Fixed scan line spacing

9. Possible Enhancements:
   - Add variable vertex count
   - Add color selection
   - Add pattern filling
   - Add animation control
*/
