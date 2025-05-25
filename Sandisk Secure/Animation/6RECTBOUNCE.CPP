#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <dos.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define DELAY_MS 20      // Animation delay in milliseconds
#define ESC 27          // ASCII code for Escape key

void main()
{
   int gd = DETECT, gm;
   int ball_radius = 15;      // Radius of the bouncing ball
   int rect_left, rect_top, rect_right, rect_bottom;  // Rectangle coordinates
   int ball_x, ball_y;        // Current ball position (center)
   int dx = 2, dy = 2;        // Ball movement direction and speed
   int num_bounces = 0;       // Counter for number of bounces
   char key = 0;              // For keyboard input
   
   // Initialize graphics mode
   initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");
   
   // Get the screen dimensions
   int screen_width = getmaxx();
   int screen_height = getmaxy();
   
   // Define rectangle dimensions (centered on screen with margin)
   rect_left = screen_width / 6;
   rect_right = screen_width * 5 / 6;
   rect_top = screen_height / 6;
   rect_bottom = screen_height * 5 / 6;
   
   // Calculate the padding to keep the ball inside the rectangle when bouncing
   int padding = ball_radius;
   
   // Initialize random seed
   randomize();
   
   // Set initial ball position (random within the rectangle)
   ball_x = rect_left + padding + random(rect_right - rect_left - 2*padding);
   ball_y = rect_top + padding + random(rect_bottom - rect_top - 2*padding);
   
   // Set initial random direction
   dx = (random(2) * 2 - 1) * (1 + random(3));  // Random -3 to -1 or 1 to 3
   dy = (random(2) * 2 - 1) * (1 + random(3));  // Random -3 to -1 or 1 to 3
   
   // Initial instructions
   cleardevice();
   setcolor(WHITE);
   settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
   outtextxy(screen_width/2 - 150, screen_height/2 - 10, "Ball bouncing inside rectangle");
   outtextxy(screen_width/2 - 150, screen_height/2 + 10, "Press ESC to exit");
   getch();
   
   // Main animation loop
   while(key != ESC) {
      // Clear the screen
      cleardevice();
      
      // Draw the rectangle
      setcolor(WHITE);
      rectangle(rect_left, rect_top, rect_right, rect_bottom);
      setfillstyle(SOLID_FILL, BLUE);
      floodfill((rect_left + rect_right)/2, (rect_top + rect_bottom)/2, WHITE);
      
      // Draw the ball
      setcolor(WHITE);
      setfillstyle(SOLID_FILL, RED);
      circle(ball_x, ball_y, ball_radius);
      floodfill(ball_x, ball_y, WHITE);
      
      // Display information
      setcolor(WHITE);
      settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
      outtextxy(10, 10, "BALL BOUNCE ANIMATION");
      
      char bounce_text[50];
      sprintf(bounce_text, "Bounces: %d", num_bounces);
      outtextxy(10, 30, bounce_text);
      
      char speed_text[50];
      sprintf(speed_text, "Speed: dx=%d, dy=%d", dx, dy);
      outtextxy(10, 50, speed_text);
      
      outtextxy(10, 70, "Press ESC to exit");
      
      // Update ball position
      ball_x += dx;
      ball_y += dy;
      
      // Check for collision with rectangle boundaries and bounce
      // Right edge
      if (ball_x + ball_radius >= rect_right) {
         ball_x = rect_right - ball_radius;
         dx = -abs(dx);  // Ensure we bounce to the left
         num_bounces++;
      }
      // Left edge
      else if (ball_x - ball_radius <= rect_left) {
         ball_x = rect_left + ball_radius;
         dx = abs(dx);   // Ensure we bounce to the right
         num_bounces++;
      }
      
      // Bottom edge
      if (ball_y + ball_radius >= rect_bottom) {
         ball_y = rect_bottom - ball_radius;
         dy = -abs(dy);  // Ensure we bounce up
         num_bounces++;
      }
      // Top edge
      else if (ball_y - ball_radius <= rect_top) {
         ball_y = rect_top + ball_radius;
         dy = abs(dy);   // Ensure we bounce down
         num_bounces++;
      }
      
      // Every 10 bounces, slightly change the ball's direction
      if (num_bounces % 10 == 0 && num_bounces > 0) {
         // Add small random adjustment to the direction
         dx += (random(3) - 1);  // -1, 0, or 1
         dy += (random(3) - 1);  // -1, 0, or 1
         
         // Ensure speed doesn't get too slow or too fast
         if (abs(dx) < 1) dx = (dx >= 0) ? 1 : -1;
         if (abs(dy) < 1) dy = (dy >= 0) ? 1 : -1;
         if (abs(dx) > 5) dx = (dx >= 0) ? 5 : -5;
         if (abs(dy) > 5) dy = (dy >= 0) ? 5 : -5;
         
         // Prevent ball from getting stuck in a perfectly horizontal or vertical path
         if (dx == 0) dx = (random(2) * 2 - 1);  // Random -1 or 1
         if (dy == 0) dy = (random(2) * 2 - 1);  // Random -1 or 1
      }
      
      // Check for keyboard input
      if (kbhit()) {
         key = getch();
      }
      
      // Delay for animation
      delay(DELAY_MS);
   }
   
   // Close the graphics
   closegraph();
}

/*
 * PROGRAM EXPLANATION:
 * 
 * 1. SETUP AND INITIALIZATION
 *    - We include necessary headers for graphics, console I/O, math functions, etc.
 *    - We define constants for animation delay and key codes
 *    - We initialize the graphics mode with initgraph()
 *    - We define a rectangle where the ball will bounce
 *    - We generate random starting position and direction for the ball
 * 
 * 2. ANIMATION PRINCIPLES
 *    - The ball moves in a straight line until it hits the edge of the rectangle
 *    - When the ball hits an edge, it bounces by reversing the direction in x or y
 *    - The ball's center is kept at least ball_radius away from each edge
 *    - We use dx and dy variables to control the direction and speed
 *    - Every 10 bounces, we add small random changes to the direction to make the motion less predictable
 * 
 * 3. DRAWING ROUTINES
 *    - cleardevice(): Clears the screen each frame
 *    - rectangle(): Draws the rectangular boundary
 *    - circle(): Draws the ball
 *    - setfillstyle() and floodfill(): Colors the shapes
 *    - text functions: Display information about the current state
 * 
 * 4. ANIMATION LOOP
 *    - The animation continues until ESC is pressed
 *    - Each iteration:
 *      1. Clears the screen
 *      2. Draws the rectangle and ball
 *      3. Displays information (bounce count, current speed)
 *      4. Updates the ball position
 *      5. Checks for collisions with the rectangle edges
 *      6. Handles bouncing by reversing direction when needed
 *      7. Occasionally adds small random changes to direction
 *      8. Checks for keyboard input
 *      9. Adds a delay to control animation speed
 * 
 * 5. COLLISION DETECTION
 *    - We check if the ball's edge (center +/- radius) hits any of the rectangle's edges
 *    - When a collision is detected:
 *      1. The ball's position is adjusted to exactly touch the boundary
 *      2. The direction (dx or dy) is reversed
 *      3. The bounce counter is incremented
 * 
 * 6. RANDOMNESS
 *    - The initial position is random within the rectangle
 *    - The initial direction and speed are random
 *    - Small random adjustments are made to direction every 10 bounces
 *    - This creates a natural, unpredictable bouncing effect
 * 
 * 7. CLEAN UP
 *    - closegraph(): Closes the graphics mode when done
 * 
 * HOW TO MODIFY:
 * - Change DELAY_MS to adjust animation speed
 * - Change ball_radius to modify the ball size
 * - Adjust rectangle dimensions by modifying rect_left, rect_top, etc.
 * - Modify dx and dy initial values to change the starting speed
 */
