#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <dos.h>
#include <math.h>

// Structure for points
struct Point {
    int x, y;
};

// Global variables
struct Point controlPoints[4];    // 4 control points
int maxIterations = 4;           // Maximum iterations

// Function to draw a point
void drawPoint(int x, int y, int color) {
    setcolor(color);
    circle(x, y, 3);
    setfillstyle(SOLID_FILL, color);
    floodfill(x, y, color);
}

// Function to draw a line
void drawLine(int x1, int y1, int x2, int y2, int color) {
    setcolor(color);
    line(x1, y1, x2, y2);
}

// Function to get midpoint of two points
struct Point getMidpoint(struct Point p1, struct Point p2) {
    struct Point mid;
    mid.x = (p1.x + p2.x) / 2;
    mid.y = (p1.y + p2.y) / 2;
    return mid;
}

// Function to draw control polygon
void drawControlPolygon() {
    int i;
    setcolor(WHITE);
    
    // Draw lines connecting control points
    for(i = 0; i < 3; i++) {
        line(controlPoints[i].x, controlPoints[i].y,
             controlPoints[i+1].x, controlPoints[i+1].y);
    }
    
    // Draw control points
    for(i = 0; i < 4; i++) {
        drawPoint(controlPoints[i].x, controlPoints[i].y, RED);
        
        // Label the points
        char label[5];
        sprintf(label, "P%d", i);
        outtextxy(controlPoints[i].x + 5, controlPoints[i].y - 15, label);
    }
}

// Function to calculate distance between two points (simplified version)
int distance(struct Point p1, struct Point p2) {
    int dx = p1.x - p2.x;
    int dy = p1.y - p2.y;
    return (dx * dx + dy * dy);  // Return squared distance to avoid sqrt
}

// Recursive function to draw Bezier curve using midpoint subdivision
void drawBezierRecursive(struct Point p0, struct Point p1, struct Point p2, struct Point p3, int depth) {
    // Base case: if we've reached maximum depth or curve is flat enough
    if(depth <= 0 || (distance(p0, p3) < 100)) {  // Changed threshold since we use squared distance
        setcolor(GREEN);
        line(p0.x, p0.y, p3.x, p3.y);
        return;
    }
    
    // Calculate midpoints for subdivision
    struct Point p01 = getMidpoint(p0, p1);
    struct Point p12 = getMidpoint(p1, p2);
    struct Point p23 = getMidpoint(p2, p3);
    
    struct Point p012 = getMidpoint(p01, p12);
    struct Point p123 = getMidpoint(p12, p23);
    
    struct Point p0123 = getMidpoint(p012, p123);
    
    // Draw intermediate construction lines (optional - for visualization)
    if(depth == maxIterations) {
        setcolor(YELLOW);
        line(p01.x, p01.y, p12.x, p12.y);
        line(p12.x, p12.y, p23.x, p23.y);
        
        setcolor(BLUE);
        line(p012.x, p012.y, p123.x, p123.y);
        
        drawPoint(p0123.x, p0123.y, CYAN);
        delay(500);
    }
    
    // Recursively draw left and right halves
    drawBezierRecursive(p0, p01, p012, p0123, depth - 1);
    drawBezierRecursive(p0123, p123, p23, p3, depth - 1);
}

// Function to draw Bezier curve with different iterations
void demonstrateBezierCurve() {
    int iter;
    
    for(iter = 1; iter <= maxIterations; iter++) {
        cleardevice();
        
        // Draw control polygon
        drawControlPolygon();
        
        // Show current iteration
        char msg[100];
        sprintf(msg, "Bezier Curve - Iteration %d (Subdivision Depth: %d)", iter, iter);
        outtextxy(10, 10, msg);
        outtextxy(10, 30, "Control Points: RED, Curve: GREEN");
        
        // Draw Bezier curve with current iteration depth
        drawBezierRecursive(controlPoints[0], controlPoints[1], 
                           controlPoints[2], controlPoints[3], iter);
        
        outtextxy(10, 450, "Press any key for next iteration...");
        getch();
    }
}

// Alternative: Draw complete smooth Bezier curve using parametric equation
void drawSmoothBezierCurve() {
    float t;
    int prevX, prevY, currX, currY;
    int firstPoint = 1;
    
    setcolor(LIGHTGREEN);
    
    // Draw curve using parametric equations
    for(t = 0.0; t <= 1.0; t += 0.01) {
        float u = 1 - t;
        float tt = t * t;
        float uu = u * u;
        float uuu = uu * u;
        float ttt = tt * t;
        
        currX = (int)(uuu * controlPoints[0].x + 
                     3 * uu * t * controlPoints[1].x + 
                     3 * u * tt * controlPoints[2].x + 
                     ttt * controlPoints[3].x);
                     
        currY = (int)(uuu * controlPoints[0].y + 
                     3 * uu * t * controlPoints[1].y + 
                     3 * u * tt * controlPoints[2].y + 
                     ttt * controlPoints[3].y);
        
        if(!firstPoint) {
            line(prevX, prevY, currX, currY);
        }
        
        prevX = currX;
        prevY = currY;
        firstPoint = 0;
    }
}

int main() {
    int gd = DETECT, gm;
    int i, choice;
    
    // Initialize graphics with multiple path attempts
    initgraph(&gd, &gm, "");
    
    if(graphresult() != grOk) {
        initgraph(&gd, &gm, "C:\\TC\\BGI");
    }
    
    if(graphresult() != grOk) {
        initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");
    }
    
    if(graphresult() != grOk) {
        printf("Graphics initialization failed!\n");
        printf("Make sure BGI files are in the correct directory.\n");
        printf("Press any key to exit...");
        getch();
        return 1;
    }
    
    // Get control points from user
    clrscr();
    printf("\n=== BEZIER CURVE GENERATOR ===\n");
    printf("Enter 4 control points for Bezier curve:\n");
    printf("(Recommended: keep coordinates between 100-500)\n\n");
    
    for(i = 0; i < 4; i++) {
        printf("Control Point %d (P%d):\n", i, i);
        printf("  X coordinate: ");
        scanf("%d", &controlPoints[i].x);
        printf("  Y coordinate: ");
        scanf("%d", &controlPoints[i].y);
        printf("\n");
    }
    
    // Show menu
    printf("Choose visualization method:\n");
    printf("1. Step-by-step midpoint subdivision (1-4 iterations)\n");
    printf("2. Smooth parametric curve\n");
    printf("3. Both methods\n");
    printf("Enter choice (1-3): ");
    scanf("%d", &choice);
    
    // Clear screen and start drawing
    cleardevice();
    
    switch(choice) {
        case 1:
            // Show control polygon first
            drawControlPolygon();
            outtextxy(10, 10, "Control Polygon and Points");
            outtextxy(10, 30, "Press any key to start subdivision...");
            getch();
            
            // Demonstrate subdivision method
            demonstrateBezierCurve();
            break;
            
        case 2:
            // Draw smooth curve
            drawControlPolygon();
            drawSmoothBezierCurve();
            outtextxy(10, 10, "Smooth Bezier Curve (Parametric Method)");
            break;
            
        case 3:
        default:
            // Show both methods
            drawControlPolygon();
            outtextxy(10, 10, "Control Polygon - Press key for subdivision demo");
            getch();
            
            demonstrateBezierCurve();
            
            // Show final smooth curve
            cleardevice();
            drawControlPolygon();
            drawSmoothBezierCurve();
            outtextxy(10, 10, "Final: Smooth Parametric Bezier Curve");
            break;
    }
    
    outtextxy(10, 470, "Press any key to exit...");
    getch();
    
    closegraph();
    return 0;
}
/*
PROGRAM EXPLANATION:

1. Algorithm Overview:
   The program implements a Bezier curve using the midpoint method, which
   recursively finds midpoints between control points to generate curve points.

2. Key Components:

   a) Data Structure:
      - Point structure to store x,y coordinates
      - Array of 4 control points
      - Iteration counter (up to 4 iterations)

   b) Main Functions:

      drawBezierIteration():
      - Recursive function implementing midpoint method
      - Takes array of points and current iteration
      - Base case: draws line between 2 points
      
      drawBezierCurve():
      - Main drawing function
      - Manages iterations
      - Shows progression of curve generation

3. Algorithm Steps:

   a) For each iteration:
      - Find midpoints between each pair of points
      - Connect midpoints to form new segments
      - Repeat until only one segment remains
      - Draw final segment as part of curve

   b) Color Scheme:
      - Control points: RED
      - Control polygon: WHITE
      - Intermediate points: YELLOW
      - Intermediate lines: BLUE
      - Final curve segments: GREEN

4. Program Flow:
   1. Initialize graphics
   2. Get 4 control points from user
   3. Draw control polygon and points
   4. For each iteration:
      - Calculate and show intermediate points
      - Draw curve segments
   5. Show final curve

5. How to Use:
   1. Run program
   2. Enter coordinates for 4 control points
   3. Watch curve generation process
   4. Press key to exit

6. Key Features:
   - Visual feedback during generation
   - Step-by-step demonstration
   - Color-coded elements
   - Interactive input

7. Advantages:
   - Clear visualization of algorithm
   - Shows intermediate steps
   - Educational tool
   - Simple implementation

8. Limitations:
   - Fixed number of control points (4)
   - Maximum 4 iterations
   - No curve modification after generation

9. Possible Enhancements:
   - Add more control points
   - Allow iteration count selection
   - Add curve editing
   - Add animation speed control
*/
