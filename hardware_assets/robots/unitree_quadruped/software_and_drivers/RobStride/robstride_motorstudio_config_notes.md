# RobStride MotorStudio Configuration Notes

## Software Source

The RobStride MotorStudio software can be downloaded from their official GitHub releases page:
[https://github.com/RobStride/MotorStudio/releases/tag/V0.0.8](https://github.com/RobStride/MotorStudio/releases/tag/V0.0.8)

## Language Configuration

To change the MotorStudio software language from Chinese to English, you need to modify its configuration file.

1.  **Locate the Configuration File:**
    *   The exact location of this file may vary depending on where you installed the MotorStudio software on your system.
    *   Look for a configuration file typically ending in `.ini`, `.conf`, `.json`, `.yaml`, or similar, within the MotorStudio installation directory. Common installation paths include `C:\Program Files\MotorStudio` or `~/Applications/MotorStudio` or similar.

2.  **Edit the Language Setting:**
    *   Open the configuration file in a text editor.
    *   Look for a line similar to `language = zh_CN` or `language: zh_CN`.
    *   Change this line to `language = en_US` or `language: en_US` to set the language to English.
    *   Save the configuration file.

3.  **Restart MotorStudio:**
    *   After saving the changes, restart the MotorStudio software for the new language setting to take effect.

**Note:** This document assumes the configuration process based on the information provided. The exact name and location of the configuration file and the language parameter might differ slightly. Always refer to any official documentation from RobStride if available.
