#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define GPIO_FAIL -1
#define GPIO_OK 0

int gpio_init(void);

bool checkDetected(void);

/* The devicetree node identifier for the "led3" alias. */
#define LED3_NODE DT_ALIAS(led3)
#if !DT_NODE_HAS_STATUS(LED3_NODE, okay)
#error "Unsupported board: led3 devicetree alias is not defined"
#endif
static const struct gpio_dt_spec led3 = GPIO_DT_SPEC_GET(DT_ALIAS(led3), gpios);

// buttons

/* The devicetree node identifier for the "sw3" alias. */
#define SW3_NODE DT_ALIAS(sw3)
#if !DT_NODE_HAS_STATUS(SW3_NODE, okay)
#error "Unsupported board: sw3 devicetree alias is not defined"
#endif
static const struct gpio_dt_spec pir = GPIO_DT_SPEC_GET(SW3_NODE, gpios);