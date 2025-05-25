#include <graphics.h>
#include <conio.h>
#include <dos.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

#define PI 3.14159265

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
   
   // Clock properties
   int clock_radius = screen_height / 3;
   int hour_hand_length = clock_radius * 0.6;
   int minute_hand_length = clock_radius * 0.8;
   int second_hand_length = clock_radius * 0.9;
   
   // Time variables
   struct time t;
   int hours, minutes, seconds;
   float hour_angle, minute_angle, second_angle;
   int i; // Loop counter declared once at the beginning
   
   // Text buffer for displaying digital time
   char time_str[50];
   
   // Animation loop
   char key = 0;
   while (key != 27) { // ESC to exit
      // Clear the screen
      cleardevice();
      
      // Get current time
      gettime(&t);
      hours = t.ti_hour;
      minutes = t.ti_min;
      seconds = t.ti_sec;
      
      // Convert to 12-hour format for the analog clock
      if (hours > 12) hours -= 12;
      if (hours == 0) hours = 12;
      
      // Calculate angles for clock hands
      // Hour hand: 30 degrees per hour + gradual movement based on minutes
      hour_angle = (hours * 30) + (minutes * 0.5) - 90;
      
      // Minute hand: 6 degrees per minute + gradual movement based on seconds
      minute_angle = (minutes * 6) + (seconds * 0.1) - 90;
      
      // Second hand: 6 degrees per second
      second_angle = (seconds * 6) - 90;
      
      // Convert angles to radians
      hour_angle = hour_angle * PI / 180;
      minute_angle = minute_angle * PI / 180;
      second_angle = second_angle * PI / 180;
      
      // Draw clock face (circle)
      setcolor(WHITE);
      circle(center_x, center_y, clock_radius);
      
      // Draw hour marks
      for (i = 0; i < 12; i++) {
         float angle = i * 30 * PI / 180;
         int outer_x = center_x + cos(angle) * clock_radius;
         int outer_y = center_y + sin(angle) * clock_radius;
         int inner_x = center_x + cos(angle) * (clock_radius - 10);
         int inner_y = center_y + sin(angle) * (clock_radius - 10);
         
         setcolor(WHITE);
         line(inner_x, inner_y, outer_x, outer_y);
      }
      
      // Draw minute marks
      for (i = 0; i < 60; i++) {
         // Skip positions where hour marks are
         if (i % 5 == 0) continue;
         
         float angle = i * 6 * PI / 180;
         int outer_x = center_x + cos(angle) * clock_radius;
         int outer_y = center_y + sin(angle) * clock_radius;
         int inner_x = center_x + cos(angle) * (clock_radius - 5);
         int inner_y = center_y + sin(angle) * (clock_radius - 5);
         
         setcolor(WHITE);
         line(inner_x, inner_y, outer_x, outer_y);
      }
      
      // Draw hour hand
      setcolor(WHITE);
      setlinestyle(SOLID_LINE, 0, 3); // Thicker line
      line(center_x, center_y, 
           center_x + cos(hour_angle) * hour_hand_length, 
           center_y + sin(hour_angle) * hour_hand_length);
      
      // Draw minute hand
      setcolor(WHITE);
      setlinestyle(SOLID_LINE, 0, 2); // Medium thickness
      line(center_x, center_y, 
           center_x + cos(minute_angle) * minute_hand_length, 
           center_y + sin(minute_angle) * minute_hand_length);
      
      // Draw second hand
      setcolor(RED);
      setlinestyle(SOLID_LINE, 0, 1); // Thin line
      line(center_x, center_y, 
           center_x + cos(second_angle) * second_hand_length, 
           center_y + sin(second_angle) * second_hand_length);
      
      // Reset line style
      setlinestyle(SOLID_LINE, 0, 1);
      
      // Draw center dot
      setcolor(WHITE);
      setfillstyle(SOLID_FILL, WHITE);
      circle(center_x, center_y, 5);
      floodfill(center_x, center_y, WHITE);
      
      // Display digital time
      sprintf(time_str, "Time: %02d:%02d:%02d", t.ti_hour, t.ti_min, t.ti_sec);
      setcolor(WHITE);
      outtextxy(center_x - 50, center_y + clock_radius + 20, time_str);
      
      // Display instructions
      outtextxy(10, 10, "ANALOG CLOCK");
      outtextxy(10, 30, "Press ESC to exit");
      
      // Check for keyboard input
      if (kbhit()) {
         key = getch();
      }
      
      // Delay for one second
      delay(1000);
   }
   
   // Close graphics
   closegraph();
}

/*
 * ANALOG CLOCK SIMULATION
 * 
 * This program creates a realistic analog clock with hour, minute, and second hands.
 * The clock shows the current system time and updates every second.
 * 
 * HOW IT WORKS:
 * 
 * 1. Clock Structure:
 *    - The clock face is drawn as a circle
 *    - Hour marks are thicker lines at 12, 1, 2, etc. positions
 *    - Minute marks are thinner lines between the hour marks
 * 
 * 2. Clock Hands:
 *    - Hour hand: Thick, shorter hand that makes a full rotation every 12 hours
 *    - Minute hand: Medium thickness, longer hand that makes a full rotation every 60 minutes
 *    - Second hand: Thin red hand that makes a full rotation every 60 seconds
 * 
 * 3. Time Calculation:
 *    - The system time is retrieved using the gettime() function
 *    - Hour angle: 30 degrees per hour plus a gradual movement based on minutes
 *    - Minute angle: 6 degrees per minute plus a gradual movement based on seconds
 *    - Second angle: 6 degrees per second
 * 
 * 4. Animation:
 *    - The clock updates every second to show the current time
 *    - A digital display shows the exact time below the clock
 * 
 * CONTROLS:
 * - Press ESC to exit the program
 * 
 * NOTES:
 * - The clock uses a 12-hour format for the analog display
 * - The digital display uses the system's hour format (24-hour)
 */
