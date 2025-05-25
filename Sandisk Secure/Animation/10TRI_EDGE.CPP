#include <graphics.h>
#include <conio.h>
#include <dos.h>
#include <math.h>
#include <stdio.h>

void main(void)
{
   int gd = DETECT, gm;
   char key = 0;
   
   // Initialize graphics
   initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");
   
   // Get screen dimensions
   int screen_width = getmaxx();
   int screen_height = getmaxy();
   
   // Define triangle
   int center_x = screen_width / 2;
   int center_y = screen_height / 2;
   int tri_size = screen_height / 3;
   
   // Triangle vertices
   int vx[3], vy[3];
   
   // Top vertex
   vx[0] = center_x;
   vy[0] = center_y - tri_size;
   
   // Bottom left vertex
   vx[1] = center_x - tri_size;
   vy[1] = center_y + tri_size/2;
   
   // Bottom right vertex
   vx[2] = center_x + tri_size;
   vy[2] = center_y + tri_size/2;
   
   // Ball properties
   int ball_radius = 10;
   float ball_x, ball_y;
   int direction = 0;  // 0 for clockwise, 1 for counterclockwise
   int current_side = 0;
   float progress = 0;  // Position along current side (0-100%)
   float ball_speed = 2;
   
   // Initial ball position
   ball_x = vx[0];
   ball_y = vy[0] - ball_radius - 2;
   
   // Display instructions
   cleardevice();
   setcolor(WHITE);
   outtextxy(center_x - 200, center_y, "Press 0: Clockwise, 1: Counterclockwise, ESC: Exit");
   getch();
   
   // Animation loop
   while (key != 27) {  // ESC key
      cleardevice();
      
      // Draw triangle
      setcolor(WHITE);
      line(vx[0], vy[0], vx[1], vy[1]);
      line(vx[1], vy[1], vx[2], vy[2]);
      line(vx[2], vy[2], vx[0], vy[0]);
      
      // Update progress along current side
      progress += ball_speed;
      
      // Check if reached the end of current side
      if (progress >= 100) {
         progress = 0;
         
         // Move to next side based on direction
         if (direction == 0) {  // Clockwise
            current_side = (current_side + 1) % 3;
         } else {  // Counterclockwise
            current_side = (current_side + 2) % 3;
         }
      }
      
      // Get current side endpoints
      int start_idx = current_side;
      int end_idx;
      
      if (direction == 0) {  // Clockwise
         end_idx = (current_side + 1) % 3;
      } else {  // Counterclockwise
         end_idx = (current_side + 2) % 3;
      }
      
      // Calculate position along current side
      float base_x = vx[start_idx] + (vx[end_idx] - vx[start_idx]) * progress / 100;
      float base_y = vy[start_idx] + (vy[end_idx] - vy[start_idx]) * progress / 100;
      
      // Calculate the normal vector (perpendicular to side)
      float dx = vx[end_idx] - vx[start_idx];
      float dy = vy[end_idx] - vy[start_idx];
      float len = sqrt(dx*dx + dy*dy);
      
      // For clockwise, we need the outward normal
      float normal_x = -dy / len;
      float normal_y = dx / len;
      
      // If counterclockwise, we need to invert the normal
      if (direction == 1) {
         normal_x = -normal_x;
         normal_y = -normal_y;
      }
      
      // Position ball outside the edge
      ball_x = base_x + normal_x * (ball_radius + 2);
      ball_y = base_y + normal_y * (ball_radius + 2);
      
      // Draw ball
      setcolor(WHITE);
      setfillstyle(SOLID_FILL, BLUE);
      circle(ball_x, ball_y, ball_radius);
      floodfill(ball_x, ball_y, WHITE);
      
      // Display information
      char dir_text[50];
      sprintf(dir_text, "Direction: %s", direction == 0 ? "Clockwise" : "Counterclockwise");
      outtextxy(10, 10, dir_text);
      outtextxy(10, 30, "Press 0: Clockwise, 1: Counterclockwise, ESC: Exit");
      
      // Check for keyboard input
      if (kbhit()) {
         key = getch();
         
         if (key == '0') {
            direction = 0;  // Clockwise
         } else if (key == '1') {
            direction = 1;  // Counterclockwise
         }
      }
      
      delay(20);  // Animation delay
   }
   
   closegraph();
}

/*
 * TRIANGLE EDGE BALL ANIMATION
 * 
 * This program shows a ball that always sticks to the outer surface of a triangle
 * while moving in either clockwise or counterclockwise direction.
 * 
 * HOW IT WORKS:
 * 
 * 1. The ball follows the perimeter of the triangle, staying just outside the edges
 * 
 * 2. For each frame:
 *    - We calculate the base position along the current edge
 *    - We calculate the normal vector (perpendicular to the edge)
 *    - We position the ball along this normal vector, outside the triangle
 * 
 * 3. Direction handling:
 *    - For clockwise motion, we use the outward normal
 *    - For counterclockwise motion, we invert the normal
 *    - This ensures the ball stays outside the triangle in both directions
 * 
 * USER CONTROLS:
 * - Press '0' for clockwise movement
 * - Press '1' for counterclockwise movement
 * - Press ESC to exit
 * 
 * The ball will always follow the outer surface of the triangle regardless of direction.
 */
