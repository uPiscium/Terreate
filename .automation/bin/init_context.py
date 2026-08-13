#!/usr/bin/env python3
from __future__ import annotations

import argparse
import json
import re
import shutil
import subprocess
import sys
from pathlib import Path

SUPPORTED_AGENT_CORE_VERSION = "2"
TASK_ID_RE = re.compile(r"(?m)^- Task ID: (.+)$")
BRANCH_RE = re.compile(r"(?m)^- Branch: (.+)$")
WORKTREE_RE = re.compile(r"(?m)^- Worktree: (.+)$")
REQUIRED_TASK_SECTIONS = (
    "Purpose",
    "Scope",
    "Prohibited changes",
    "Dependencies",
    "Acceptance criteria",
    "Test plan",
    "Stop conditions",
    "Coordination surfaces",
    "External resources",
)


class InitError(RuntimeError):
    pass


def run(command: list[str], *, cwd: Path, check: bool = True) -> subprocess.CompletedProcess[str]:
    result = subprocess.run(command, cwd=cwd, text=True, capture_output=True)
    if check and result.returncode != 0:
        detail = result.stderr.strip() or result.stdout.strip() or f"exit {result.returncode}"
        raise InitError(f"{' '.join(command)}: {detail}")
    return result


def git(root: Path, *args: str, check: bool = True) -> str:
    return run(["git", *args], cwd=root, check=check).stdout.strip()


def repo_root(cwd: Path | None = None) -> Path:
    start = cwd or Path.cwd()
    return Path(git(start, "rev-parse", "--show-toplevel")).resolve()


def default_branch(root: Path) -> str:
    symbolic = git(root, "symbolic-ref", "--quiet", "--short", "refs/remotes/origin/HEAD", check=False)
    if symbolic.startswith("origin/"):
        return symbolic.removeprefix("origin/")
    result = run(["gh", "repo", "view", "--json", "defaultBranchRef"], cwd=root, check=False)
    if result.returncode == 0:
        try:
            value = json.loads(result.stdout).get("defaultBranchRef", {}).get("name")
        except json.JSONDecodeError:
            value = None
        if value:
            return value
    raise InitError("cannot resolve default branch")


def current_branch(root: Path) -> str:
    branch = git(root, "branch", "--show-current")
    if not branch:
        raise InitError("detached HEAD is not supported")
    return branch


def common_git_dir(root: Path) -> Path:
    value = Path(git(root, "rev-parse", "--git-common-dir"))
    return value if value.is_absolute() else (root / value).resolve()


def read_required(path: Path, label: str) -> str:
    if not path.is_file():
        raise InitError(f"missing {label}: {path}")
    return path.read_text(encoding="utf-8").strip()


def task_state(root: Path) -> dict | None:
    path = root / ".task-state" / "task.md"
    if not path.is_file():
        return None
    text = path.read_text(encoding="utf-8")
    for section in REQUIRED_TASK_SECTIONS:
        if f"## {section}" not in text:
            raise InitError(f"Task State missing required section: {section}")
    values: dict[str, str] = {}
    for key, pattern in (("taskId", TASK_ID_RE), ("branch", BRANCH_RE), ("worktree", WORKTREE_RE)):
        match = pattern.search(text)
        if not match:
            raise InitError(f"Task State missing identity field: {key}")
        values[key] = match.group(1).strip()
    values["path"] = str(path)
    return values


def task_state_is_ignored(root: Path) -> bool:
    result = run(["git", "check-ignore", "-q", ".task-state/task.md"], cwd=root, check=False)
    if result.returncode == 0:
        return True
    exclude = common_git_dir(root) / "info" / "exclude"
    if not exclude.is_file():
        return False
    return "/.task-state/" in exclude.read_text(encoding="utf-8").splitlines()


def validate_identity(root: Path, branch: str, base: str, state: dict | None) -> str | None:
    if branch == base:
        if state is not None:
            raise InitError("Task State must not exist on the default branch worktree")
        return None
    if state is None:
        raise InitError("non-default branch requires Task State")
    task_id = state["taskId"]
    valid_branch = branch.startswith(f"task/{task_id}-") or branch.startswith(f"fix/{task_id}-")
    if not valid_branch:
        raise InitError(f"branch/Task identity mismatch: branch={branch}, task={task_id}")
    if state["branch"] != branch:
        raise InitError("Task State branch does not match current branch")
    if Path(state["worktree"]).resolve() != root.resolve():
        raise InitError("Task State worktree does not match current worktree")
    if not task_state_is_ignored(root):
        raise InitError(".task-state is not ignored")
    return task_id


def context(root: Path) -> dict:
    version = read_required(root / ".automation" / "VERSION", "Agent Core VERSION")
    if version != SUPPORTED_AGENT_CORE_VERSION:
        raise InitError(
            f"unsupported Agent Core version: repository={version}, runtime={SUPPORTED_AGENT_CORE_VERSION}"
        )
    adapter = read_required(root / ".automation" / "ADAPTER", "Project Adapter marker")
    branch = current_branch(root)
    base = default_branch(root)
    state = task_state(root)
    task_id = validate_identity(root, branch, base, state)
    return {
        "repositoryRoot": str(root),
        "worktree": str(root),
        "branch": branch,
        "defaultBranch": base,
        "taskId": task_id,
        "taskState": state["path"] if state else None,
        "agentCoreVersion": version,
        "adapter": adapter,
        "head": git(root, "rev-parse", "HEAD"),
        "gitStatus": git(root, "status", "--short").splitlines(),
    }


def doctor(root: Path) -> dict:
    missing = [tool for tool in ("git", "gh", "just", "python3") if shutil.which(tool) is None]
    if missing:
        raise InitError("missing required tools: " + ", ".join(missing))
    for relative in (
        "AGENTS.md",
        ".automation/INIT.md",
        ".automation/VERSION",
        ".automation/ADAPTER",
        ".automation/policy.toml",
        "just/project/mod.just",
        "opencode.json",
    ):
        if not (root / relative).is_file():
            raise InitError(f"missing required repository file: {relative}")
    data = context(root)
    return {"status": "PASS", "readOnly": True, **data}


def parser() -> argparse.ArgumentParser:
    result = argparse.ArgumentParser(description="Read-only Agent initialization checks")
    result.add_argument("command", choices=("doctor", "context"))
    return result


def main() -> int:
    args = parser().parse_args()
    try:
        root = repo_root()
        result = doctor(root) if args.command == "doctor" else context(root)
        print(json.dumps(result, sort_keys=True))
    except InitError as exc:
        print(f"ERROR: {exc}", file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
