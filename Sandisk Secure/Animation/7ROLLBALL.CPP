#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <dos.h>
#include <stdlib.h>
#include <stdio.h>

#define PI 3.14159
#define DELAY_MS 30      // Animation delay in milliseconds
#define ESC 27          // ASCII code for Escape key

void drawBallWithCross(int x, int y, int radius, float angle);

void main()
{
   int gd = DETECT, gm;
   int ball_radius = 20;      // Radius of the rolling ball
   float angle = 0;           // Current rotation angle of the ball
   int tri_x1, tri_y1;        // Triangle point 1 (right angle)
   int tri_x2, tri_y2;        // Triangle point 2 (bottom right)
   int tri_x3, tri_y3;        // Triangle point 3 (top left)
   float ball_x, ball_y;      // Current ball position (center)
   float slope;               // Slope of the hypotenuse
   float distance_traveled = 0; // How far the ball has traveled
   float total_distance;      // Total distance of the slope
   float speed = 1.5;         // Speed of the ball
   float gravity_accel = 0.05; // Gravity acceleration
   char key = 0;              // For keyboard input
   
   // Initialize graphics mode
   initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");
   
   // Get the screen dimensions
   int screen_width = getmaxx();
   int screen_height = getmaxy();
   
   // Define triangle coordinates
   tri_x1 = screen_width / 5;       // Right angle point (left)
   tri_y1 = screen_height * 3 / 4;  // Right angle point (bottom)
   tri_x2 = screen_width * 4 / 5;   // Bottom right
   tri_y2 = tri_y1;                 // Same y as right angle
   tri_x3 = tri_x1;                 // Same x as right angle
   tri_y3 = screen_height / 4;      // Top point
   
   // Calculate slope properties
   slope = (float)(tri_y1 - tri_y3) / (tri_x2 - tri_x1);
   total_distance = sqrt(pow(tri_x2 - tri_x1, 2) + pow(tri_y1 - tri_y3, 2));
   
   // Set initial ball position (top of the slope)
   ball_x = tri_x3;
   ball_y = tri_y3;
   
   // Initial instructions
   cleardevice();
   setcolor(WHITE);
   settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
   outtextxy(screen_width/2 - 200, screen_height/2 - 10, "Ball with cross rolling down a triangle slope");
   outtextxy(screen_width/2 - 200, screen_height/2 + 10, "Press any key to start, ESC to exit");
   getch();
   
   // Main animation loop
   while(key != ESC && distance_traveled < total_distance) {
      // Clear the screen
      cleardevice();
      
      // Draw the triangle
      setcolor(WHITE);
      line(tri_x1, tri_y1, tri_x2, tri_y2);  // Bottom line
      line(tri_x1, tri_y1, tri_x3, tri_y3);  // Left line
      line(tri_x3, tri_y3, tri_x2, tri_y2);  // Hypotenuse
      
      // Fill the triangle
      setfillstyle(SOLID_FILL, BLUE);
      int triangle_points[6] = {tri_x1, tri_y1, tri_x2, tri_y2, tri_x3, tri_y3};
      fillpoly(3, triangle_points);
      
      // Draw ball with cross
      drawBallWithCross(ball_x, ball_y, ball_radius, angle);
      
      // Display information
      setcolor(WHITE);
      settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
      outtextxy(10, 10, "BALL ROLLING ANIMATION");
      
      char speed_text[50];
      sprintf(speed_text, "Speed: %.2f", speed);
      outtextxy(10, 30, speed_text);
      
      char progress_text[50];
      sprintf(progress_text, "Progress: %.0f%%", (distance_traveled / total_distance) * 100);
      outtextxy(10, 50, progress_text);
      
      outtextxy(10, 70, "Press ESC to exit");
      
      // Update ball position
      distance_traveled += speed;
      
      // Increase speed due to gravity (accelerate down the slope)
      speed += gravity_accel;
      
      // Calculate new position based on distance traveled
      if (distance_traveled < total_distance) {
         float ratio = distance_traveled / total_distance;
         ball_x = tri_x3 + ratio * (tri_x2 - tri_x3);
         ball_y = tri_y3 + ratio * (tri_y2 - tri_y3);
      } else {
         // Ensure the ball stops exactly at the end of the slope
         ball_x = tri_x2;
         ball_y = tri_y2;
      }
      
      // Update rotation angle
      // This rotates the ball based on how far it has rolled
      // 2*PI radians for each circumference length traveled
      angle = distance_traveled / ball_radius;
      
      // Check for keyboard input
      if (kbhit()) {
         key = getch();
      }
      
      // Delay for animation
      delay(DELAY_MS);
   }
   
   // If we got here by reaching the end, show a small pause
   if (distance_traveled >= total_distance) {
      delay(1000);  // Pause for 1 second at the end
   }
   
   // Close the graphics
   closegraph();
}

// Function to draw a ball with a cross inside
void drawBallWithCross(int x, int y, int radius, float angle)
{
   // Draw the ball
   setcolor(WHITE);
   circle(x, y, radius);
   setfillstyle(SOLID_FILL, RED);
   floodfill(x, y, WHITE);
   
   // Calculate cross endpoints based on rotation angle
   int cross_length = radius * 0.7;  // Cross is 70% of radius
   
   // Horizontal line of cross
   int x1 = x + cross_length * cos(angle);
   int y1 = y + cross_length * sin(angle);
   int x2 = x + cross_length * cos(angle + PI);
   int y2 = y + cross_length * sin(angle + PI);
   
   // Vertical line of cross
   int x3 = x + cross_length * cos(angle + PI/2);
   int y3 = y + cross_length * sin(angle + PI/2);
   int x4 = x + cross_length * cos(angle + 3*PI/2);
   int y4 = y + cross_length * sin(angle + 3*PI/2);
   
   // Draw the cross
   setcolor(WHITE);
   line(x1, y1, x2, y2);  // Horizontal line
   line(x3, y3, x4, y4);  // Vertical line
}

/*
 * PROGRAM EXPLANATION:
 * 
 * 1. SETUP AND INITIALIZATION
 *    - We include necessary headers for graphics, console I/O, math functions, etc.
 *    - We define constants for PI, animation delay, and key codes
 *    - We initialize the graphics mode with initgraph()
 *    - We define a right-angle triangle where the ball will roll
 *    - We calculate the slope, total distance, and set initial ball position
 * 
 * 2. ANIMATION PRINCIPLES
 *    - The ball rolls down the hypotenuse of the right-angle triangle
 *    - As the ball moves, it rotates to simulate real rolling motion
 *    - We use parametric equations to calculate the position along the slope
 *    - The ball accelerates due to gravity as it rolls down
 *    - We track the distance traveled and the current rotation angle
 * 
 * 3. DRAWING ROUTINES
 *    - cleardevice(): Clears the screen each frame
 *    - line(): Draws the triangle edges
 *    - fillpoly(): Fills the triangle with color
 *    - drawBallWithCross(): Custom function to draw the ball with a cross inside
 *      * circle() and floodfill(): Draws and fills the ball
 *      * line(): Draws the cross lines
 *      * Uses trigonometry to rotate the cross based on the ball's rotation angle
 * 
 * 4. ANIMATION LOOP
 *    - The animation continues until ESC is pressed or the ball reaches the end
 *    - Each iteration:
 *      1. Clears the screen
 *      2. Draws the triangle
 *      3. Draws the ball with a rotating cross inside
 *      4. Displays information about speed and progress
 *      5. Updates the ball position and rotation
 *      6. Increases speed due to gravity acceleration
 *      7. Checks for keyboard input
 *      8. Adds a delay to control animation speed
 * 
 * 5. PHYSICS SIMULATION
 *    - The ball accelerates as it rolls down due to gravity_accel
 *    - The rotation angle is calculated based on the distance traveled
 *    - When a ball rolls without slipping, it rotates 2π radians (360°) for each
 *      circumference length it travels along the surface
 * 
 * 6. CLEAN UP
 *    - Pause briefly when the ball reaches the end
 *    - closegraph(): Closes the graphics mode when done
 * 
 * HOW TO MODIFY:
 * - Change DELAY_MS to adjust animation speed
 * - Change ball_radius to modify the ball size
 * - Adjust triangle dimensions by modifying tri_x1, tri_y1, etc.
 * - Modify gravity_accel to change how quickly the ball accelerates
 * - Change the initial speed value to start faster or slower
 */
