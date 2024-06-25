#include "buzzer.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>

LOG_MODULE_REGISTER(buzzer, LOG_LEVEL_INF);

#define PWM_PIN DT_PROP(DT_ALIAS(pwm_led0), pin)
#define PWM_CTLR DT_PWMS_CTLR(DT_ALIAS(pwm_led0))
#define PWM_CHANNEL DT_PWMS_CHANNEL(DT_ALIAS(pwm_led0))
#define PWM_FLAGS DT_PWMS_FLAGS(DT_ALIAS(pwm_led0))

static const struct device *pwm_dev = DEVICE_DT_GET(PWM_CTLR);

// 주파수 정의 (레, 도, 시b, 라, 솔, 파)
int notes_warning[] = {391, 415, 466, 391, 415, 466, 391, 415, 466, 554};
int notes[] = {294, 262, 233, 220, 0, 220, 0, 220, 0, 220, 0, 392, 0, 0, 220, 523, 349, 349, 220, 220, 220, 220, 220, 220, 220, 392, 0, 0, 220, 523, 349, 349, 0, 220, 0, 220, 0, 220, 0, 392, 0, 0, 220, 523, 349, 349, 220, 220, 220, 220, 220, 220, 220, 392, 0, 0, 220, 523, 349, 349};
// 음표 지속 시간 (8분음표: 125ms, 8분 쉼표: 125ms, 4분음표: 250ms, 4분 쉼표: 250ms)
int durations_warning[] = {250, 250, 250, 250, 250, 250, 250, 250, 250, 250};
int durations[] = {250, 250, 250, 250, 125, 250, 125, 250, 125, 250, 125, 250, 125, 125, 125, 125, 125, 250, 125, 125, 125, 125, 125, 125, 125, 250, 125, 125, 125, 125, 125, 250, 125, 125, 125, 125, 125, 250, 125, 125, 125, 125, 125, 250, 125, 125, 125, 125, 125, 125, 125, 250, 125, 125, 125, 125, 125, 250, 125, 125};
int numNotes = sizeof(notes) / sizeof(notes[0]);
int warningNumNotes = sizeof(notes_warning) / sizeof(notes_warning[0]);

void play_tone(int frequency, int duration)
{
  if (frequency == 0)
  {
    // 쉼표
    pwm_set_cycles(pwm_dev, PWM_CHANNEL, 0, 0, PWM_FLAGS);
    k_sleep(K_MSEC(duration));
  }
  else
  {
    uint32_t cycles = 1000000 / frequency; // 주기 계산
    uint32_t duty_cycle = cycles / 2;      // 50% 듀티 사이클

    pwm_set_cycles(pwm_dev, PWM_CHANNEL, cycles, duty_cycle, PWM_FLAGS);
    k_sleep(K_MSEC(duration));
    pwm_set_cycles(pwm_dev, PWM_CHANNEL, 0, 0, PWM_FLAGS); // 음 멈추기
  }
}

void playWarning()
{
  for (int i = 0; i < warningNumNotes; i++)
  {
    play_tone(notes_warning[i], durations_warning[i]);
    k_sleep(K_MSEC(25)); // 음표 사이의 짧은 휴식
  }
}

void playCommon()
{
  for (int i = 0; i < numNotes; i++)
  {
    play_tone(notes[i], durations[i]);
    k_sleep(K_MSEC(25)); // 음표 사이의 짧은 휴식
  }
}

void initBuzzer()
{
  printk("Initialize... Buzzer\n");
  if (!device_is_ready(pwm_dev))
  {
    LOG_ERR("PWM device %s is not ready", pwm_dev->name);
    return;
  }

  // while (1)
  // {
  //   for (int i = 0; i < numNotes; i++)
  //   {
  //     play_tone(notes[i], durations[i]);
  //     k_sleep(K_MSEC(25)); // 음표 사이의 짧은 휴식
  //   }
  // }
}