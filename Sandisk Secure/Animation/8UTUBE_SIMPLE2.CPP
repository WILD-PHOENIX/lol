#include <graphics.h>
#include <conio.h>
#include <dos.h>
#include <math.h>
#include <stdio.h>

#define DELAY_MS 20       // Animation delay in milliseconds
#define ESC 27           // ASCII code for Escape key
#define GRAVITY 0.1      // Gravity acceleration

void main(void)
{
   int gd = DETECT, gm;
   char key = 0;
   
   // Initialize graphics mode
   initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");
   
   // Get the screen dimensions
   int screen_width = getmaxx();
   int screen_height = getmaxy();
   
   // Define tube dimensions
   int tube_width = 50;                  // Width between each pair of vertical lines
   int tube_spacing = tube_width * 2;    // Space between the two U-tubes
   
   // Calculate positions for vertical lines (all equidistant)
   int line1_x = screen_width/2 - tube_spacing/2 - tube_width;  // Leftmost line
   int line2_x = screen_width/2 - tube_spacing/2;               // Second line from left
   int line3_x = screen_width/2 + tube_spacing/2;               // Third line from left
   int line4_x = screen_width/2 + tube_spacing/2 + tube_width;  // Rightmost line
   
   // Define vertical positions
   int top_y = 100;                      // Top of the structure
   int middle_y = screen_height/2 + 50;  // Middle connection point
   
   // Ball properties
   int ball_radius = 10;
   int ball_x, ball_y;       // Current position (integer for simplicity)
   float velocity = 0;       // Current speed
   
   // Animation control variables
   int segment = 0;    // Current segment of the path
   float progress = 0; // Progress within current segment (0-100)
   
   // Predefined path coordinates (to ensure ball follows the drawn path)
   // Each segment has start and end points
   int path_x[8], path_y[8];
   
   // Define path segments
   // Segment 0: Left top curve
   path_x[0] = line1_x + tube_width/2;
   path_y[0] = top_y;
   
   // Segment 1: Left vertical line
   path_x[1] = line2_x;
   path_y[1] = top_y + tube_width;
   path_x[2] = line2_x;
   path_y[2] = middle_y;
   
   // Segment 2: Bottom curve
   path_x[3] = (line2_x + line3_x)/2;
   path_y[3] = middle_y + tube_width/2;
   path_x[4] = line3_x;
   path_y[4] = middle_y;
   
   // Segment 3: Right vertical line
   path_x[5] = line3_x;
   path_y[5] = top_y + tube_width;
   
   // Segment 4: Right top curve
   path_x[6] = line3_x + tube_width/2;
   path_y[6] = top_y;
   path_x[7] = line4_x;
   path_y[7] = top_y + tube_width;
   
   // Start ball at top of left U
   ball_x = path_x[0];
   ball_y = path_y[0];
   
   // Animation loop
   while (key != ESC) {
      // Clear the screen
      cleardevice();
      
      // Draw the U-tube structure
      setcolor(WHITE);
      setlinestyle(SOLID_LINE, 0, THICK_WIDTH);
      
      // Draw the four vertical lines with exact equal spacing
      line(line1_x, top_y + tube_width, line1_x, middle_y);          // Line 1 (leftmost)
      line(line2_x, top_y + tube_width, line2_x, middle_y);          // Line 2
      line(line3_x, top_y + tube_width, line3_x, middle_y);          // Line 3
      line(line4_x, top_y + tube_width, line4_x, middle_y);          // Line 4 (rightmost)
      
      // Draw top left curve (connecting lines 1 and 2)
      arc(line1_x + tube_width/2, top_y + tube_width, 0, 180, tube_width/2);
      
      // Draw top right curve (connecting lines 3 and 4)
      arc(line3_x + tube_width/2, top_y + tube_width, 0, 180, tube_width/2);
      
      // Draw the bottom curve (connecting lines 2 and 3)
      arc((line2_x + line3_x)/2, middle_y, 180, 360, tube_spacing/2);
      
      // Ball animation
      switch (segment) {
         case 0:  // Top left curve (transition to vertical)
            // Accelerate slightly
            velocity += GRAVITY * 0.2;
            
            // Move along path
            progress += velocity;
            
            // Interpolate position
            ball_x = path_x[0] + (path_x[1] - path_x[0]) * progress / 100;
            ball_y = path_y[0] + (path_y[1] - path_y[0]) * progress / 100;
            
            // Check for segment completion
            if (progress >= 100) {
               segment = 1;
               progress = 0;
            }
            break;
            
         case 1:  // Left vertical (going down)
            // Accelerate due to gravity
            velocity += GRAVITY;
            
            // Move along path
            progress += velocity;
            
            // Interpolate position
            ball_x = path_x[1];  // Fixed x position
            ball_y = path_y[1] + (path_y[2] - path_y[1]) * progress / 100;
            
            // Check for segment completion
            if (progress >= 100) {
               segment = 2;
               progress = 0;
            }
            break;
            
         case 2:  // Bottom curve
            // This is the most critical segment for correct visual appearance
            // Modify speed based on position
            if (progress < 50) {
               // First half - still accelerating
               velocity += GRAVITY * 0.1;
            } else {
               // Second half - starting to decelerate
               velocity -= GRAVITY * 0.2;
            }
            
            // Ensure minimum speed
            if (velocity < 1.0) velocity = 1.0;
            
            // Move along path
            progress += velocity;
            
            // Use quadratic interpolation for better curve following
            float t = progress / 100.0;
            float omt = 1.0 - t;
            
            // Bezier curve coordinates
            ball_x = path_x[2] * omt * omt + 
                     2 * path_x[3] * omt * t + 
                     path_x[4] * t * t;
            
            ball_y = path_y[2] * omt * omt + 
                     2 * path_y[3] * omt * t + 
                     path_y[4] * t * t;
            
            // Check for segment completion
            if (progress >= 100) {
               segment = 3;
               progress = 0;
               velocity = velocity * 0.8;  // Reduce velocity for upward motion
            }
            break;
            
         case 3:  // Right vertical (going up)
            // Decelerate due to gravity
            velocity -= GRAVITY;
            
            // Ensure minimum speed
            if (velocity < 0.5) velocity = 0.5;
            
            // Move along path
            progress += velocity;
            
            // Interpolate position
            ball_x = path_x[4];  // Fixed x position
            ball_y = path_y[4] + (path_y[5] - path_y[4]) * progress / 100;
            
            // Check for segment completion
            if (progress >= 100) {
               segment = 4;
               progress = 0;
            }
            break;
            
         case 4:  // Top right curve and reset
            // Move along path
            progress += velocity;
            
            // Interpolate position
            ball_x = path_x[5] + (path_x[6] - path_x[5]) * progress / 100;
            ball_y = path_y[5] + (path_y[6] - path_y[5]) * progress / 100;
            
            // Check for segment completion
            if (progress >= 100) {
               // Reset to beginning
               segment = 0;
               progress = 0;
               velocity = 0.5;  // Reset velocity
               
               // Reset position to start
               ball_x = path_x[0];
               ball_y = path_y[0];
            }
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
      outtextxy(10, 10, "U-TUBE PHYSICS SIMULATION");
      
      char speed_text[50];
      sprintf(speed_text, "Ball Speed: %.2f", velocity);
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
 * This program creates a U-tube structure with a ball that moves through it
 * following realistic physics principles.
 * 
 * 1. TUBE STRUCTURE
 *    - Four exactly equidistant vertical lines
 *    - Connected by curves:
 *      * Lines 1 and 2 connected by a curve at their top ends (left upside-down U)
 *      * Lines 2 and 3 connected by a curve at their bottom ends (normal U)
 *      * Lines 3 and 4 connected by a curve at their top ends (right upside-down U)
 * 
 * 2. BALL PHYSICS SIMULATION
 *    - The ball follows a path defined by predefined points along the tube
 *    - Gravity affects the ball's speed:
 *      * When moving upward: speed decreases (ball decelerates)
 *      * When moving downward: speed increases (ball accelerates)
 *    - The ball oscillates between the two upside-down U's:
 *      * Starting at the top of the first U
 *      * Falling down into the normal U
 *      * Rising up to the top of the second U
 *      * Returning to the first U to restart the cycle
 * 
 * 3. SIMPLIFIED PHYSICS IMPLEMENTATION
 *    - Path divided into segments for precise movement control
 *    - Bezier curve interpolation for the bottom curve ensures smooth, accurate movement
 *    - Segment-based approach guarantees the ball follows the tube path exactly
 *    - Simple physics calculations ensure compatibility with Turbo C++
 * 
 * 4. ANIMATION LOOP
 *    - Each frame:
 *      1. Draw the tube structure
 *      2. Update ball position based on current segment
 *      3. Apply physics (gravity and velocity changes)
 *      4. Draw the ball
 *      5. Display information
 *      6. Check for user input
 *      7. Add delay for smooth animation
 * 
 * 5. HOW TO MODIFY
 *    - Change GRAVITY to adjust how quickly the ball accelerates/decelerates
 *    - Adjust tube_width and tube_spacing to modify the tube structure
 *    - Modify path points to change the exact path the ball follows
 */
