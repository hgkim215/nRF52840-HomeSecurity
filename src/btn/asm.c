// #include "asm.h"
#include <stdint.h>

#define LED1_MASK 0x00002000
#define LED2_MASK 0x00004000
#define LED3_MASK 0x00008000
#define LED4_MASK 0x00010000

#define GPIO_P0_BASE 0x50000000
#define GPIO_OUT_REG_OFFSET 0x0504
#define GPIO_DIR_REG_OFFSET 0x0514
#define GPIO_DIRSET_REG_OFFSET 0x0518
#define GPIO_IN_REG_OFFSET 0x510

#define BTN1_MASK 0x00800
#define BTN2_MASK 0x01000
#define GPIO_PIN_CNF_11_OFFSET 0x72C

void volatile SETTING()
{
  *((uint32_t *)(GPIO_P0_BASE + GPIO_DIR_REG_OFFSET)) = LED3_MASK;
  *((uint32_t *)(GPIO_P0_BASE + GPIO_PIN_CNF_11_OFFSET)) = 0x0003000c;
}

void volatile TOGGLE_LED4()
{
  volatile uint32_t *OUT_REG = (uint32_t *)(GPIO_P0_BASE + GPIO_OUT_REG_OFFSET);
  *OUT_REG ^= LED4_MASK;
}

void volatile TOGGLE_LED3()
{
  __asm(
      "LDR R1, =0x50000504\n\t"
      "LDR R0, [R1]\n\t"
      "LDR R2, =0x00008000\n\t"
      "EOR R0, R0, R2\n\t"
      "STR R0, [R1]\n\t");
  // "MOV PC, LR\n\t");
}
