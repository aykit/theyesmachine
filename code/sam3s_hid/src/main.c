#include <sam3s.h>
#include <pio.h>
#include <pmc.h>
#include <udc.h>
#include <sysclk.h>
#include <wdt.h>

#define LEDS_PIO PIOA
#define BUTTONS_PIO PIOA

#define BUTTONS_PIO_ID ID_PIOA

#define LED_ORANGE PIO_PA17
#define LED_GREEN PIO_PA18
#define ALL_LEDS (LED_ORANGE | LED_GREEN)

#define BUTTON1 PIO_PA19
#define BUTTON2 PIO_PA20
#define ALL_BUTTONS (BUTTON1 | BUTTON2)

static bool enabled = false;

static void kbd_delay(void)
{
	for (int i = 0; i < 10000; i++);
}

// This is pretty unreliable so far
static bool send_chr(char c)
{
	uint8_t code;
	static bool shift = false;

	if (c >= 'a' && c <= 'z')
	{
		code = c - 'a' + HID_A;
	}
	else if (c >= 'A' && c <= 'Z')
	{
		code = c - 'A' + HID_A;
		shift = true;
	}
	else if (c >= '1' && c <= '9')
	{
		code = c - '1' + HID_1;
	}
	else if (c == '0')
	{
		code = HID_0;
	}
	else if (c == ' ')
	{
		code = HID_SPACEBAR;
	}
	else
	{
		return false;
	}

	if (shift)
	{
		udi_hid_kbd_modifier_down(HID_MODIFIER_RIGHT_SHIFT);
		kbd_delay();
	}
	udi_hid_kbd_down(code);
	kbd_delay();
	udi_hid_kbd_up(code);
	kbd_delay();
	if (shift)
	{
		udi_hid_kbd_modifier_up(HID_MODIFIER_RIGHT_SHIFT);
		kbd_delay();
	}
	kbd_delay();
	kbd_delay();
	kbd_delay();

	return true;
}

static void send_str(const char *s)
{
	while (*s)
		send_chr(*s++);
}

int main(void)
{
	// Disable watchdog timer
	wdt_disable(WDT);

	sysclk_init();

	pmc_enable_periph_clk(BUTTONS_PIO_ID);
	while (!pmc_is_periph_clk_enabled(BUTTONS_PIO_ID));

	pio_set_output(LEDS_PIO, ALL_LEDS, 1, false, false);
	pio_set_input(BUTTONS_PIO, ALL_BUTTONS, PIO_DEBOUNCE);
	pio_set_debounce_filter(BUTTONS_PIO, ALL_BUTTONS, 10); // is this necessary?

	udc_start();

	pio_clear(LEDS_PIO, LED_ORANGE);

	while (!enabled);

	pio_clear(LEDS_PIO, LED_GREEN);

	while (true)
	{
		bool btn1, btn2;
		do
		{
			btn1 = pio_get(BUTTONS_PIO, PIO_INPUT, BUTTON1);
			btn2 = pio_get(BUTTONS_PIO, PIO_INPUT, BUTTON2);
		}
		while (btn1 && btn2);
		if (btn2)
			send_str("yes ");
		else
			send_str("no ");
		while (!pio_get(BUTTONS_PIO, PIO_INPUT, BUTTON1) ||
		       !pio_get(BUTTONS_PIO, PIO_INPUT, BUTTON2));
	}

	return 0;
}

bool hid_kbd_enable(void)
{
	enabled = true;
	return true;
}

void hid_kbd_disable(void) { }

void hid_kbd_change_led(uint8_t value) { }
