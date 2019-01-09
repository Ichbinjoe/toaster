#include "toaster.h"

#include <bcm2835.h>

#include "max31856.h"

void setup_thermo(uint8_t cs, uint8_t mode)
{
    // so i'm lazy and am just going to set the thermocouples into constant poll mode
    struct {
        uint8_t addr;
        struct max31856_cr0 cr0;
        struct max31856_cr1 cr1;
        struct max31856_mask mask;
    } query = {
            0x80 + MAX31856_CR0_ADDR,
            {1, // cmode
             0, // oneshot
             OCFAULT_ENABLED_LOWR, // ocfault2
             0, // cj
             0, // fault
             1, // faultclr
             0}, // Reject 60Hz
            {0, // reserved
             AVGSEL_16_SAMPLE, // avgsel
             mode // tc_type
            }
    };

    max31856_transfer(cs, &query, sizeof(query));
}

int toaster_init(uint8_t thermo1_mode, uint8_t thermo2_mode)
{
    if (!bcm2835_init())
        return 0;

    if (!max31856_begin())
        return 0;

    bcm2835_gpio_fsel(TOASTER_HEAT_ENABLE_GPIO_PIN, BCM2835_GPIO_FSEL_INPT);

    bcm2835_gpio_fsel(TOASTER_THERMO1_DRDY_GPIO_PIN, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(TOASTER_THERMO1_FAULT_GPIO_PIN, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(TOASTER_THERMO2_DRDY_GPIO_PIN, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(TOASTER_THERMO2_FAULT_GPIO_PIN, BCM2835_GPIO_FSEL_INPT);

    // alt5 is pwm
    bcm2835_gpio_fsel(TOASTER_SERVO_PWM_GPIO_PIN, BCM2835_GPIO_FSEL_ALT5);
    bcm2835_gpio_fsel(TOASTER_BUZZER_PWM_GPIO_PIN, BCM2835_GPIO_FSEL_ALT5);

    // subdivided to 37500Hz
    bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_512);

    // we are targeting 200Hz for the servo
    bcm2835_pwm_set_range(TOASTER_SERVO_PWM_CHANNEL, 188);

    setup_thermo(0, thermo1_mode);
    setup_thermo(1, thermo2_mode);

    return 1;
}

int toaster_close()
{
    max31856_end();
    return bcm2835_close();
}

void toaster_setActivate(int activate)
{
    if (activate)
        bcm2835_gpio_set(TOASTER_HEAT_ENABLE_GPIO_PIN);
    else
        bcm2835_gpio_clr(TOASTER_HEAT_ENABLE_GPIO_PIN);
}

void toaster_buzzer_off()
{
    bcm2835_pwm_set_data(TOASTER_BUZZER_PWM_CHANNEL, 0);
}

void toaster_buzzer_hz(uint32_t hz)
{
    uint32_t range = 37500 / hz;
    uint32_t data = range / 2;
    bcm2835_pwm_set_range(TOASTER_BUZZER_PWM_CHANNEL, range);
    bcm2835_pwm_set_data(TOASTER_BUZZER_PWM_CHANNEL, data);
}

void toaster_servo_set(float v)
{
    bcm2835_pwm_set_data(TOASTER_SERVO_PWM_CHANNEL, (uint32_t) (v * 188));
}

int toaster_t1_hasfault()
{
    return bcm2835_gpio_lev(TOASTER_THERMO1_FAULT_GPIO_PIN) == 0;
}

int toaster_t2_hasfault()
{
    return bcm2835_gpio_lev(TOASTER_THERMO2_FAULT_GPIO_PIN) == 0;
}

int toaster_t1_datardy()
{
    return bcm2835_gpio_lev(TOASTER_THERMO1_DRDY_GPIO_PIN) == 0;
}

int toaster_t2_datardy()
{
    return bcm2835_gpio_lev(TOASTER_THERMO2_DRDY_GPIO_PIN) == 0;
}

struct max31856_sr toaster_thermo_readfault(int thermo)
{
    struct {
        uint8_t addr;
        struct max31856_sr sr;
    } buf;
    buf.addr = MAX31856_SR_ADDR;

    max31856_transfer((uint8_t)thermo, &buf, sizeof(buf));

    return buf.sr;
}

struct max31856_sr toaster_thermo_readtemp(int thermo, float *value)
{
    struct {
        uint8_t addr;
        uint8_t reading[3];
        struct max31856_sr sr;
    } buf;
    buf.addr = MAX31856_LTCB_ADDR;

    max31856_transfer((uint8_t)thermo, &buf, sizeof(buf));

    *value = max31856_LTCBH_to_host(buf.reading);
    return buf.sr;
}




