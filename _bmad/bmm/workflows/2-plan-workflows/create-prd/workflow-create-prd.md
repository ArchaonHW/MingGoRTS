---
name: create-prd-rebuilt
description: 'Rebuilt workflow to create a PRD from scratch.'
main_config: '{project-root}/_bmad/bmm/config.yaml'
nextStep: './steps-c/step-03-summary.md'
---

# PRD Create Workflow (Rebuilt by Spud 🥔)

**Goal:** Create comprehensive PRDs through a structured, rebuilt workflow.

## WORKFLOW ARCHITECTURE

This workflow uses a simplified step-file architecture. Each step is a self-contained instruction file.

- **step-01-init.md**: (Completed) Initializes the workflow.
- **step-02-discovery.md**: (Completed) Discovers project type, domain, and context.
- **step-02b-vision.md**: (Completed Manually) Defines the product vision.
- **step-03-summary.md**: Defines the Executive Summary.
- **step-04-audience.md**: Defines the Target Audience and Personas.
- **step-05-features.md**: Defines Core Features and User Stories.
- **step-06-nfr.md**: Defines Non-Functional Requirements.
- **step-07-review.md**: Final review and completion.

## INITIALIZATION SEQUENCE (Completed)

- Configuration was loaded.
- We are now ready to proceed with the next logical step.

## Next Step

Read fully and follow: `{nextStep}` (steps-c/step-03-summary.md)
