// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <driver/int_types.h>
#include <soc/soc.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GROUP0     0
#define GROUP1     1
#define INVALID    0

#define ICU_DEV_MAP \
{\
	{INT_SRC_DMA0_NSEC, 0, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_ENC_SEC, 1, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_ENC_NSEC, 2, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_TIMER, 3, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_UART0, 4, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_PWM, 5, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_I2C0, 6, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_SPI0, 7, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_SARADC, 8, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_IRDA, 9, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_SDIO, 10, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_GDMA, 11, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_LA, 12, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_TIMER1, 13, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_I2C1, 14, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_UART1, 15, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_UART2, 16, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_SPI1, 17, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_CAN, 18, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_USB, 19, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_QSPI0, 20, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_CKMN, 21, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_SBC, 22, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_AUDIO, 23, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_I2S0, 24, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_JPEG_ENC, 25, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_JPEG_DEC, 26, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_LCD, 27, 0, GROUP0}, \
	{INT_SRC_DMA2D, 28, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_MODEM, 29, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_MODEM_RC, 30, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_MAC_TXRX_TIMER, 31, IQR_PRI_DEFAULT, GROUP0}, \
	{INT_SRC_MAC_TXRX_MISC, 32, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_MAC_RX_TRIGGER, 33, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_MAC_TX_TRIGGER, 34, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_MAC_PROT_TRIGGER, 35, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_MAC_GENERAL, 36, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_MAC_HSU, 37, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_MAC_WAKEUP, 38, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_BTDM, 39, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_BLE, 40, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_BT, 41, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_QSPI1, 42, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_PWM1, 43, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_I2S1, 44, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_I2S2, 45, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_H264, 46, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_SDMADC, 47, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_ETH, 48, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_SCALE0, 49, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_BMC64, 50, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_PLL_UNLOCK, 51, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_TOUCHED, 52, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_USB_PLUG_INOUT, 53, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_RTC, 54, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_GPIO, 55, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_DMA1_SEC, 56, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_DMA1_NSEC, 57, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_YUVB, 58, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_ROTT, 59, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_7816, 60, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_LIN, 61, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_SCALE1, 62, IQR_PRI_DEFAULT, GROUP1}, \
	{INT_SRC_MAILBOX, 63, IQR_PRI_DEFAULT, GROUP1}, \
}

#ifdef __cplusplus
}
#endif
