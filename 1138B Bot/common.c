/*
 * common.c - Common functions used in my robotC code.
 * By Edward Pedemonte, for Team 1138 "Eagle Engineering"
 *
 * This file contains some functions that remove some repetitive
 * code from my robotC code, and makes the resulting code look
 * neater. This file is for code that is NOT robot-specific,
 * and for code that may distract from the actual logic
 * that controls the robot.
 *
 * This file may also contain some code that makes code
 * significantly easier to read or understand from a
 * non-programmer perspective. For example, to a person
 * with no understanding of the underlying data types of
 * robotC and how they work, this code (which returns 50
 * if Button 6U is pressed, and -50 if Button 6D is
 * pressed) would not make sense:
 *
 *   (vexRT[Btn6U] * 50) + (vexRT[Btn6D] * -50)
 *
 * In order to make the code understandable without
 * breaking it into multiple lines, which can also be
 * distracting from the robot-controlling logic, a function
 * that performs the same function is more legible:
 *
 *   getButtonChannel(Btn5)
 *
 * In order to support that syntax of using Btn5 without
 * any additional button "specifier", a few constants are
 * also defined in this file.
 */

#pragma systemFile // Hide "unreferenced" function warnings

// Define constants used in this file
typedef enum ControllerConstants {
	// Abstract buttons, such as Btn5, for the trigger buttons
	Btn5,
	Btn6,
	Btn5Xmtr2,
	Btn6Xmtr2,
	// Fake channels made from directional pads 7 and 8
	Ch7Vert,
	Ch7Horz,
	Ch8Vert,
	Ch8Horz,
	Ch7Xmtr2Vert,
	Ch7Xmtr2Horz,
	Ch8Xmtr2Vert,
	Ch8Xmtr2Horz,
	// Joystick Constants, for things like
	// Arcade or Tank Control
	SingleJoystickLeft,
	SingleJoystickRight,
	DualJoystick,
	SingleJoystickLeftXmtr2,
	SingleJoystickRightXmtr2,
	DualJoystickXmtr2,
} ControllerConstants;

const float wheelDiameter = 4.0; // Inches
const float encoderRevolutionTicks = 627.2; // Encoder ticks per revolution
const float inchesPerRevolution = wheelDiameter * PI;
const float encoderValuePerInch = encoderRevolutionTicks / inchesPerRevolution;

/*
 * compare(valueOne, valueTwo)
 *
 * Compare two numbers. Returns:
 *   -1 if valueOne is less than valueTwo
 *    0 if valueOne is equal to valueTwo
 *    1 if valueOne is equal to valueTwo
 *
 * Equivelant to the following code, but is
 * branchless, resulting in zero jumps in the
 * assembled code:
 *    if (valueOne < valueTwo) {
 *      return -1;
 *    } else if (valueOne > Two) {
 *      return 1;
 *    } else {
 *      return 0;
 *    }
 *
 * Uses the concept that a boolean "true" value
 * is equivelant to "1" and a boolean "false" value
 * is equivalant to "0".
 */
int compare(int valueOne, int valueTwo) {
	return (valueOne > valueTwo) - (valueOne < valueTwo);
}

/* SECTION: CONTROLLER */
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
 * or the directional buttons (buttons in groups 7 and 8)
 * on the vex joystick. Up returns positive values,
 * whereas down returns negative values.
 */
int getButtonChannel(ControllerConstants btn, int multiplier = 50) {
	// Shoulder buttons on controller 1
	switch(btn) {
		case Btn5: return compare(vexRT[Btn5U], vexRT[Btn5D]) * multiplier;
		case Btn6: return compare(vexRT[Btn6U], vexRT[Btn6D]) * multiplier;
		case Btn5Xmtr2: return compare(vexRT[Btn5UXmtr2], vexRT[Btn5DXmtr2]) * multiplier;
		case Btn6Xmtr2:	return compare(vexRT[Btn6UXmtr2], vexRT[Btn6DXmtr2]) * multiplier;
		// Directional buttons on controller 1
		case Ch7Vert: return compare(vexRT[Btn7U], vexRT[Btn7D]) * multiplier;
		case Ch7Horz: return compare(vexRT[Btn7L], vexRT[Btn7R]) * multiplier;
		case Ch8Vert: return compare(vexRT[Btn8U], vexRT[Btn8D]) * multiplier;
		case Ch8Horz: return compare(vexRT[Btn8L], vexRT[Btn8R]) * multiplier;
		// Directional buttons on controller 2
		case Ch7Xmtr2Vert: return compare(vexRT[Btn7UXmtr2], vexRT[Btn7DXmtr2]) * multiplier;
		case Ch7Xmtr2Horz: return compare(vexRT[Btn7LXmtr2], vexRT[Btn7RXmtr2]) * multiplier;
		case Ch8Xmtr2Vert: return compare(vexRT[Btn8UXmtr2], vexRT[Btn8DXmtr2]) * multiplier;
		case Ch8Xmtr2Horz:return compare(vexRT[Btn8LXmtr2], vexRT[Btn8RXmtr2]) * multiplier;
	}
	return 0;
}



/* SECTION: ARCADE CONTROL */

// Variables
tMotor ac_leftMotor;
tMotor ac_rightMotor;
TVexJoysticks ac_xAxis;
TVexJoysticks ac_yAxis;
int ac_fixedSpeed;

/*
 * setupArcadeControl(mode, leftMotor, rightMotor)
 * setupArcadeControl(mode, leftMotor, rightMotor, fixedSpeed)
 *
 * This runs sets up arcade control based on the
 * given mode, using the main left motor and
 * main right motor, the first and second arguments
 * to this function. They must be reversed in the
 * "Motor and Sensor Setup" dialog for this to work.
 * There is also the option to use a fixed speed for
 * the arcade control should the need arise.
 */
void setupArcadeControl(
                   ControllerConstants mode,
                   tMotor leftMotor,
                   tMotor rightMotor,
                   int fixedSpeed = 0
                   ) {
	TVexJoysticks xAxis;
	TVexJoysticks yAxis;
	// Set up the axis to read values from
	if (mode == SingleJoystickLeft) {
		xAxis = Ch4;
		yAxis = Ch3;
	}	else if (mode == SingleJoystickRight) {
		xAxis = Ch1;
		yAxis = Ch2;
	} else if (mode == SingleJoystickLeftXmtr2) {
		xAxis = Ch4Xmtr2;
		yAxis = Ch3Xmtr2;
	} else if (mode == SingleJoystickRightXmtr2) {
		xAxis = Ch1Xmtr2;
		yAxis = Ch2Xmtr2;
	} else if (mode == DualJoystick) {
		xAxis = Ch1;
		yAxis = Ch3;
	} else if (mode == DualJoystickXmtr2) {
		xAxis = Ch1Xmtr2;
		yAxis = Ch3Xmtr2;
	}
	ac_leftMotor = leftMotor;
	ac_rightMotor = rightMotor;
	ac_fixedSpeed = fixedSpeed;
	ac_xAxis = xAxis;
	ac_yAxis = yAxis;
}

/*
 * startTask(arcadeControl);
 *
 * The task used for arcade control of
 * the robot. setupArcadeControl must
 * be called BEFORE using this, otherwise
 * undefined behaviour may occur.
 */
task arcadeControl() {
	// Define a few variables
	int xAxis;
	int yAxis;
	int leftMotor;
	int rightMotor;
	float multiplier;
	// Loop that runs the actual arcade control
	while (true) {
		// Get new joystick values
		xAxis = getValueOf(ac_xAxis, 10); // Thresholded to 10
		yAxis = getValueOf(ac_yAxis);
		if (ac_fixedSpeed && yAxis) {
			// Set the speed to the given "fixed"
			// speed only if it is both defined and
			// we have a valid joystick value
			yAxis = ac_fixedSpeed;
		}
		leftMotor = (yAxis * 3 / 4) + ((xAxis / 127) * (yAxis / 4));
		rightMotor = (xAxis * 3 / 4) - ((xAxis / 127) * (yAxis / 4));
		multiplier = 0;
		if (leftMotor || rightMotor) {
			if (leftMotor > rightMotor) {
				multiplier = yAxis / leftMotor;
			} else {
				multiplier = yAxis / rightMotor;
			}
		} else {
			if (xAxis < 0) {
				motor[ac_leftMotor] = -127;
				motor[ac_rightMotor] = 127;
			} else if (xAxis > 0) {
				motor[ac_leftMotor] = 127;
				motor[ac_rightMotor] = -127;
			}
		}
		motor[ac_leftMotor] = leftMotor * multiplier;
		motor[ac_rightMotor] = rightMotor * multiplier;
	}
}

/* SECTION: TANK DRIVE CONTROL */

// Variables
tMotor td_leftMotor;
tMotor td_rightMotor;
TVexJoysticks td_yAxisOne;
TVexJoysticks td_yAxisTwo;
int td_fixedSpeed;

/*
 * setupTankDriveControl(mode, leftMotor, rightMotor)
 * setupTankDriveControl(mode, leftMotor, rightMotor, fixedSpeed)
 *
 * This runs sets up arcade control based on the
 * given mode, using the main left motor and
 * main right motor, the first and second arguments
 * to this function. They must be reversed in the
 * "Motor and Sensor Setup" dialog for this to work.
 * There is also the option to use a fixed speed for
 * the arcade control should the need arise.
 */
void setupTankDriveControl(
                   ControllerConstants mode,
                   tMotor leftMotor,
                   tMotor rightMotor,
                   int fixedSpeed = 0
                   ) {
	TVexJoysticks yAxisOne;
	TVexJoysticks yAxisTwo;
	// Set up the axis to read values from
	if (mode == DualJoystick) {
		yAxisOne = Ch3;
		yAxisTwo = Ch2;
	} else if (mode == DualJoystickXmtr2) {
		yAxisOne = Ch3Xmtr2;
		yAxisTwo = Ch2Xmtr2;
	}
	td_leftMotor = leftMotor;
	td_rightMotor = rightMotor;
	td_fixedSpeed = fixedSpeed;
	td_yAxisOne = yAxisOne;
	td_yAxisTwo = yAxisTwo;
}

/*
 * startTask(tankDriveControl);
 *
 * The task used for arcade control of
 * the robot. setupTankDriveControl must
 * be called BEFORE using this, otherwise
 * undefined behaviour may occur.
 */
task tankDriveControl() {
	// Define a few variables
	int leftSide;
	int rightSide;
	// Loop that runs the actual tank drive control
	while (true) {
		// Get new joystick values
		leftSide = getValueOf(td_yAxisOne);
		rightSide = getValueOf(td_yAxisTwo);
		if (td_fixedSpeed) {
			// Set the fixed speed if needed
			if (leftSide) leftSide = td_fixedSpeed;
			if (rightSide) rightSide = td_fixedSpeed;
		}
		motor[td_leftMotor] = leftSide;
		motor[td_rightMotor] = rightSide;
	}
}
