#include "delay.h"

void Delay_us(uint16_t us){
	/***********STEPS*********
	1.RESET the Counter
	2.Wait for the Counter to reach the entrered value.As each count will take 1 us
	*/
	TIM11->CNT=0;
	while(TIM11->CNT < us);
}

void Delay_ms(uint16_t ms){
	for (uint16_t i=0;i<ms ;i++){
		Delay_us(1000);
	}
	
}
