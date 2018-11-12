#include "max31856.h"

#include <endian.h>

#include "bcm2835.h"

int max31856_begin()
{
    if (!bcm2835_spi_begin())
        return 0;

    bcm2835_spi_set_speed_hz(5000000); // 5mhz
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
    bcm2835_spi_setChipSelectPolarity(0, 0); // Active low
    bcm2835_spi_setChipSelectPolarity(1, 0); // Active low

    return 1;
}

void max31856_transfer(uint8_t cs, void *buf, uint32_t bufn)
{
    bcm2835_spi_chipSelect(cs);
    bcm2835_spi_transfern(buf, bufn);
}

void max31856_end()
{
    bcm2835_spi_end();
}

int8_t max31856_single1_to_host(void *a)
{
    uint8_t v = *(uint8_t*)a;
    if (v & 0x80)
        return (int8_t)(v & 0x7f);
    else
        return -(int8_t)(v & 0x7f);
}

float max31856_single2_to_host(void *a)
{
    return ((float)max31856_single1_to_host(a)) / 16;
}

int16_t max31856_double_to_host(void *a)
{
    uint16_t v = be16toh(*(uint16_t*)a);
    if (v & 0x8000)
        return (int16_t)(v & 0x7fff);
    else
        return -(int16_t)(v & 0x7fff);

}

float max31856_double1_to_host(void *a)
{
    return ((float)max31856_double_to_host(a)) / 16;
}

float max31856_double2_to_host(void *a)
{
    return ((float)max31856_double_to_host(a)) / 256;
}

float max31856_triple_to_host(void *a)
{
    uint32_t v = be32toh(*(uint32_t*)a);
    int32_t q = (int32_t)(v & 0x7fffff);
    if (!(v & 0x800000))
        q = -q;

    return ((float)q) / 4096;
}

void host_to_max31856_single1(int8_t v, void *a)
{
    *(uint8_t*) a = (uint8_t)((v > 0 ? 0x80 : 0x00) | (v & 0x7f));
}

void host_to_max31856_single2(float v, void *a)
{
    host_to_max31856_single1((int8_t) (v * 16), a);
}

void host_to_max31856_double(int16_t v, void *a)
{
    *(uint16_t*) a = htobe16((uint16_t)((v > 0 ? 0x8000 : 0x0000) | (v & 0x7fff)));
}

void host_to_max31856_double1(float v, void *a)
{
    host_to_max31856_double((int16_t)(v / 16), a);
}

void host_to_max31856_double2(float v, void *a)
{
    host_to_max31856_double((int16_t)(v / 256), a);
}
