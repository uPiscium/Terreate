#!/usr/bin/env python3
from __future__ import annotations

import argparse
import json
import re
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path

TASK_RE = re.compile(r"^[A-Za-z0-9][A-Za-z0-9._-]*$")
SLUG_RE = re.compile(r"^[a-z0-9][a-z0-9-]*$")
VALID_STATES = {
    "initialized",
    "researching",
    "planning",
    "implementing",
    "verification-pending",
    "local-verified",
    "review-pending",
    "publication-ready",
    "draft-pr-created",
    "integration-pending",
    "merged",
    "blocked",
    "cancelled",
}
TERMINAL_STATES = {"merged", "cancelled"}
LINEAR_TRANSITIONS = {
    "initialized": {"researching", "planning", "blocked", "cancelled"},
    "researching": {"planning", "blocked", "cancelled"},
    "planning": {"implementing", "blocked", "cancelled"},
    "implementing": {"verification-pending", "blocked", "cancelled"},
    "verification-pending": {"implementing", "local-verified", "blocked", "cancelled"},
    "local-verified": {"review-pending", "implementing", "blocked", "cancelled"},
    "review-pending": {"publication-ready", "implementing", "blocked", "cancelled"},
    "publication-ready": {"draft-pr-created", "implementing", "blocked", "cancelled"},
    "draft-pr-created": {"integration-pending", "implementing", "blocked", "cancelled"},
    "integration-pending": {"merged", "implementing", "blocked", "cancelled"},
    "blocked": {"planning", "implementing", "verification-pending", "cancelled"},
    "merged": set(),
    "cancelled": set(),
}


class LifecycleError(RuntimeError):
    pass


@dataclass(frozen=True)
class WorktreeRecord:
    path: Path
    branch: str | None
    head: str | None


def run(
    command: list[str], *, cwd: Path | None = None, check: bool = True
) -> subprocess.CompletedProcess[str]:
    result = subprocess.run(command, cwd=cwd, text=True, capture_output=True)
    if check and result.returncode != 0:
        detail = result.stderr.strip() or result.stdout.strip() or f"exit {result.returncode}"
        raise LifecycleError(f"{' '.join(command)}: {detail}")
    return result


def git(*args: str, cwd: Path, check: bool = True) -> str:
    return run(["git", *args], cwd=cwd, check=check).stdout.strip()


def repo_root(cwd: Path | None = None) -> Path:
    result = run(["git", "rev-parse", "--show-toplevel"], cwd=cwd)
    return Path(result.stdout.strip()).resolve()


def common_git_dir(root: Path) -> Path:
    value = Path(git("rev-parse", "--git-common-dir", cwd=root))
    return value if value.is_absolute() else (root / value).resolve()


def default_branch(root: Path) -> str:
    symbolic = git(
        "symbolic-ref",
        "--quiet",
        "--short",
        "refs/remotes/origin/HEAD",
        cwd=root,
        check=False,
    )
    if symbolic.startswith("origin/"):
        return symbolic.removeprefix("origin/")
    result = run(
        ["gh", "repo", "view", "--json", "defaultBranchRef"],
        cwd=root,
        check=False,
    )
    if result.returncode == 0:
        try:
            name = json.loads(result.stdout).get("defaultBranchRef", {}).get("name")
        except json.JSONDecodeError:
            name = None
        if name:
            return name
    raise LifecycleError(
        "cannot resolve default branch; configure origin/HEAD or GitHub CLI access"
    )


def validate_task(task: str) -> None:
    if not TASK_RE.fullmatch(task):
        raise LifecycleError(f"invalid Task ID: {task!r}")


def validate_slug(slug: str) -> None:
    if not SLUG_RE.fullmatch(slug):
        raise LifecycleError(f"invalid Task slug: {slug!r}")


def branch_matches_task(branch: str | None, task: str) -> bool:
    if not branch:
        return False
    return branch.startswith(f"task/{task}-") or branch.startswith(f"fix/{task}-")


def parse_worktrees(root: Path) -> list[WorktreeRecord]:
    records: list[WorktreeRecord] = []
    current: dict[str, str] = {}
    lines = git("worktree", "list", "--porcelain", cwd=root).splitlines() + [""]
    for line in lines:
        if not line:
            if current:
                branch = current.get("branch")
                records.append(
                    WorktreeRecord(
                        path=Path(current["worktree"]).resolve(),
                        branch=branch.removeprefix("refs/heads/") if branch else None,
                        head=current.get("HEAD"),
                    )
                )
                current = {}
            continue
        key, _, value = line.partition(" ")
        current[key] = value
    return records


def current_worktree(root: Path) -> WorktreeRecord:
    matches = [record for record in parse_worktrees(root) if record.path == root.resolve()]
    if len(matches) != 1:
        raise LifecycleError(
            f"cannot uniquely resolve current worktree {root}: found {len(matches)}"
        )
    return matches[0]


def main_worktree(root: Path) -> WorktreeRecord:
    base = default_branch(root)
    matches = [record for record in parse_worktrees(root) if record.branch == base]
    if len(matches) != 1:
        raise LifecycleError(
            f"cannot uniquely resolve default-branch worktree for {base}: found {len(matches)}"
        )
    return matches[0]


def require_main_worktree(root: Path) -> WorktreeRecord:
    current = current_worktree(root)
    main = main_worktree(root)
    if current.path != main.path or current.branch != main.branch:
        raise LifecycleError(
            f"operation must run from the default-branch worktree: {main.path}"
        )
    return current


def worktree_for_task(root: Path, task: str) -> WorktreeRecord:
    validate_task(task)
    candidates = [
        record for record in parse_worktrees(root) if branch_matches_task(record.branch, task)
    ]
    if len(candidates) != 1:
        raise LifecycleError(
            f"expected exactly one registered worktree for {task}, found {len(candidates)}"
        )
    return candidates[0]


def require_local_task(root: Path, task: str) -> WorktreeRecord:
    record = worktree_for_task(root, task)
    if record.path != root.resolve():
        raise LifecycleError(
            f"Task {task} belongs to sibling worktree {record.path}; current worktree is {root}"
        )
    assert_task_identity(record, task)
    return record


def ensure_excludes(root: Path) -> None:
    exclude = common_git_dir(root) / "info" / "exclude"
    exclude.parent.mkdir(parents=True, exist_ok=True)
    existing = exclude.read_text(encoding="utf-8").splitlines() if exclude.exists() else []
    if "/.task-state/" not in existing:
        with exclude.open("a", encoding="utf-8") as handle:
            if existing and existing[-1] != "":
                handle.write("\n")
            handle.write("/.task-state/\n")


def state_path(worktree: Path) -> Path:
    return worktree / ".task-state" / "task.md"


def state_status(path: Path) -> str:
    if not path.is_file():
        raise LifecycleError(f"missing Task State: {path}")
    match = re.search(
        r"(?m)^- Status: ([A-Za-z0-9._-]+)$", path.read_text(encoding="utf-8")
    )
    if not match or match.group(1) not in VALID_STATES:
        raise LifecycleError(f"invalid or missing Task State status in {path}")
    return match.group(1)


def set_state_status(path: Path, status: str) -> None:
    if status not in VALID_STATES:
        raise LifecycleError(f"invalid Task State status: {status}")
    previous = state_status(path)
    if status == previous:
        return
    if status not in LINEAR_TRANSITIONS[previous]:
        raise LifecycleError(f"invalid Task State transition: {previous} -> {status}")
    text = path.read_text(encoding="utf-8")
    updated, count = re.subn(
        r"(?m)^- Status: [A-Za-z0-9._-]+$",
        f"- Status: {status}",
        text,
        count=1,
    )
    if count != 1:
        raise LifecycleError(f"cannot update Task State status in {path}")
    path.write_text(updated, encoding="utf-8")


def initialize_state(
    worktree: Path, task: str, branch: str, base: str, base_revision: str
) -> None:
    template = worktree / ".automation" / "templates" / "task-state.md"
    if not template.is_file():
        raise LifecycleError(f"missing Task State template: {template}")
    destination = state_path(worktree)
    destination.parent.mkdir(parents=True, exist_ok=True)
    text = template.read_text(encoding="utf-8")
    values = {
        "@@TASK_ID@@": task,
        "@@BRANCH@@": branch,
        "@@WORKTREE@@": str(worktree),
        "@@BASE_BRANCH@@": base,
        "@@BASE_REVISION@@": base_revision,
    }
    for marker, value in values.items():
        text = text.replace(marker, value)
    destination.write_text(text, encoding="utf-8")


def assert_task_identity(record: WorktreeRecord, task: str) -> None:
    if not branch_matches_task(record.branch, task):
        raise LifecycleError(
            f"worktree branch does not match Task {task}: {record.branch}"
        )
    state = state_path(record.path)
    if not state.is_file():
        raise LifecycleError(f"missing Task State for {task}: {state}")
    text = state.read_text(encoding="utf-8")
    expected = {
        f"- Task ID: {task}",
        f"- Branch: {record.branch}",
        f"- Worktree: {record.path}",
    }
    missing = [line for line in expected if line not in text]
    if missing:
        raise LifecycleError("Task State identity mismatch: " + ", ".join(missing))


def task_start(root: Path, task: str, slug: str) -> None:
    require_main_worktree(root)
    validate_task(task)
    validate_slug(slug)
    branch = f"task/{task}-{slug}"
    worktree = root / ".worktrees" / f"{task}-{slug}"
    records = parse_worktrees(root)

    if any(branch_matches_task(record.branch, task) for record in records):
        raise LifecycleError(f"Task already has a registered worktree: {task}")
    if any(record.branch == branch for record in records):
        raise LifecycleError(f"branch is already registered in a worktree: {branch}")
    if any(record.path == worktree.resolve() for record in records):
        raise LifecycleError(f"worktree is already registered: {worktree}")
    if worktree.exists():
        raise LifecycleError(f"worktree path already exists: {worktree}")
    if (
        run(
            ["git", "show-ref", "--verify", "--quiet", f"refs/heads/{branch}"],
            cwd=root,
            check=False,
        ).returncode
        == 0
    ):
        raise LifecycleError(f"branch already exists: {branch}")

    base = default_branch(root)
    remote_base = f"refs/remotes/origin/{base}"
    base_revision = git("rev-parse", "--verify", remote_base, cwd=root, check=False)
    if not base_revision:
        base_revision = git("rev-parse", "--verify", base, cwd=root)

    worktree.parent.mkdir(parents=True, exist_ok=True)
    run(
        ["git", "worktree", "add", "-b", branch, str(worktree), base_revision],
        cwd=root,
    )
    try:
        ensure_excludes(worktree)
        initialize_state(worktree, task, branch, base, base_revision)
    except Exception:
        run(
            ["git", "worktree", "remove", "--force", str(worktree)],
            cwd=root,
            check=False,
        )
        run(["git", "branch", "-D", branch], cwd=root, check=False)
        raise
    print(
        json.dumps(
            {
                "task": task,
                "branch": branch,
                "worktree": str(worktree),
                "base": base,
                "baseRevision": base_revision,
                "status": "initialized",
            }
        )
    )


def task_status(root: Path, task: str) -> None:
    current = current_worktree(root)
    main = main_worktree(root)
    record = worktree_for_task(root, task)
    if current.path != main.path and current.path != record.path:
        raise LifecycleError(
            f"cannot inspect sibling Task worktree {record.path} from {current.path}"
        )
    assert_task_identity(record, task)
    status = state_status(state_path(record.path))
    dirty = git("status", "--short", cwd=record.path).splitlines()
    print(
        json.dumps(
            {
                "task": task,
                "branch": record.branch,
                "worktree": str(record.path),
                "head": record.head,
                "status": status,
                "dirty": dirty,
            }
        )
    )


def task_state_set(root: Path, task: str, status: str) -> None:
    record = require_local_task(root, task)
    set_state_status(state_path(record.path), status)
    print(json.dumps({"task": task, "status": status}))


def extract_identity_value(path: Path, label: str) -> str | None:
    match = re.search(
        rf"(?m)^- {re.escape(label)}: (.+)$", path.read_text(encoding="utf-8")
    )
    return match.group(1).strip() if match else None


def unpushed_commits(record: WorktreeRecord, state: Path) -> int:
    assert record.branch is not None
    upstream = git(
        "rev-parse",
        "--abbrev-ref",
        f"{record.branch}@{{upstream}}",
        cwd=record.path,
        check=False,
    )
    if upstream:
        count = git(
            "rev-list",
            "--count",
            f"{upstream}..{record.branch}",
            cwd=record.path,
        )
        return int(count or "0")

    base_revision = extract_identity_value(state, "Base revision")
    if not base_revision:
        raise LifecycleError("Task State is missing Base revision")
    count = git(
        "rev-list",
        "--count",
        f"{base_revision}..{record.branch}",
        cwd=record.path,
    )
    return int(count or "0")


def task_cleanup(root: Path, task: str) -> None:
    require_main_worktree(root)
    record = worktree_for_task(root, task)
    assert_task_identity(record, task)
    state = state_path(record.path)
    status = state_status(state)
    if status not in TERMINAL_STATES:
        raise LifecycleError(
            f"cleanup refused while Task status is {status}; expected one of {sorted(TERMINAL_STATES)}"
        )
    if git("status", "--porcelain", cwd=record.path):
        raise LifecycleError("cleanup refused: Task worktree has uncommitted changes")
    ahead = unpushed_commits(record, state)
    if ahead:
        raise LifecycleError(f"cleanup refused: Task branch has {ahead} unpushed commit(s)")

    branch = record.branch
    assert branch is not None
    pr_result = run(
        ["gh", "pr", "view", branch, "--json", "state,headRefName,headRefOid"],
        cwd=record.path,
        check=False,
    )
    if status == "merged":
        if pr_result.returncode != 0:
            raise LifecycleError("cleanup refused: merged Task has no resolvable pull request")
        data = json.loads(pr_result.stdout)
        if data.get("state") != "MERGED" or data.get("headRefName") != branch:
            raise LifecycleError(
                "cleanup refused: Task pull request is not merged for the expected branch"
            )
    elif pr_result.returncode == 0:
        data = json.loads(pr_result.stdout)
        if data.get("state") == "OPEN":
            raise LifecycleError("cleanup refused: cancelled Task still has an open pull request")

    removed_path = str(record.path)
    run(["git", "worktree", "remove", str(record.path)], cwd=root)
    run(["git", "branch", "-D", branch], cwd=root)
    print(
        json.dumps(
            {
                "task": task,
                "removedWorktree": removed_path,
                "removedBranch": branch,
                "taskStateDiscarded": True,
            }
        )
    )


def extract_list(path: Path, heading: str) -> list[str]:
    text = path.read_text(encoding="utf-8")
    pattern = rf"(?ms)^## {re.escape(heading)}\n\n(.*?)(?=^## |\Z)"
    match = re.search(pattern, text)
    if not match:
        return []
    values: list[str] = []
    for line in match.group(1).splitlines():
        line = line.strip()
        if line.startswith("- "):
            value = line[2:].strip()
            if value and value.lower() not in {"none", "none recorded", "tbd"}:
                values.append(value)
    return values


def task_summary(root: Path, task: str) -> dict:
    record = worktree_for_task(root, task)
    assert_task_identity(record, task)
    path = state_path(record.path)
    return {
        "task": task,
        "branch": record.branch,
        "worktree": str(record.path),
        "status": state_status(path),
        "dependencies": extract_list(path, "Dependencies"),
        "scope": extract_list(path, "Scope"),
        "coordinationSurfaces": extract_list(path, "Coordination surfaces"),
        "externalResources": extract_list(path, "External resources"),
    }


def normalized(values: list[str]) -> set[str]:
    return {value.strip().lower() for value in values if value.strip()}


def overlap_reason(label: str, left: list[str], right: list[str]) -> str | None:
    overlap = sorted(normalized(left) & normalized(right))
    if not overlap:
        return None
    return f"overlapping {label}: " + ", ".join(overlap)


def batch_conflicts(summaries: list[dict]) -> list[dict]:
    conflicts: list[dict] = []
    for index, left in enumerate(summaries):
        for right in summaries[index + 1 :]:
            reasons: list[str] = []
            left_deps = normalized(left["dependencies"])
            right_deps = normalized(right["dependencies"])
            if right["task"].lower() in left_deps or left["task"].lower() in right_deps:
                reasons.append("declared dependency")
            for label, key in (
                ("declared scope", "scope"),
                ("coordination surface", "coordinationSurfaces"),
                ("external resource", "externalResources"),
            ):
                reason = overlap_reason(label, left[key], right[key])
                if reason:
                    reasons.append(reason)
            if reasons:
                conflicts.append(
                    {"tasks": [left["task"], right["task"]], "reasons": reasons}
                )
    return conflicts


def batch_plan(root: Path, tasks: list[str]) -> None:
    require_main_worktree(root)
    if len(tasks) < 2:
        raise LifecycleError("batch-plan requires at least two explicit Task IDs")
    if len(set(tasks)) != len(tasks):
        raise LifecycleError("batch-plan contains duplicate Task IDs")
    summaries = [task_summary(root, task) for task in tasks]
    conflicts = batch_conflicts(summaries)
    print(
        json.dumps(
            {
                "tasks": summaries,
                "parallelSafe": not conflicts,
                "conflicts": conflicts,
            }
        )
    )


def parser() -> argparse.ArgumentParser:
    result = argparse.ArgumentParser(description="Task/worktree lifecycle")
    sub = result.add_subparsers(dest="command", required=True)
    start = sub.add_parser("start")
    start.add_argument("task")
    start.add_argument("slug")
    status = sub.add_parser("status")
    status.add_argument("task")
    state = sub.add_parser("state-set")
    state.add_argument("task")
    state.add_argument("status")
    cleanup = sub.add_parser("cleanup")
    cleanup.add_argument("task")
    batch = sub.add_parser("batch-plan")
    batch.add_argument("tasks", nargs="+")
    return result


def main() -> int:
    args = parser().parse_args()
    try:
        root = repo_root()
        if args.command == "start":
            task_start(root, args.task, args.slug)
        elif args.command == "status":
            task_status(root, args.task)
        elif args.command == "state-set":
            task_state_set(root, args.task, args.status)
        elif args.command == "cleanup":
            task_cleanup(root, args.task)
        elif args.command == "batch-plan":
            batch_plan(root, args.tasks)
        else:  # pragma: no cover
            raise LifecycleError(f"unsupported command: {args.command}")
    except LifecycleError as exc:
        print(f"ERROR: {exc}", file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
