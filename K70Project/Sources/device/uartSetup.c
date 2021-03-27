/**
 * uartSetup.c
 * UART2 Serial I/O
 * Demonstrates using UART2 serial I/O
 *
 * ARM-based K70F120M microcontroller board
 *   for educational purposes only
 * CSCI E-92 Spring 2017, Professor James L. Frankel, Harvard Extension School
 *
 */

#include <stdio.h>
#include "derivative.h"
#include "uart.h"
#include "../util/delay.h"
#include "../int/nvic.h"

int uartSetup(void) {

	/* On reset (i.e., before calling mcgInit), the processor
	 * clocking starts in FEI (FLL Engaged Internal) mode.  In FEI
	 * mode and with default settings (DRST_DRS = 00, DMX32 = 0),
	 * the MCGFLLCLK, the MCGOUTCLK (MCG (Multipurpose Clock
	 * Generator) clock), and the Bus (peripheral) clock are all set
	 * to 640 * IRC.  IRC is the Internal Reference Clock which runs
	 * at 32 KHz. [See K70 Sub-Family Reference Manual, Rev. 4,
	 * Section 25.4.1.1, Table 25-22 on labeled page 670 (PDF page
	 * 677) and MCG Control 4 Register (MCG_C4) Section 25.3.4 on
	 * labeled page 655 (PDF page 662); See K70 Sub-Family Reference
	 * Manual, Rev. 2, Section 25.4.1.1, Table 25-22 on page 657 and
	 * MCG Control 4 Register (MCG_C4) Section 25.3.4 on page 641]
	 */

	/* After calling mcgInit, MCGOUTCLK is set to 120 MHz and the Bus
	 * (peripheral) clock is set to 60 MHz.*/

	/* Table 5-2 on labeled page 225 (PDF page 232) in Rev. 4
	 * (Table 5-2 on page 221 in Rev. 2) indicates that the clock
	 * used by UART0 and UART1 is the System clock (i.e., MCGOUTCLK)
	 * and that the clock used by UART2-5 is the Bus clock. */

	const int peripheralClock = 60000000;
	const int KHzInHz = 1000;
	const int baud = 115200;

	uartInit(UART2_BASE_PTR, peripheralClock/KHzInHz, baud);

	/* Enable the receiver full interrupt for UART2 using the UART2_C2 register
	 * (UART Control Register 2) (See 57.3.4 on page 1909 of the K70 Sub-Family Reference
	 * Manual, Rev. 2, Dec 2011) */
	UART2_C2 |= UART_C2_RIE_MASK;

	/* Enable interrupts from UART2 status sources and set its interrupt priority */
	NVICEnableIRQ(UART2_STATUS_IRQ_NUMBER, UART2_STATUS_INTERRUPT_PRIORITY);

	return 0;
}
