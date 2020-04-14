#include "contiki.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"
#include "dev/leds.h"

#include <string.h>

/*---------------------------------------------------------------------------*/
PROCESS(lab1_slave, "Lab1 Slave");
AUTOSTART_PROCESSES(&lab1_slave);

/*---------------------------------------------------------------------------*/
void input_callback(const void *data, uint16_t len,
                    const linkaddr_t *src, const linkaddr_t *dest)
{
    if (len == sizeof(unsigned))
    {
        unsigned int led;
        memcpy(&led, data, sizeof(led));
        leds_toggle(led);
    }
}

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(lab1_slave, ev, data)
{
    PROCESS_BEGIN();

    /* Initialize NullNet */
    nullnet_set_input_callback(input_callback);

    while (1)
        PROCESS_YIELD();

    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
