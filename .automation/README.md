# Agent Core automation

This directory contains the language-independent Task lifecycle, publication,
and integration layer shared by every Agent-ready template.

Public operations are exposed through the top-level Just modules rather than by
calling these scripts directly. Project-specific build, lint, test, and toolchain
behavior belongs under `just/project/` in the selected Project Adapter.

The current implementation provides guarded Task-local commit/push/PR operations,
integration head-SHA checkpoints, disposable Task State templates, and common
safety policy. Repository-local OpenCode agents and permissions are added by the
separate OpenCode configuration work.
