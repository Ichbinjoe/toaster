#ifndef TOASTER_MAX31856_H
#define TOASTER_MAX31856_H

#include <endian.h>
#include <inttypes.h>

typedef uint8_t bit;

#define OCFAULT_DISABLED            0x0
#define OCFAULT_ENABLED_LOWR        0x1
#define OCFAULT_ENABLED_HIGHR_FAST  0x2
#define OCFAULT_ENABLED_HIGHR_SLOW  0x3

struct max31856_cr0
{
    bit cmode       : 1;
    bit oneshot     : 1;
    uint8_t ocfault2: 2;
    bit cj          : 1;
    bit fault       : 1;
    bit faultclr    : 1;
    bit fiftysixty  : 1;
};

#define AVGSEL_1_SAMPLE     0x0
#define AVGSEL_2_SAMPLE     0x1
#define AVGSEL_4_SAMPLE     0x2
#define AVGSEL_8_SAMPLE     0x3
#define AVGSEL_16_SAMPLE    0x4

#define TC_TYPE_B       0x0
#define TC_TYPE_E       0x1
#define TC_TYPE_J       0x2
#define TC_TYPE_K       0x3
#define TC_TYPE_N       0x4
#define TC_TYPE_R       0x5
#define TC_TYPE_S       0x6
#define TC_TYPE_T       0x7
#define TC_VM_GAIN_8    0x8
#define TC_VM_GAIN_32   0xB

struct max31856_cr1
{
    bit     reserved    : 1;
    uint8_t avgsel      : 3;
    uint8_t tc_type     : 4;
};

struct max31856_mask
{
    uint8_t reserved    : 2;
    bit     cjhigh      : 1;
    bit     cjlow       : 1;
    bit     tchigh      : 1;
    bit     tclow       : 1;
    bit     ovuv        : 1;
    bit     open        : 1;
};

struct max31856_sr
{
    bit     cjrange     : 1;
    bit     tcrange     : 1;
    bit     cjhigh      : 1;
    bit     cjlow       : 1;
    bit     tchigh      : 1;
    bit     tclow       : 1;
    bit     ovuv        : 1;
    bit     open        : 1;
};

#define MAX31856_CR0_ADDR   0x0
#define MAX31856_CR1_ADDR   0x1
#define MAX31856_MASK_ADDR  0x2
#define MAX31856_CJHF_ADDR  0x3
#define MAX31856_CJLF_ADDR  0x4
#define MAX31856_LTHFT_ADDR 0x5
#define MAX31856_LTLFT_ADDR 0x7
#define MAX31856_CJTO_ADDR  0x9
#define MAX31856_CJT_ADDR   0xA
#define MAX31856_LTCB_ADDR  0xC
#define MAX31856_SR_ADDR    0xF

struct max31856_hdr
{
    bit     write   : 1;
    uint8_t addr    : 7;
};

extern int max31856_begin();
extern void max31856_transfer(uint8_t cs, void *buf, uint32_t bufn);
extern void max31856_end();

extern int8_t max31856_single1_to_host(void *);
extern float max31856_single2_to_host(void *);
extern float max31856_double1_to_host(void *);
extern float max31856_double2_to_host(void *);
extern float max31856_triple_to_host(void *);

extern void host_to_max31856_single1(int8_t, void *);
extern void host_to_max31856_single2(float, void *);
extern void host_to_max31856_double1(float, void *);
extern void host_to_max31856_double2(float, void *);

#define max31856_CJHFT_to_host  max31856_single1_to_host
#define host_to_max31856_CJHFT  host_to_max31856_single1

#define max31856_CJLFT_to_host  max31856_single1_to_host
#define host_to_max31856_CJLFT  host_to_max31856_single1

#define max31856_LTHFT_to_host  max31856_double1_to_host
#define host_to_max31856_LTHFT  host_to_max31856_double1

#define max31856_LTLFT_to_host  max31856_double1_to_host
#define host_to_max31856_LTLFT  host_to_max31856_double1

#define max31856_CJTO_to_host  max31856_single2_to_host
#define host_to_max31856_CJTO  host_to_max31856_single2

#define max31856_CJTH_to_host  max31856_double2_to_host
#define host_to_max31856_CJTH  host_to_max31856_double2

#define max31856_LTCBH_to_host max31856_triple_to_host

#endif //TOASTER_MAX31856_H
