#ifndef TOASTER_TOASTER_H
#define TOASTER_TOASTER_H

#include <inttypes.h>
#include "max31856.h"

#define TOASTER_HEAT_ENABLE_GPIO_PIN 16

#define TOASTER_SERVO_PWM_GPIO_PIN  18
#define TOASTER_SERVO_PWM_CHANNEL 0

#define TOASTER_BUZZER_PWM_GPIO_PIN 13
#define TOASTER_BUZZER_PWM_CHANNEL 1

#define TOASTER_THERMO1_DRDY_GPIO_PIN   22
#define TOASTER_THERMO1_FAULT_GPIO_PIN   23
#define TOASTER_THERMO2_DRDY_GPIO_PIN   24
#define TOASTER_THERMO2_FAULT_GPIO_PIN   25

int toaster_init(uint8_t thermo1_mode, uint8_t thermo2_mode);
int toaster_close();

void toaster_setActivate(int activate);

void toaster_buzzer_off();
void toaster_buzzer_hz(uint32_t hz);

void toaster_servo_set(float v);

int toaster_t1_hasfault();
int toaster_t2_hasfault();

int toaster_t1_datardy();
int toaster_t2_datardy();

struct max31856_sr toaster_thermo_readtemp(int thermo, float *value);

#endif //TOASTER_TOASTER_H
