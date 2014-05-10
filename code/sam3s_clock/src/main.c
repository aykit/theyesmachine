#include <sam3s.h>
#include <pio.h>
#include <pmc.h>

#define LEDS_PIO PIOA

#define LED_ORANGE PIO_PA17
#define LED_GREEN PIO_PA18
#define ALL_LEDS (LED_ORANGE | LED_GREEN)

void delay(void)
{
	for (int i = 0; i < 100000; i++);
}

void blink(int leds)
{
	while (true)
	{
		pio_clear(LEDS_PIO, leds);
		delay();
		pio_set(LEDS_PIO, leds);
		delay();
	}
}

// MOSCXTST: Main Crystal Oscillator Start-up Time
// Specifies the number of Slow Clock cycles multiplied by 8 for the Main
// Crystal Oscillator start-up time.
// 0xff seems to be the maximum
#define MOSCXTST 0xff

// PLLACOUNT: PLLA Counter
// Specifies the number of Slow Clock cycles x8 before the LOCKA bit is set in
// PMC_SR after CKGR_PLLAR is written.
// 0x3F seems to be the maximum
#define PLLACOUNT 0x3F

int main(void)
{
	// Switch main clock to external crystal with 12MHz
	pmc_switch_mainck_to_xtal(false, MOSCXTST);

	// Enable PLLA with multiplier 16 and divisor 3 to obtain the maximal
	// processor frequence of 64MHz
	pmc_enable_pllack(16 - 1, PLLACOUNT, 3);

	// Configure Flash Wait State for 64MHz
	system_init_flash(64000000);

	// Switch master clock to PLLA
	pmc_switch_mck_to_pllack(PMC_MCKR_PRES_CLK_1);

	// Configure pins
	pio_set_output(LEDS_PIO, ALL_LEDS, 1, false, false);

	pio_clear(LEDS_PIO, LED_ORANGE);
	blink(LED_GREEN);

	return 0;
}
