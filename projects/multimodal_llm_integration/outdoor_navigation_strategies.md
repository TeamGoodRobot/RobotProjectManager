# Strategies for Improving Outdoor Robot Navigation

This document outlines various strategies and experiments aimed at enhancing robot navigation capabilities in open-air outdoor environments, such as parks. The primary goal is to improve environmental description, localization, and path following.

## Core Strategies

### a) Drone Shots for Overhead Mapping
- **Description:** Utilize a drone to capture overhead photographs of the test environment (e.g., the park). These images can be processed to create bird's-eye view maps.
- **Key Questions/Hypotheses:**
    - How detailed can these maps be?
    - Can they be effectively correlated with ground-level sensor data?
    - Can these maps aid in global path planning or high-level localization?
- **Potential Sub-tasks/Experiments:**
    - Plan and execute drone flights over the target area.
    - Investigate photogrammetry or image stitching software.
    - Develop methods to register drone maps with robot's coordinate frame.
- **Detailed Notes/Data:** (Link to `../../../data_repository/sensory_data/drone_imagery/README.md` or specific experiment logs)

### b) Increased Density of Ground-Level 360° Photos
- **Description:** Collect a larger set of 360° photos from more diverse locations within the park.
- **Key Questions/Hypotheses:**
    - Can a higher density of photos help differentiate unique areas more effectively?
    - What is the optimal density or distribution of photos for reliable localization?
- **Potential Sub-tasks/Experiments:**
    - Systematically collect new 360° photos, noting GPS coordinates if possible.
    - Test localization algorithms with varying densities of reference photos.
- **Detailed Notes/Data:** (Link to relevant data folders or experiment logs)

### c) Enhanced Prompt Engineering for 360° Photo JSON Descriptions
- **Description:** Refine the prompts used with multimodal LLMs to generate more useful JSON descriptions from 360° photos, specifically for localization.
- **Key Questions/Hypotheses:**
    - Can prompts be designed to effectively separate near vs. far landmarks?
    - Can JSON output highlight unique or salient features more reliably?
    - How can relative sizes of objects be captured in JSON?
- **Potential Sub-tasks/Experiments:**
    - Iteratively test different prompt structures.
    - Analyze generated JSON for accuracy and utility in localization tasks.
    - Compare localization performance with baseline vs. improved JSON.
- **Detailed Notes/Data:** (Link to `../data_processing/prompt_engineering_for_localization.md`)

### d) Linear Photo Sequences for Path Documentation & Following
- **Description:** Capture sequences of photos (either standard FOV or 360°) taken linearly along specific paths or journeys within the park.
- **Key Questions/Hypotheses:**
    - How accurately can a robot follow a path defined by a sequence of images?
    - Can visual odometry or sequence matching techniques be used for progress tracking along such paths?
    - How robust is this method to small environmental changes?
- **Potential Sub-tasks/Experiments:**
    - Define and document several test paths.
    - Collect photo sequences along these paths.
    - Develop/test algorithms for a robot to follow these visual waypoints.
- **Detailed Notes/Data:** (Link to `../../../data_repository/sensory_data/path_photo_sequences/README.md` or specific experiment logs)

### e) Investigating Directionality in Photo Collection
- **Description:** Explore how the orientation or directionality of photos can aid in navigation and localization.
- **Key Questions/Hypotheses:**
    - Does having the robot point in its intended direction of travel when capturing path photos improve path following?
    - How much does labeling images with compass directions (manually or automatically) help?
    - Is there a benefit to ensuring all general-purpose survey photos (like those in strategy 'b') are taken facing a consistent, predefined compass direction?
- **Potential Sub-tasks/Experiments:**
    - Conduct A/B tests for path following with and without consistent forward-facing photos.
    - Experiment with adding compass data to image metadata or directly onto images.
    - Analyze localization performance with directionally consistent vs. variably oriented photo sets.
- **Detailed Notes/Data:** (Link to specific experiment logs or notes)

```
