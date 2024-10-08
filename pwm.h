/*
 * pwm.h
 *
 *  Created on: Sep 28, 2024
 *      Author: HS
 */

#ifndef PWM_H_
#define PWM_H_

#include "em_cmu.h"
#include "em_gpio.h"
#include "em_timer.h"

/** Map TIMER reference to index of device and CMU Clock. */
#if defined(TIMER9)
#define TIMER_DEVICE_ID(timer) ( \
    (timer) == TIMER0     ? 0    \
    : (timer) == TIMER1   ? 1    \
    : (timer) == TIMER2   ? 2    \
    : (timer) == TIMER3   ? 3    \
    : (timer) == TIMER4   ? 4    \
    : (timer) == TIMER5   ? 5    \
    : (timer) == TIMER6   ? 6    \
    : (timer) == TIMER7   ? 7    \
    : (timer) == TIMER8   ? 8    \
    : (timer) == TIMER9   ? 9    \
    : 10)
#define TIMER_2_CMU_CLOCK(timer) ( \
    (timer) == TIMER0     ? cmuClock_TIMER0    \
    : (timer) == TIMER1   ? cmuClock_TIMER1    \
    : (timer) == TIMER2   ? cmuClock_TIMER2    \
    : (timer) == TIMER3   ? cmuClock_TIMER3    \
    : (timer) == TIMER4   ? cmuClock_TIMER4    \
    : (timer) == TIMER5   ? cmuClock_TIMER5    \
    : (timer) == TIMER6   ? cmuClock_TIMER6    \
    : (timer) == TIMER7   ? cmuClock_TIMER7    \
    : (timer) == TIMER8   ? cmuClock_TIMER8    \
    : (timer) == TIMER9   ? cmuClock_TIMER9    \
    : 10)
#elif defined(TIMER7)
#define TIMER_DEVICE_ID(timer) ( \
    (timer) == TIMER0     ? 0    \
    : (timer) == TIMER1   ? 1    \
    : (timer) == TIMER2   ? 2    \
    : (timer) == TIMER3   ? 3    \
    : (timer) == TIMER4   ? 4    \
    : (timer) == TIMER5   ? 5    \
    : (timer) == TIMER6   ? 6    \
    : (timer) == TIMER7   ? 7    \
    : 10)
#define TIMER_2_CMU_CLOCK(timer) ( \
    (timer) == TIMER0     ? cmuClock_TIMER0    \
    : (timer) == TIMER1   ? cmuClock_TIMER1    \
    : (timer) == TIMER2   ? cmuClock_TIMER2    \
    : (timer) == TIMER3   ? cmuClock_TIMER3    \
    : (timer) == TIMER4   ? cmuClock_TIMER4    \
    : (timer) == TIMER5   ? cmuClock_TIMER5    \
    : (timer) == TIMER6   ? cmuClock_TIMER6    \
    : (timer) == TIMER7   ? cmuClock_TIMER7    \
    : 10)
#elif defined(TIMER4)
#define TIMER_DEVICE_ID(timer) ( \
    (timer) == TIMER0   ? 0      \
    : (timer) == TIMER1 ? 1      \
    : (timer) == TIMER2 ? 2      \
    : (timer) == TIMER3 ? 3      \
    : (timer) == TIMER4 ? 4      \
    : 10)
#define TIMER_2_CMU_CLOCK(timer) ( \
    (timer) == TIMER0   ? cmuClock_TIMER0      \
    : (timer) == TIMER1 ? cmuClock_TIMER1      \
    : (timer) == TIMER2 ? cmuClock_TIMER2      \
    : (timer) == TIMER3 ? cmuClock_TIMER3      \
    : (timer) == TIMER4 ? cmuClock_TIMER4      \
    : 10)
#else
#define TIMER_DEVICE_ID(timer) ( \
    (timer) == TIMER0   ? 0      \
    : (timer) == TIMER1 ? 1      \
    : (timer) == TIMER2 ? 2      \
    : (timer) == TIMER3 ? 3      \
    : 10)
#define TIMER_2_CMU_CLOCK(timer) ( \
    (timer) == TIMER0   ? cmuClock_TIMER0      \
    : (timer) == TIMER1 ? cmuClock_TIMER1      \
    : (timer) == TIMER2 ? cmuClock_TIMER2      \
    : (timer) == TIMER3 ? cmuClock_TIMER3      \
    : 10)
#endif

typedef enum
{
  PWM_DONE = 0,
  PWM_INVALID_PARAM = -1,
  PWM_TIMER_NOT_INITIALIZED = -2,

} PWM_RETURN_E;

typedef struct
{
    GPIO_Port_TypeDef port;
    uint8_t pin;
    uint32_t dutyCycle1000;

} CC_CH_S;

typedef struct
{
    uint32_t pwmFreq;
    uint8_t numOfChsUsed;
    CC_CH_S ch[3];

} PWM_INIT_S;

PWM_RETURN_E pwm_init(TIMER_TypeDef *timer, PWM_INIT_S *pwmInit);
PWM_RETURN_E pwm_start(TIMER_TypeDef *timer);
PWM_RETURN_E pwm_stop(TIMER_TypeDef *timer);
PWM_RETURN_E pwm_setPwmFreq(TIMER_TypeDef *timer, uint32_t pwmFreq);
uint32_t pwm_getPwmFreq(TIMER_TypeDef *timer);
PWM_RETURN_E pwm_setDutyCycle(TIMER_TypeDef *timer, uint8_t ch, uint32_t dutyCycle1000);
uint32_t pwm_getDutyCycle(TIMER_TypeDef *timer, uint8_t ch);


#endif /* PWM_H_ */
