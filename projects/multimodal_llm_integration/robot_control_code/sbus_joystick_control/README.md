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

---

## External Code Components & Related Controllers

The core implementation of the Web UI for SBUS joystick control, as well as an alternative PlayStation controller interface, resides in the `TeamGoodRobot/Robotics` repository.

### SBUS Web Joystick UI
- **Location:** `TeamGoodRobot/Robotics` repository.
- **Files:**
    - `index.html`: The main HTML structure for the web interface.
    - `script.js`: Contains the Javascript logic for joystick interaction, SBUS packet generation, and WebSerial communication. A summary of this script's functionality can be found here: [Web Joystick UI Code Summary](./web_joystick_ui_summary.md).
    - `style.css`: Handles the visual styling of the web interface.

### PlayStation Bluetooth Gamepad Interface
- **Location:** `TeamGoodRobot/Robotics` repository.
- **Purpose:** Provides an alternative method for robot control using a physical PlayStation controller connected via Bluetooth.
- **Files:**
    - `gamepad_reader.html`: Main HTML page for the gamepad interface.
    - `gamepad_script.js`: Javascript for reading gamepad inputs and likely mapping them to control signals (potentially also SBUS or another format).
    - `gamepad_style.css`: Styling for the gamepad interface page.

**Note:** While the code for these interfaces is external, this project directory (`sbus_joystick_control`) may contain notes, configuration details for integration, or scripts related to how these control methods interface with higher-level robot autonomy developed here (e.g., scripts to translate LLM commands into SBUS channel values that this UI could then process or that mimic its output).
