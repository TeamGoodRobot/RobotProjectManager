# Leg Movement Rebalance Threshold Tests

## Objective
To determine the amount of lateral or longitudinal displacement of individual legs (while the robot is standing) that triggers a rebalancing action (e.g., trotting to stabilize) or an error/collapse state.

## Methodology
- Robot is in a stable standing position (basic mode).
- Individual legs are manually moved or pushed until the robot reacts.
- Distances are measured approximately.

## Observations (Date: Unspecified, prior to this documentation update)

### Front Right (FR) Leg:
-   **Instep (moved inward):** Rebalance trot triggered at approximately 17.5 cm.
-   **Backward (moved towards rear):** Rebalance trot triggered at approximately 17 cm.

### Front Left (FL) Leg:
-   **Inward (moved towards center):** Rebalance trot triggered at approximately 17.5 cm.
-   **Backward (moved towards rear):** Rebalance trot triggered at approximately 19 cm.

### Both Front Legs (Together):
-   **Inward (both legs moved towards each other):** Rebalance trot triggered at approximately 11 cm displacement for each leg.
-   **Rearward (both legs moved towards rear):** Rebalance trot triggered at approximately 12.5 cm displacement for each leg.

### Rear Right (RR) Leg:
-   **Instep (moved inward):** Rebalance trot triggered at approximately 19.5 cm.
-   **Forward (moved towards front):** Rebalance trot triggered at approximately 24 cm.
-   **Outward (moved away from body):** At 36 cm+, no noticeable effect on balance immediately.
-   **Outward and slightly backward:** At 42 cm, the leg became very rigid; testing stopped to avoid issues.

### Rear Left (RL) Leg:
-   **Inward (moved towards center/right):** Rebalance trot triggered at approximately 24.5 cm.
-   **Forward (moved towards front):** Rebalance trot triggered at approximately 23.5 cm.
-   **Outward (moved away from body):** At 50 cm, the leg went limp, and the robot shut down into a "damp mode" (assumed to be a safe/error mode).

## General Notes:
-   Moving legs along the **instep** (inward) generally triggered rebalancing with smaller displacements.
-   Moving legs along the **outside stance** (outward) allowed for larger displacements.
    -   Excessive outward movement could lead to the robot freezing up and collapsing in an error mode (observed with FR/FL before specific measurements, and explicitly with RL).
