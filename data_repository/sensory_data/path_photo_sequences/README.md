# Path-Based Photo Sequences

## Purpose
This directory is for storing sequences of photographs (standard FOV or 360°) taken along predefined linear paths. These sequences can be used to document journeys and test visual path following algorithms for robots.

## Methodology for Collection
- **Path Definition:** Clearly define and document specific paths within the test environment (e.g., "Park Trail A", "Sidewalk Route 1").
- **Image Capture:**
    - Specify camera type (e.g., robot's onboard camera, handheld 360° camera).
    - Determine frequency of shots (e.g., every X meters, at key waypoints).
    - Note camera orientation (e.g., forward-facing, consistent compass bearing).
- **Data Organization:** Each path sequence should be stored in its own subdirectory here.

## Planned Experiments
- Test visual odometry techniques.
- Develop/evaluate algorithms for robots to follow these visual waypoints.
- Analyze robustness to minor environmental changes or occlusions.

## Link to Main Strategy Document
This work supports Strategy 'd' (Linear Photo Sequences) in the main [Outdoor Navigation Strategies document](../../../projects/multimodal_llm_integration/outdoor_navigation_strategies.md).
