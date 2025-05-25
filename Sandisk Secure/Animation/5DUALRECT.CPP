#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <dos.h>
#include <stdlib.h>
#include <stdio.h>

#define DELAY_MS 25      // Animation delay in milliseconds
#define ESC 27          // ASCII code for Escape key

// Movement states
#define LEFT_BOT_RIGHT 0
#define LEFT_RIGHT_UP 1
#define PATH_LEFT_TO_RIGHT 2
#define RIGHT_LEFT_DOWN_TO_BOT 3  // New state: go down to bottom left after entering right rectangle
#define RIGHT_BOT_RIGHT 4
#define RIGHT_UP 5
#define RIGHT_TOP_LEFT 6
#define RIGHT_LEFT_DOWN 7
#define PATH_RIGHT_TO_LEFT 8
#define LEFT_UP 9
#define LEFT_TOP_LEFT 10
#define LEFT_DOWN 11

void main()
{
   int gd = DETECT, gm;
   int ball_radius = 10;    // Radius of the moving ball
   int ball_x, ball_y;      // Coordinates of the ball center
   int movement_state = LEFT_BOT_RIGHT;  // Start at left bottom going right
   char key = 0;           // For keyboard input
   int speed = 2;          // Speed of ball movement
   
   // Initialize graphics mode
   initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");
   
   // Get screen dimensions
   int screen_width = getmaxx();
   int screen_height = getmaxy();
   
   // Define rectangles dimensions
   int rect_width = screen_width / 3;
   int rect_height = screen_height * 2 / 3;
   int path_width = rect_width / 3;
   int path_height = rect_height / 6;
   int path_y = rect_height / 2;
   
   // Left rectangle coordinates
   int left_rect_left = screen_width / 8;
   int left_rect_top = (screen_height - rect_height) / 2;
   int left_rect_right = left_rect_left + rect_width;
   int left_rect_bottom = left_rect_top + rect_height;
   
   // Right rectangle coordinates
   int right_rect_left = left_rect_right + path_width;
   int right_rect_top = left_rect_top;
   int right_rect_right = right_rect_left + rect_width;
   int right_rect_bottom = left_rect_bottom;
   
   // Path coordinates
   int path_left = left_rect_right;
   int path_right = right_rect_left;
   int path_top = left_rect_top + path_y - path_height / 2;
   int path_bottom = path_top + path_height;
   
   // Calculate padding to keep ball inside boundaries
   int padding = ball_radius + 2;  // 2 pixels extra space
   
   // Set initial ball position (starting at bottom left corner)
   ball_x = left_rect_left + padding;
   ball_y = left_rect_bottom - padding;
   
   // Initial instructions
   cleardevice();
   setcolor(WHITE);
   settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
   outtextxy(screen_width/2 - 200, screen_height/2 - 10, "Ball moves through two connected rectangles");
   outtextxy(screen_width/2 - 200, screen_height/2 + 10, "Press ESC to exit");
   getch();
   
   // Main animation loop
   while(key != ESC) {
      // Clear the screen
      cleardevice();
      
      // Draw the two rectangles and connecting path
      setcolor(WHITE);
      
      // Left rectangle
      rectangle(left_rect_left, left_rect_top, left_rect_right, left_rect_bottom);
      setfillstyle(SOLID_FILL, BLUE);
      floodfill((left_rect_left + left_rect_right)/2, (left_rect_top + left_rect_bottom)/2, WHITE);
      
      // Right rectangle
      rectangle(right_rect_left, right_rect_top, right_rect_right, right_rect_bottom);
      setfillstyle(SOLID_FILL, BLUE);
      floodfill((right_rect_left + right_rect_right)/2, (right_rect_top + right_rect_bottom)/2, WHITE);
      
      // Connecting path
      rectangle(path_left, path_top, path_right, path_bottom);
      setfillstyle(SOLID_FILL, BLUE);
      floodfill((path_left + path_right)/2, (path_top + path_bottom)/2, WHITE);
      
      // Draw arrows to indicate the path
      setcolor(WHITE);
      
      // Left rectangle bottom arrow (left to right)
      line(left_rect_left + 30, left_rect_bottom - 20, left_rect_right - 30, left_rect_bottom - 20);
      line(left_rect_right - 30, left_rect_bottom - 20, left_rect_right - 40, left_rect_bottom - 25);
      line(left_rect_right - 30, left_rect_bottom - 20, left_rect_right - 40, left_rect_bottom - 15);
      
      // Left rectangle right side arrow (bottom to top)
      line(left_rect_right - 20, left_rect_bottom - 30, left_rect_right - 20, path_bottom + 30);
      line(left_rect_right - 20, path_bottom + 30, left_rect_right - 25, path_bottom + 40);
      line(left_rect_right - 20, path_bottom + 30, left_rect_right - 15, path_bottom + 40);
      
      // Path arrow (left to right)
      line(path_left + 20, (path_top + path_bottom)/2, path_right - 20, (path_top + path_bottom)/2);
      line(path_right - 20, (path_top + path_bottom)/2, path_right - 30, (path_top + path_bottom)/2 - 5);
      line(path_right - 20, (path_top + path_bottom)/2, path_right - 30, (path_top + path_bottom)/2 + 5);
      
      // Right rectangle bottom arrow (left to right)
      line(right_rect_left + 30, right_rect_bottom - 20, right_rect_right - 30, right_rect_bottom - 20);
      line(right_rect_right - 30, right_rect_bottom - 20, right_rect_right - 40, right_rect_bottom - 25);
      line(right_rect_right - 30, right_rect_bottom - 20, right_rect_right - 40, right_rect_bottom - 15);
      
      // Right rectangle right side arrow (bottom to top)
      line(right_rect_right - 20, right_rect_bottom - 30, right_rect_right - 20, right_rect_top + 30);
      line(right_rect_right - 20, right_rect_top + 30, right_rect_right - 25, right_rect_top + 40);
      line(right_rect_right - 20, right_rect_top + 30, right_rect_right - 15, right_rect_top + 40);
      
      // Right rectangle top arrow (right to left)
      line(right_rect_right - 30, right_rect_top + 20, right_rect_left + 30, right_rect_top + 20);
      line(right_rect_left + 30, right_rect_top + 20, right_rect_left + 40, right_rect_top + 15);
      line(right_rect_left + 30, right_rect_top + 20, right_rect_left + 40, right_rect_top + 25);
      
      // Right rectangle left side arrow (top to bottom)
      line(right_rect_left + 20, right_rect_top + 30, right_rect_left + 20, path_top - 30);
      line(right_rect_left + 20, path_top - 30, right_rect_left + 15, path_top - 40);
      line(right_rect_left + 20, path_top - 30, right_rect_left + 25, path_top - 40);
      
      // Path arrow (right to left)
      line(path_right - 20, (path_top + path_bottom)/2 - 15, path_left + 20, (path_top + path_bottom)/2 - 15);
      line(path_left + 20, (path_top + path_bottom)/2 - 15, path_left + 30, (path_top + path_bottom)/2 - 20);
      line(path_left + 20, (path_top + path_bottom)/2 - 15, path_left + 30, (path_top + path_bottom)/2 - 10);
      
      // Left rectangle top arrow (right to left)
      line(left_rect_right - 30, left_rect_top + 20, left_rect_left + 30, left_rect_top + 20);
      line(left_rect_left + 30, left_rect_top + 20, left_rect_left + 40, left_rect_top + 15);
      line(left_rect_left + 30, left_rect_top + 20, left_rect_left + 40, left_rect_top + 25);
      
      // Left rectangle left side arrow (top to bottom)
      line(left_rect_left + 20, left_rect_top + 30, left_rect_left + 20, left_rect_bottom - 30);
      line(left_rect_left + 20, left_rect_bottom - 30, left_rect_left + 15, left_rect_bottom - 40);
      line(left_rect_left + 20, left_rect_bottom - 30, left_rect_left + 25, left_rect_bottom - 40);
      
      // Draw the ball
      setcolor(WHITE);
      setfillstyle(SOLID_FILL, RED);
      circle(ball_x, ball_y, ball_radius);
      floodfill(ball_x, ball_y, WHITE);
      
      // Display information
      setcolor(WHITE);
      settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
      outtextxy(10, 10, "DUAL RECTANGLE MOTION ANIMATION");
      outtextxy(10, 30, "Press ESC to exit");
      
      // Move the ball according to current state
      switch(movement_state) {
         case LEFT_BOT_RIGHT:  // Moving right along bottom of left rectangle
            ball_x += speed;
            if (ball_x >= left_rect_right - padding) {
               ball_x = left_rect_right - padding;
               movement_state = LEFT_RIGHT_UP;
            }
            break;
            
         case LEFT_RIGHT_UP:  // Moving up along right side of left rectangle to path
            ball_y -= speed;
            if (ball_y <= path_bottom - padding) {
               ball_y = path_bottom - padding;
               movement_state = PATH_LEFT_TO_RIGHT;
            }
            break;
            
         case PATH_LEFT_TO_RIGHT:  // Moving right through the path to right rectangle
            ball_x += speed;
            if (ball_x >= right_rect_left + padding) {
               ball_x = right_rect_left + padding;
               movement_state = RIGHT_LEFT_DOWN_TO_BOT;  // Go down to bottom left first
            }
            break;
            
         case RIGHT_LEFT_DOWN_TO_BOT:  // Moving down along left side of right rectangle to bottom
            ball_y += speed;
            if (ball_y >= right_rect_bottom - padding) {
               ball_y = right_rect_bottom - padding;
               movement_state = RIGHT_BOT_RIGHT;
            }
            break;
            
         case RIGHT_BOT_RIGHT:  // Moving right along bottom of right rectangle
            ball_x += speed;
            if (ball_x >= right_rect_right - padding) {
               ball_x = right_rect_right - padding;
               movement_state = RIGHT_UP;
            }
            break;
            
         case RIGHT_UP:  // Moving up along right side of right rectangle
            ball_y -= speed;
            if (ball_y <= right_rect_top + padding) {
               ball_y = right_rect_top + padding;
               movement_state = RIGHT_TOP_LEFT;
            }
            break;
            
         case RIGHT_TOP_LEFT:  // Moving left along top of right rectangle
            ball_x -= speed;
            if (ball_x <= right_rect_left + padding) {
               ball_x = right_rect_left + padding;
               movement_state = RIGHT_LEFT_DOWN;
            }
            break;
            
         case RIGHT_LEFT_DOWN:  // Moving down along left side of right rectangle to path
            ball_y += speed;
            if (ball_y >= path_top + padding) {
               ball_y = path_top + padding;
               movement_state = PATH_RIGHT_TO_LEFT;
            }
            break;
            
         case PATH_RIGHT_TO_LEFT:  // Moving left through the path to left rectangle
            ball_x -= speed;
            if (ball_x <= left_rect_right - padding) {
               ball_x = left_rect_right - padding;
               movement_state = LEFT_UP;
            }
            break;
            
         case LEFT_UP:  // Moving up along right side of left rectangle to top
            ball_y -= speed;
            if (ball_y <= left_rect_top + padding) {
               ball_y = left_rect_top + padding;
               movement_state = LEFT_TOP_LEFT;
            }
            break;
            
         case LEFT_TOP_LEFT:  // Moving left along top of left rectangle
            ball_x -= speed;
            if (ball_x <= left_rect_left + padding) {
               ball_x = left_rect_left + padding;
               movement_state = LEFT_DOWN;
            }
            break;
            
         case LEFT_DOWN:  // Moving down along left side of left rectangle to bottom
            ball_y += speed;
            if (ball_y >= left_rect_bottom - padding) {
               ball_y = left_rect_bottom - padding;
               movement_state = LEFT_BOT_RIGHT;  // Back to start
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
 *    - We define movement states to track the ball's position along the path
 *    - We initialize the graphics mode with initgraph()
 *    - We calculate the dimensions of two rectangles and the connecting path
 * 
 * 2. ANIMATION PRINCIPLES
 *    - The ball moves through two connected rectangles following a specific path
 *    - The movement is controlled using a state machine approach with 11 states
 *    - Each state represents a specific segment of the path:
 *      * LEFT_BOT_RIGHT: Bottom edge of left rectangle, moving right
 *      * LEFT_RIGHT_UP: Right edge of left rectangle, moving up to pathway
 *      * PATH_LEFT_TO_RIGHT: Through the connecting path, left to right
 *      * RIGHT_LEFT_DOWN_TO_BOT: Left edge of right rectangle, moving down to bottom
 *      * RIGHT_BOT_RIGHT: Bottom edge of right rectangle, moving right
 *      * RIGHT_UP: Right edge of right rectangle, moving up
 *      * RIGHT_TOP_LEFT: Top edge of right rectangle, moving left
 *      * RIGHT_LEFT_DOWN: Left edge of right rectangle, moving down to pathway
 *      * PATH_RIGHT_TO_LEFT: Through the connecting path, right to left
 *      * LEFT_UP: Right edge of left rectangle, moving up to top
 *      * LEFT_TOP_LEFT: Top edge of left rectangle, moving left
 *      * LEFT_DOWN: Left edge of left rectangle, moving down to starting point
 * 
 * 3. DRAWING ROUTINES
 *    - cleardevice(): Clears the screen each frame
 *    - rectangle(): Draws the two rectangles and the connecting path
 *    - circle(): Draws the ball
 *    - setfillstyle() and floodfill(): Colors the shapes
 *    - line(): Draws arrows indicating the movement path
 * 
 * 4. ANIMATION LOOP
 *    - The animation continues until ESC is pressed
 *    - Each iteration:
 *      1. Clears the screen
 *      2. Draws the rectangles, connecting path, and arrows
 *      3. Draws the ball at its current position
 *      4. Displays information
 *      5. Moves the ball according to its current state
 *      6. Checks if the ball has reached a transition point and needs to change state
 *      7. Checks for keyboard input
 *      8. Adds a delay to control animation speed
 * 
 * 5. MOVEMENT ALGORITHM
 *    - The ball moves at a constant speed (controlled by the 'speed' variable)
 *    - At each step, the ball's position is updated based on its current state
 *    - When the ball reaches the end of a segment, its state changes to the next segment
 *    - This creates a continuous path through both rectangles
 *    - The path forms a figure-8 like pattern, completing a cycle through both rectangles
 * 
 * 6. CLEAN UP
 *    - closegraph(): Closes the graphics mode when done
 * 
 * HOW TO MODIFY:
 * - Change DELAY_MS to adjust animation speed
 * - Change the 'speed' variable to control how fast the ball moves
 * - Adjust rectangle and path dimensions to change the appearance
 * - Change the ball_radius to modify the ball size
 */
