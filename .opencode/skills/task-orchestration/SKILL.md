---
name: task-orchestration
description: Run one already-created Task through its Task Orchestrator without merging
---

# Task orchestration

Use this skill when the Main Orchestrator is asked to run a specific Task that already has a dedicated branch/worktree and Task State.

1. Resolve the explicit Task ID and assigned worktree.
2. Confirm the Task is not already owned by another active Task Orchestrator.
3. Launch exactly one `task-orchestrator` for that Task.
4. Require the Task Orchestrator to operate only in the assigned worktree and to use bounded Work Units.
5. Require the Task Orchestrator to enforce leaf escalation-only status contract: Depth-2 units may only return `COMPLETED`, `BLOCKED`, `NEEDS_APPROVAL`, or `NEEDS_DECISION`.
6. Require the Task Orchestrator to treat `NEEDS_APPROVAL`/`NEEDS_DECISION` as its own decision point:
   - re-validate scope, authority, least privilege, safety, alternatives, and evidence;
   - do not automatically relay/launder leaf requests or change a leaf's deny-default profile;
   - only issue a new Depth-1 permission request after independent re-evaluation and when the operation is already Ask/allow within the Task Orchestrator's configured role authority;
   - on rejection, pick a safe alternative or return `BLOCKED` with evidence.
   - treat a user-rejected Depth-1 operation as final for that Task; do not retry, rephrase, re-delegate, or substitute an equivalent operation to verify/bypass rejection.
   - for unresolved `NEEDS_DECISION`, use `question` from Depth 1 with concrete options, tradeoffs, known facts, and a recommendation; apply the answer rather than relaying the Leaf request or escalating it to Depth 0.
7. If the Task Orchestrator invocation fails with a usage/quota/rate-limit condition explicitly listed in `.automation/model-fallback.toml`, retry the identical Task once with the configured `task-orchestrator-fallback` variant.
8. Inside the Task, leaf Work Units may use the same controlled retry rule for their role-specific fallback variants. Do not fallback for authentication, permission, validation, context-window, tool, or safety errors.
9. Record every attempted fallback in Task State evidence. When a configured chain is exhausted, mark the Task BLOCKED instead of inventing another model.
10. Accept only evidence-backed completion: changed files, verification results, review results, commit/PR state, blockers, and unverified checks.
11. Confirm the Task did not report PASS for any unexecuted command or unperformed Work Unit.
12. Stop at integration-pending. Do not merge from this skill.

Main Orchestrator fallback is not transparent: if the active `build` model hits a usage limit, switch manually to the explicitly configured `build-fallback` agent. OpenCode currently has no safe native cross-model failover for the already-active primary session.

Do not silently select another Issue or Task. Do not weaken permissions to work around a blocked approval or missing tool/model.
Depth-2 Ask behavior is tracked as a non-gating upstream compatibility canary for anomalyco/opencode#13715; release readiness is determined by correct Leaf→Depth-1 escalation approval/rejection decisions.
