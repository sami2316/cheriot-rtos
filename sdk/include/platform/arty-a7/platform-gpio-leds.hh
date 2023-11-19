#pragma once
#include <stdint.h>

/**
 * The Arty A7 configuration has two LEDs exposed over GPIO.
 *
 * This provides a simple driver for them.
 */
struct LEDs
{
	/**
	 * Input register.  This is unused for the LEDs.
	 */
	uint32_t read;
	/**
	 * The output register.  This is a bitmap of GPIO lines to set.
	 */
	uint32_t write;
	/**
	 * Write enable.  This sets the pins that should be controlled by `write`.
	 */
	uint32_t writeEnable;

	/**
	 * The index of the first GPIO pin connected to a LED.
	 */
	static constexpr uint32_t FirstLED = 14;
	/**
	 * The index of the last GPIO pin connected to a LED.
	 */
	static constexpr uint32_t LastLED = 15;
	/**
	 * The number of GPIO pins.
	 */
	static constexpr uint32_t LEDCount = LastLED - FirstLED + 1;

	/**
	 * Helper that maps from an LED index to a bit to set / clear to control
	 * that LED.  Returns 0 for out-of-bounds LED values and so can be safely
	 * masked.
	 */
	constexpr static uint32_t led_bit(uint32_t index)
	{
		if (index >= LEDCount)
		{
			return 0;
		}
		return 1 << (index + FirstLED);
	}

	/**
	 * Enable  all of the LED GPIO pins.
	 *
	 * This must be called before `led_on` or `led_off`.
	 */
	void enable_all() volatile
	{
		uint32_t bitmap = 0;
		for (uint32_t i=0 ; i<LEDCount ; i++)
		{
			bitmap |= led_bit(i);
		}
		writeEnable = bitmap;
	}

	/**
	 * Turn on the LED specified by `index`.
	 */
	void led_on(uint32_t index) volatile
	{
		write = write | led_bit(index);
	}

	/**
	 * Turn off the LED specified by `index`.
	 */
	void led_off(uint32_t index) volatile
	{
		write = write & ~led_bit(index);
	}
};
