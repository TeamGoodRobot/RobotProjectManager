# Note to Self (Jules): Process for User Activity Updates

When the user provides a list of activities they've been working on or changes that have occurred (similar to an R&D log or progress update), the general process should be:

## 1. Initial Review & Clarification
-   Carefully read through all points in the user's update.
-   Identify items that are actionable for repository updates (e.g., new experimental results, hardware changes, software notes, project status updates, networking logs).
-   If any points are unclear or require more specific information to be documented effectively, ask clarifying questions.

## 2. Repository Exploration (if needed)
-   For each actionable item, determine the most appropriate location in the existing repository structure. This may involve using `ls()` to check existing directories and files.
-   Consider whether the information should update an existing file or be a new file.

## 3. Plan Creation
-   Develop a structured plan using `set_plan()`.
-   Group related items into logical plan steps (e.g., all hardware updates, all marketing updates).
-   For each step, specify:
    *   The file(s) to be created or modified.
    *   A summary of the content to be added or changed.
-   If new directories are needed, include their creation in the plan.

## 4. User Approval
-   If it's the first plan for the task, or a significant deviation from a previous plan, request user approval using `request_user_input()` after setting the plan. Record approval with `record_user_approval_for_plan()`.

## 5. Execution (Step-by-Step)
-   Execute the plan one step at a time.
-   Use `read_files()` to understand existing content before modifying.
-   Use `run_subtask()` for creating/modifying files and adding content. Subtasks should be specific and clearly define the changes.
-   Mark each step complete using `plan_step_complete()`.

## 6. Handling New Information Mid-Process
-   If the user provides additional details or clarifies points *after* the initial plan is made, be prepared to:
    *   Acknowledge the new information.
    *   If necessary, update the current plan (`set_plan()`) or create a new focused plan to incorporate these details. Re-confirm with the user if the changes are substantial.

## 7. Interim User Check-ins (for long tasks)
-   If the documentation task is extensive, use `request_user_input()` at logical breakpoints to summarize progress and ensure continued alignment with user expectations.

## 8. Final Review & Submission
-   Before submitting, perform a final review against the user's original request to ensure all points have been addressed.
-   Use `submit()` with a clear, descriptive branch name and a comprehensive commit message that summarizes all changes made in that submission batch. If the work was done over multiple submissions, ensure the final commit message for a "phase" is clear about what it covers.

## Key Learnings from Past Interactions:
-   User updates can be dense and cover diverse topics. Break them down systematically.
-   Clarifying questions are good, but also be prepared to make reasonable assumptions for initial documentation drafts if information is slightly ambiguous.
-   Flexibility is key: The user might provide more details or shift focus, requiring plan adjustments.
-   For multi-part submissions, keep track of what was submitted in which batch to ensure full coverage.
