#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <dos.h>
#include <stdlib.h>
#include <stdio.h>

#define DELAY_MS 30      // Animation delay in milliseconds
#define ESC 27          // ASCII code for Escape key

// Movement directions
#define RIGHT 0
#define UP 1
#define LEFT 2
#define DOWN 3

void main()
{
   int gd = DETECT, gm;
   int rect_left, rect_top, rect_right, rect_bottom;  // Rectangle coordinates
   int ball_radius = 15;    // Radius of the moving ball
   int ball_x, ball_y;      // Coordinates of the ball center
   int direction = DOWN;    // Start by moving downward
   char key = 0;           // For keyboard input
   
   // Initialize graphics mode
   initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");
   
   // Define rectangle dimensions (centered on screen with some margin)
   int screen_width = getmaxx();
   int screen_height = getmaxy();
   rect_left = screen_width / 4;
   rect_right = screen_width * 3 / 4;
   rect_top = screen_height / 4;
   rect_bottom = screen_height * 3 / 4;
   
   // Calculate the padding to keep the ball inside the rectangle
   int padding = ball_radius + 2;  // 2 pixels extra space
   
   // Set initial ball position (starting in top-left corner, moving downward)
   ball_x = rect_left + padding;
   ball_y = rect_top + padding;
   
   // Initial instructions
   cleardevice();
   setcolor(WHITE);
   settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
   outtextxy(screen_width/2 - 150, screen_height/2 - 10, "Ball moves in anticlockwise direction");
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
      outtextxy(10, 10, "RECTANGLE MOTION ANIMATION");
      outtextxy(10, 30, "Direction: Anticlockwise");
      outtextxy(10, 50, "Press ESC to exit");
      
      // Draw arrows to indicate anticlockwise motion
      setcolor(WHITE);
      
      // Top arrow (right to left)
      line(rect_right-50, rect_top-20, rect_left+50, rect_top-20);
      line(rect_left+50, rect_top-20, rect_left+60, rect_top-25);
      line(rect_left+50, rect_top-20, rect_left+60, rect_top-15);
      
      // Left arrow (top to bottom)
      line(rect_left-20, rect_top+50, rect_left-20, rect_bottom-50);
      line(rect_left-20, rect_bottom-50, rect_left-25, rect_bottom-60);
      line(rect_left-20, rect_bottom-50, rect_left-15, rect_bottom-60);
      
      // Bottom arrow (left to right)
      line(rect_left+50, rect_bottom+20, rect_right-50, rect_bottom+20);
      line(rect_right-50, rect_bottom+20, rect_right-60, rect_bottom+15);
      line(rect_right-50, rect_bottom+20, rect_right-60, rect_bottom+25);
      
      // Right arrow (bottom to top)
      line(rect_right+20, rect_bottom-50, rect_right+20, rect_top+50);
      line(rect_right+20, rect_top+50, rect_right+15, rect_top+60);
      line(rect_right+20, rect_top+50, rect_right+25, rect_top+60);
      
      // Move the ball according to current direction (anticlockwise only)
      switch(direction) {
         case RIGHT:  // Moving right along bottom edge
            ball_x += 2;
            if (ball_x >= rect_right - padding) {
               ball_x = rect_right - padding;
               direction = UP;  // Change to up
            }
            break;
            
         case UP:     // Moving up along right edge
            ball_y -= 2;
            if (ball_y <= rect_top + padding) {
               ball_y = rect_top + padding;
               direction = LEFT;  // Change to left
            }
            break;
            
         case LEFT:   // Moving left along top edge
            ball_x -= 2;
            if (ball_x <= rect_left + padding) {
               ball_x = rect_left + padding;
               direction = DOWN;  // Change to down
            }
            break;
            
         case DOWN:   // Moving down along left edge
            ball_y += 2;
            if (ball_y >= rect_bottom - padding) {
               ball_y = rect_bottom - padding;
               direction = RIGHT;  // Change to right
            }
            break;
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
 *    - We include necessary headers for graphics, console I/O, etc.
 *    - We define constants for animation delay and key codes
 *    - We define direction constants (RIGHT, UP, LEFT, DOWN)
 *    - We initialize the graphics mode with initgraph()
 *    - We calculate the rectangle dimensions based on screen size
 * 
 * 2. ANIMATION PRINCIPLES
 *    - The ball moves along the inner perimeter of a rectangle in anticlockwise direction
 *    - The ball's movement is divided into four distinct phases (edges):
 *      * DOWN: Moving down along the left edge
 *      * RIGHT: Moving right along the bottom edge
 *      * UP: Moving up along the right edge
 *      * LEFT: Moving left along the top edge
 *    - When the ball reaches a corner, it changes direction to follow the next edge
 *    - A padding is added to keep the ball inside the rectangle edges
 * 
 * 3. DRAWING ROUTINES
 *    - cleardevice(): Clears the screen each frame
 *    - rectangle(): Draws the rectangular path
 *    - circle(): Draws the ball
 *    - setfillstyle() and floodfill(): Colors the shapes
 *    - line(): Draws arrows indicating anticlockwise motion
 * 
 * 4. ANIMATION LOOP
 *    - The animation continues until ESC is pressed
 *    - Each iteration:
 *      1. Clears the screen
 *      2. Draws the rectangle and ball
 *      3. Displays information
 *      4. Moves the ball according to its current direction
 *      5. Checks if the ball has reached a corner and needs to change direction
 *      6. Checks for keyboard input
 *      7. Adds a delay to control animation speed
 * 
 * 5. MOVEMENT ALGORITHM
 *    - The ball starts in the top-left corner moving downward
 *    - It follows the edge it's on until it reaches a corner
 *    - At each corner, it changes direction to follow the next edge
 *    - This creates a continuous anticlockwise motion around the rectangle
 * 
 * 6. CLEAN UP
 *    - closegraph(): Closes the graphics mode when done
 * 
 * HOW TO MODIFY:
 * - Change DELAY_MS to adjust animation speed
 * - Change the rectangle dimensions by modifying rect_left, rect_top, etc.
 * - Change ball_radius to modify the ball size
 */
