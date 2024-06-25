#include "led.h"

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/led.h>
#include <zephyr/drivers/kscan.h>
#include <zephyr/devicetree.h>
#include <zephyr/logging/log.h>

#define LED_NODE DT_NODELABEL(ht16k33_ledmatix)

LOG_MODULE_REGISTER(main,
                    CONFIG_LOG_DEFAULT_LEVEL);

const struct device *const led = DEVICE_DT_GET(LED_NODE);
// #define
// DT_COMPAT_GET_ANY_STATUS_OKAY(holtek_ht16k33);

// 알파벳 'O'의 LED 패턴
static const uint8_t letter_O[8][16] = {
    {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0}};

static const uint8_t letter_X[8][16] = {
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}};

static const uint8_t welcome[8][16] = {
    {0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0},
    {0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0},
    {0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0},
    {0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0},
    {0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0},
    {0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0},
    {0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0}};

void initLED()
{
  printk("Initialize LED...\n");

  if (!device_is_ready(led))
  {
    LOG_ERR("LED device not ready");
    return 0;
  }

  firstLED();
}

void firstLED()
{
  int i, j, err;

  while (1)
  {
    LOG_INF("Turning on all LEDs in a circular pattern");

    // 알파벳 'O' 패턴을 LED 매트릭스에 표시
    for (i = 0; i < 8; i++)
    {
      for (j = 0; j < 16; j++)
      {
        if (welcome[i][j])
        {
          err = led_on(led, i * 16 + j);
          if (err)
          {
            LOG_ERR("Error writing to LED device: %d", err);
            return 0;
          }
        }
        else
        {
          err = led_off(led, i * 16 + j);
          if (err)
          {
            LOG_ERR("Error writing to LED device: %d", err);
            return 0;
          }
        }
      }
      k_sleep(K_MSEC(50)); // 50ms 대기
      // 켰던거 다시 끄기
      for (j = 0; j < 16; j++)
      {
        err = led_off(led, i * 16 + j);
        if (err)
        {
          LOG_ERR("Error writing to LED device: %d", err);
          return 0;
        }
      }
    }

    // 반대 방향으로 알파벳 'O' 패턴을 LED 매트릭스에 표시
    for (i = 7; i >= 0; i--)
    {
      for (j = 15; j >= 0; j--)
      {
        if (welcome[i][j])
        {
          err = led_on(led, i * 16 + j);
          if (err)
          {
            LOG_ERR("Error writing to LED device: %d", err);
            return 0;
          }
        }
        else
        {
          err = led_off(led, i * 16 + j);
          if (err)
          {
            LOG_ERR("Error writing to LED device: %d", err);
            return 0;
          }
        }
      }
    }
    k_sleep(K_MSEC(5000)); // 50ms 대기
    for (i = 0; i < 8; i++)
    {
      for (j = 0; j < 16; j++)
      {
        if (welcome[i][j])
        {
          err = led_off(led, i * 16 + j);
          if (err)
          {
            LOG_ERR("Error writing to LED device: %d", err);
            return 0;
          }
        }
        else
        {
          err = led_off(led, i * 16 + j);
          if (err)
          {
            LOG_ERR("Error writing to LED device: %d", err);
            return 0;
          }
        }
      }
      k_sleep(K_MSEC(50)); // 50ms 대기
    }
    // 반대 방향으로 알파벳 'O' 패턴을 LED 매트릭스에 표시
    for (i = 7; i >= 0; i--)
    {
      for (j = 15; j >= 0; j--)
      {
        if (welcome[i][j])
        {
          err = led_off(led, i * 16 + j);
          if (err)
          {
            LOG_ERR("Error writing to LED device: %d", err);
            return 0;
          }
        }
        else
        {
          err = led_off(led, i * 16 + j);
          if (err)
          {
            LOG_ERR("Error writing to LED device: %d", err);
            return 0;
          }
        }
      }
      k_sleep(K_MSEC(50)); // 50ms 대기
    }

    return 0;
  }
}

void screenO()
{
  int i, j, err;

  while (1)
  {
    LOG_INF("Turning on all LEDs in a circular pattern");

    // 알파벳 'O' 패턴을 LED 매트릭스에 표시
    for (i = 0; i < 8; i++)
    {
      for (j = 0; j < 16; j++)
      {
        if (letter_O[i][j])
        {
          err = led_on(led, i * 16 + j);
          if (err)
          {
            LOG_ERR("Error writing to LED device: %d", err);
            return 0;
          }
        }
        else
        {
          err = led_off(led, i * 16 + j);
          if (err)
          {
            LOG_ERR("Error writing to LED device: %d", err);
            return 0;
          }
        }
      }
      k_sleep(K_MSEC(50)); // 50ms 대기
      // 켰던거 다시 끄기
      for (j = 0; j < 16; j++)
      {
        err = led_off(led, i * 16 + j);
        if (err)
        {
          LOG_ERR("Error writing to LED device: %d", err);
          return 0;
        }
      }
    }

    // 반대 방향으로 알파벳 'O' 패턴을 LED 매트릭스에 표시
    for (i = 7; i >= 0; i--)
    {
      for (j = 15; j >= 0; j--)
      {
        if (letter_O[i][j])
        {
          err = led_on(led, i * 16 + j);
          if (err)
          {
            LOG_ERR("Error writing to LED device: %d", err);
            return 0;
          }
        }
        else
        {
          err = led_off(led, i * 16 + j);
          if (err)
          {
            LOG_ERR("Error writing to LED device: %d", err);
            return 0;
          }
        }
      }
    }
    k_sleep(K_MSEC(5000)); // 50ms 대기
    for (i = 0; i < 8; i++)
    {
      for (j = 0; j < 16; j++)
      {
        if (letter_O[i][j])
        {
          err = led_off(led, i * 16 + j);
          if (err)
          {
            LOG_ERR("Error writing to LED device: %d", err);
            return 0;
          }
        }
        else
        {
          err = led_off(led, i * 16 + j);
          if (err)
          {
            LOG_ERR("Error writing to LED device: %d", err);
            return 0;
          }
        }
      }
      k_sleep(K_MSEC(50)); // 50ms 대기
    }
    // 반대 방향으로 알파벳 'O' 패턴을 LED 매트릭스에 표시
    for (i = 7; i >= 0; i--)
    {
      for (j = 15; j >= 0; j--)
      {
        if (letter_O[i][j])
        {
          err = led_off(led, i * 16 + j);
          if (err)
          {
            LOG_ERR("Error writing to LED device: %d", err);
            return 0;
          }
        }
        else
        {
          err = led_off(led, i * 16 + j);
          if (err)
          {
            LOG_ERR("Error writing to LED device: %d", err);
            return 0;
          }
        }
      }
      k_sleep(K_MSEC(50)); // 50ms 대기
    }

    return 0;
  }
}

void screenX()
{
  int i, j, err;

  while (1)
  {
    LOG_INF("Turning on all LEDs in a circular pattern");

    // 알파벳 'O' 패턴을 LED 매트릭스에 표시
    for (i = 0; i < 8; i++)
    {
      for (j = 0; j < 16; j++)
      {
        if (letter_X[i][j])
        {
          err = led_on(led, i * 16 + j);
          if (err)
          {
            LOG_ERR("Error writing to LED device: %d", err);
            return 0;
          }
        }
        else
        {
          err = led_off(led, i * 16 + j);
          if (err)
          {
            LOG_ERR("Error writing to LED device: %d", err);
            return 0;
          }
        }
      }
      k_sleep(K_MSEC(50)); // 50ms 대기
      // 켰던거 다시 끄기
      for (j = 0; j < 16; j++)
      {
        err = led_off(led, i * 16 + j);
        if (err)
        {
          LOG_ERR("Error writing to LED device: %d", err);
          return 0;
        }
      }
    }

    // 반대 방향으로 알파벳 'O' 패턴을 LED 매트릭스에 표시
    for (i = 7; i >= 0; i--)
    {
      for (j = 15; j >= 0; j--)
      {
        if (letter_X[i][j])
        {
          err = led_on(led, i * 16 + j);
          if (err)
          {
            LOG_ERR("Error writing to LED device: %d", err);
            return 0;
          }
        }
        else
        {
          err = led_off(led, i * 16 + j);
          if (err)
          {
            LOG_ERR("Error writing to LED device: %d", err);
            return 0;
          }
        }
      }
    }
    k_sleep(K_MSEC(5000)); // 50ms 대기
    for (i = 0; i < 8; i++)
    {
      for (j = 0; j < 16; j++)
      {
        if (letter_X[i][j])
        {
          err = led_off(led, i * 16 + j);
          if (err)
          {
            LOG_ERR("Error writing to LED device: %d", err);
            return 0;
          }
        }
        else
        {
          err = led_off(led, i * 16 + j);
          if (err)
          {
            LOG_ERR("Error writing to LED device: %d", err);
            return 0;
          }
        }
      }
      k_sleep(K_MSEC(50)); // 50ms 대기
    }
    // 반대 방향으로 알파벳 'O' 패턴을 LED 매트릭스에 표시
    for (i = 7; i >= 0; i--)
    {
      for (j = 15; j >= 0; j--)
      {
        if (letter_X[i][j])
        {
          err = led_off(led, i * 16 + j);
          if (err)
          {
            LOG_ERR("Error writing to LED device: %d", err);
            return 0;
          }
        }
        else
        {
          err = led_off(led, i * 16 + j);
          if (err)
          {
            LOG_ERR("Error writing to LED device: %d", err);
            return 0;
          }
        }
      }
      k_sleep(K_MSEC(50)); // 50ms 대기
    }

    return 0;
  }
}
