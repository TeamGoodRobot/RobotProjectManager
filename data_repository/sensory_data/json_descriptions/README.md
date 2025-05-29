# JSON Descriptions of Environments

## Overview
This directory stores JSON files generated from 360-degree images. These descriptions aim to provide a structured understanding of the robot's environment.

## Generation Process
- **Method:** Prompts are used (likely with a multimodal LLM) to transform the visual information from 360-degree photos into descriptive JSON files.

## Current Effectiveness
- **Indoors:** The process has proven effective for recognizing the type of room the robot is in.
- **Outdoors:** Challenges exist, as current JSON files tend to combine near and far objects without distinction, making precise localization difficult.

## Desired Improvements & Future Work
- **Enhanced JSON Structure:**
    - Differentiate between near and far objects.
    - Specify the relative size of distant objects.
    - Emphasize unique or salient features critical for localizing the robot.
- **Improved Prompts:** Explore prompt engineering to achieve the desired JSON structure.
