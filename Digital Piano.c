// Lab6.c
// Runs on TM4C123
// Use SysTick interrupts to implement a 4-key digital piano
// EE319K lab6 starter
// Program written by: put your names here
// Date Created: 3/6/17 
// Last Modified: 1/11/22  
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE*******

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "Sound.h"
#include "Key.h"
#include "Music.h"
#include "Lab6Grader.h"
// put both EIDs in the next two lines
char EID1[] = "dl36526"; //  ;replace abc123 with your EID
char EID2[] = "ak46428"; //  ;replace abc123 with your EID

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void DAC_Init(void);          // your lab 6 solution
void DAC_Out(uint8_t data);   // your lab 6 solution
uint8_t Testdata;

// lab video Lab6_voltmeter, Program 6.1
// A simple program that outputs sixteen DAC values. Use this main if you have a voltmeter.
const uint32_t Inputs[16]={0,1,7,8,15,16,17,18,31,32,33,47,48,49,62,63};
int voltmetermain(void){ uint32_t i;  
  DisableInterrupts();
  TExaS_Init(SCOPE);    
  LaunchPad_Init();
  DAC_Init(); // your lab 6 solution
  i = 0;
  EnableInterrupts();
  while(1){                
    Testdata = Inputs[i];
    DAC_Out(Testdata); // your lab 6 solution
    i=(i+1)&0x0F;  // <---put a breakpoint here
  }
}

// DelayMs
//  - busy wait n milliseconds
// Input: time to wait in msec
// Outputs: none
void static DelayMs(uint32_t n){
  volatile uint32_t time;
  while(n){
    time = 6665;  // 1msec, tuned at 80 MHz
    while(time){
      time--;
    }
    n--;
  }
}
// lab video Lab6_static. Program 6.2
// A simple program that outputs sixteen DAC values. 
// Use this main if you do not have a voltmeter. 
// Connect PD3 to your DACOUT and observe the voltage using TExaSdisplay in scope mode.
int staticmain(void){  
  uint32_t last,now,i;  
  DisableInterrupts();
  TExaS_Init(SCOPE);    // bus clock at 80 MHz
  LaunchPad_Init();
  DAC_Init(); // your lab 6 solution
  i = 0;
  EnableInterrupts();
  last = LaunchPad_Input();
  while(1){                
    now = LaunchPad_Input();
    if((last != now)&&now){
      Testdata = Inputs[i];
      DAC_Out(Testdata); // your lab 6 solution
      i=(i+1)&0x0F;
    }
    last = now;
    DelayMs(25);   // debounces switch
  }
}
//heartbeat function
// input: None
// output: None
// function: LED flashes on and off to visualize that the program is running correctly.
void heartBeat(void){
			GPIO_PORTF_DATA_R ^= 0x02; 
}

     
int main(void){       
  DisableInterrupts();
  TExaS_Init(REALBOARDGRADER);    // bus clock at 80 MHz
  Key_Init();
  LaunchPad_Init();
  Sound_Init();
  Music_Init();
  EnableInterrupts();
	uint32_t heart = 0;
  while(1){
		
		if(heart == 800000){
			heartBeat();
			heart = 0;
		}
		uint8_t keys = Key_In();
		uint32_t period = 0;
		if(keys == 1)  //key 0
		{
			period = 3792; //  329.6 HZ, E0
			Sound_Start(period);
		}
		else if(keys == 2)  //key 1
		{
			period = 3010; // 415.3 HZ, AF0
			Sound_Start(period);
		}
		else if(keys == 4)  //key 2
		{
			period = 2531; //493.9 HZ, B0
			Sound_Start(period);
		}
		else if (keys == 8)   //key 3
		{
			period = 2255; //554.4 HZ, DF
			Sound_Start(period);
		}
		else
			Sound_Start(0);
		heart++;
  }             
}


