#include <graphics.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>

#define MAX_POINTS 20

// Structure to store x,y coordinates
struct Point {
    int x, y;
};

// Global variables
Point polygon[MAX_POINTS];  // Array to store polygon vertices
int vertices = 0;   // Number of vertices in polygon

// Function to draw the polygon
void drawPolygon() {
    int i;
    setcolor(WHITE);

    // Draw lines between consecutive vertices
    for(i = 0; i < vertices; i++) {
	line(polygon[i].x, polygon[i].y,
	     polygon[(i+1)%vertices].x, polygon[(i+1)%vertices].y);
    }

    // Mark vertices with small circles
    for(i = 0; i < vertices; i++) {
	circle(polygon[i].x, polygon[i].y, 3);
    }
}

// Function to swap two integers
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Edge Fill implementation
void edgeFill() {
    int i, j, k, y;
    int intersectX[MAX_POINTS];
    int minY = 480, maxY = 0;

    // Find minimum and maximum y coordinates
    for(i = 0; i < vertices; i++) {
	if(polygon[i].y < minY) minY = polygon[i].y;
	if(polygon[i].y > maxY) maxY = polygon[i].y;
    }

    // Process each scan line
    for(y = minY; y <= maxY; y++) {
	int count = 0;  // Number of intersections

	// Find intersections with all edges
	for(i = 0; i < vertices; i++) {
	    // Get edge endpoints
	    int x1 = polygon[i].x;
	    int y1 = polygon[i].y;
	    int x2 = polygon[(i+1)%vertices].x;
	    int y2 = polygon[(i+1)%vertices].y;

	    // Check if scan line intersects with edge
	    if((y1 <= y && y < y2) || (y2 <= y && y < y1)) {
		// Calculate x-intersection using line equation
		if(y2 - y1 != 0) {  // Avoid division by zero
		    int x = x1 + (y - y1) * (x2 - x1) / (y2 - y1);
		    intersectX[count++] = x;
		}
	    }
	}

	// Sort intersections from left to right
	for(i = 0; i < count-1; i++) {
	    for(j = 0; j < count-1-i; j++) {
		if(intersectX[j] > intersectX[j+1]) {
		    swap(&intersectX[j], &intersectX[j+1]);
		}
	    }
	}

	// Fill between pairs of intersections
	for(i = 0; i < count-1; i += 2) {
	    for(j = intersectX[i]; j <= intersectX[i+1]; j++) {
		putpixel(j, y, RED);
	    }
	    delay(5);  // Delay to see the filling process
	}
    }
}

// Function to input polygon vertices
void inputPolygon() {
    int i;

    clrscr();
    printf("\nEnter number of vertices (3-10): ");
    scanf("%d", &vertices);

    if(vertices < 3 || vertices > 10) {
	printf("Invalid number! Setting to 3 vertices.\n");
	vertices = 3;
    }

    printf("\nEnter coordinates for each vertex:\n");
    printf("(Keep x between 100-500 and y between 100-350)\n\n");

    for(i = 0; i < vertices; i++) {
	printf("Vertex %d:\n", i+1);
	printf("X coordinate: ");
	scanf("%d", &polygon[i].x);
	printf("Y coordinate: ");
	scanf("%d", &polygon[i].y);

	// Ensure coordinates are within safe bounds
	if(polygon[i].x < 100) polygon[i].x = 100;
	if(polygon[i].x > 500) polygon[i].x = 500;
	if(polygon[i].y < 100) polygon[i].y = 100;
	if(polygon[i].y > 350) polygon[i].y = 350;
    }

    printf("\nPolygon created successfully! Press any key to continue...");
    getch();
}

int main() {
    int gd = DETECT, gm;

    // Initialize graphics
    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");

    if(graphresult() != grOk) {
        printf("Graphics error! Make sure BGI files are available.");
        getch();
        return 1;
    }
    
    // Clear screen
    cleardevice();
    
    // Input polygon vertices
    inputPolygon();
    
    // Draw the polygon
    drawPolygon();
    
    // Display message
    setcolor(WHITE);
    outtextxy(10, 10, "Starting edge fill...");
    
    // Perform edge fill
    edgeFill();
    
    // Display completion message
    outtextxy(10, 450, "Edge fill complete! Press any key to exit...");
    
    // Wait for key press
    getch();
    closegraph();
    return 0;
}

/*
PROGRAM EXPLANATION:

1. Algorithm Overview:
   Edge Fill (Scan Line) is a polygon filling algorithm that works by:
   - Finding intersections of scan lines with polygon edges
   - Sorting intersections from left to right
   - Filling between pairs of intersections

2. Key Components:

   a) Data Structures:
      - Point structure: Stores x,y coordinates
      - polygon[] array: Stores vertices
      - intersectX[] array: Stores x-intersections for each scan line

   b) Main Functions:

      edgeFill():
      - Core algorithm implementation
      - Steps:
        1. Find min and max y-coordinates
        2. For each scan line y:
           * Find intersections with all edges
           * Sort intersections
           * Fill between intersection pairs
      
      drawPolygon():
      - Draws polygon outline in WHITE
      - Marks vertices with circles
      
      inputPolygon():
      - Handles user input for vertices
      - Validates coordinate ranges

3. Algorithm Steps in Detail:

   a) For each scan line y:
      - Initialize intersection count to 0
      - For each polygon edge:
        * Check if scan line intersects edge
        * If yes, calculate x-intersection
        * Store x-intersection
      - Sort x-intersections
      - Fill pixels between intersection pairs

   b) Intersection calculation:
      - Uses the line equation:
        x = x1 + (y - y1)(x2 - x1)/(y2 - y1)
      - Handles special cases (horizontal edges)

4. Program Flow:
   1. Initialize graphics mode
   2. Input polygon vertices
   3. Draw polygon outline
   4. Perform edge fill
   5. Display completion message

5. How to Use:
   1. Run the program
   2. Enter number of vertices (3-10)
   3. Enter coordinates for each vertex
      - Keep x between 100-500
      - Keep y between 100-350
   4. Watch the filling process
   5. Press any key to exit

6. Key Features:
   - Efficient scan line algorithm
   - Visual feedback during filling
   - Input validation
   - Coordinate bounds checking
   - Simple and educational implementation

7. Advantages over Other Fill Algorithms:
   - More efficient than flood fill
   - No recursion (no stack overflow)
   - Works well for complex polygons
   - Systematic filling approach

8. Limitations:
   - Only works for simple polygons
   - Requires vertices in order
   - No pattern filling (solid color only)

9. Possible Enhancements:
   - Add pattern filling
   - Support multiple polygons
   - Add color selection
   - Handle concave polygons
*/
