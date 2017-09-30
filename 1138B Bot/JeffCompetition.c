#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    gyro,           sensorGyro)
#pragma config(Sensor, in2,    accelerometerZ, sensorAccelerometer)
#pragma config(Sensor, in3,    accelerometerY, sensorAccelerometer)
#pragma config(Sensor, in4,    accelerometerX, sensorAccelerometer)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_3,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_4,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_5,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           rightBaseOne,  tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           rightBaseTwo,  tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_2)
#pragma config(Motor,  port3,           rightBaseThree, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           rightTopLift,  tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_4)
#pragma config(Motor,  port5,           rightBottomLift, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           clawMotor,     tmotorVex393_MC29, openLoop, encoderPort, I2C_5)
#pragma config(Motor,  port7,           leftBottomLift, tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_3)
#pragma config(Motor,  port8,           leftBaseThree, tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           leftBaseTwo,   tmotorVex393_MC29, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port10,          leftBaseOne,   tmotorVex393_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

// Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"

// Common includes for functions
#include "common.c"

// Definitions to make the code more readable when referencing
// an assembly on the robot
#define LeftBase leftBaseTwo // The main left base motor.
#define RightBase rightBaseTwo // The main right base motor.
#define Lift rightTopLift // A lift motor.
#define Claw clawMotor // A claw motor.

/*
 * setupMotors()
 *
 * Sets up all the motors for the robot.
 * This includes slaving together multiple physical motor
 * mechanisms from the robot together
 */
void setupMotors() {
	// Slave the right side together
	slaveMotor(rightBaseOne, rightBaseTwo);
	slaveMotor(rightBaseThree, rightBaseTwo);

	// Slave the left side together
	slaveMotor(leftBaseOne, leftBaseTwo);
	slaveMotor(leftBaseThree, leftBaseTwo);

	// Slave the lift assembly together
	slaveMotor(rightBottomLift, rightTopLift);
	slaveMotor(leftBottomLift, rightTopLift);
}


void pre_auton()
{
  // Stop user created tasks between Autonomous and Driver controlled modes.
  bStopTasksBetweenModes = true;

	// Take control of the LCD
	bDisplayCompetitionStatusOnLcd = false;

  // Set up the motor assemblies as slaves, for use in autonomous and
	// driver control.
	setupMotors();
}


task autonomous() {
}


/*
 * User control code.
 *
 * Controls:
 *   Main Controller:
 *     Left Joystick - Left Base
 *     Right Joystick - Right Base
 *     Button 5U - Lift Up
 *     Button 5D - Lift Down
 */
task usercontrol() {
  while (true)
  {
		motor[LeftBase] = getValueOf(Ch3);
		motor[RightBase] = getValueOf(Ch2);
		motor[Lift] = btnAsChannel(Btn6);
		motor[Claw] = btnAsChannel(Btn5);
  }
}
