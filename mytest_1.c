//This test case is non-original and is just testing the "one process" example given on the handout
//This file is exactly the same as test_1.c, but without all the code for the p2 process

#include "3140_concur.h"
#include "led.h"

void p1 (void)
{
	int i;
	for (i=0; i < 20; i++) {  //Make the process run for longer by changing the amount of times toggled to 20
		delay(500);
    	red_toggle_frdm();        //Could have changed to green_toggle_frdm to toggle the green LED, but the process is the same
	}
}


int main (void){
  led_init();
  set15MHz();

 if (process_create (p1,32) < 0) {
 	return -1;
 }

 process_start ();

 /*
After process_start p1 should run. The process p1 is with the red LED (should blink on 10 times, off 10 times).
*/

 return 0;
}
