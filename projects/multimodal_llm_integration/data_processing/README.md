# Data Processing for Multimodal LLM Integration

## Overview
This directory concerns the scripts, notes, and workflows for processing sensory data (especially 360 images and their JSON derivatives) to be used with Multimodal LLMs for enhanced robot capabilities.

## Key Data Sources & Initial Processing
- **360 Images:** Collected from the robot's perspective. Details on collection can be found in `../../../../data_repository/sensory_data/images_360/README.md`.
- **JSON Environmental Descriptions:** Generated from the 360 images. Details on the generation process, current effectiveness, and desired improvements can be found in `../../../../data_repository/sensory_data/json_descriptions/README.md`.

## Future Development Ideas for Localization and Navigation
- **Path Definition:** Investigate methods for defining paths or trails based on sequences of unique objects or features identified in the JSON descriptions.
- **Movement & Distance Tracking:** Develop systems to record robot movements and distances travelled. This data can be correlated with environmental features to improve localization.
- **Dynamic Mapping & Re-localization:**
    - Implement strategies for the robot to build a map or understanding of its environment through open-ended movement.
    - Develop algorithms to compare expected environmental features (based on its map and path) with actual sensory input.
    - Enable the robot to readjust its position, get back on a planned path, or re-determine its location if it has gone off-course.
