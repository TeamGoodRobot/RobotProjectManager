// Unitree GO2 SBUS Control for XIAO ESP32-S3
//
// IMPORTANT USER INFORMATION & WARNINGS:
// 1. SBUS Channel Mapping: The SBUS channel assignments used in this sketch
//    (SBUS_CHANNEL_RY, SBUS_CHANNEL_RX, etc.) are based on user-provided
//    feedback, reportedly from a Reddit comment. These mappings MUST BE VERIFIED
//    against the official Unitree GO2 documentation or your specific remote
//    controller's configuration. Incorrect channel mapping can lead to
//    unexpected robot behavior.
// 2. Test With Extreme Caution: Always test this code with the robot in a safe,
//    clear area, preferably with the robot elevated or restrained initially to
//    prevent damage or injury from unexpected movements.
// 3. Movement Range: Start with a very small `SBUS_MOVEMENT_RANGE` (e.g., 50-100)
//    and gradually increase it once you are confident about the robot's response.
// 4. Serial Port & Pins: This sketch is configured for `Serial1` on a
//    Seeed Studio XIAO ESP32-S3, using GPIO1 as RX and GPIO2 as TX.
//    If you are using a different ESP32 board or different pins, you MUST
//    update the `sbusSerial`, `SBUS_RX_PIN`, and `SBUS_TX_PIN` definitions.
// 5. Unlock Sequence: The "unlock" sequence is based on an analysis of external
//    code (mechzrobotics/Unitree_GO2_SBUS). While it aims to replicate a
//    functional unlock, its behavior on your specific robot should be carefully
//    verified. Ensure default states of channels are safe.
// 6. Power: Ensure your ESP32 and the robot are powered appropriately. SBUS
//    communication requires a common ground.

#include "sbus.h" // Bolder Flight Systems SBUS Library

// SBUS Serial Configuration (for XIAO ESP32-S3, Serial1 is on GPIO1(RX) and GPIO2(TX))
HardwareSerial sbusSerial(1); // Use UART1
const int SBUS_RX_PIN = 1;    // Serial1 RX pin on XIAO ESP32-S3
const int SBUS_TX_PIN = 2;    // Serial1 TX pin on XIAO ESP32-S3

// SBUS object, for writing SBUS data
// Parameters: Serial port, RX pin, TX pin, inverted_logic, use_fast_sbus
// For ESP32, RX/TX pins must be provided to the SbusTx constructor.
// Standard SBUS is inverted, so 'true'. 'false' for standard 100000 baud.
bfs::SbusTx sbus(&sbusSerial, SBUS_RX_PIN, SBUS_TX_PIN, true, false);
bfs::SbusData sbusData; // Holds the SBUS channel data

// SBUS Channel Constants (0-indexed for array access)
// Based on user feedback (from Reddit), VERIFY THESE FOR YOUR ROBOT!
const int SBUS_CHANNEL_RY = 0;        // CH1: Ry - Right stick Y (often Pitch)
const int SBUS_CHANNEL_RX = 1;        // CH2: Rx - Right stick X (often Roll)
const int SBUS_CHANNEL_LX = 2;        // CH3: Lx - Left stick X (often Yaw)
const int SBUS_CHANNEL_LY = 3;        // CH4: Ly - Left stick Y (often Throttle/Forward)
const int SBUS_CHANNEL_SWITCH_L = 4;  // CH5: LOW L2 HIGH L1 (3-pos switch or buttons)
const int SBUS_CHANNEL_SWITCH_R = 5;  // CH6: LOW R2 HIGH R1 (3-pos switch or buttons)
const int SBUS_CHANNEL_NC7 = 6;       // CH7: NC (No Connection - but used for Unlock Seq.)
const int SBUS_CHANNEL_BUTTON_A = 7;  // CH8: HIGH A (Button A / Mode A)
const int SBUS_CHANNEL_BUTTON_B = 8;  // CH9: HIGH B (Button B) - Used for specific value in unlock
const int SBUS_CHANNEL_BUTTON_C = 9;  // CH10: HIGH C (Button C) - Used for specific value in unlock
const int SBUS_CHANNEL_BUTTON_D = 10; // CH11: HIGH D (Button D)
const int SBUS_CHANNEL_SELECT = 11;   // CH12: Select
const int SBUS_CHANNEL_START = 12;    // CH13: Start
const int SBUS_CHANNEL_NC14 = 13;     // CH14: NC
const int SBUS_CHANNEL_NC15 = 14;     // CH15: NC
const int SBUS_CHANNEL_NC16 = 15;     // CH16: NC

// SBUS Value Constants
// FrSky receivers typically output 172-1811 for -100% to +100%.
// Full 11-bit range is 0-2047.
// The mechzrobotics code uses 192 (LOW), 992 (MID), 1792 (HIGH).
#define SBUS_VALUE_LOW 192
#define SBUS_VALUE_MID 992
#define SBUS_VALUE_HIGH 1792
#define SBUS_NEUTRAL_VALUE SBUS_VALUE_MID // Alias for clarity

// Specific channel values from mechzrobotics example for unlock sequence context
#define SBUS_SPECIFIC_CH9 1690 // CH9 is SBUS_CHANNEL_BUTTON_B (index 8)
#define SBUS_SPECIFIC_CH10 154  // CH10 is SBUS_CHANNEL_BUTTON_C (index 9)


// Movement & Timing Constants
const int SBUS_MOVEMENT_RANGE = 100;   // Adjust carefully! Defines magnitude of movement from neutral.
const int MOVEMENT_PAUSE_MS = 1000;    // Pause between steps within a movement sequence (1 sec)
const int SEQUENCE_PAUSE_MS = 2000;    // Pause between different movement sequences (2 sec)
const int SBUS_PACKET_INTERVAL_MS = 14; // Approx 71Hz, common for SBUS. (Can be 10-20ms)
const int UNLOCK_STEP_DELAY_US = 700;   // Microseconds delay for unlock sequence steps
const int UNLOCK_INITIAL_DELAY_MS = 50; // Milliseconds delay for initial state in unlock

void printHelp() {
    Serial.println(F("--------------------------------------------------"));
    Serial.println(F("Unitree SBUS Control - XIAO ESP32-S3"));
    Serial.println(F("Available commands (send newline terminated):"));
    Serial.println(F("  nod        - Perform pitch sequence (CH1/Ry)"));
    Serial.println(F("  shake      - Perform yaw sequence (CH3/Lx)"));
    Serial.println(F("  tilt       - Perform roll sequence (CH2/Rx)"));
    Serial.println(F("  neutral    - Set control channels to neutral"));
    Serial.println(F("  unlock     - Attempt robot unlock sequence (CH5/CH7)"));
    Serial.println(F("  help       - Shows this command list"));
    Serial.println(F("--------------------------------------------------"));
    Serial.println(F("IMPORTANT: Verify CH mappings & test cautiously!"));
    Serial.println(F("Unlock sequence is experimental. Monitor robot."));
}

// Function to perform a nodding motion (Pitch)
// Assumes SBUS_CHANNEL_RY (CH1) controls pitch
void performNod(bfs::SbusTx &sbus_obj, bfs::SbusData &data_obj) {
    Serial.println(F("Executing: Nod sequence (Pitch on CH1/Ry)"));

    // Upward nod
    data_obj.ch[SBUS_CHANNEL_RY] = SBUS_NEUTRAL_VALUE + SBUS_MOVEMENT_RANGE;
    sbus_obj.data(data_obj);
    sbus_obj.Write();
    delay(MOVEMENT_PAUSE_MS);

    // Downward nod
    data_obj.ch[SBUS_CHANNEL_RY] = SBUS_NEUTRAL_VALUE - SBUS_MOVEMENT_RANGE;
    sbus_obj.data(data_obj);
    sbus_obj.Write();
    delay(MOVEMENT_PAUSE_MS);

    // Return to neutral
    data_obj.ch[SBUS_CHANNEL_RY] = SBUS_NEUTRAL_VALUE;
    sbus_obj.data(data_obj);
    sbus_obj.Write();
    Serial.println(F("Nod sequence complete."));
}

// Function to perform a shaking motion (Yaw)
// Assumes SBUS_CHANNEL_LX (CH3) controls yaw
void performShake(bfs::SbusTx &sbus_obj, bfs::SbusData &data_obj) {
    Serial.println(F("Executing: Shake sequence (Yaw on CH3/Lx)"));

    // Left shake
    data_obj.ch[SBUS_CHANNEL_LX] = SBUS_NEUTRAL_VALUE + SBUS_MOVEMENT_RANGE;
    sbus_obj.data(data_obj);
    sbus_obj.Write();
    delay(MOVEMENT_PAUSE_MS);

    // Right shake
    data_obj.ch[SBUS_CHANNEL_LX] = SBUS_NEUTRAL_VALUE - SBUS_MOVEMENT_RANGE;
    sbus_obj.data(data_obj);
    sbus_obj.Write();
    delay(MOVEMENT_PAUSE_MS);

    // Return to neutral
    data_obj.ch[SBUS_CHANNEL_LX] = SBUS_NEUTRAL_VALUE;
    sbus_obj.data(data_obj);
    sbus_obj.Write();
    Serial.println(F("Shake sequence complete."));
}

// Function to perform a tilting motion (Roll)
// Assumes SBUS_CHANNEL_RX (CH2) controls roll
void performTilt(bfs::SbusTx &sbus_obj, bfs::SbusData &data_obj) {
    Serial.println(F("Executing: Tilt sequence (Roll on CH2/Rx)"));

    // Tilt left
    data_obj.ch[SBUS_CHANNEL_RX] = SBUS_NEUTRAL_VALUE + SBUS_MOVEMENT_RANGE;
    sbus_obj.data(data_obj);
    sbus_obj.Write();
    delay(MOVEMENT_PAUSE_MS);

    // Tilt right
    data_obj.ch[SBUS_CHANNEL_RX] = SBUS_NEUTRAL_VALUE - SBUS_MOVEMENT_RANGE;
    sbus_obj.data(data_obj);
    sbus_obj.Write();
    delay(MOVEMENT_PAUSE_MS);

    // Return to neutral
    data_obj.ch[SBUS_CHANNEL_RX] = SBUS_NEUTRAL_VALUE;
    sbus_obj.data(data_obj);
    sbus_obj.Write();
    Serial.println(F("Tilt sequence complete."));
}

// Function to perform the robot unlock sequence
// Based on mechzrobotics UnitreeInterface code (START button press)
// This sequence involves CH5 (SBUS_CHANNEL_SWITCH_L) and CH7 (SBUS_CHANNEL_NC7)
void performUnlockSequence(bfs::SbusTx &sbus_obj, bfs::SbusData &data_obj) {
    Serial.println(F("Executing: Unlock sequence... (Experimental!)"));
    Serial.println(F("Ensure robot is in a safe state to unlock."));

    // Set a defined initial state for all channels, especially CH5-CH8, plus CH9, CH10
    // as per the reference code's implied defaults before button actions.
    for (int i = 0; i < bfs::SbusData::NUM_CH; i++) {
        data_obj.ch[i] = SBUS_NEUTRAL_VALUE; // Default for most
    }

    // Specific "default" states from mechzrobotics context before START press:
    data_obj.ch[SBUS_CHANNEL_SWITCH_L] = SBUS_VALUE_MID;   // CH5 (index 4)
    data_obj.ch[SBUS_CHANNEL_SWITCH_R] = SBUS_VALUE_LOW;   // CH6 (index 5)
    data_obj.ch[SBUS_CHANNEL_NC7]      = SBUS_VALUE_LOW;   // CH7 (index 6) - Starts LOW for the toggle
    data_obj.ch[SBUS_CHANNEL_BUTTON_A] = SBUS_VALUE_MID;   // CH8 (index 7)
    
    // Specific values for CH9, CH10 from the `writeSBUS` function in the example
    data_obj.ch[SBUS_CHANNEL_BUTTON_B] = SBUS_SPECIFIC_CH9;  // CH9 (index 8)
    data_obj.ch[SBUS_CHANNEL_BUTTON_C] = SBUS_SPECIFIC_CH10; // CH10 (index 9)
    
    // Send this initial state packet
    sbus_obj.data(data_obj);
    sbus_obj.Write();
    delay(UNLOCK_INITIAL_DELAY_MS); // Allow this state to register

    // Unlock Sequence Step 1: CH5 = MID, CH7 = LOW (already set in initial state)
    // The example code sends this state again explicitly.
    // data_obj.ch[SBUS_CHANNEL_SWITCH_L] = SBUS_VALUE_MID;
    // data_obj.ch[SBUS_CHANNEL_NC7]      = SBUS_VALUE_LOW;
    // sbus_obj.data(data_obj); // Data for CH5 and CH7 is already set
    // sbus_obj.Write();
    // delayMicroseconds(UNLOCK_STEP_DELAY_US); // Per example

    // Unlock Sequence Step 2: CH5 = MID, CH7 = HIGH
    data_obj.ch[SBUS_CHANNEL_SWITCH_L] = SBUS_VALUE_MID;   // CH5 (index 4)
    data_obj.ch[SBUS_CHANNEL_NC7]      = SBUS_VALUE_HIGH;  // CH7 (index 6)
    sbus_obj.data(data_obj);
    sbus_obj.Write();
    delayMicroseconds(UNLOCK_STEP_DELAY_US);

    // Unlock Sequence Step 3: CH5 = MID, CH7 = LOW
    data_obj.ch[SBUS_CHANNEL_SWITCH_L] = SBUS_VALUE_MID;   // CH5 (index 4)
    data_obj.ch[SBUS_CHANNEL_NC7]      = SBUS_VALUE_LOW;   // CH7 (index 6)
    sbus_obj.data(data_obj);
    sbus_obj.Write();
    delayMicroseconds(UNLOCK_STEP_DELAY_US);

    Serial.println(F("Unlock sequence sent. Monitor robot for response."));

    // After sequence, ensure primary control channels (Sticks) are neutral.
    // CH5, CH6, CH7, CH8, CH9, CH10 will be as they were at the end of the sequence.
    // (CH7 ends LOW, CH5 MID, CH6 LOW, CH8 MID, CH9/10 specific).
    data_obj.ch[SBUS_CHANNEL_RY] = SBUS_NEUTRAL_VALUE;
    data_obj.ch[SBUS_CHANNEL_RX] = SBUS_NEUTRAL_VALUE;
    data_obj.ch[SBUS_CHANNEL_LX] = SBUS_NEUTRAL_VALUE;
    data_obj.ch[SBUS_CHANNEL_LY] = SBUS_NEUTRAL_VALUE;
    
    sbus_obj.data(data_obj); // Send final packet with sticks neutral
    sbus_obj.Write();
    delay(SBUS_PACKET_INTERVAL_MS); // Standard interval
}


void setup() {
    // Initialize USB Serial for debugging and commands
    Serial.begin(115200);
    // while (!Serial); // Wait for Serial Monitor to connect (useful for some boards)
    delay(1000); // Short delay to allow serial monitor to catch up

    printHelp();

    // Begin SBUS communication
    // Baud rate and other serial parameters are handled by the library's Begin() method.
    sbus.Begin();

    // Initialize all SBUS data channels to neutral
    for (int i = 0; i < bfs::SbusData::NUM_CH; i++) {
        sbusData.ch[i] = SBUS_NEUTRAL_VALUE;
    }
    sbusData.failsafe = false;
    sbusData.lost_frame = false;
    
    // Send an initial neutral packet
    sbus.data(sbusData);
    sbus.Write();
}

void loop() {
    // Set all channels to neutral at the start of each loop iteration.
    // This ensures that if no command is active, neutral signals are sent.
    // Specific commands will override these values temporarily.
    // For unlock, CH5-CH8, CH9, CH10 have specific defaults.
    // For regular operation, most other aux channels might also sit at neutral/low.
    for (int i = 0; i < bfs::SbusData::NUM_CH; i++) {
        sbusData.ch[i] = SBUS_NEUTRAL_VALUE;
    }
    // Override with specific resting states for relevant aux channels from example,
    // if not actively being controlled by a sequence.
    sbusData.ch[SBUS_CHANNEL_SWITCH_R] = SBUS_VALUE_LOW;   // CH6 default low
    sbusData.ch[SBUS_CHANNEL_NC7]      = SBUS_VALUE_LOW;   // CH7 default low
    sbusData.ch[SBUS_CHANNEL_BUTTON_B] = SBUS_SPECIFIC_CH9;  // CH9 specific
    sbusData.ch[SBUS_CHANNEL_BUTTON_C] = SBUS_SPECIFIC_CH10; // CH10 specific


    sbusData.failsafe = false;
    sbusData.lost_frame = false;

    // Check for incoming serial commands
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        command.trim();
        command.toLowerCase();

        // Reset main control channels to neutral before executing a new command sequence.
        // The sequences themselves will set their specific channels.
        sbusData.ch[SBUS_CHANNEL_RY] = SBUS_NEUTRAL_VALUE;
        sbusData.ch[SBUS_CHANNEL_RX] = SBUS_NEUTRAL_VALUE;
        sbusData.ch[SBUS_CHANNEL_LX] = SBUS_NEUTRAL_VALUE;
        sbusData.ch[SBUS_CHANNEL_LY] = SBUS_NEUTRAL_VALUE;
        // Also ensure CH5 and CH8 are neutral unless part of a specific command like unlock
        sbusData.ch[SBUS_CHANNEL_SWITCH_L] = SBUS_NEUTRAL_VALUE; // CH5
        sbusData.ch[SBUS_CHANNEL_BUTTON_A] = SBUS_NEUTRAL_VALUE; // CH8


        if (command.equals("nod")) {
            performNod(sbus, sbusData);
        } else if (command.equals("shake")) {
            performShake(sbus, sbusData);
        } else if (command.equals("tilt")) {
            performTilt(sbus, sbusData);
        } else if (command.equals("neutral")) {
            Serial.println(F("Setting all control channels to neutral."));
            // All relevant channels already set to neutral above.
            // Just need to ensure this state is written out.
        } else if (command.equals("unlock")) {
            performUnlockSequence(sbus, sbusData);
        } else if (command.equals("help")) {
            printHelp();
        } else if (command.length() > 0) {
            Serial.print(F("Unknown command: '"));
            Serial.print(command);
            Serial.println(F("'"));
        }
        // After a command, the sbusData reflects the last state of that command.
        // The continuous write at the end of loop() will send it.
    }

    // Continuously send the current sbusData state
    sbus.data(sbusData);
    sbus.Write();
    delay(SBUS_PACKET_INTERVAL_MS); // Maintain SBUS packet rate
}
