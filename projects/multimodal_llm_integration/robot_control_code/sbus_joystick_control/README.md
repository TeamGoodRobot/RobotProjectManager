# SBUS Joystick Control Project

## Goal
To enable control of Unitree robots using SBUS packets generated from a simulated joystick interface.

## Hardware Interface
- An SBUS jack is reportedly available under the strap on the top of the robots (requires unscrewing the cover).
- A USB serial adapter will be needed to connect a computer to the SBUS jack for sending packets.

## Software Components
- **Webpage Interface:** A web-based interface provides a simulated joystick. User inputs (X,Y locations of two virtual joysticks) are captured here.
- **Serial Communication Layer:** Code that takes the joystick data from the webpage, converts it into the appropriate SBUS packet format, and sends it via a serial connection to the robot.

## Current Status
- The code for the webpage interface and the serial communication layer (for generating and sending SBUS packets) has been written.
- **Next Step:** Testing the entire system with a proper USB serial adapter connected to a Unitree robot.

## Future Considerations / Code Location
- Actual code files for the webpage and Python controller can be stored in this directory.
  (e.g., `webpage_joystick.html`, `sbus_controller.py`)
