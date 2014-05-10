#include <sam3s.h>
#include <pio.h>
#include <pmc.h>
#include <wdt.h>

#define LEDS_PIO PIOA
#define BUTTONS_PIO PIOA

#define BUTTONS_PIO_ID ID_PIOA

#define LED_ORANGE PIO_PA17
#define LED_GREEN PIO_PA18 // big board
// #define LED_GREEN PIO_PA8 // small board
#define ALL_LEDS (LED_ORANGE | LED_GREEN)

#define BUTTON1 PIO_PA19
#define BUTTON2 PIO_PA20
#define ALL_BUTTONS (BUTTON1 | BUTTON2)

int main(void)
{
	// Disable watchdog timer
	wdt_disable(WDT);

	// Enable peripheral clock to be able to read button state
	pmc_enable_periph_clk(BUTTONS_PIO_ID);
	while (!pmc_is_periph_clk_enabled(BUTTONS_PIO_ID));

	// Configure pins
	pio_set_output(LEDS_PIO, ALL_LEDS, 1, false, false);
	pio_set_input(BUTTONS_PIO, ALL_BUTTONS, PIO_PULLUP);

	while (true)
	{
		pio_set(LEDS_PIO, LED_ORANGE);
		pio_clear(LEDS_PIO, LED_GREEN);
		while (pio_get(BUTTONS_PIO, PIO_INPUT, BUTTON1));
		pio_clear(LEDS_PIO, LED_ORANGE);
		pio_set(LEDS_PIO, LED_GREEN);
		while (pio_get(BUTTONS_PIO, PIO_INPUT, BUTTON2));
	}

	return 0;
}
