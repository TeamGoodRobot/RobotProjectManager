// Unitree SBUS Control Sketch for XIAO ESP32-S3
//
// This sketch sends SBUS commands to a Unitree robot controller
// using an XIAO ESP32-S3. It allows control via USB Serial commands
// to trigger basic movement sequences (nod, shake, tilt) and an unlock sequence.
//
// -----------------------------------------------------------------------------
// !!! IMPORTANT USER VERIFICATION REQUIRED !!!
// -----------------------------------------------------------------------------
// 1. SBUS CHANNEL ASSIGNMENTS: The SBUS channel mapping provided below is based
//    on user feedback (e.g., a Reddit comment) and common RC controller layouts.
//    You MUST VERIFY these channels against the Unitree GO2's (or your specific
//    robot's) SBUS documentation and UPDATE them if necessary. Incorrect channel
//    assignments can lead to unexpected or dangerous robot behavior.
//
// 2. UNLOCK SEQUENCE: The "unlock" sequence is based on analysis of external code
//    (e.g., mechzrobotics GitHub for Unitree GO1) and may need adjustment for
//    the GO2 or your specific robot. TEST WITH EXTREME CAUTION.
//
// 3. TEST WITH CAUTION: Always test SBUS control with extreme caution.
//    Start with a very small `SBUS_MOVEMENT_RANGE` (e.g., 20-50) to observe
//    robot response before increasing it. Ensure the robot is in a safe,
//    clear area during testing. Be prepared to disconnect power if needed.
//
// 4. HARDWARE CONFIGURATION: This sketch is configured for a
//    Seeed Studio XIAO ESP32-S3 using its default Serial1 pins:
//    - TX: GPIO2
//    - RX: GPIO1
//    If you are using a different ESP32-S3 board or different pins,
//    you MUST update `sbusSerial`, `SBUS_RX_PIN`, and `SBUS_TX_PIN`
//    definitions accordingly.
// -----------------------------------------------------------------------------

// Include the SBUS library for ESP32.
#include "sbus.h"

// Define the HardwareSerial port to be used for SBUS communication.
HardwareSerial& sbusSerial = Serial1;

// Define the specific RX and TX pins for the chosen SBUS serial port.
const int SBUS_RX_PIN = 1; // RX pin for sbusSerial (GPIO1 for XIAO ESP32-S3 Serial1)
const int SBUS_TX_PIN = 2; // TX pin for sbusSerial (GPIO2 for XIAO ESP32-S3 Serial1)

// --- SBUS Configuration Constants ---

// SBUS Channel Definitions (Indices 0-15 for sbusData.ch[])
// Based on user feedback (e.g., Reddit comment for Unitree Go2).
// !!! VERIFY THESE CHANNELS AGAINST YOUR ROBOT'S DOCUMENTATION !!!
const int SBUS_CHANNEL_RY = 0;  // CH1: Ry - Right stick Y (Likely Pitch)
const int SBUS_CHANNEL_RX = 1;  // CH2: Rx - Right stick X (Likely Roll)
const int SBUS_CHANNEL_LX = 2;  // CH3: Lx - Left stick X (Likely Yaw)
const int SBUS_CHANNEL_LY = 3;  // CH4: Ly - Left stick Y (Likely Throttle/Forward)
const int SBUS_CHANNEL_SWITCH_L = 4; // CH5: Left Switch (L2/L1 on gamepad) - Used in Unlock
const int SBUS_CHANNEL_SWITCH_R = 5; // CH6: Right Switch (R2/R1 on gamepad) - Used in Unlock
const int SBUS_CHANNEL_NC7 = 6;      // CH7: (Historically NC) Used as CH7_STATE in Unlock Sequence (START button logic)
const int SBUS_CHANNEL_BUTTON_A = 7; // CH8: Button A (Mode A) - Used in Unlock
const int SBUS_CHANNEL_BUTTON_B = 8; // CH9: Button B - Used in Unlock
const int SBUS_CHANNEL_BUTTON_C = 9; // CH10: Button C - Used in Unlock
const int SBUS_CHANNEL_BUTTON_D = 10;// CH11: Button D
const int SBUS_CHANNEL_SELECT = 11;  // CH12: Select Button
const int SBUS_CHANNEL_START = 12;   // CH13: Start Button (Note: Unlock logic uses CH7, not this directly)
const int SBUS_CHANNEL_NC14 = 13;    // CH14: Not Connected / Unused
const int SBUS_CHANNEL_NC15 = 14;    // CH15: Not Connected / Unused
const int SBUS_CHANNEL_NC16 = 15;    // CH16: Not Connected / Unused

// SBUS Value Definitions.
// Standard SBUS range is approximately 172-1811.
// Using defines for clarity in sequences, especially for unlock.
#define SBUS_VALUE_LOW 192      // A low SBUS signal value.
#define SBUS_VALUE_MID 992      // A mid-point/neutral SBUS signal value.
#define SBUS_VALUE_HIGH 1792    // A high SBUS signal value.

// Specific values used in the unlock sequence (derived from mechzrobotics example)
#define SBUS_SPECIFIC_CH9 1690  // Value for CH9 (SBUS_CHANNEL_BUTTON_B) during unlock sequence's default state.
#define SBUS_SPECIFIC_CH10 154 // Value for CH10 (SBUS_CHANNEL_BUTTON_C) during unlock sequence's default state.

const int SBUS_NEUTRAL_VALUE = SBUS_VALUE_MID; // Consistent neutral value.
const int SBUS_MOVEMENT_RANGE = 100;     // Defines +/- deflection from neutral for movements.
                                         // START WITH A SMALL VALUE (e.g., 20-50) FOR SAFETY.

// Timing Definitions
const int MOVEMENT_PAUSE_MS = 1000; // Pause duration within a movement part.
const int SBUS_PACKET_INTERVAL_MS = 14; // SBUS standard packet interval (approx 70Hz).
const int UNLOCK_STEP_DELAY_US = 700; // Delay in microseconds for unlock sequence steps.
const int UNLOCK_INITIAL_DELAY_MS = 50; // Initial delay for default packet in unlock.


// Create the SbusTx object.
bfs::SbusTx sbus(&sbusSerial, SBUS_RX_PIN, SBUS_TX_PIN, true, false);

// Global SbusData object.
bfs::SbusData sbusData;

// --- Movement & Sequence Functions ---

/**
 * @brief Performs a nodding motion by controlling SBUS_CHANNEL_RY (CH1 - Pitch).
 */
void performNod(bfs::SbusTx &sbus_obj, bfs::SbusData &data) {
  data.ch[SBUS_CHANNEL_RY] = SBUS_NEUTRAL_VALUE + SBUS_MOVEMENT_RANGE;
  sbus_obj.data(data); sbus_obj.Write(); delay(MOVEMENT_PAUSE_MS);
  data.ch[SBUS_CHANNEL_RY] = SBUS_NEUTRAL_VALUE - SBUS_MOVEMENT_RANGE;
  sbus_obj.data(data); sbus_obj.Write(); delay(MOVEMENT_PAUSE_MS);
  data.ch[SBUS_CHANNEL_RY] = SBUS_NEUTRAL_VALUE;
  // Final write handled by main loop's continuous send.
}

/**
 * @brief Performs a shaking motion by controlling SBUS_CHANNEL_LX (CH3 - Yaw).
 */
void performShake(bfs::SbusTx &sbus_obj, bfs::SbusData &data) {
  data.ch[SBUS_CHANNEL_LX] = SBUS_NEUTRAL_VALUE + SBUS_MOVEMENT_RANGE;
  sbus_obj.data(data); sbus_obj.Write(); delay(MOVEMENT_PAUSE_MS);
  data.ch[SBUS_CHANNEL_LX] = SBUS_NEUTRAL_VALUE - SBUS_MOVEMENT_RANGE;
  sbus_obj.data(data); sbus_obj.Write(); delay(MOVEMENT_PAUSE_MS);
  data.ch[SBUS_CHANNEL_LX] = SBUS_NEUTRAL_VALUE;
}

/**
 * @brief Performs a tilting motion by controlling SBUS_CHANNEL_RX (CH2 - Roll).
 */
void performTilt(bfs::SbusTx &sbus_obj, bfs::SbusData &data) {
  data.ch[SBUS_CHANNEL_RX] = SBUS_NEUTRAL_VALUE + SBUS_MOVEMENT_RANGE;
  sbus_obj.data(data); sbus_obj.Write(); delay(MOVEMENT_PAUSE_MS);
  data.ch[SBUS_CHANNEL_RX] = SBUS_NEUTRAL_VALUE - SBUS_MOVEMENT_RANGE;
  sbus_obj.data(data); sbus_obj.Write(); delay(MOVEMENT_PAUSE_MS);
  data.ch[SBUS_CHANNEL_RX] = SBUS_NEUTRAL_VALUE;
}

/**
 * @brief Implements the robot unlock sequence.
 * This sequence is based on the logic found in mechzrobotics' UnitreeInterface
 * for the START button, which toggles CH7_STATE (mapped to our SBUS_CHANNEL_NC7, index 6).
 * It involves setting specific values for channels 5, 6, 7, 8, 9, 10 and then
 * toggling CH7 (SBUS_CHANNEL_NC7).
 * !!! THIS SEQUENCE IS EXPERIMENTAL AND REQUIRES VERIFICATION FOR YOUR ROBOT !!!
 */
void performUnlockSequence(bfs::SbusTx &sbus_obj, bfs::SbusData &data) {
    Serial.println("Executing: Unlock sequence...");

    // Step 1: Set initial "default" SBUS packet state based on mechzrobotics example
    // Initialize all channels to mid first, then set specifics.
    for (int i = 0; i < bfs::SbusData::NUM_CH; i++) {
        data.ch[i] = SBUS_VALUE_MID; // Default neutral for most
    }
    data.ch[SBUS_CHANNEL_SWITCH_L] = SBUS_VALUE_MID;    // CH5  (idx 4)
    data.ch[SBUS_CHANNEL_SWITCH_R] = SBUS_VALUE_LOW;    // CH6  (idx 5)
    data.ch[SBUS_CHANNEL_NC7] = SBUS_VALUE_LOW;        // CH7  (idx 6) - This is CH7_STATE in example, starts LOW
    data.ch[SBUS_CHANNEL_BUTTON_A] = SBUS_VALUE_MID;    // CH8  (idx 7)
    data.ch[SBUS_CHANNEL_BUTTON_B] = SBUS_SPECIFIC_CH9; // CH9  (idx 8)
    data.ch[SBUS_CHANNEL_BUTTON_C] = SBUS_SPECIFIC_CH10;// CH10 (idx 9)

    sbus_obj.data(data);
    sbus_obj.Write();
    delay(UNLOCK_INITIAL_DELAY_MS); // Allow this default state to be processed

    // Unlock Toggle Sequence (simulating START button press logic from mechzrobotics)
    // The sequence involves toggling CH7 (SBUS_CHANNEL_NC7) HIGH then LOW.
    // CH5 (SBUS_CHANNEL_SWITCH_L) remains SBUS_VALUE_MID throughout.
    // Other channels (CH6, CH8, CH9, CH10) remain as set in the initial packet.

    // Unlock Step 2: CH7 (SBUS_CHANNEL_NC7) goes HIGH
    // data.ch[SBUS_CHANNEL_SWITCH_L] is already SBUS_VALUE_MID
    data.ch[SBUS_CHANNEL_NC7] = SBUS_VALUE_HIGH;     // CH7 (idx 6) toggled HIGH
    sbus_obj.data(data);
    sbus_obj.Write();
    delayMicroseconds(UNLOCK_STEP_DELAY_US);

    // Unlock Step 3: CH7 (SBUS_CHANNEL_NC7) goes LOW again
    // data.ch[SBUS_CHANNEL_SWITCH_L] is already SBUS_VALUE_MID
    data.ch[SBUS_CHANNEL_NC7] = SBUS_VALUE_LOW;      // CH7 (idx 6) toggled back to LOW
    sbus_obj.data(data);
    sbus_obj.Write();
    delayMicroseconds(UNLOCK_STEP_DELAY_US);

    Serial.println("Unlock sequence complete. Robot should be responsive if successful.");

    // For safety, ensure all primary control/stick channels (0-3) are neutral after this.
    // CH5-CH10 will remain in the state they were left by the sequence (CH7 is LOW).
    data.ch[SBUS_CHANNEL_RY] = SBUS_NEUTRAL_VALUE;
    data.ch[SBUS_CHANNEL_RX] = SBUS_NEUTRAL_VALUE;
    data.ch[SBUS_CHANNEL_LX] = SBUS_NEUTRAL_VALUE;
    data.ch[SBUS_CHANNEL_LY] = SBUS_NEUTRAL_VALUE;

    // Send one final packet with controls neutral.
    // The main loop will continue sending this state or subsequent commands.
    sbus_obj.data(data);
    sbus_obj.Write();
    // A small delay to ensure packet is sent before loop continues, though main loop has its own delay.
    delay(SBUS_PACKET_INTERVAL_MS);
}


/**
 * @brief Prints the help menu to the Serial monitor.
 */
void printHelp() {
    Serial.println("Available commands (send newline terminated):");
    Serial.println("  nod    - Perform nod sequence (Pitch on CH1/Ry)");
    Serial.println("  shake  - Perform shake sequence (Yaw on CH3/Lx)");
    Serial.println("  tilt   - Perform tilt sequence (Roll on CH2/Rx)");
    Serial.println("  unlock - Attempt robot unlock sequence");
    Serial.println("  neutral- Set all control channels to neutral");
    Serial.println("  help   - Shows this list");
}

/**
 * @brief Arduino setup function, runs once at startup.
 */
void setup() {
  sbus.Begin(); // Initialize SBUS communication (Serial1)
  Serial.begin(115200); // Initialize USB Serial for commands
  while (!Serial);      // Wait for Serial Monitor

  Serial.println("\nUnitree SBUS Control Initialized.");
  printHelp();
}

/**
 * @brief Arduino main loop function, runs repeatedly after setup().
 */
void loop() {
  // Initialize all 16 SBUS channels to neutral at the start of each loop iteration.
  for (int i = 0; i < bfs::SbusData::NUM_CH; i++) {
    sbusData.ch[i] = SBUS_NEUTRAL_VALUE;
  }
  // Explicitly ensure any other potentially used channels (like buttons not part of unlock's specific values) are neutral.
  // sbusData.ch[SBUS_CHANNEL_BUTTON_A] = SBUS_NEUTRAL_VALUE; // Example, though unlock sets it. Redundant here if unlock used.

  // Check for incoming USB Serial commands
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    command.toLowerCase();

    if (command.equals("nod")) {
      Serial.println("Executing: Nod sequence (Pitch on CH1/Ry)");
      performNod(sbus, sbusData);
    } else if (command.equals("shake")) {
      Serial.println("Executing: Shake sequence (Yaw on CH3/Lx)");
      performShake(sbus, sbusData);
    } else if (command.equals("tilt")) {
      Serial.println("Executing: Tilt sequence (Roll on CH2/Rx)");
      performTilt(sbus, sbusData);
    } else if (command.equals("unlock")) {
      performUnlockSequence(sbus, sbusData); // This function handles its own Serial.println feedback
    } else if (command.equals("neutral")) {
      Serial.println("Setting all control channels to neutral.");
      // All channels already set to neutral at loop start. This command ensures it's sent.
    } else if (command.equals("help")) {
      printHelp();
    } else if (command.length() > 0) {
      Serial.print("Unknown command: '");
      Serial.print(command);
      Serial.println("'");
    }
    // sbusData is now updated by the command or remains neutral from loop start.
  }

  // --- Autonomous Movement Sequences (Commented out for Serial Control) ---
  /*
  performNod(sbus, sbusData); delay(2000);
  performShake(sbus, sbusData); delay(2000);
  performTilt(sbus, sbusData); delay(2000);
  */

  // Continuously send the current sbusData state.
  sbus.data(sbusData);
  sbus.Write();

  // Delay to maintain SBUS packet rate.
  delay(SBUS_PACKET_INTERVAL_MS);
}
