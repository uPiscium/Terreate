---
description: Run an explicit existing Task with its Task Orchestrator
agent: build
---

First load the `initialize` skill and complete the mandatory read-only initialization checks for the current Main worktree. Stop if initialization fails.

Then load the `task-orchestration` skill and run the explicitly supplied Task: `$ARGUMENTS`. Require the Task Orchestrator to run the same initialize skill in its assigned Task worktree before planning, editing, or delegation.

Do not create or guess a Task when the arguments do not identify one. Do not implement directly in the Main Orchestrator. Stop before merge.
