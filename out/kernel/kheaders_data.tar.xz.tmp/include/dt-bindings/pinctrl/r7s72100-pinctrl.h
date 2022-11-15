/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __DT_BINDINGS_PINCTRL_RENESAS_RZA1_H
#define __DT_BINDINGS_PINCTRL_RENESAS_RZA1_H

#define RZA1_PINS_PER_PORT	16


#define RZA1_PINMUX(b, p, f)	((b) * RZA1_PINS_PER_PORT + (p) | (f << 16))

#endif 
