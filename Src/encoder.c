#include "encoder.h"
#include "stm32f4xx_hal_exti.h"
#include "stm32f4xx_hal_gpio.h"
#include <math.h>

#define ppr1 537.6
#define ppr2 537.6

#define PREV_MASK 0x1 //Mask for the previous state in determining direction
//of rotation.
#define CURR_MASK 0x2 //Mask for the current state in determining direction
//of rotation.
#define INVALID   0x3 //XORing two states where both bits have changed.

int state_encoder_a1 = 0;
int state_encoder_b1 = 0;
int currstate1 = 0;
int prevstate1 = 0;
int change1 = 0;
volatile int pulses1 = 0;

int state_encoder_a2 = 0;
int state_encoder_b2 = 0;
int currstate2 = 0;
int prevstate2 = 0;
int change2 = 0;
volatile int pulses2 = 0;

void encoder_init() {

	state_encoder_a1 = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0);
	state_encoder_b1 = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1);
	currstate1 = (state_encoder_a1 << 1) | state_encoder_b1;
	prevstate1 = currstate1;
	//TM_EXTI_Attach(GPIOD, GPIO_Pin_0, TM_EXTI_Trigger_Rising_Falling);
	//TM_EXTI_Attach(GPIOD, GPIO_Pin_1, TM_EXTI_Trigger_Rising_Falling);
}

void enc1_reset() {
  pulses1 = 0;
}
void enc2_reset() {
  pulses2 = 0;
}

double enc1() {
  return (double)pulses1/ppr1*360.0;
}
double enc2() {
  return (double)pulses2/ppr2*360.0;
}
//HAL_EXTI_SetConfigLine();
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* Prevent unused argument(s) compilation warning */
  //UNUSED(GPIO_Pin);
	if (GPIO_Pin == GPIO_PIN_0) {
		ext_handler1();
	}
	if (GPIO_Pin == GPIO_PIN_1) {
		ext_handler1();
	}
  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_GPIO_EXTI_Callback could be implemented in the user file
   */
}
void ext_handler1() {
	change1 = 0;
	state_encoder_a1 = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0);
	state_encoder_b1 = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0);
	currstate1 = (state_encoder_a1 << 1) | state_encoder_b1;
	if (((currstate1 ^ prevstate1) != INVALID) && (currstate1 != prevstate1)) {
		change1 = (prevstate1 & PREV_MASK) ^ ((currstate1 & CURR_MASK) >> 1);

		if (change1 == 0) {
			change1 = -1;
		}

		pulses1 += change1;
	}
    prevstate1 = currstate1;
}