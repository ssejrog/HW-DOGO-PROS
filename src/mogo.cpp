#include "main.h"


const int MOGO_OUT  = 550; // Intaking Mogos


// Driver Control Variables
bool mogo_up = true;
bool is_at_neut = false;
int mogo_lock   = 0;
int controller_mogo_timer = 0;
int mogo_out_timer = 0;

bool is_up = false;
bool is_out = false;


pros::Motor mogo(15, MOTOR_GEARSET_36, false, MOTOR_ENCODER_DEGREES);


void set_mogo(int input)  { mogo = input; }

void zero_mogo()    { mogo.tare_position(); }
int  get_mogo()     { return mogo.get_position(); }
int  get_mogo_vel() { return mogo.get_actual_velocity(); }

void
set_mogo_position(int target, int speed) {
  mogo.move_absolute(target, speed);
}


///
// Mogo Control
//  - we like to use the same functions for autonomous and driver,
//  - so these functions are used for both.
///

// Mogo In
void
mogo_in (bool hold) {
  // If mogo lift is almost at 0...
  if (get_mogo()<150) {
    // If velocity is zero or the position is < 0, then stop the mogo
    if (get_mogo_vel()==0 || get_mogo()<0) {
      is_up = true;
      set_mogo(0);
    }
    // Otherwise, come up slowly
    else {
      set_mogo(is_up?0:-40);
    }
  }
  // Otherwise, run mogo lift at full speed in
  else {
    is_up = false;
    set_mogo(-127);
  }

  // If running during autonomous,
  if (hold) {
    // Set states so the mogo will be in the last position in driver it was in during auto
    mogo_up = true;
    // Loop if robot isn't there yet
    pros::delay(DELAY_TIME);
    mogo_in(!is_up);
  }
}

// Mogo Out
void
mogo_out(bool hold) {
  // If mogo lift is almost at MOGO_OUT...
  if (get_mogo() > (MOGO_OUT-100)) {
    // If velocity is 0, turn motor off
    if (get_mogo_vel()==0) {
      set_mogo(2);
      is_out = true;
    }
    // Otherwise, start a timer for 500ms, and then turn the motors off
    else {
      mogo_out_timer+=DELAY_TIME;
      if (mogo_out_timer<500) {
        set_mogo(20);
        is_out = false;
      }
      else {
        set_mogo(0);
        is_out = true;
      }
    }
  }
  // Otherwise, run the mogo lift out at full speed
  else {
    set_mogo(127);
    mogo_out_timer = 0;
    is_out = false;
  }

  // If running during autonomous,
  if (hold) {
    // Set states so the mogo will be in the last position in driver it was in during auto
    mogo_up = false;
    // Loop if robot isn't there yet
    pros::delay(DELAY_TIME);
    mogo_out(!is_out);
  }
}


///
// Driver Control
//  - when L1 is pressed, toggle between in and out.
///
void
mogo_control() {
  // Toggle for mogo
  if (master.get_digital(DIGITAL_L1) && mogo_lock==0) {
    mogo_up = !mogo_up;

    is_at_neut = false;
    mogo_lock = 1;
  }
  else if (!master.get_digital(DIGITAL_L1)) {
    mogo_lock  = 0;
    controller_mogo_timer = 0;
  }

  // Bring moog to position based on mogo_up
  if (mogo_up)
    mogo_in();
  else if (!mogo_up)
    mogo_out();
}
