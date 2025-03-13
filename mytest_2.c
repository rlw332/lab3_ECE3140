//This test case is original and is testing if the board can handle many running processes across both boards
//Two of the processes are finite and will be removed from the queue after their execution
//This test case has all procedures from both test_1.c and test_2.c into one file 

#include "3140_concur.h"
#include "led.h"

//Finite processes from test_1.c
void p1 (void)
{
	int i;
	for (i=0; i < 15; i++) {  //Odd number should leave the red LED on when finished
		delay(100);    //Made delay shorter as there were many running processes all at once (and was pretty slow with 500 as delay)
    	red_toggle_frdm();
	}
}

void p2 (void)
{
	int i;
	for (i=0; i < 10; i++) {  //Even number should toggle the green LED off when finished
		delay(100);     //Made delay shorter as there were many running processes all at once (and was pretty slow with 500 as delay)
    	green_toggle_frdm();

	}
}

//Ininite processes from test_2.c (changed names to p3-p5)
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
		delay(4); //Reduced delay
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
		delay(3); //Reduced delay
	}
}

void p5 (void){
	while(1){
		__disable_irq(); //globally disable interrupts
		set_led(val1);
		set_led(val2);
		__enable_irq(); //enable interrupts again so that other
						//processes get a chance to run
		delay(2); //Reduced delay
	}
}


int main (void){     //All processes now under main()
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

 /*
After process_start p1 - p5 should be running concurrently.
Both LEDs on the LED board and the FRDM board should behave as they did in the two provided test cases.
The green FRDM led process should finish first (turning off), and the the red FRDM led process (should remain on). 
The processes p3 - p5 should continuously execute after p1 and p2 finish.
*/

 return 0;
}
