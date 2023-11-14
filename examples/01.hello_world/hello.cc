#include <platform-gpio-leds.hh>
#include <thread.h>
#include <compartment.h>
#include <debug.hh>
#include <fail-simulator-on-error.h>

/// Expose debugging features unconditionally for this compartment.
using Debug = ConditionalDebug<true, "Hello world compartment">;

/// Thread entry point.
void __cheri_compartment("hello") say_hello()
{
	auto leds = MMIO_CAPABILITY(LEDs, gpio_led0);
	leds->enable_all();
	bool on = true;
	Debug::log("Hello world");
	while (true)
	{
		if (on)
		{
			leds->led_off(0);
			leds->led_on(1);
		}
		else
		{
			leds->led_on(0);
			leds->led_off(1);
		}
		on = !on;
		Timeout t{100};
		thread_sleep(&t);
	}
	// Print hello world, along with the compartment's name to the default UART.
}
