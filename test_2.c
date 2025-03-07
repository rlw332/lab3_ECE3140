#include "3140_concur.h"
#include "led.h"


/*
 * Global variables used by all 3 processes
 */
volatile grb32_t val1 = {0,0,0,0};
volatile grb32_t val2 = {0,0,0,0};


void p1 (void){
	int up=1;
	while(1){
		if(up){
			val1.red += 2;
			if(val1.red > 200){
				up=0;
			}
		}else{
			val1.red -= 2;
			if(val1.red < 10){
				up=1;
			}
		}
		delay(11);
	}
}

void p2 (void){
	int up=1;
	while(1){
		if(up){
			val2.green += 1;
			val2.red += 1;

			if(val2.green > 100){
				up=0;
			}
		}else{
			val2.green -= 1;
			val2.red -= 1;

			if(val2.green < 10){
				up=1;
			}
		}
		delay(10); //Different delay from p1
	}
}

void p3 (void){
	while(1){
		__disable_irq(); //globally disable interrupts
		set_led(val1);
		set_led(val2);
		__enable_irq(); //enable interrupts again so that other
						//processes get a chance to run
		delay(9);
	}
}


int main (void){
	led_init();
 

 if (process_create (p1,32) < 0) {
 	return -1;
 }

 if (process_create (p2,32) < 0) {
 	return -1;
 }
 	
 if (process_create (p3,32) < 0) {
  	return -1;
  }

 process_start ();

 /*
After process_start p1,p2, and p3 should be running concurrently.
P1 manipulates the RGB values for LED1, and p2 manipulates the RGB
values for LED2, and p3 writes out both values to the LEDs.
The pulsing frequencies of the two LEDs are off by about 10%.

Think about how messy the code would if there was only one process!
The updates would be different, and whenever you want to set LED2, you
need to set LED1 first!

Here you can treat each conceptually separate thing independently.
*/


 return 0; //execution should never get here.
}
