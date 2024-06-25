/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/reboot.h>

int main(void)
{
	printk("Starting Final Project\n");

	initBTN();

	initLED();

	initBuzzer();

	initNFC();

	initPIR();

	initBleUART();

	initServo();

	return 0;
}
