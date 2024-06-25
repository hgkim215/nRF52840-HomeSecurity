#include "btn.h"

#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>

LOG_MODULE_REGISTER(btn, LOG_LEVEL_DBG);

static struct gpio_callback button1_cb_data;

void initBTN()
{
  printk("Initialize BTN...\n");

  int err;

  err = gpio_btn_init();
  if (err != GPIO_OK)
  {
    printk("Error gpio_init %d\n", err);
    return 0;
  }
}

void button1_callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
  printk("Button 1 pressed\n");
  TOGGLE_LED3();
}

int gpio_btn_init(void)
{
  int err = GPIO_FAIL;

  printk("GPIO INIT\n");
  // Set button1 interrupt
  printk("Setting button1 interrupt\n");

  err = gpio_is_ready_dt(&button1);
  if (!err)
  {
    printk("Error gpio_is_ready_dt led0 pin %d\n", err);
    return GPIO_FAIL;
  }

  err = gpio_pin_configure_dt(&button1, GPIO_INPUT | GPIO_PULL_UP);
  if (err < 0)
  {
    printk("Error configuring button1 pin %d\n", err);
    return GPIO_FAIL;
  }

  err = gpio_pin_interrupt_configure_dt(&button1, GPIO_INT_EDGE_TO_ACTIVE);
  if (err != 0)
  {
    printk("Error configuring interrupt on button1 pin %d\n", err);
    return GPIO_FAIL;
  }
  gpio_init_callback(&button1_cb_data, button1_callback, BIT(button1.pin));
  gpio_add_callback(button1.port, &button1_cb_data);

  //*-------------------------------------

  // Set led1
  err = gpio_is_ready_dt(&led1);
  if (!err)
  {
    printk("Error gpio_is_ready_dt led1 pin %d\n", err);
    return GPIO_FAIL;
  }

  err = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
  if (err < 0)
  {
    printk("Error %d: failed to configure LED1 pin\n", err);
    return GPIO_FAIL;
  }

  // Set led2
  err = gpio_is_ready_dt(&led2);
  if (!err)
  {
    printk("Error gpio_is_ready_dt led2 pin %d\n", err);
    return GPIO_FAIL;
  }

  err = gpio_pin_configure_dt(&led2, GPIO_OUTPUT_ACTIVE);
  if (err < 0)
  {
    printk("Error %d: failed to configure LED2 pin\n", err);
    return GPIO_FAIL;
  }

  return GPIO_OK;
}