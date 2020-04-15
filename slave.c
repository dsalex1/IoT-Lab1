#include "contiki.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"
#include "dev/leds.h"
#include "node-id.h"

#include <random.h>
#include <string.h>

/*---------------------------------------------------------------------------*/
PROCESS(lab1_slave, "Lab1 Slave");
AUTOSTART_PROCESSES(&lab1_slave);

/*---------------------------------------------------------------------------*/
void input_callback(const void *data, uint16_t len,
                    const linkaddr_t *src, const linkaddr_t *dest)
{
    if (len == sizeof(uint8_t))
    {
        unsigned int led;
        memcpy(&led, data, sizeof(led));
        leds_toggle(led);
    }
}
/*---------------------------------------------------------------------------*/
void sendTemp(int16_t temp)
{
    nullnet_buf = (uint8_t *)&temp;
    nullnet_len = sizeof(temp);
    NETSTACK_NETWORK.output(NULL);
}

unsigned short rseed;

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(lab1_slave, ev, data)
{
    PROCESS_BEGIN();

    static struct etimer et;

    etimer_set(&et, CLOCK_SECOND);

    /*make random actually random...damn hardcoded seed*/
    random_init(node_id);
    /* Initialize NullNet */
    nullnet_set_input_callback(input_callback);

    while (1)
    {
        PROCESS_YIELD();

        if (ev == PROCESS_EVENT_TIMER && data == &et)
        {
            sendTemp(random_rand() % 200 - 100);
            etimer_set(&et, CLOCK_SECOND);
        }
    }

    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
