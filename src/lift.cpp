#include "main.h"


const int num_of_pos = 3; // Number of lift positions
const int lift_heights[num_of_pos] = {0, 310, 500}; // Lift Positions


// Driver Control Variables
int up_lock = 0;
int down_lock = 0;
int lift_state = 0;


pros::Motor lift(6, MOTOR_GEARSET_36, false, MOTOR_ENCODER_DEGREES);


void set_lift(int input)  { lift = input; }

void zero_lift()    { lift.tare_position(); }
int  get_lift()     { return lift.get_position(); }
int  get_lift_vel() { return lift.get_actual_velocity(); }

void
set_lift_position(int target, int speed) {
  lift.move_absolute(target, speed);
}

///
// Driver Control
//  - when R1 is pressed, bring the lift up the position ladder
//  - when R2 is pressed, bring the lift down the position ladder
///
void
lift_control() {
  // Lift Up
  if (master.get_digital(DIGITAL_R1) && up_lock==0) {
    // If lift is at max height, bring it down to 0
    if(lift_state==num_of_pos-1)
      lift_state = 0;
    // Otherwise, bring the lift up
    else
      lift_state++;

    up_lock = 1;
  }
  else if (!master.get_digital(DIGITAL_R1)) {
    up_lock = 0;
  }

  // Lift Down
  if (master.get_digital(DIGITAL_R2) && down_lock==0) {
    // If the lift is down, bring the lift to max height
    if (lift_state==0)
      lift_state = num_of_pos-1;
    // Otherwise, bring the lift down
    else
      lift_state--;

    down_lock = 1;
  }
  else if (!master.get_digital(DIGITAL_R2)) {
    down_lock = 0;
  }

  // Set the lift to the current position in the array
  set_lift_position(lift_heights[lift_state], 100);
}
