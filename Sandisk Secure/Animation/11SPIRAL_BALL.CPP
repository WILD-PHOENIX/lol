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
   int center_x = screen_width / 2;
   int center_y = screen_height / 2;
   
   // Spiral properties
   int max_radius = screen_height / 3;
   int num_revolutions = 3;
   int num_points = 100 * num_revolutions; // Points along the spiral
   int spacing = 20; // Space between spiral loops
   
   // Ball properties
   int ball_radius = 10;
   float ball_pos = 0;    // Position along the spiral (0 to 1)
   float ball_speed = 0.005; // Speed of movement
   int direction = 1;     // 1 = outward, -1 = inward
   
   // Calculate all points of the spiral for drawing
   int spiral_x[1000], spiral_y[1000];
   for (int i = 0; i < num_points; i++) {
      float t = (float)i / num_points;
      float angle = 2 * PI * num_revolutions * t;
      float radius = max_radius * t;
      
      spiral_x[i] = center_x + radius * cos(angle);
      spiral_y[i] = center_y + radius * sin(angle);
   }
   
   // Display title
   cleardevice();
   setcolor(WHITE);
   settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
   outtextxy(center_x - 150, center_y, "SPIRAL BALL ANIMATION - Press any key to start");
   getch();
   
   // Animation loop
   char key = 0;
   while (key != 27) { // ESC to exit
      cleardevice();
      
      // Draw the spiral
      setcolor(WHITE);
      for (int i = 1; i < num_points; i++) {
         line(spiral_x[i-1], spiral_y[i-1], spiral_x[i], spiral_y[i]);
      }
      
      // Calculate position on the spiral
      int ball_index = ball_pos * num_points;
      if (ball_index >= num_points) ball_index = num_points - 1;
      if (ball_index < 0) ball_index = 0;
      
      // Get the current point on the spiral
      float path_x = spiral_x[ball_index];
      float path_y = spiral_y[ball_index];
      
      // Calculate the direction to the center of the spiral
      float center_dx = center_x - path_x;
      float center_dy = center_y - path_y;
      float dist = sqrt(center_dx*center_dx + center_dy*center_dy);
      
      // Normalize this vector
      if (dist > 0) {
         center_dx /= dist;
         center_dy /= dist;
      }
      
      // Position the ball on the inner surface of the spiral
      // The entire ball will be inside the spiral
      float ball_x = path_x + center_dx * ball_radius;
      float ball_y = path_y + center_dy * ball_radius;
      
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
         direction = -1; // Change to inward
      } else if (ball_pos <= 0.0) {
         ball_pos = 0.0;
         direction = 1;  // Change to outward
      }
      
      // Display info
      char direction_text[50];
      sprintf(direction_text, "Direction: %s", direction == 1 ? "Outward" : "Inward");
      outtextxy(10, 10, "SPIRAL BALL ANIMATION");
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
 * SPIRAL BALL ANIMATION
 * 
 * This program shows a ball rolling along a spiral path, moving from
 * the center outward and then back inward in a continuous motion.
 * 
 * HOW IT WORKS:
 * 
 * 1. A spiral path is generated using parametric equations:
 *    x = center_x + radius * cos(angle)
 *    y = center_y + radius * sin(angle)
 *    Where radius increases with the angle to create the spiral effect
 * 
 * 2. The ball moves along this path:
 *    - When it reaches the outer end, it reverses direction and moves inward
 *    - When it reaches the center, it reverses again and moves outward
 *    - This creates a continuous back-and-forth motion
 * 
 * 3. The spiral is drawn as a series of connected line segments
 * 
 * 4. The ball's position is calculated by interpolating between points on the spiral
 * 
 * CONTROLS:
 * - Press any key to start the animation
 * - Press ESC to exit
 * 
 * The design resembles a snail shell with the ball continuously moving
 * from the center outward and back again.
 */
