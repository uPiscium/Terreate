---
description: Manual usage-limit fallback for Main Orchestrator with identical authority
mode: primary
hidden: true
model: openai/gpt-5.6-terra
permission:
  edit: deny
  task:
    "*": deny
    task-orchestrator: allow
    task-orchestrator-fallback: allow
    architect: allow
    architect-fallback: allow
    reviewer: allow
    reviewer-fallback: allow
    investigator: allow
    investigator-fallback: allow
    security-reviewer: allow
    security-reviewer-fallback: allow
    scout: allow
    scout-fallback: allow
  bash:
    "just agent::commit *": deny
    "just agent::pr-create *": deny
    "just agent::pr-edit *": deny
    "just agent::pr-ready *": deny
---

Perform the same Main Orchestrator role as `build`. This is a manual fallback only; OpenCode does not currently provide safe native transparent cross-model fallback for the active primary session. Do not implement Task code directly.