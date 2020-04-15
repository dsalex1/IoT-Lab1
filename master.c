#include "contiki.h"
#include "sys/etimer.h"

#include "dev/leds.h"

#include "net/netstack.h"
#include "net/nullnet/nullnet.h"

#include <random.h>
#include <string.h>
#include <stdio.h>

/*---------------------------------------------------------------------------*/
PROCESS(lab1_master, "Lab1 Master");
AUTOSTART_PROCESSES(&lab1_master);

/*---------------------------------------------------------------------------*/
void input_callback(const void *data, uint16_t len,
                    const linkaddr_t *src, const linkaddr_t *dest)
{
  static int16_t receivedData[16];
  static int16_t receivedFlags = 0;
  static int16_t receivedCount = 0;

  if (len == sizeof(int16_t))
  {
    int8_t srcId = src->u8[0];
    if (srcId > 15)
    {
      printf("ERROR, cant process data from nodes with id>15: %d", srcId);
      return;
    }

    if (((receivedFlags >> srcId) & 1) == 1) //already received from this node, guess we got all in this iteration
    {
      int16_t average = 0;
      printf("Temperatures: ");

      int i = 0;
      for (i = 0; i < receivedCount; i++)
      {
        average += receivedData[i];
        printf("%d", receivedData[i]);
        if (i != receivedCount - 1)
          printf(", ");
      }
      average /= receivedCount;
      printf("\nAverage: %d\n", average);

      receivedCount = 0;
      receivedFlags = 0;
    }

    int16_t temp;
    memcpy(&temp, data, sizeof(temp));

    receivedData[receivedCount] = temp;
    receivedCount++;
    receivedFlags |= 1 << srcId;
  }
}

/*---------------------------------------------------------------------------*/
void toggleLedAndBroadcast(uint8_t led)
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

  etimer_set(&et, CLOCK_SECOND);

  /* Initialize NullNet */
  nullnet_set_input_callback(input_callback);

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

      etimer_set(&et, CLOCK_SECOND);
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
