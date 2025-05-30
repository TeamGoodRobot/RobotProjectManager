# Web Joystick UI Script (`script.js`) Summary

## Overall Purpose
The `script.js` file implements a web-based user interface with two virtual joysticks and three digital buttons. Its primary goal is to capture user inputs from these elements, convert them into the SBUS (Serial Bus) protocol format, and send the resulting SBUS packets to a connected device (presumably a robot or microcontroller) via the WebSerial API. It also displays the raw SBUS channel values and the hexadecimal representation of the generated SBUS packet for debugging and monitoring.

## Interaction with HTML Elements
The script interacts with several HTML elements:
-   **Joysticks (`joystick1`, `joystick2`):** These are container elements for the virtual joysticks.
-   **Knobs (`knob1`, `knob2`):** Movable parts of the joysticks, visually representing the user's input.
-   **Buttons (`button1`, `button2`, `button3`):** Clickable buttons for digital inputs.
-   **SBUS Output Area (`sbusOutput`):** A textarea element used to display the generated SBUS packet (in hex) and the raw values of all 16 SBUS channels.
-   **Connect Button (`connectButton`):** Used to initiate and terminate the WebSerial connection.
-   **Serial Status (`serialStatus`):** A text element to display the current status of the WebSerial connection (e.g., "Connected", "Disconnected", error messages).

## State Management for Joysticks
For each of the two joysticks, the script maintains a state object (`joystick1State`, `joystick2State`). This state includes:
-   `isActive`: A boolean flag indicating if the user is currently interacting with (dragging) the joystick knob.
-   `knobX`, `knobY`: Normalized values (-1 to +1) representing the knob's current position relative to the joystick center.
-   `sbusX`, `sbusY`: The corresponding SBUS values (0-2047) for the X and Y axes, though these are primarily written directly to the global `sbusChannels` array.

Mouse events (`mousedown`, `mousemove`, `mouseup`) on the joystick knobs and the document control the `isActive` state and trigger joystick movement logic.

## Mapping Joystick Positions to SBUS Values
The normalized joystick positions (`state.knobX`, `state.knobY`, ranging from -1 to +1) are mapped to the standard SBUS channel value range of 0 to 2047. This is achieved using a `mapRange` helper function.
-   Joystick 1's X and Y positions are mapped to `sbusChannels[0]` and `sbusChannels[1]` respectively.
-   Joystick 2's X and Y positions are mapped to `sbusChannels[2]` and `sbusChannels[3]` respectively.
The script assumes positive Y movement (downwards on the screen) maps to higher SBUS values.

## SBUS Packet Structure and Generation
The script generates a 25-byte SBUS packet:
-   **Start Byte (`SBUS_START_BYTE`):** `0x0F`.
-   **Channel Data (22 bytes):** Contains data for 16 channels. Each channel uses 11 bits of data. These 16 channels (176 bits) are packed into 22 bytes (bytes 1-22 of the packet) using intricate bitwise operations. The values for these channels are taken from the `sbusChannels` array.
-   **Flags Byte (1 byte):** Byte 23 of the packet. In this script, it's hardcoded to `0x00`, implying no special flags like "frame lost" or "failsafe" are actively set by this script, nor are digital channels 17 and 18 (which are sometimes packed into this byte) utilized here.
-   **End Byte (`SBUS_END_BYTE`):** `0x00`.

The `generateSbusPacket()` function is responsible for assembling this byte array.

## Handling of Digital Button Inputs
-   Three digital buttons (`button1`, `button2`, `button3`) are implemented.
-   Their states (on/off) are mapped to SBUS channels 4, 5, and 6 respectively.
-   An "on" state is represented by the SBUS value `2047` (`SBUS_BUTTON_ON`).
-   An "off" state is represented by the SBUS value `0` (`SBUS_BUTTON_OFF`).
-   Clicking a button toggles its corresponding SBUS channel value and updates its visual appearance.

## WebSerial API Usage
The script utilizes the WebSerial API to communicate with an external device:
-   **Connection (`connectSerial`):**
    -   Checks if `navigator.serial` is supported.
    -   Prompts the user to select a serial port (`navigator.serial.requestPort()`).
    -   Opens the selected port with a baud rate of 100,000 (standard for SBUS).
    -   Gets a `WritableStreamDefaultWriter` (`writer`) to send data.
    -   Updates the connect button text and status display.
-   **Disconnection (`disconnectSerial`):**
    -   Closes the writer and the port if they are active.
    -   Updates the connect button text and status display.
-   **Sending Data:** The `updateSbusOutput()` function, after generating an SBUS packet, attempts to send it using `writer.write(sbusPacket)` if a serial connection is active.
-   **Error Handling:** Basic error handling is present for WebSerial operations, updating the status display and potentially auto-disconnecting on write errors.

## Key Functions and Their Roles
-   **`initializeJoysticks()`:** Sets the initial visual state of the joystick knobs (centering them) and populates the `sbusChannels` array with default resting values for joysticks and off states for buttons. Triggers an initial SBUS output update.
-   **`moveJoystick(joystickElement, knobElement, state, event)`:** Called during mouse movement when a joystick is active. It calculates the knob's new position based on mouse coordinates, constrains it within the joystick boundaries, updates the knob's visual style, converts the position to normalized values (-1 to +1), maps these to SBUS values, and updates the relevant entries in the `sbusChannels` array. Finally, it calls `updateSbusOutput()`.
-   **`updateSbusOutput()`:** Generates the SBUS packet using `generateSbusPacket()`. It then formats this packet and the raw `sbusChannels` values into human-readable strings and displays them in the `sbusOutput` textarea. If a serial connection is active (`writer` exists), it sends the binary SBUS packet over the serial port.
-   **`generateSbusPacket()`:** Constructs the 25-byte SBUS packet array using the `SBUS_START_BYTE`, data from the `sbusChannels` array (packed bitwise), the flags byte, and the `SBUS_END_BYTE`.
-   **`connectSerial()`:** Handles the process of connecting to a serial port via the WebSerial API.
-   **`disconnectSerial()`:** Handles the process of disconnecting from an active serial port.
-   **Helper Functions (`mapRange`, `clamp`):** Standard utility functions for mapping values between ranges and constraining a value within a min/max.
-   **Event Listeners:**
    -   `DOMContentLoaded`: Centers the joystick knobs visually once the page loads.
    -   `connectButton` ('click'): Toggles between `connectSerial()` and `disconnectSerial()`.
    -   Joystick knobs ('mousedown'): Set the respective joystick to active.
    -   Document ('mousemove'): Calls `moveJoystick` for any active joystick.
    -   Document ('mouseup'): Sets any active joystick to inactive.
    -   Digital buttons ('click'): Toggle their respective SBUS channel values and visual state, then call `updateSbusOutput()`.

The script initializes the joysticks and output on load and then primarily operates based on user interactions (mouse events for joysticks, clicks for buttons and serial connection). Each interaction that changes an SBUS channel's state triggers an update to the SBUS output display and attempts to send the new SBUS packet via serial if connected.
