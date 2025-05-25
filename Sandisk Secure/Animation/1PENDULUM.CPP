/*
 * PENDULUM ANIMATION USING BORLAND GRAPHICS INTERFACE
 * Computer Graphics and Gaming Assignment
 */

#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <dos.h>

#define PI 3.14159
#define DELAY_MS 50      // Animation delay in milliseconds
#define AMPLITUDE 30     // Maximum angle of swing in degrees
#define CYCLES 5         // Number of complete oscillations

void main()
{
   int gd = DETECT, gm;
   int midx, midy;       // Screen middle coordinates
   int length = 150;     // Length of pendulum rod
   int bob_radius = 20;  // Radius of the pendulum bob
   float angle;          // Current angle of pendulum
   float x, y;           // Coordinates of the pendulum bob
   float increment = 0.1; // Angular increment per frame
   float current_time = 0; // Time variable

   // Initialize graphics mode
   initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");
   
   // Get the middle of the screen
   midx = getmaxx() / 2;
   midy = getmaxy() / 3;
   
   // Main animation loop
   while(!kbhit() && current_time < CYCLES * 2 * PI) {
      // Clear the screen
      cleardevice();
      
      // Calculate the current angle using a sine function
      angle = AMPLITUDE * sin(current_time) * PI / 180;
      
      // Calculate the position of the pendulum bob
      x = midx + length * sin(angle);
      y = midy + length * cos(angle);
      
      // Draw the pendulum rod
      line(midx, midy, x, y);
      
      // Draw the pivot point
      circle(midx, midy, 5);
      setfillstyle(SOLID_FILL, WHITE);
      floodfill(midx, midy, WHITE);
      
      // Draw the pendulum bob
      circle(x, y, bob_radius);
      setfillstyle(SOLID_FILL, RED);
      floodfill(x, y, WHITE);
      
      // Display information
      settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
      outtextxy(10, 10, "PENDULUM ANIMATION");
      outtextxy(10, 30, "Press any key to exit");
      
      // Update time and delay
      current_time += increment;
      delay(DELAY_MS);
   }
   
   // Close the graphics
   closegraph();
}

/*
 * PROGRAM EXPLANATION:
 * 
 * 1. SETUP AND INITIALIZATION
 *    - We include necessary headers:
 *      * graphics.h: For Borland Graphics Interface functions
 *      * conio.h: For console I/O (kbhit)
 *      * math.h: For trigonometric functions
 *      * dos.h: For delay function
 *    - We define constants for the animation:
 *      * PI: Mathematical constant
 *      * DELAY_MS: Controls animation speed
 *      * AMPLITUDE: Maximum angle of swing
 *      * CYCLES: Number of complete oscillations
 *    - We initialize the graphics mode with initgraph()
 * 
 * 2. ANIMATION PRINCIPLES
 *    - The pendulum motion follows the physics of simple harmonic motion
 *    - We use a sine function to calculate the angle: angle = AMPLITUDE * sin(time)
 *    - From this angle, we calculate the (x,y) position of the pendulum bob using:
 *      * x = midx + length * sin(angle)
 *      * y = midy + length * cos(angle)
 *    - The angle is converted to radians when needed (PI/180)
 * 
 * 3. DRAWING ROUTINES
 *    - cleardevice(): Clears the screen each frame
 *    - line(): Draws the pendulum rod
 *    - circle(): Draws the pivot point and pendulum bob
 *    - setfillstyle() and floodfill(): Colors the shapes
 * 
 * 4. ANIMATION LOOP
 *    - The animation continues until a key is pressed or the specified cycles complete
 *    - Each iteration:
 *      1. Clears the screen
 *      2. Calculates the new position
 *      3. Draws the pendulum
 *      4. Displays information
 *      5. Adds a delay to control animation speed
 * 
 * 5. CLEAN UP
 *    - closegraph(): Closes the graphics mode when done
 * 
 * HOW TO MODIFY:
 * - Change AMPLITUDE to adjust the swing angle
 * - Change DELAY_MS to adjust animation speed
 * - Change length and bob_radius to modify the pendulum appearance
 * - Change CYCLES to control how many oscillations before ending
 */
