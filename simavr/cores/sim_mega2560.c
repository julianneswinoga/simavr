/*
   sim_mega2560.c

   Copyright 2008, 2009 Michel Pollet <buserror@gmail.com>
   Copyright 2013 Yann GOUY <yann_gouy@yahoo.fr>

   This file is part of simavr.

   simavr is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   simavr is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with simavr.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "sim_avr.h"
#include "sim_mega2560.h"
#include "uart_pty.h"
#include "sim_vcd_file.h"

static uart_pty_t uart_pty0;
static uart_pty_t uart_pty1;
static uart_pty_t uart_pty2;
static avr_vcd_t vcd_file;

static avr_t * make()
{
	return avr_core_allocate(&mcu_mega2560.core, sizeof(struct mcu_t));
}

avr_kind_t mega2560 = {
		.names = { "atmega2560", "atmega2561" },
		.make = make
};

void avr_special_deinit( avr_t* avr, void * data)
{
	uart_pty_stop(&uart_pty0);
	// uart_pty_stop(&uart_pty1);
	// uart_pty_stop(&uart_pty2);
}

void m2560_init(struct avr_t * avr)
{
	struct mcu_t * mcu = (struct mcu_t*)avr;
	
	avr_eeprom_init(avr, &mcu->eeprom);
	avr_flash_init(avr, &mcu->selfprog);
	avr_extint_init(avr, &mcu->extint);
	avr_watchdog_init(avr, &mcu->watchdog);
	avr_ioport_init(avr, &mcu->porta);
	avr_ioport_init(avr, &mcu->portb);
	avr_ioport_init(avr, &mcu->portc);
	avr_ioport_init(avr, &mcu->portd);
	avr_ioport_init(avr, &mcu->porte);
	avr_ioport_init(avr, &mcu->portf);
	avr_ioport_init(avr, &mcu->portg);
	avr_ioport_init(avr, &mcu->porth);
	avr_ioport_init(avr, &mcu->portj);
	avr_ioport_init(avr, &mcu->portk);
	avr_ioport_init(avr, &mcu->portl);

	avr_uart_init(avr, &mcu->uart0);
	avr_uart_init(avr, &mcu->uart1);
	avr_uart_init(avr, &mcu->uart2);
	avr_uart_init(avr, &mcu->uart3);
        avr_acomp_init(avr, &mcu->acomp);
	avr_adc_init(avr, &mcu->adc);
	avr_timer_init(avr, &mcu->timer0);
	avr_timer_init(avr, &mcu->timer1);
	avr_timer_init(avr, &mcu->timer2);
	avr_timer_init(avr, &mcu->timer3);
	avr_timer_init(avr, &mcu->timer4);
	avr_timer_init(avr, &mcu->timer5);
	avr_spi_init(avr, &mcu->spi);
	avr_twi_init(avr, &mcu->twi);


	avr->custom.deinit = avr_special_deinit;
	uart_pty_init(avr, &uart_pty0);
	uart_pty_init(avr, &uart_pty1);
	uart_pty_init(avr, &uart_pty2);
	uart_pty_connect(&uart_pty0, '0');
	uart_pty_connect(&uart_pty1, '1');
	uart_pty_connect(&uart_pty2, '2');

	// VCD output
	avr_vcd_init(avr, "gtkwave_output.vcd", &vcd_file, 1000 /* usec */);
	/*
	 * Ref https://www.electronicshub.org/wp-content/smush-webp/2021/01/Arduino-Mega-Pinout.jpg.webp
	 * Arduino D41 - PORTG 0
	 * Arduino D40 - PORTG 1
	 */
	avr_vcd_add_signal(&vcd_file, avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('G'), 0), 1, "RA_STEP");
	avr_vcd_add_signal(&vcd_file, avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('G'), 1), 1, "DEC_STEP");

	avr_vcd_add_signal(&vcd_file, avr_io_getirq(avr, AVR_IOCTL_UART_GETIRQ('0'), UART_IRQ_OUTPUT), 8, "UART0_OUT");
	avr_vcd_add_signal(&vcd_file, avr_io_getirq(avr, AVR_IOCTL_UART_GETIRQ('0'), UART_IRQ_INPUT), 8, "UART0_IN");

	avr_vcd_add_signal(&vcd_file, avr_io_getirq(avr, AVR_IOCTL_UART_GETIRQ('1'), UART_IRQ_OUTPUT), 8, "UART1_OUT");

	avr_vcd_start(&vcd_file);
}

void m2560_reset(struct avr_t * avr)
{
// struct mcu_t * mcu = (struct mcu_t*)avr;
}


