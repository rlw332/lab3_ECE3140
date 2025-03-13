//This test case is non-original and is just testing the "one process" example given on the handout
//This file is exactly the same as test_1.c, but without all the code for the p2 process

#include "3140_concur.h"
#include "led.h"

void p1 (void)
{
	int i;
	for (i=0; i < 20; i++) {  //Make the process run for longer
		delay(500);
    	red_toggle_frdm();
	}
}


int main (void){
  led_init();
  set15MHz();
  set15MHz();

 if (process_create (p1,32) < 0) {
 	return -1;
 }

 process_start ();

 /*
After process_start p1 and p2 should be running concurrently.
Both LEDs should blink. The process with the red LED should
finish first, and after that the green LED should blink a few
more times, but twice as fast. Why?
*/

 return 0;
}
