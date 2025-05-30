# 360° Image and JSON Data Management Strategy

This document outlines the strategy for managing, storing, and structuring 360° image data and their corresponding descriptive JSON files. The goal is to create a scalable and organized system that supports robot navigation, environmental understanding, and complex queries about locations and objects.

## 1. Image Storage

- **Primary Storage for Large Images:**
    - **Recommendation:** Use external cloud storage (e.g., Google Cloud Storage, AWS S3, Dropbox, Google Drive) for the original high-resolution 360° image files.
    - **Reasoning:** Git repositories are not optimized for large binary files, leading to performance issues and repository bloat.
- **In-Repository Image Derivatives (Optional):**
    - **Thumbnails/Previews:** Consider storing small, low-resolution thumbnails (e.g., < 200KB each) directly in the Git repository (perhaps in a parallel directory structure to the JSON files) for quick visual reference.
    - **To-Do:** Define standard thumbnail size and generation process if this approach is adopted.
- **Linking:** The `Manifest File` (see section 7) will be crucial for linking metadata in Git to the actual image files in external storage.

## 2. File Naming Conventions

- **Goal:** Consistent, predictable, and informative naming.
- **Proposed Structure for Image Files (External Storage):**
    - `YYYYMMDD_HHMMSS_<locationID>_<camera_type>_orig.jpg` (e.g., `20240529_143000_park_meadow_loc001_theta_orig.jpg`)
    - `<locationID>` should be unique and descriptive if possible.
- **Proposed Structure for JSON Description Files (In Git):**
    - `<locationID>.json` (e.g., `park_meadow_loc001.json`)
    - These will be stored in a structured directory path (see section 3).
- **Proposed Structure for Canonical Object JSON Files (In Git):**
    - `<object_type>_<objectID>.json` (e.g., `building_mainstreet_123.json`, `tree_large_oak_001.json`)
- **To-Do:** Finalize the exact format for `<locationID>` and `<objectID>`. Consider if hierarchical IDs are useful (e.g., `park-area1-meadow-001`).

## 3. Directory Structure (for JSON files and optional thumbnails in Git)

- **Base Directories:**
    - `data_repository/sensory_data/json_descriptions/`
    - `data_repository/sensory_data/image_thumbnails/` (if implemented)
    - `data_repository/canonical_objects/`
- **Subdirectory Organization:** Hierarchical, based on geographical area and sub-areas.
    - Example:
        ```
        json_descriptions/
        ├── park_A/
        │   ├── entrance_area/
        │   │   ├── pa_ent_loc001.json
        │   │   └── pa_ent_loc002.json
        │   ├── central_fountain/
        │   │   └── pa_cf_loc001.json
        │   └── park_A_index.json  (See Section 6: Spatial Indexing)
        ├── indoor_studio/
        │   ├── room_1/
        │   │   └── is_r1_loc001.json
        │   └── indoor_studio_index.json
        canonical_objects/
        ├── buildings/
        │   └── main_street_building_001.json
        └── flora/
            └── park_A_large_oak_001.json
        ```
- **To-Do:** Define the top-level geographical areas relevant to the project.

## 4. JSON Structure - Per Location (`<locationID>.json`)

- **Purpose:** Describe a single 360° photo capture point.
- **Draft Example (`park_A/entrance_area/pa_ent_loc001.json`):**
    ```json
    {
      "version": "1.0",
      "location_id": "pa_ent_loc001", // Unique ID for this capture
      "timestamp": "2024-05-29T14:30:00Z",
      "gps_coordinates": { // Optional, if available
        "latitude": 34.0522,
        "longitude": -118.2437,
        "altitude": 70.0,
        "accuracy": "5m"
      },
      "image_reference": { // Link to external image file (see Manifest)
        "original_filename": "20240529_143000_pa_ent_loc001_theta_orig.jpg",
        "external_url_placeholder": "path/to/cloud/storage/20240529_143000_pa_ent_loc001_theta_orig.jpg",
        "thumbnail_url_placeholder": "image_thumbnails/park_A/entrance_area/pa_ent_loc001_thumb.jpg" // If implemented
      },
      "description_general": "View from the main entrance of Park A, looking inwards. Sunny day.",
      "camera_details": {
          "type": "Ricoh Theta",
          "orientation_notes": "Camera facing North during capture (if standardized)"
      },
      "visible_objects": [
        {
          "instance_id": "obj_001", // Unique within this JSON file
          "label": "park_bench",
          "description": "Wooden park bench to the left.",
          "distance_estimate": "3m",
          "direction_from_camera": "West (270 degrees)",
          "is_occluded": false,
          "bounding_box_placeholder": [[x1,y1],[x2,y2]] // Optional: 2D bounding box in equirectangular
        },
        {
          "instance_id": "obj_002",
          "label": "building_facade",
          "canonical_object_id": "main_street_building_001", // Reference to canonical object
          "view_description": "Front facade of the large brick building across the street.",
          "distance_estimate": "30m",
          "direction_from_camera": "North (0 degrees)"
        }
      ],
      "connections": { // How this location links to others (for path/graph building)
        "path_forward": { // If part of a defined path
            "next_location_id": "pa_ent_loc002",
            "action_to_reach": "Walk 5m East"
        },
        "adjacent_locations": [ // General adjacency
            {"location_id": "pa_ent_loc002", "direction": "East"},
            {"location_id": "pa_cf_loc001", "direction": "North-East", "notes": "Across the plaza"}
        ]
      },
      "custom_tags": ["entrance", "sunny", "park_A_main_gate"]
    }
    ```
- **To-Do:** Refine fields, especially for `visible_objects` and `connections`. Decide on controlled vocabularies for labels/directions.

## 5. JSON Structure - Canonical Objects (`<object_type>_<objectID>.json`)

- **Purpose:** Provide a detailed, view-agnostic description of significant, recurring objects.
- **Draft Example (`canonical_objects/buildings/main_street_building_001.json`):**
    ```json
    {
      "version": "1.0",
      "canonical_object_id": "main_street_building_001",
      "type": "building",
      "name": "Main Street Library Building",
      "address_placeholder": "123 Main St, Anytown, USA",
      "description_general": "Three-story brick building with white columns at the entrance. Built in 1920.",
      "properties": {
        "material": "brick",
        "style": "neoclassical",
        "num_floors": 3
      },
      "sub_elements": [ // E.g., different facades, interior spaces if known
        {"element_id": "south_facade", "description": "Main entrance, faces south onto Main Street."},
        {"element_id": "interior_lobby_ref", "description": "Lobby area accessible from south_facade."}
      ],
      "known_views_references": [ // Optional: list of location_ids from which this object is well-documented
        "pa_ent_loc001",
        "street_view_loc055"
      ]
    }
    ```
- **To-Do:** Define types of canonical objects (buildings, trees, sculptures, etc.) and their specific relevant properties.

## 6. Representing Paths

- **Method:** An ordered list of `location_id`s.
- **Storage:** In `data_repository/sensory_data/path_photo_sequences/`, as planned.
- **Example (`path_parkA_main_walk.json`):**
    ```json
    {
      "path_id": "parkA_main_walk_v1",
      "description": "Main walking path from Park A entrance to the central fountain.",
      "creation_date": "2024-05-30",
      "ordered_location_ids": [
        "pa_ent_loc001",
        "pa_ent_loc002",
        // ... more location_ids ...
        "pa_cf_loc001"
      ]
    }
    ```
- **Alternative:** Store path information directly within `location_id.json` files using the `connections.path_forward` field (as shown in section 4). This might be more suitable for graph-based navigation. Both methods can coexist.

## 7. Spatial Indexing / Directory JSONs (`<area_name>_index.json`)

- **Purpose:** Provide an overview of a larger area, listing its sub-locations, sub-areas, and key features.
- **Draft Example (`json_descriptions/park_A/park_A_index.json`):**
    ```json
    {
      "area_id": "park_A",
      "area_name": "Park A",
      "description": "Main public park in the downtown district.",
      "bounding_box_gps_placeholder": [[min_lat, min_lon], [max_lat, max_lon]],
      "sub_areas": [
        {"id": "entrance_area", "name": "Entrance Plaza", "description": "Paved area around main gates.", "entry_points": ["pa_ent_loc001"]},
        {"id": "central_fountain", "name": "Central Fountain", "description": "Large fountain in the geometric center."}
      ],
      "key_locations_within_area": [ // Direct locations not within a named sub-area
        "pa_general_loc010",
        "pa_general_loc011"
      ],
      "key_canonical_objects_in_area": [
        "park_A_large_oak_001"
      ]
    }
    ```
- **To-Do:** Determine the necessary level of detail for these index files.

## 8. Manifest File

- **Purpose:** A master index file (likely CSV or a root JSON) that provides a complete list of all managed 360° images and their core metadata.
- **Storage:** At the root of `data_repository/` (e.g., `image_manifest.csv`).
- **Draft CSV Columns:**
    - `location_id` (Primary Key, matches JSON filename)
    - `original_image_filename`
    - `external_storage_url` (Direct link to the high-res image)
    - `json_description_path` (Path to the corresponding JSON file in this repo)
    - `thumbnail_path` (Path to thumbnail in this repo, if applicable)
    - `timestamp_capture`
    - `gps_latitude` (if available)
    - `gps_longitude` (if available)
    - `brief_description`
    - `tags` (comma-separated)
- **To-Do:** Develop a script to help generate or update this manifest from the JSON files or a manual input process.

## Next Steps
1.  Refine the proposed JSON structures and naming conventions based on project needs.
2.  Define the top-level geographical areas for the directory structure.
3.  Decide on external storage for high-resolution images.
4.  Start populating the structure with existing JSON data, adapting it to the new format.
5.  Develop scripts for manifest generation and data consistency checks (longer term).
```
