---
description: Create a dedicated branch, worktree, and disposable Task State
agent: build
---

Create the explicitly supplied Task with `just agent::task-start <TASK-ID> <slug>`.

Run this only from the default-branch worktree. Do not implement the Task in the Main Orchestrator. After creation, report the resolved branch and worktree and stop unless the user also asked to run the Task.
