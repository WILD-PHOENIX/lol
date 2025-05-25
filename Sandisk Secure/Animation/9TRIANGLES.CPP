#include <graphics.h>
#include <conio.h>
#include <dos.h>
#include <math.h>
#include <stdio.h>

#define DELAY_MS 20       // Animation delay in milliseconds
#define ESC 27           // ASCII code for Escape key
#define GRAVITY 0.1      // Gravity acceleration
#define FRICTION 0.99    // Friction factor

void main(void)
{
   int gd = DETECT, gm;
   char key = 0;
   
   // Initialize graphics mode
   initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");
   
   // Get the screen dimensions
   int screen_width = getmaxx();
   int screen_height = getmaxy();
   
   // Define triangle dimensions
   int valley_height = screen_height * 2/3;    // Valley bottom y-position
   int left_top_x = screen_width / 6;          // Left triangle top x-position
   int left_top_y = screen_height / 4;         // Left triangle top y-position (higher)
   int valley_x = screen_width / 2;            // Valley bottom x-position
   int valley_y = valley_height;               // Valley bottom y-position
   int right_top_x = screen_width * 5/6;       // Right triangle top x-position
   int right_top_y = valley_y - (valley_y - left_top_y) * 0.7; // Right triangle top y-position (lower than left)
   
   // Ball properties
   int ball_radius = 10;
   float ball_x, ball_y;         // Current position
   float ball_vx = 0, ball_vy = 0; // Velocity components
   
   // Animation path segments
   int segment = 0;  // 0 = left slope, 1 = right slope, 2 = reset
   float slope1, slope2;  // Slopes of the two triangles
   
   // Calculate slopes
   slope1 = (float)(valley_y - left_top_y) / (valley_x - left_top_x);
   slope2 = (float)(valley_y - right_top_y) / (valley_x - right_top_x);
   
   // Start ball at top of left triangle
   ball_x = left_top_x;
   ball_y = left_top_y;
   
   // Animation loop
   while (key != ESC) {
      // Clear the screen
      cleardevice();
      
      // Draw the triangles
      setcolor(WHITE);
      
      // Left triangle
      line(left_top_x, left_top_y, valley_x, valley_y);  // Hypotenuse
      line(left_top_x, left_top_y, left_top_x, valley_y); // Vertical side
      line(left_top_x, valley_y, valley_x, valley_y);    // Horizontal side
      
      // Right triangle
      line(valley_x, valley_y, right_top_x, right_top_y); // Hypotenuse
      line(right_top_x, right_top_y, right_top_x, valley_y); // Vertical side
      line(valley_x, valley_y, right_top_x, valley_y);    // Horizontal side
      
      // Draw direction arrows (as in your drawing)
      setcolor(WHITE);
      
      // Left triangle arrow
      int arrow_x1 = left_top_x + (valley_x - left_top_x) / 3;
      int arrow_y1 = left_top_y + (valley_y - left_top_y) / 3;
      int arrow_x2 = arrow_x1 + 20;
      int arrow_y2 = arrow_y1 + 10;
      line(arrow_x1, arrow_y1, arrow_x2, arrow_y2);
      line(arrow_x2, arrow_y2, arrow_x2 - 5, arrow_y2 - 5);
      line(arrow_x2, arrow_y2, arrow_x2 - 8, arrow_y2 + 2);
      
      // Right triangle arrow
      int arrow_x3 = valley_x + (right_top_x - valley_x) / 2;
      int arrow_y3 = valley_y - (valley_y - right_top_y) / 2;
      int arrow_x4 = arrow_x3 + 20;
      int arrow_y4 = arrow_y3 - 10;
      line(arrow_x3, arrow_y3, arrow_x4, arrow_y4);
      line(arrow_x4, arrow_y4, arrow_x4 - 5, arrow_y4 - 3);
      line(arrow_x4, arrow_y4, arrow_x4 - 8, arrow_y4 + 5);
      
      // Ball physics based on current segment
      switch(segment) {
         case 0: // Left slope (moving down)
            // Apply gravity along the slope
            ball_vx += GRAVITY;
            ball_vy += GRAVITY * slope1;
            
            // Apply friction
            ball_vx *= FRICTION;
            ball_vy *= FRICTION;
            
            // Update position
            ball_x += ball_vx;
            ball_y += ball_vy;
            
            // Keep ball on the hypotenuse line
            ball_y = left_top_y + slope1 * (ball_x - left_top_x);
            
            // Check if reached valley
            if (ball_x >= valley_x) {
               ball_x = valley_x;
               ball_y = valley_y;
               segment = 1;
               
               // Transfer momentum with enhancement to ensure climbing the second slope
               ball_vx = ball_vx * 1.2; // Boost horizontal velocity
               ball_vy = 0;
            }
            break;
            
         case 1: // Right slope (moving up)
            // Apply gentler gravity effect for the less steep slope
            ball_vx -= GRAVITY * 0.6;
            
            // Apply minimal friction to ensure it reaches the top
            ball_vx *= 0.995;
            
            // Ensure sufficient velocity to reach the top
            if (ball_vx < 0.5) ball_vx = 0.5;
            
            // Update position
            ball_x += ball_vx;
            
            // Keep ball on the hypotenuse line
            ball_y = valley_y + slope2 * (ball_x - valley_x);
            
            // Check if reached the top right
            if (ball_x >= right_top_x) {
               ball_x = right_top_x;
               ball_y = right_top_y;
               
               // Pause briefly at the top
               delay(500);
               segment = 2;  // Reset
            }
            break;
            
         case 2: // Reset to start
            // Reset ball position
            ball_x = left_top_x;
            ball_y = left_top_y;
            
            // Reset velocity
            ball_vx = 0;
            ball_vy = 0;
            
            // Back to first segment
            segment = 0;
            break;
      }
      
      // Draw the ball
      setcolor(WHITE);
      setfillstyle(SOLID_FILL, BLUE);
      circle(ball_x, ball_y, ball_radius);
      floodfill(ball_x, ball_y, WHITE);
      
      // Display information
      setcolor(WHITE);
      settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
      outtextxy(10, 10, "TRIANGLES BALL SIMULATION");
      
      char speed_text[50];
      sprintf(speed_text, "Ball Speed: %.2f", sqrt(ball_vx*ball_vx + ball_vy*ball_vy));
      outtextxy(10, 30, speed_text);
      
      outtextxy(10, 50, "Press ESC to exit");
      
      // Check for keyboard input
      if (kbhit()) {
         key = getch();
      }
      
      // Add delay for animation
      delay(DELAY_MS);
   }
   
   // Close the graphics
   closegraph();
}

/*
 * PROGRAM EXPLANATION:
 * 
 * This program creates a simulation of a ball rolling down two connected right-angle
 * triangles that form a valley.
 * 
 * 1. TRIANGLE STRUCTURE
 *    - Two right-angle triangles connected at their bases
 *    - Left triangle: steeper slope (as shown in the image)
 *    - Right triangle: gentler slope
 *    - They form a valley at their connection point
 * 
 * 2. BALL PHYSICS SIMULATION
 *    - The ball follows the hypotenuse (sloped side) of each triangle
 *    - Realistic physics applied:
 *      * Gravity accelerates the ball down the first slope
 *      * Momentum carries it up the second slope
 *      * Friction gradually reduces the ball's speed
 *    - The ball's path is determined by:
 *      * Keeping it exactly on the hypotenuse line of each triangle
 *      * Accelerating or decelerating based on the direction of motion
 * 
 * 3. ANIMATION SEGMENTS
 *    - Left slope: Ball accelerates down the first triangle's hypotenuse
 *    - Right slope: Ball uses momentum to climb the second triangle's hypotenuse
 *    - Reset: When the ball stops or reaches the top right, it resets to start again
 * 
 * 4. PHYSICS IMPLEMENTATION
 *    - Velocity components (vx, vy) track the ball's speed and direction
 *    - Gravity constantly applies acceleration appropriate to the slope
 *    - Friction simulates energy loss over time
 *    - The ball is constrained to move along the hypotenuse lines
 * 
 * 5. VISUAL ELEMENTS
 *    - White lines draw the triangles
 *    - Direction arrows show the expected path (as in your image)
 *    - Blue ball rolls along the triangles
 *    - Information display shows current speed
 * 
 * 6. HOW TO MODIFY
 *    - Change GRAVITY to adjust how quickly the ball accelerates
 *    - Change FRICTION to adjust energy loss rate
 *    - Adjust triangle dimensions by modifying the coordinate variables
 */
