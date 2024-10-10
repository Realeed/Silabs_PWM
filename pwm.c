/*
 * pwm.c
 *
 *  Created on: Sep 28, 2024
 *      Author: HS
 */

#include "pwm.h"

bool timerInitialized[MAX_POSSIBLE_NUM_OF_TIMERS + 1] = {
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
};

PWM_RETURN_E pwm_setDutyCycle(TIMER_TypeDef *timer, uint8_t ch, uint16_t dutyCycle1000)
{
  if (!timerInitialized[TIMER_DEVICE_ID(timer)]) {
      return PWM_TIMER_NOT_INITIALIZED;
  }
  if (dutyCycle1000 > DIMMING_SCALE) {
      return PWM_INVALID_PARAM;
  }
  uint32_t top = TIMER_TopGet(timer);
  uint32_t compare = (top+1) * dutyCycle1000 / DIMMING_SCALE;
  TIMER_CompareSet(timer, ch, compare);

  return PWM_DONE;
}

PWM_RETURN_E pwm_setPwmFreq(TIMER_TypeDef *timer, uint32_t pwmFreq)
{
  if (!timerInitialized[TIMER_DEVICE_ID(timer)]) {
      return PWM_TIMER_NOT_INITIALIZED;
  }
  if (pwmFreq < MIN_PWM_FREQUENCY) {
      return PWM_INVALID_PARAM;
  }
  CMU_Clock_TypeDef clock = TIMER_2_CMU_CLOCK(timer);
  uint32_t clockFreq = CMU_ClockFreqGet(clock);
  uint32_t currentTop = TIMER_TopGet(timer);
  uint32_t top = clockFreq / pwmFreq - 1;
  TIMER_TopSet(timer, top);

  for (int i = 0; i < MAX_NUM_OF_CHANNELS; i++) {
      uint32_t dutyCycle1000 = timer->CC[i].OC * DIMMING_SCALE / (currentTop + 1);
      pwm_setDutyCycle(timer, i, dutyCycle1000);
  }

  return PWM_DONE;
}

PWM_RETURN_E pwm_start(TIMER_TypeDef *timer)
{
  if (!timerInitialized[TIMER_DEVICE_ID(timer)]) {
      return PWM_TIMER_NOT_INITIALIZED;
  }
  TIMER_Enable(timer, true);

  return PWM_DONE;
}

PWM_RETURN_E pwm_stop(TIMER_TypeDef *timer)
{
  if (!timerInitialized[TIMER_DEVICE_ID(timer)]) {
      return PWM_TIMER_NOT_INITIALIZED;
  }
  TIMER_Enable(timer, false);
  TIMER_CounterSet(timer, _TIMER_CNT_RESETVALUE);

  return PWM_DONE;
}

PWM_RETURN_E pwm_init(TIMER_TypeDef *timer, PWM_INIT_S *pwmInit)
{
  if ((pwmInit->numOfChsUsed > MAX_NUM_OF_CHANNELS) || (pwmInit->numOfChsUsed < MIN_NUM_OF_CHANNELS)
      || (pwmInit->pwmFreq < MIN_PWM_FREQUENCY)
      || (pwmInit->ch[0].dutyCycle1000 > DIMMING_SCALE)
      || (pwmInit->ch[1].dutyCycle1000 > DIMMING_SCALE)
      || (pwmInit->ch[2].dutyCycle1000 > DIMMING_SCALE)) {
      return PWM_INVALID_PARAM;
  }

  uint8_t timerID = TIMER_DEVICE_ID(timer);

  for (int i = 0; i < pwmInit->numOfChsUsed; i++) {
      GPIO_PinModeSet(pwmInit->ch[i].port, pwmInit->ch[i].pin, gpioModePushPull, GPIO_OUT_LOW);
      GPIO->TIMERROUTE_SET[timerID].ROUTEEN = (GPIO_TIMER_ROUTEEN_CC_PIN_ENABLE_VAL << i);
  }

  switch (pwmInit->numOfChsUsed) {
    case 1:
     GPIO->TIMERROUTE_SET[timerID].CC0ROUTE = ((pwmInit->ch[0].pin << GPIO_TIMER_CCROUTE_PIN_SHIFT) | pwmInit->ch[0].port);
     break;
    case 2:
     GPIO->TIMERROUTE_SET[timerID].CC0ROUTE = ((pwmInit->ch[0].pin << GPIO_TIMER_CCROUTE_PIN_SHIFT) | pwmInit->ch[0].port);
     GPIO->TIMERROUTE_SET[timerID].CC1ROUTE = ((pwmInit->ch[1].pin << GPIO_TIMER_CCROUTE_PIN_SHIFT) | pwmInit->ch[1].port);
     break;
    case 3:
     GPIO->TIMERROUTE_SET[timerID].CC0ROUTE = ((pwmInit->ch[0].pin << GPIO_TIMER_CCROUTE_PIN_SHIFT) | pwmInit->ch[0].port);
     GPIO->TIMERROUTE_SET[timerID].CC1ROUTE = ((pwmInit->ch[1].pin << GPIO_TIMER_CCROUTE_PIN_SHIFT) | pwmInit->ch[1].port);
     GPIO->TIMERROUTE_SET[timerID].CC2ROUTE = ((pwmInit->ch[2].pin << GPIO_TIMER_CCROUTE_PIN_SHIFT) | pwmInit->ch[2].port);
     break;
  }

  TIMER_Init_TypeDef timerInit = TIMER_INIT_DEFAULT;
  timerInit.enable = false;
  TIMER_Init(timer, &timerInit);

  TIMER_InitCC_TypeDef ccInit = TIMER_INITCC_DEFAULT;
  ccInit.mode = timerCCModePWM;
  for (int i = 0; i < pwmInit->numOfChsUsed; i++) {
      TIMER_InitCC(timer, i, &ccInit);
  }
  timerInitialized[timerID] = true;
  pwm_setPwmFreq(timer, pwmInit->pwmFreq);

  for (int i = 0; i < pwmInit->numOfChsUsed; i++) {
      pwm_setDutyCycle(timer, i, pwmInit->ch[i].dutyCycle1000);
  }

  return PWM_DONE;
}
