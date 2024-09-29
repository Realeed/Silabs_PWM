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
      .pwmFreq = 700,
      .numOfChsUsed = 3,
      .ch[0] = {
          .port = gpioPortB,
          .pin = 0,
          .dutyCycle1000 = 100,
      },
      .ch[1] = {
          .port = gpioPortA,
          .pin = 7,
          .dutyCycle1000 = 100,
      },
      .ch[2] = {
          .port = gpioPortC,
          .pin = 6,
          .dutyCycle1000 = 100,
      },
  };
  pwm_init(TIMER1, &pwmInit1);
  pwm_start(TIMER1);
  pwm_setPwmFreq(TIMER1, 5000);
}
