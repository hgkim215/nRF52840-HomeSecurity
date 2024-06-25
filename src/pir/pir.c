#include "pir.h"

#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>

LOG_MODULE_REGISTER(pir, LOG_LEVEL_DBG);

static struct gpio_callback pir_cb_data;

bool pirDetected = false;

void initPIR()
{
  printk("Initialize PIR...\n");

  int err;

  err = gpio_init();
  if (err != GPIO_OK)
  {
    printk("Error gpio_init %d\n", err);
    return 0;
  }

  printk("PIR initialized\n");
}

bool checkDetected()
{
  return pirDetected;
}

void pir_callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
  int ret;
  ret = gpio_pin_toggle_dt(&led3);
  pirDetected = !pirDetected;
  if (pirDetected)
  {
    printk("PIR DETECTED\n");
  }
  else
  {
    printk("PIR NOT DETECTED\n");
  }
  if (ret < 0)
  {
    return;
  }
}

int gpio_init(void)
{
  int err = GPIO_FAIL;

  //* Set button4 interrupt
  printk("GPIO INIT\n");

  //* PIR SETTING
  err = gpio_is_ready_dt(&pir);
  if (!err)
  {
    printk("Error gpio_is_ready_dt led3 pin %d\n", err);
    return GPIO_FAIL;
  }

  err = gpio_pin_configure_dt(&pir, GPIO_INPUT | GPIO_PULL_UP);
  if (err < 0)
  {
    printk("Error configuring button3 pin %d\n", err);
    return GPIO_FAIL;
  }

  err = gpio_pin_interrupt_configure_dt(&pir, GPIO_INT_EDGE_TO_ACTIVE);
  if (err != 0)
  {
    printk("Error configuring interrupt on button3 pin %d\n", err);
    return GPIO_FAIL;
  }
  gpio_init_callback(&pir_cb_data, pir_callback, BIT(pir.pin));
  gpio_add_callback(pir.port, &pir_cb_data);
  //*-------------------------------------

  // Set led3
  err = gpio_is_ready_dt(&led3);
  if (!err)
  {
    printk("Error gpio_is_ready_dt led3 pin %d\n", err);
    return GPIO_FAIL;
  }

  err = gpio_pin_configure_dt(&led3, GPIO_OUTPUT_ACTIVE);
  if (err < 0)
  {
    printk("Error %d: failed to configure LED3 pin\n", err);
    return GPIO_FAIL;
  }

  return GPIO_OK;
}