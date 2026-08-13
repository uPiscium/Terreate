---
description: Run repository read-only initialization checks
agent: build
---

Load the `initialize` skill and execute its mandatory initialization sequence for the current worktree.

This command intentionally overrides OpenCode's built-in `/init`. Do not generate, rewrite, or repair `AGENTS.md`; do not bootstrap the repository; do not begin implementation. Return the resolved context and blockers only.
