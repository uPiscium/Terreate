---
description: Evaluate explicitly named Tasks for safe parallel execution
agent: build
---

First load the `initialize` skill and complete the mandatory read-only initialization checks for the current Main worktree. Stop if initialization fails.

Then evaluate only the Task IDs supplied in `$ARGUMENTS` with `just agent::batch-plan ...`. For every Task selected to run, require its Task Orchestrator to execute the same initialize skill inside that Task worktree before planning, editing, or delegation.

Do not discover or auto-select additional Tasks. If dependencies, declared scope, coordination surfaces, or external resources conflict, report the conflicting pairs and serialize them. Launch Task Orchestrators only for Tasks that are explicitly requested, initialized successfully, and assessed as parallel-safe.
