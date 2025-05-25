#include <graphics.h>
#include <conio.h>
#include <dos.h>
#include <math.h>
#include <stdio.h>

#define PI 3.14159265
#define DELAY_MS 30

void main(void)
{
   int gd = DETECT, gm;
   
   // Initialize graphics
   initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");
   
   // Get screen dimensions
   int screen_width = getmaxx();
   int screen_height = getmaxy();
   
   // Define centers for the two spirals - closer together for infinity symbol
   int left_center_x = screen_width / 2 - screen_width / 6;
   int right_center_x = screen_width / 2 + screen_width / 6;
   int center_y = screen_height / 2;
   
   // Spiral properties
   int spiral_radius = screen_height / 4;
   int num_revolutions = 2;
   int points_per_revolution = 80;
   int total_points_per_spiral = num_revolutions * points_per_revolution;
   
   // Total path properties
   int total_points = 2 * total_points_per_spiral;
   float path_x[1000], path_y[1000];
   
   int i;
   
   // Generate left spiral (clockwise from inside out)
   for (i = 0; i < total_points_per_spiral; i++) {
      float t = (float)i / total_points_per_spiral;
      float angle = 2 * PI * num_revolutions * (1 - t); // Clockwise
      float radius = spiral_radius * t;
      
      path_x[i] = left_center_x + radius * cos(angle);
      path_y[i] = center_y + radius * sin(angle);
   }
   
   // Generate right spiral (counter-clockwise from outside in, mirrored top-to-bottom)
   for (i = 0; i < total_points_per_spiral; i++) {
      float t = (float)i / total_points_per_spiral;
      float angle = 2 * PI * num_revolutions * t; // Counter-clockwise
      float radius = spiral_radius * (1 - t);
      
      // Mirror the spiral horizontally and vertically
      path_x[i + total_points_per_spiral] = right_center_x - radius * cos(angle);
      path_y[i + total_points_per_spiral] = center_y - radius * sin(angle);
   }
   
   // Ball properties
   int ball_radius = 8;
   float ball_pos = 0;     // Position along the path (0 to 1)
   float ball_speed = 0.005; // Speed of movement
   int direction = 1;      // 1 = forward, -1 = backward
   
   // Display title
   cleardevice();
   setcolor(WHITE);
   settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
   outtextxy(screen_width/2 - 150, center_y, "DUAL SPIRAL ANIMATION - Press any key to start");
   getch();
   
   // Animation loop
   char key = 0;
   while (key != 27) { // ESC to exit
      cleardevice();
      
      // Draw both spirals
      setcolor(WHITE);
      for (i = 1; i < total_points; i++) {
         line(path_x[i-1], path_y[i-1], path_x[i], path_y[i]);
      }
      
      // Calculate current position along the path
      int path_index = ball_pos * total_points;
      if (path_index >= total_points) path_index = total_points - 1;
      if (path_index < 0) path_index = 0;
      
      // Get point on the path
      float point_x = path_x[path_index];
      float point_y = path_y[path_index];
      
      // Determine which spiral we're on
      int on_left_spiral = (path_index < total_points_per_spiral);
      
      // Calculate the ball position
      float ball_x, ball_y;
      
      if (on_left_spiral) {
         // On left spiral - ball should be on inner surface
         // Calculate direction to center
         float to_center_x = left_center_x - point_x;
         float to_center_y = center_y - point_y;
         float dist = sqrt(to_center_x*to_center_x + to_center_y*to_center_y);
         
         // Normalize
         if (dist > 0) {
            to_center_x /= dist;
            to_center_y /= dist;
         }
         
         // Position ball on inner surface
         ball_x = point_x + to_center_x * ball_radius;
         ball_y = point_y + to_center_y * ball_radius;
      } else {
         // On right spiral - ball should be on outer surface
         // For the mirrored spiral, we need to calculate direction correctly
         float from_center_x = point_x - right_center_x;
         float from_center_y = point_y - center_y;
         float dist = sqrt(from_center_x*from_center_x + from_center_y*from_center_y);
         
         // Normalize
         if (dist > 0) {
            from_center_x /= dist;
            from_center_y /= dist;
         }
         
         // Position ball on outer surface
         ball_x = point_x + from_center_x * ball_radius;
         ball_y = point_y + from_center_y * ball_radius;
      }
      
      // Draw the ball
      setcolor(WHITE);
      setfillstyle(SOLID_FILL, BLUE);
      circle(ball_x, ball_y, ball_radius);
      floodfill(ball_x, ball_y, WHITE);
      
      // Update ball position
      ball_pos += ball_speed * direction;
      
      // Check if we need to change direction
      if (ball_pos >= 1.0) {
         ball_pos = 1.0;
         direction = -1; // Change to backward
      } else if (ball_pos <= 0.0) {
         ball_pos = 0.0;
         direction = 1;  // Change to forward
      }
      
      // Display info
      char direction_text[50];
      sprintf(direction_text, "Direction: %s", 
             direction == 1 ? "Left to Right" : "Right to Left");
      outtextxy(10, 10, "DUAL SPIRAL ANIMATION");
      outtextxy(10, 30, direction_text);
      outtextxy(10, 50, "Press ESC to exit");
      
      // Check for keyboard input
      if (kbhit()) {
         key = getch();
      }
      
      delay(DELAY_MS);
   }
   
   closegraph();
}

/*
 * INFINITY SPIRAL ANIMATION
 * 
 * This program shows a ball moving through two connected spiral paths forming an infinity symbol.
 * The ball starts on the inner surface of the left spiral, moves outward,
 * then continues to the right spiral moving inward. Then it reverses direction
 * and returns to the starting point.
 * 
 * HOW IT WORKS:
 * 
 * 1. Two spiral paths are generated to form an infinity symbol:
 *    - Left spiral: Clockwise from inside to outside
 *    - Right spiral: Counter-clockwise from outside to inside (mirrored and flipped top-to-bottom)
 * 
 * 2. The ball's position is calculated differently based on which spiral it's on:
 *    - On the left spiral, the ball stays on the inner surface
 *    - On the right spiral, the ball stays on the outer surface
 * 
 * 3. Movement control:
 *    - The ball moves from left to right initially
 *    - When it reaches the end of the right spiral, it reverses direction
 *    - When it returns to the start of the left spiral, it reverses again
 *    - This creates a continuous back-and-forth motion
 * 
 * 4. The spirals are drawn as a series of connected line segments
 * 
 * CONTROLS:
 * - Press any key to start the animation
 * - Press ESC to exit
 * 
 * This animation resembles an infinity symbol with the ball continuously
 * traversing the path between the two connected spirals.
 */
