
/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include "servo.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <dk_buttons_and_leds.h>

// #define PWM_LED0 DT_ALIAS(pwm_led0)

// static const struct pwm_dt_spec pwm_led0 = PWM_DT_SPEC_GET(PWM_LED0);

/* STEP 5.4 - Retrieve the device structure for the servo motor */
LOG_MODULE_REGISTER(servo, LOG_LEVEL_INF);

#define SERVO_MOTOR DT_NODELABEL(servo)

/* STEP 5.5 - Use DT_PROP() to obtain the minimum and maximum duty cycle */
#define PWM_SERVO_MIN_DUTY_CYCLE DT_PROP(SERVO_MOTOR, min_pulse)
#define PWM_SERVO_MAX_DUTY_CYCLE DT_PROP(SERVO_MOTOR, max_pulse)

#define PWM_PERIOD PWM_MSEC(20)

#define PWM_MIN_DUTY_CYCLE 20000000
#define PWM_MAX_DUTY_CYCLE 50000000

struct pwm_dt_spec pwm_servo = PWM_DT_SPEC_GET(SERVO_MOTOR);

int setDefaultMotorAngle()
{
  printk("Setting default motor angle\n");

  int err;
  uint32_t duty_cycle;

  // 90도 각도에 해당하는 duty cycle 계산
  duty_cycle = PWM_SERVO_MIN_DUTY_CYCLE + (PWM_SERVO_MAX_DUTY_CYCLE - PWM_SERVO_MIN_DUTY_CYCLE) / 2;

  // PWM 출력을 한 번만 설정하고 종료
  err = pwm_set_dt(&pwm_servo, PWM_PERIOD, duty_cycle);
  if (err)
  {
    LOG_ERR("pwm_set_dt_returned %d", err);
  }

  k_msleep(3000);

  return err;
}

int stopServoMotor(void)
{
  int err;

  // PWM 출력을 0으로 설정하여 서보 모터 정지
  err = pwm_set_dt(&pwm_servo, PWM_PERIOD, PWM_SERVO_MIN_DUTY_CYCLE);
  if (err)
  {
    LOG_ERR("Failed to stop servo motor: %d", err);
    return err;
  }

  return 0;
}

int set_motor_angle(uint32_t duty_cycle_ns)
{
  int err;

  err = pwm_set_dt(&pwm_servo, PWM_PERIOD, duty_cycle_ns);
  if (err)
  {
    LOG_ERR("pwm_set_dt_returned %d", err);
  }
  return err;
}

void initServo()
{
  printk("Initialize... Servo\n");

  int err = 0;

  // if (dk_buttons_init(button_handler))
  // {
  //   LOG_ERR("Failed to initialize the buttons library");
  // }
  /* STEP 5.7 - Check if the motor device is ready and set its initial value */
  LOG_INF("Setting initial motor");
  if (!pwm_is_ready_dt(&pwm_servo))
  {
    LOG_ERR("Error: PWM device %s is not ready", pwm_servo.dev->name);
    return 0;
  }
  setDefaultMotorAngle();

  err = pwm_set_dt(&pwm_servo, PWM_PERIOD, 0);

  if (err)
  {
    LOG_ERR("pwm_set_dt returned %d", err);
    return 0;
  }

  return 0;
}