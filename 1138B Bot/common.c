/*
 * common.c - Common functions used in my robotC code.
 * By Edward Pedemonte, for Team 1138 "Eagle Engineering"
 *
 * This file contains some functions that remove some repetitive
 * code from my robotC code, and makes the resulting code look
 * neater. This file is for code that is NOT robot-specific,
 * and may distract from the actual logic that controls
 * the robot.
 *
 * This file may also contain some code that makes code
 * significantly easier to read or understand from a
 * non-programmer perspective. For example, to a person
 * with no understanding of the underlying data types of
 * robotC, this code (which returns 50 if Button 6U is
 * pressed, and -50 if Button 6D is pressed) would not
 * make sense:
 *
 *   (vexRT[Btn6U] * 50) + (vexRT[Btn6D] * -50)
 *
 * In order to make the code understandable without
 * breaking it into multiple lines, which can also be
 * distracting from the robot-controlling logic, a function
 * that performs the same function is more legible:
 *
 *   valueOf(Btn5)
 *
 * In order to support that syntax of using Btn5 without
 * any additional button "specifier", a few constants are
 * also defined in this file.
 */

#pragma systemFile // Hide "unreferenced" function warnings

// Define constants used in this file
typedef enum JoystickConstants {
	// Abstract buttons, such as Btn5, for the trigger buttons
	Btn5,
	Btn6,
	// Fake channels made from directional pads 7 and 8
	Ch7Vert,
	Ch7Horz,
	Ch8Vert,
	Ch8Horz,
} JoystickConstants;

/*
 * getValueOf(joystick)
 * getValueOf(joystick, threshold)
 *
 * This function returns a joystick value, cut off before
 * a threshold, which defaults to 50, is reached. This is
 * used for joysticks, which may not be completely in the
 * center when they are let go of.
 */
int getValueOf(TVexJoysticks joystick, int threshold = 50) {
	int joystickValue = vexRT[joystick]; // Get the joystick value
	if (abs(joystickValue) > threshold) {
		// We are above the threshold. Return the original value.
		return joystickValue;
	} else {
		// We are below the threshold. Return zero.
		return 0;
	}
}


/*
 * btnAsChannel(button)
 * btnAsChannel(button, multiplier)
 *
 * This function returns a value depending on
 * which button is pressed from the shoulder buttons
 * on the vex joystick. Up returns positive values,
 * whereas down returns negative values.
 */
int btnAsChannel(JoystickConstants btn, int multiplier = 50) {
	if (btn == Btn5) {
		switch (vexRT[Btn5U] + (vexRT[Btn5D] * -1)) {
			case 1:
				// Button 5U is pressed
				return multiplier; // (1 * multiplier)
			case 0:
				// Neither are pressed
				return 0;
			case -1:
				// Button 5D is pressed
				return -1 * multiplier;
		}
	} else if (btn == Btn6) {
		switch (vexRT[Btn6U] + (vexRT[Btn6D] * -1)) {
			case 1:
				// Button 6U is pressed
				return multiplier; // (1 * multiplier)
			case 0:
				// Neither are pressed
				return 0;
			case -1:
				// Button 6D is pressed
				return -1 * multiplier;
		}
	}
	// Neither are pressed
	return 0;
}
