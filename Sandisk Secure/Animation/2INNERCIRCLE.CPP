#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <dos.h>
#include <stdlib.h>
#include <stdio.h>

#define PI 3.14159
#define DELAY_MS 30      // Animation delay in milliseconds
// Key codes
#define KEY_ZERO 48  // ASCII code for '0'
#define KEY_ONE 49   // ASCII code for '1'
#define ESC 27      // ASCII code for Escape key

void main()
{
   int gd = DETECT, gm;
   int midx, midy;       // Screen middle coordinates
   int circle_radius = 150;  // Radius of the outer circle
   int ball_radius = 15;    // Radius of the moving ball
   float angle = 0;         // Current angle of ball
   float x, y;              // Coordinates of the ball
   float angular_velocity = 0.1;  // Speed of rotation
   int direction = 1;       // 1 for clockwise, -1 for counterclockwise
   int direction_changes = 0;  // Counter for direction changes
   char key = 0;           // For keyboard input
   
   // Initialize graphics mode
   initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");
   
   // Get the middle of the screen
   midx = getmaxx() / 2;
   midy = getmaxy() / 2;
   
   // Set random seed
   randomize();
   
   // Initial instructions
   cleardevice();
   setcolor(WHITE);
   settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
   outtextxy(midx - 200, midy - 20, "Press 0 for clockwise motion");
   outtextxy(midx - 200, midy, "Press 1 for counter-clockwise motion");
   outtextxy(midx - 200, midy + 20, "Press ESC to exit");
   getch();
   
   // Main animation loop
   while(key != ESC) {
      // Clear the screen
      cleardevice();
      
      // Calculate the position of the ball on the circle
      x = midx + (circle_radius - ball_radius) * cos(angle);
      y = midy + (circle_radius - ball_radius) * sin(angle);
      
      // Draw the outer circle
      circle(midx, midy, circle_radius);
      setfillstyle(SOLID_FILL, BLUE);
      floodfill(midx, midy, WHITE);
      
      // No arrow drawing code as per user request
      
      // Draw the ball on the circle's edge
      setcolor(WHITE);
      setfillstyle(SOLID_FILL, RED);
      circle(x, y, ball_radius);
      floodfill(x, y, WHITE);
      
      // Display information
      setcolor(WHITE);
      settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
      outtextxy(10, 10, "CIRCULAR MOTION ANIMATION");
      
      char dir_text[50];
      sprintf(dir_text, "Direction: %s", direction > 0 ? "Clockwise" : "Counter-Clockwise");
      outtextxy(10, 30, dir_text);
      
      char changes_text[50];
      sprintf(changes_text, "Direction Changes: %d", direction_changes);
      outtextxy(10, 50, changes_text);
      
      outtextxy(10, 70, "Press 0: Clockwise, 1: Counter-Clockwise, ESC: Exit");
      
      // Update the angle based on direction
      angle += direction * angular_velocity;
      
      // Keep angle in range [0, 2*PI)
      if (angle < 0) angle += 2 * PI;
      if (angle >= 2 * PI) angle -= 2 * PI;
      
      // Check for keyboard input
      if (kbhit()) {
         key = getch();
         
         // Change direction based on key press
         if (key == KEY_ZERO && direction != 1) {
            direction = 1;  // Clockwise
            direction_changes++;
            
            // Flash the screen to indicate direction change
            setfillstyle(SOLID_FILL, YELLOW);
            bar(0, 0, getmaxx(), getmaxy());
            delay(100);
         }
         else if (key == KEY_ONE && direction != -1) {
            direction = -1;  // Counter-clockwise
            direction_changes++;
            
            // Flash the screen to indicate direction change
            setfillstyle(SOLID_FILL, YELLOW);
            bar(0, 0, getmaxx(), getmaxy());
            delay(100);
         }
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
 *    - We define constants:
 *      * PI: Mathematical constant
 *      * DELAY_MS: Controls animation speed
 *      * DIRECTION_CHANGE_TIME: Time before the ball changes direction
 *    - We initialize the graphics mode with initgraph()
 *    - We calculate the center of the screen
 * 
 * 2. ANIMATION PRINCIPLES
 *    - The ball moves along the inner edge of a circle (staying just inside it)
 *    - We use parametric equations to calculate the position:
 *      * x = midx + (circle_radius - ball_radius) * cos(angle)
 *      * y = midy + (circle_radius - ball_radius) * sin(angle)
 *    - The 'direction' variable controls clockwise (1) or counter-clockwise (-1) motion
 *    - The direction changes at regular intervals defined by DIRECTION_CHANGE_TIME
 * 
 * 3. DRAWING ROUTINES
 *    - cleardevice(): Clears the screen each frame
 *    - circle(): Draws the outer circle and the ball
 *    - setfillstyle() and floodfill(): Colors the shapes
 *    - line(): Draws direction arrows
 *    - text functions: Display information about the current state
 * 
 * 4. ANIMATION LOOP
 *    - The animation continues until a key is pressed
 *    - Each iteration:
 *      1. Clears the screen
 *      2. Calculates the new position
 *      3. Draws the circle, arrow, and ball
 *      4. Displays information
 *      5. Updates the angle based on direction
 *      6. Checks if it's time to change direction
 *      7. Adds a delay to control animation speed
 * 
 * 5. DIRECTION CHANGES
 *    - The program checks for keyboard input:
 *      * Press '0' to set clockwise motion
 *      * Press '1' to set counter-clockwise motion
 *      * Press 'ESC' to exit the program
 *    - When direction changes:
 *      1. The direction_changes counter is incremented
 *      2. A brief screen flash indicates the change
 * 
 * 6. CLEAN UP
 *    - closegraph(): Closes the graphics mode when done
 * 
 * HOW TO MODIFY:
 * - Change DELAY_MS to adjust animation speed
 * - Change DIRECTION_CHANGE_TIME to control how often the direction changes
 * - Change circle_radius and ball_radius to modify the appearance
 * - Modify the angular_velocity to change how fast the ball moves
 */
