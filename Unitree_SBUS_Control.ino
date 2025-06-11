// Unitree SBUS Control Sketch for XIAO ESP32-S3
//
// This sketch sends SBUS commands to a Unitree robot controller
// using an XIAO ESP32-S3 to perform basic movement sequences:
// nodding (pitch), shaking (yaw), and tilting (roll).
//
// -----------------------------------------------------------------------------
// !!! IMPORTANT USER VERIFICATION REQUIRED !!!
// -----------------------------------------------------------------------------
// 1. SBUS CHANNEL ASSIGNMENTS: The SBUS channels defined in this sketch
//    (SBUS_CHANNEL_PITCH, SBUS_CHANNEL_YAW, SBUS_CHANNEL_ROLL) are
//    ASSUMPTIONS. You MUST VERIFY these channels against the Unitree
//    GO2's (or your specific robot's) SBUS documentation and UPDATE them
//    as necessary. Incorrect channel assignments can lead to unexpected
//    robot behavior.
//
// 2. TEST WITH CAUTION: Always test SBUS control with extreme caution.
//    Start with a very small `SBUS_MOVEMENT_RANGE` (e.g., 20-50) to observe
//    robot response before increasing it. Ensure the robot is in a safe,
//    clear area during testing. Be prepared to disconnect power if needed.
//
// 3. HARDWARE CONFIGURATION: This sketch is configured for a
//    Seeed Studio XIAO ESP32-S3 using its default Serial1 pins:
//    - TX: GPIO2
//    - RX: GPIO1
//    If you are using a different ESP32-S3 board or different pins,
//    you MUST update `sbusSerial`, `SBUS_RX_PIN`, and `SBUS_TX_PIN`
//    definitions accordingly.
// -----------------------------------------------------------------------------

// Include the SBUS library for ESP32.
// This library handles the SBUS protocol encoding and serial communication.
#include "sbus.h"

// Define the HardwareSerial port to be used for SBUS communication.
// For XIAO ESP32-S3, Serial1 is typically available and uses GPIO1 (RX) and GPIO2 (TX) by default.
HardwareSerial& sbusSerial = Serial1;

// Define the specific RX and TX pins for the chosen SBUS serial port.
// These must match the pins connected to your SBUS signal line (via an inverter if needed).
const int SBUS_RX_PIN = 1; // RX pin for sbusSerial (GPIO1 for XIAO ESP32-S3 Serial1)
const int SBUS_TX_PIN = 2; // TX pin for sbusSerial (GPIO2 for XIAO ESP32-S3 Serial1)

// --- SBUS Configuration Constants ---

// SBUS Channel Definitions for movements.
// !!! THESE ARE ASSUMPTIONS - VERIFY AND UPDATE FOR YOUR ROBOT !!!
const int SBUS_CHANNEL_PITCH = 0; // Assumed channel for Pitch control (e.g., nodding).
const int SBUS_CHANNEL_YAW = 1;   // Assumed channel for Yaw control (e.g., shaking head).
const int SBUS_CHANNEL_ROLL = 2;  // Assumed channel for Roll control (e.g., tilting side to side).

// SBUS Value Definitions.
// SBUS typically uses a range from around 172 to 1811 (FrSky specific, but common).
// Mid-point is (172 + 1811) / 2 = 991.5. We use 992.
const int SBUS_NEUTRAL_VALUE = 992;      // The neutral or center value for an SBUS channel.
const int SBUS_MOVEMENT_RANGE = 100;     // Defines the +/- deflection from neutral for movements.
                                         // Adjust this value to control the intensity of the movement.
                                         // START WITH A SMALL VALUE (e.g., 20-50) FOR SAFETY.

// Timing Definitions for movement sequences.
const int MOVEMENT_PAUSE_MS = 1000; // Pause duration within a movement part (e.g., after nodding up, before nodding down).
const int SEQUENCE_PAUSE_MS = 2000; // Pause duration between different movement sequences (e.g., after nod sequence, before shake sequence).

// Create the SbusTx object.
// This object is responsible for formatting and sending SBUS data.
// Parameters:
//   - &sbusSerial: Reference to the HardwareSerial object (e.g., &Serial1).
//   - SBUS_RX_PIN, SBUS_TX_PIN: The RX and TX pins for the serial communication.
//     (Note: For SbusTx, typically only TX is actively used, RX might be for configuration or duplex modes not used here).
//   - inv: `true` for inverted SBUS logic (standard for SBUS), `false` for non-inverted.
//   - fast: `false` for standard SBUS baud rate of 100000. `true` would use 200000 (less common).
bfs::SbusTx sbus(&sbusSerial, SBUS_RX_PIN, SBUS_TX_PIN, true, false);

// Global SbusData object.
// This structure holds the values for all 16 SBUS channels, plus failsafe and lost frame flags.
bfs::SbusData sbusData;

// --- Movement Functions ---

/**
 * @brief Performs a nodding motion by controlling the Pitch SBUS channel.
 * The robot is commanded to pitch up, pause, pitch down, pause, then return to neutral.
 * @param sbus_obj Reference to the SbusTx object used for sending commands.
 * @param data Reference to the SbusData object holding channel values.
 *             This function will modify data.ch[SBUS_CHANNEL_PITCH].
 */
void performNod(bfs::SbusTx &sbus_obj, bfs::SbusData &data) {
  // Nod Up (Pitch forward/up): Set channel to neutral + range
  data.ch[SBUS_CHANNEL_PITCH] = SBUS_NEUTRAL_VALUE + SBUS_MOVEMENT_RANGE;
  sbus_obj.data(data); // Update the sbus_obj's internal data buffer
  sbus_obj.Write();    // Send the SBUS packet
  delay(MOVEMENT_PAUSE_MS);

  // Nod Down (Pitch backward/down): Set channel to neutral - range
  data.ch[SBUS_CHANNEL_PITCH] = SBUS_NEUTRAL_VALUE - SBUS_MOVEMENT_RANGE;
  sbus_obj.data(data);
  sbus_obj.Write();
  delay(MOVEMENT_PAUSE_MS);

  // Return to Neutral Pitch: Set channel back to neutral
  data.ch[SBUS_CHANNEL_PITCH] = SBUS_NEUTRAL_VALUE;
  sbus_obj.data(data);
  sbus_obj.Write();
}

/**
 * @brief Performs a shaking motion by controlling the Yaw SBUS channel.
 * The robot is commanded to yaw left, pause, yaw right, pause, then return to neutral.
 * @param sbus_obj Reference to the SbusTx object.
 * @param data Reference to the SbusData object.
 *             This function will modify data.ch[SBUS_CHANNEL_YAW].
 */
void performShake(bfs::SbusTx &sbus_obj, bfs::SbusData &data) {
  // Shake Left (Yaw left): Set channel to neutral + range
  data.ch[SBUS_CHANNEL_YAW] = SBUS_NEUTRAL_VALUE + SBUS_MOVEMENT_RANGE;
  sbus_obj.data(data);
  sbus_obj.Write();
  delay(MOVEMENT_PAUSE_MS);

  // Shake Right (Yaw right): Set channel to neutral - range
  data.ch[SBUS_CHANNEL_YAW] = SBUS_NEUTRAL_VALUE - SBUS_MOVEMENT_RANGE;
  sbus_obj.data(data);
  sbus_obj.Write();
  delay(MOVEMENT_PAUSE_MS);

  // Return to Neutral Yaw: Set channel back to neutral
  data.ch[SBUS_CHANNEL_YAW] = SBUS_NEUTRAL_VALUE;
  sbus_obj.data(data);
  sbus_obj.Write();
}

/**
 * @brief Performs a tilting motion by controlling the Roll SBUS channel.
 * The robot is commanded to roll left, pause, roll right, pause, then return to neutral.
 * @param sbus_obj Reference to the SbusTx object.
 * @param data Reference to the SbusData object.
 *             This function will modify data.ch[SBUS_CHANNEL_ROLL].
 */
void performTilt(bfs::SbusTx &sbus_obj, bfs::SbusData &data) {
  // Tilt Left (Roll left): Set channel to neutral + range
  data.ch[SBUS_CHANNEL_ROLL] = SBUS_NEUTRAL_VALUE + SBUS_MOVEMENT_RANGE;
  sbus_obj.data(data);
  sbus_obj.Write();
  delay(MOVEMENT_PAUSE_MS);

  // Tilt Right (Roll right): Set channel to neutral - range
  data.ch[SBUS_CHANNEL_ROLL] = SBUS_NEUTRAL_VALUE - SBUS_MOVEMENT_RANGE;
  sbus_obj.data(data);
  sbus_obj.Write();
  delay(MOVEMENT_PAUSE_MS);

  // Return to Neutral Roll: Set channel back to neutral
  data.ch[SBUS_CHANNEL_ROLL] = SBUS_NEUTRAL_VALUE;
  sbus_obj.data(data);
  sbus_obj.Write();
}

/**
 * @brief Arduino setup function, runs once at startup.
 */
void setup() {
  // Initialize SBUS communication.
  // The sbus.Begin() method configures the specified serial port (sbusSerial)
  // with the correct baud rate (100000 for standard SBUS) and serial parameters
  // (typically 8 data bits, even parity, 2 stop bits - 8E2).
  sbus.Begin();
}

/**
 * @brief Arduino main loop function, runs repeatedly after setup().
 */
void loop() {
  // Initialize all 16 SBUS channels to the neutral value at the start of each loop iteration.
  // This ensures that any channels not actively controlled by a movement sequence
  // are sending a known, neutral command. It also resets channels after a sequence
  // (though our movement functions also restore their specific channel).
  for (int i = 0; i < 16; i++) {
    sbusData.ch[i] = SBUS_NEUTRAL_VALUE;
  }

  // Set failsafe and lost_frame status.
  // For active control, these are typically set to false.
  // `failsafe = true` would indicate a failsafe condition triggered by the receiver.
  // `lost_frame = true` would indicate a lost SBUS frame.
  sbusData.failsafe = false;
  sbusData.lost_frame = false;

  // --- Perform Movement Sequences ---
  // Each sequence is called, followed by a pause.
  // The movement functions directly send SBUS commands.

  // Perform Nodding sequence (controls Pitch channel)
  performNod(sbus, sbusData);
  delay(SEQUENCE_PAUSE_MS); // Pause after completing the nod sequence

  // Perform Shaking sequence (controls Yaw channel)
  performShake(sbus, sbusData);
  delay(SEQUENCE_PAUSE_MS); // Pause after completing the shake sequence

  // Perform Tilting sequence (controls Roll channel)
  performTilt(sbus, sbusData);
  delay(SEQUENCE_PAUSE_MS); // Pause after completing the tilt sequence

  // The loop will then repeat, re-initializing channels to neutral and
  // executing the sequences again. The `SEQUENCE_PAUSE_MS` after the last
  // sequence provides a delay before the entire cycle restarts.
}
