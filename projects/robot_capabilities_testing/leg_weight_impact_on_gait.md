# Experiment: Impact of Added Weight on Robot Gait & Motor Control

## 1. Goal
To investigate how added weight to the robot's legs or body affects its walking gait and overall stability. This experiment aims to provide insights into whether the robot's motor control system for walking operates primarily on an open-loop or closed-loop basis.

## 2. Background & Hypothesis
- **Hypothesis 1 (Closed-Loop):** If the control system is sufficiently closed-loop, the robot will attempt to compensate for added weight by adjusting motor torque and joint positions to maintain its gait, up to a certain physical limit. Observable effects might include increased motor noise, higher power consumption, or slight changes in leg kinematics.
- **Hypothesis 2 (Open-Loop):** If the control system is primarily open-loop (relying on pre-programmed trajectories), the gait is expected to degrade more significantly and predictably as weight increases, with less active compensation.
- **FOC Motors Consideration:** The presence of Field-Oriented Control (FOC) motors might inherently provide some level of torque compensation, potentially masking purely open-loop trajectory execution. Extreme weights might be needed to see if the FOC system reaches its limits, and how the higher-level gait planner responds.

## 3. Methodology

### 3.1 Robot & Setup
- **Robot:** (Specify Joule or Perl)
- **Test Environment:** (e.g., Flat indoor surface, specific outdoor terrain)
- **Baseline Data:** Capture video and observations of the robot walking with no added weight. Note any existing gait characteristics.

### 3.2 Application of Weight
- **Type of Weights:** (e.g., Small ankle weights, custom-made weights)
- **Attachment Points:** (e.g., Lower leg segments, upper leg segments, central body. Be specific.)
- **Weight Increments:** (e.g., Start with X grams, increase by Y grams per trial)
- **Distribution:** (e.g., Single leg, all legs symmetrically, centrally on body)

### 3.3 Data Collection & Observation
For each weight configuration:
- **Quantitative Data:**
    - Amount of weight added (grams/kg).
    - (If possible) Power consumption readings.
- **Qualitative Observations:**
    - Video recording of walking trials from multiple angles (side, front).
    - Detailed notes on gait changes (e.g., stumbling, leg dragging, reduced speed, changes in step height/length).
    - Any audible changes in motor sounds (strain, whining).
    - Robot's ability to maintain balance and complete a short walking task (e.g., walk 5 meters).
    - Any error messages or unusual behaviors noted via control apps (if applicable).

## 4. Safety Considerations
- **Maximum Load:** Do not exceed any manufacturer-specified payload limits (if known). Start with very small weights and increase cautiously.
- **Weight Attachment:** Ensure weights are securely attached and will not fall off during movement, potentially damaging the robot or themselves.
- **Stability:** Be prepared for the robot to become unstable, especially with asymmetrically applied or heavier loads. Test in an area where a fall will not cause significant damage.
- **Motor Overheating:** Monitor for signs of motor overheating if tests are prolonged or weights are substantial.

## 5. Analysis
- Compare video footage across different weight levels.
- Correlate observed gait changes with the amount and placement of weight.
- Assess if the robot's response aligns more with open-loop or closed-loop control characteristics.

## 6. Experimental Results (Informal)

-   **FR Leg Weight Test (Date: Unspecified, prior to this documentation update):**
    *   **Robot:** Assumed to be one of the Go2 models.
    *   **Weight Application:** Weights were added incrementally to the Front Right (FR) leg.
    *   **Observation:** With the 3rd addition of weight, totaling approximately 1255.7 grams:
        *   The leg started to be a "tiny bit sluggish."
        *   Examples of sluggishness included not lifting quite as high.
        *   The robot maintained a generally normal gait but caused it to turn ever so slightly in the direction of the FR leg with the weight.
    *   **Note:** This was an informal observation.

```
