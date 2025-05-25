#include <graphics.h>
#include <conio.h>
#include <dos.h>
#include <math.h>
#include <stdio.h>

#define PI 3.14159265
#define GRAVITY 0.1       // Gravity constant
#define FRICTION 0.995    // Friction coefficient

void main(void)
{
   int gd = DETECT, gm;
   
   // Initialize graphics
   initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");
   
   // Get screen dimensions
   int screen_width = getmaxx();
   int screen_height = getmaxy();
   
   // Define curve dimensions
   int curve_width = screen_width * 2/3;
   int curve_height = screen_height / 3;
   int left_x = (screen_width - curve_width) / 2;
   int right_x = left_x + curve_width;
   int top_y = screen_height / 3;
   int curve_segments = 40;
   
   // Ball properties
   int ball_radius = 10;
   float position = 0.9;  // Position along curve (0 to 1) - start near right side
   float velocity = 0;    // Velocity along the curve
   
   // Display instructions
   cleardevice();
   setcolor(WHITE);
   outtextxy(screen_width/2 - 150, screen_height/2, "CURVE BALL SIMULATION - Press any key to start");
   getch();
   
   // Animation loop
   char key = 0;
   while (key != 27) { // ESC to exit
      // Clear the screen
      cleardevice();
      
      // Draw the curve
      setcolor(WHITE);
      for (int i = 0; i < curve_segments; i++) {
         float t1 = (float)i / curve_segments;
         float t2 = (float)(i + 1) / curve_segments;
         
         // Calculate curve points using a parabola
         // y = k * x * (1-x) where x goes from 0 to 1
         int x1 = left_x + t1 * curve_width;
         int y1 = top_y + curve_height * 4 * t1 * (1 - t1);
         
         int x2 = left_x + t2 * curve_width;
         int y2 = top_y + curve_height * 4 * t2 * (1 - t2);
         
         line(x1, y1, x2, y2);
      }
      
      // Draw supporting structure
      // Left vertical line
      line(left_x, top_y, left_x, top_y + curve_height * 2);
      
      // Right vertical line
      line(right_x, top_y, right_x, top_y + curve_height * 2);
      
      // Bottom horizontal line
      line(left_x, top_y + curve_height * 2, right_x, top_y + curve_height * 2);
      
      // Calculate position on the curve
      float ball_t = position;
      int curve_x = left_x + ball_t * curve_width;
      int curve_y = top_y + curve_height * 4 * ball_t * (1 - ball_t);
      
      // Calculate the normal vector (perpendicular to the curve)
      // The tangent is (1, slope), so the normal is (-slope, 1) normalized
      float slope = 4 * curve_height * (1 - 2 * ball_t) / curve_width;
      float normal_x = -slope;
      float normal_y = 1.0;
      
      // Normalize the normal vector
      float normal_length = sqrt(normal_x*normal_x + normal_y*normal_y);
      normal_x /= normal_length;
      normal_y /= normal_length;
      
      // Position the ball above the curve by one radius
      int ball_x = curve_x - normal_x * ball_radius;
      int ball_y = curve_y - normal_y * ball_radius;
      
      // Slope was already calculated above when positioning the ball
      
      // Calculate acceleration due to gravity along the curve
      // Tangential acceleration = g * sin(angle) = g * slope / sqrt(1 + slope²)
      float angle = atan(slope);
      float acceleration = GRAVITY * sin(angle);
      
      // Update velocity
      velocity += acceleration;
      velocity *= FRICTION; // Apply friction
      
      // Update position
      position += velocity;
      
      // Bounce at the ends of the curve
      if (position < 0) {
         position = 0;
         velocity = -velocity * 0.8; // Bounce with energy loss
      }
      if (position > 1) {
         position = 1;
         velocity = -velocity * 0.8; // Bounce with energy loss
      }
      
      // Draw the ball
      setcolor(WHITE);
      setfillstyle(SOLID_FILL, BLUE);
      circle(ball_x, ball_y, ball_radius);
      floodfill(ball_x, ball_y, WHITE);
      
      // Display information
      setcolor(WHITE);
      outtextxy(10, 10, "CURVE BALL SIMULATION");
      
      // Display velocity info
      char vel_text[50];
      sprintf(vel_text, "Velocity: %.3f", velocity);
      outtextxy(10, 30, vel_text);
      
      outtextxy(10, 50, "Press ESC to exit");
      
      // Check for keyboard input
      if (kbhit()) {
         key = getch();
      }
      
      // Animation delay
      delay(15);
   }
   
   // Close graphics
   closegraph();
}

/*
 * CURVE BALL SIMULATION
 * 
 * This program simulates a ball rolling back and forth on a curved surface
 * under the influence of gravity, similar to a pendulum.
 * 
 * HOW IT WORKS:
 * 
 * 1. Curve Structure:
 *    - A U-shaped curve is drawn with vertical supports and a horizontal base
 *    - The curve is a parabola, giving it a smooth, bowl-like shape
 * 
 * 2. Ball Physics:
 *    - The ball's position is represented as a point (0 to 1) along the curve
 *    - Gravity creates an acceleration based on the slope at each point:
 *      * At the bottom (position = 0.5), there's no acceleration
 *      * On the sides, there's acceleration toward the center
 *      * Steeper parts of the curve create more acceleration
 *    - Friction gradually reduces the ball's velocity
 * 
 * 3. Ball Movement:
 *    - The ball starts near the right side of the curve
 *    - It rolls down under gravity, gaining speed
 *    - It passes through the bottom and up the other side
 *    - It slows down as it climbs and eventually reverses direction
 *    - This creates a pendulum-like oscillation that gradually dampens
 * 
 * CONTROLS:
 * - Press ESC to exit the program
 * 
 * This simulation demonstrates principles of physics including:
 * - Gravitational force on an inclined surface
 * - Conservation of energy (with some loss due to friction)
 * - Simple harmonic motion similar to a pendulum
 */
