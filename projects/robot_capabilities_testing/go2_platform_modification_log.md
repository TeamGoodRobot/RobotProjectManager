# Go2 Robot Platform Modification Log

## Date: Approx. June 8-9, 2024

### Objective:
Create mounting holes for a platform atop the Unitree Go2 robot. This platform is intended for future experiments such as:
-   Attaching a serving tray for event demonstrations.
-   Mounting cameras or other sensors.

### Tools & Software:
-   CNC machine
-   CAD Program: Easel (easel.com) - CAD models for CNC work are stored here.
-   Drill press (for corrections)

### Process & Observations:
-   Used the CNC machine to drill mounting holes.
-   Encountered a slight misalignment issue with the holes. This may also be related to challenges in aligning the corners of the workpiece to the CNC machine's origin.
-   Initial M3 screws (25mm) were found to be slightly too short for secure attachment.

### Planned Corrections & Next Steps:
-   Correct the misalignment using a drill press.
-   Utilize newly ordered longer M3 screws (30mm and 35mm) for a more secure fit.

### SRED R&D Context:
This work contributes to ongoing research and development. Documenting these modifications and experiments is important for potential SRED tax credit claims. The ability to modify and extend the robot's hardware is crucial for testing new capabilities.

### Future Considerations:
-   Further work may be needed to address any stabilization issues once payloads (like cameras or trays) are mounted.

## Follow-up & Testing: June 10, 2024 (Evening)

### Modifications:
-   The front two mounting holes on the platform were re-drilled, moving each approximately 1mm forward and 1mm towards the center.
-   This adjustment allowed for better alignment of the tray atop the robot.

### Assembly & Bolt Usage:
-   The platform was mounted using M3 bolts with 10mm spacers.
-   Most mounting points utilized the new 30mm M3 bolts.
-   **Exception:** The rear right screw encountered significant resistance when a 30mm bolt was hand-tightened to lower depths. It's suspected this may be due to proximity to or intersection with the motor hub or internal wiring.
    -   **Precaution:** To avoid damage, a 25mm M3 bolt with a 10mm nut spacer was used in this specific location. The other locations used 30mm M3 bolts with 10mm spacers.

### Tray Testing:
1.  **Flat Tray:**
    -   Once mounted, the platform itself was very solid.
    -   A flat tray placed on the platform demonstrated that while stable, items like glasses could slide off during movement. Flat-bottomed items generally stayed in place.
2.  **Round Tray with Rim:**
    -   A round tray with a raised rim was subsequently attached to the flat platform surface using double-sided tape.
    -   This setup proved much more effective for serving, as the rim successfully contained items, including sliding glasses.

### Conclusion:
-   The modifications and use of appropriate length bolts (with caution for the rear right position) resulted in a successful and solid platform attachment.
-   The round, rimmed tray is viable for basic serving tasks.

### Future Improvement Ideas:
-   Consider designing a system with motors for active tray stabilization (counteracting robot movement/shake).
-   Explore adding motorized lifting capabilities to the tray for enhanced serving ergonomics.
