#pragma systemFile // Eliminate warnings for "Unused" functions when imported

int MAX_TIME = 15000; // Run for 15 seconds, before stopping
int SMALLEST_INC = 50; // Minimum wait before sleep command is created

bool HUMAN_READABLE = false;

typedef struct {
	int i;
} Int;

/*
 * Autonomous-Running Code
 *
 * Allows running pre-recorded autonomous code. Auton,
 * the first parameter of this function, is an array,
 * with a repeating pattern of:
 *   {"instruction","arg1",["arg2" (if needed)], "instruction"...}
 */
void doAutonomous(Int *Auton) {
	int curr = 0;
	int instruction = 0;
	int arg = 0;
	for (int i = 0; true; i++) {
		curr = Auton[i].i;
		if (instruction == 0) {
			instruction = curr;
		} else if (instruction == 1) {
			// "sleep" command
			sleep(curr);
			instruction = 0;
		} else if (instruction == 2) {
			// "motor[] = value" command
			arg = curr;
			curr = Auton[++i].i;
			motor[arg] = curr;
			instruction = 0;
		} else if (instruction == 3) {
			// Stop all motors, and exit.
			for (int i = 0; i < 10; i++) {
				motor[i] = 0;
			}
			return;
		}
	}
}


task autonRecorder()
{
	writeDebugStream("Start auton recorder\n");
	int lastMotors[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int lastUpdateTime = 0;
	clearTimer(T1);
	string sleepFormat = "sleep(%d);\n";
	string motorFormat = "motor[%d] = %d;\n";
	if (HUMAN_READABLE) {
		for (int i = 0; i < 10; i++) {
			writeDebugStream("bMotorReflected[%d] = %d;", i, bMotorReflected[i]);
		}
	} else {
		sleepFormat = "1,%d,";
		motorFormat = "2,%d,%d";
		writeDebugStream("Int userAuton[]={");
	}
	while (time1[T1] <= MAX_TIME)
	{
		for (int i = 0; i < 10; i++) {
			if (motor[i] != lastMotors[i]) {
				lastMotors[i] = motor[i];
				if (abs(time1[T1] - lastUpdateTime) > SMALLEST_INC) {
					writeDebugStream(sleepFormat,time1[T1] - lastUpdateTime);
					lastUpdateTime = time1[T1];
				}
				writeDebugStream(motorFormat,i,motor[i]);
			}
		}
	}
	for (int i = 0; i < 10; i++) {
		motor[i] = 0; // Stop all motors
		if (!HUMAN_READABLE) {
			writeDebugStream("3};\ndoAutonomous(userAuton)");
		}
	}
	writeDebugStream("\nEnd Auton Recorder\n");
	stopAllTasks(); // Terminate the program and the task main() stuff
}
