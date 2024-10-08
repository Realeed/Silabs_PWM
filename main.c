/***************************************************************************//**
 * @file main.c
 * @brief main() function.
 *
 *  Created on: Sep 28, 2024
 *      Author: HS
 *
 ******************************************************************************/

#include "pwm.h"

int main(void)
{
  CMU_ClockEnable(cmuClock_GPIO, true);
  CMU_ClockEnable(cmuClock_TIMER0, true);
  CMU_ClockEnable(cmuClock_TIMER1, true);
  CMU_ClockEnable(cmuClock_TIMER2, true);
  CMU_ClockEnable(cmuClock_TIMER3, true);
  CMU_ClockEnable(cmuClock_TIMER4, true);

  PWM_INIT_S pwmInit1 = {
      .pwmFreq = 1000,
      .numOfChsUsed = 3,
      .ch[0] = {
          .port = gpioPortC,
          .pin = 0,
          .dutyCycle1000 = 750,
      },
      .ch[1] = {
          .port = gpioPortC,
          .pin = 1,
          .dutyCycle1000 = 500,
      },
      .ch[2] = {
          .port = gpioPortC,
          .pin = 2,
          .dutyCycle1000 = 250,
      },
  };
  pwm_init(TIMER1, &pwmInit1);
  pwm_start(TIMER1);
  //pwm_setPwmFreq(TIMER1, 5000);
  pwm_setDutyCycle(TIMER1, 1, pwm_getDutyCycle(TIMER1, 1) + 420);
  pwm_setPwmFreq(TIMER1, pwm_getPwmFreq(TIMER1) + 2300);
//  while (1) {
//
//  }

}
