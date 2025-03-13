#include "3140_concur.h"
#include "led.h"

void p1 (void)
{
	int i;
	for (i=0; i < 15; i++) {
		delay(100);
    	red_toggle_frdm();
	}
}

void p2 (void)
{
	int i;
	for (i=0; i < 10; i++) {
		delay(100);
    	green_toggle_frdm();

	}
}

volatile grb32_t val1 = {0,0,0,0};
volatile grb32_t val2 = {0,0,0,0};


void p3 (void){
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
		delay(4);
	}
}

void p4 (void){
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
		delay(3); //Different delay from p1
	}
}

void p5 (void){
	while(1){
		__disable_irq(); //globally disable interrupts
		set_led(val1);
		set_led(val2);
		__enable_irq(); //enable interrupts again so that other
						//processes get a chance to run
		delay(2);
	}
}


int main (void){
	led_init();

	set15MHz();

 if (process_create (p1,32) < 0) {
	return -1;
 }

 if (process_create (p2,32) < 0) {
 	return -1;
 }


 if (process_create (p3,32) < 0) {
 	return -1;
 }

 if (process_create (p4,32) < 0) {
 	return -1;
 }

 if (process_create (p5,32) < 0) {
  	return -1;
  }


 process_start ();

 //We thought memory might be the issue so we changed 32 -> 64, but nothing different happened

 /*
After process_start p1 and p2 should be running concurrently.
Both LEDs should blink. The process with the red LED should
finish first, and after that the green LED should blink a few
more times, but twice as fast. Why?
*/

 return 0;
}
