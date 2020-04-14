#include "contiki.h"
#include "sys/etimer.h"

#include "dev/leds.h"

#include "net/netstack.h"
#include "net/nullnet/nullnet.h"

#include <random.h>
#include <stdio.h>

/*---------------------------------------------------------------------------*/
PROCESS(lab1_master, "Lab1 Master");
AUTOSTART_PROCESSES(&lab1_master);

/*---------------------------------------------------------------------------*/
void toggleLedAndBroadcast(uint16_t led)
{
  leds_toggle(led);
  nullnet_buf = (uint8_t *)&led;
  nullnet_len = sizeof(led);
  NETSTACK_NETWORK.output(NULL);
}

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(lab1_master, ev, data)
{
  PROCESS_BEGIN();

  static struct etimer et;
  static uint8_t counter = 0;

  etimer_set(&et, CLOCK_SECOND);

  while (1)
  {

    PROCESS_YIELD();

    if (ev == PROCESS_EVENT_TIMER && data == &et)
    {
      uint8_t rnd = random_rand() % 3;
      if (rnd == 0)
        toggleLedAndBroadcast(LEDS_RED);
      else if (rnd == 1)
        toggleLedAndBroadcast(LEDS_GREEN);
      else if (rnd == 2)
        toggleLedAndBroadcast(LEDS_YELLOW);

      counter++;
      etimer_set(&et, CLOCK_SECOND);
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
