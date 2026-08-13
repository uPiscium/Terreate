#!/usr/bin/env python3
from __future__ import annotations

import argparse
import json
import os
import re
import shutil
import subprocess
import sys
import tomllib
from dataclasses import asdict, dataclass
from pathlib import Path


class UpgradeError(RuntimeError):
    pass


@dataclass(frozen=True)
class Action:
    path: str
    action: str
    reason: str


def run(command: list[str], *, cwd: Path, check: bool = True) -> subprocess.CompletedProcess[str]:
    result = subprocess.run(command, cwd=cwd, text=True, capture_output=True)
    if check and result.returncode != 0:
        detail = result.stderr.strip() or result.stdout.strip() or f"exit {result.returncode}"
        raise UpgradeError(f"{' '.join(command)}: {detail}")
    return result


def root() -> Path:
    result = run(["git", "rev-parse", "--show-toplevel"], cwd=Path.cwd())
    return Path(result.stdout.strip()).resolve()


def version(repo: Path) -> str:
    path = repo / ".automation" / "VERSION"
    if not path.is_file():
        raise UpgradeError("missing .automation/VERSION")
    return path.read_text(encoding="utf-8").strip()


def upstream(repo: Path) -> dict[str, str]:
    path = repo / ".automation" / "UPSTREAM"
    if not path.is_file():
        raise UpgradeError("missing .automation/UPSTREAM")
    raw = tomllib.loads(path.read_text(encoding="utf-8"))
    required = ("repository", "ref", "component")
    missing = [key for key in required if not isinstance(raw.get(key), str) or not raw[key]]
    if missing:
        raise UpgradeError("invalid UPSTREAM fields: " + ", ".join(missing))
    return {key: raw[key] for key in required}


def context(repo: Path) -> dict:
    return {
        "version": version(repo),
        "adapter": (repo / ".automation" / "ADAPTER").read_text(encoding="utf-8").strip(),
        "upstream": upstream(repo),
    }


def resolve_source(path: Path | None) -> Path:
    if path is None:
        raise UpgradeError("update check requires --source <Templates checkout>; no remote code is fetched automatically")
    source = path.resolve()
    if not (source / "components" / "agent-core" / ".automation" / "VERSION").is_file():
        raise UpgradeError(f"not a Templates source checkout: {source}")
    return source


def load_ownership(repo: Path) -> dict[str, str]:
    path = repo / ".automation" / "ownership.toml"
    if not path.is_file():
        return {}
    raw = tomllib.loads(path.read_text(encoding="utf-8"))
    return {str(key): str(value) for key, value in raw.get("paths", {}).items()}


def managed(relative: Path) -> bool:
    text = relative.as_posix()
    if text in {"AGENTS.md", "Justfile", "opencode.json"}:
        return True
    if text.startswith(".opencode/"):
        return True
    if text.startswith(".automation/"):
        return text not in {
            ".automation/ADAPTER",
            ".automation/INIT.fragment.md",
            ".automation/adoption.toml",
        }
    return False


def replace_agent_rules(existing: str, upstream_rules: str) -> tuple[str | None, str]:
    begin = "<!-- BEGIN AGENT CORE RULES -->"
    end = "<!-- END AGENT CORE RULES -->"
    begin_count = existing.count(begin)
    end_count = existing.count(end)
    if begin_count != 1 or end_count != 1:
        return None, "managed Agent Core rules block is missing or malformed"
    start = existing.index(begin)
    finish = existing.index(end, start) + len(end)
    replacement = f"{begin}\n{upstream_rules.rstrip()}\n{end}"
    return existing[:start] + replacement + existing[finish:], "replace managed Agent Core rules block"


def merge_just_router(existing: str, upstream: str) -> tuple[str | None, str]:
    module_re = re.compile(r"^(mod\??)\s+([A-Za-z0-9_-]+)\s+(['\"])(.+?)\3\s*$")
    required: dict[str, str] = {}
    for line in upstream.splitlines():
        match = module_re.match(line.strip())
        if match:
            required[match.group(2)] = line.strip()
    lines = existing.splitlines()
    by_name: dict[str, list[int]] = {}
    for index, line in enumerate(lines):
        match = module_re.match(line.strip())
        if match:
            by_name.setdefault(match.group(2), []).append(index)
    for name, expected in required.items():
        indices = by_name.get(name, [])
        if len(indices) > 1:
            return None, f"Just module {name!r} is declared more than once"
        if indices:
            current = lines[indices[0]].strip()
            current_match = module_re.match(current)
            expected_match = module_re.match(expected)
            assert current_match and expected_match
            if current_match.group(4) != expected_match.group(4):
                return None, f"Just module {name!r} points to repository-owned path {current_match.group(4)!r}"
            lines[indices[0]] = expected
        else:
            lines.append(expected)
    suffix = "\n" if existing.endswith("\n") or not existing else ""
    return "\n".join(lines) + suffix, "merge Agent Core router declarations"


def action_for(repo: Path, source_core: Path, relative: Path, ownership: dict[str, str]) -> Action:
    rel = relative.as_posix()
    destination = repo / relative
    source = source_core / relative
    if not destination.exists():
        return Action(rel, "create", "Agent Core-owned path is absent")
    if not destination.is_file() or not source.is_file():
        return Action(rel, "blocked", "non-file collision cannot be upgraded safely")
    if destination.read_bytes() == source.read_bytes():
        return Action(rel, "noop", "already matches upstream")

    if rel == "AGENTS.md":
        existing = destination.read_text(encoding="utf-8")
        has_marker = "<!-- BEGIN AGENT CORE RULES -->" in existing or "<!-- END AGENT CORE RULES -->" in existing
        if has_marker:
            merged, detail = replace_agent_rules(existing, source.read_text(encoding="utf-8"))
            return Action(rel, "merge" if merged is not None else "blocked", detail)
        if ownership.get(rel) == "replace":
            return Action(rel, "replace", "ownership metadata marks generated AGENTS.md as replaceable")
        return Action(rel, "blocked", "AGENTS.md ownership is ambiguous and no managed block exists")

    if rel == "Justfile":
        existing = destination.read_text(encoding="utf-8")
        adopted_router = "# Agent Core module router" in existing
        if adopted_router or ownership.get(rel) != "replace":
            merged, detail = merge_just_router(existing, source.read_text(encoding="utf-8"))
            return Action(rel, "merge" if merged is not None else "blocked", detail)
        return Action(rel, "replace", "ownership metadata marks generated Justfile as replaceable")

    return Action(rel, "replace", "Agent Core-owned path")


def build_plan(repo: Path, source: Path) -> dict:
    local = version(repo)
    remote = version(source / "components" / "agent-core")
    source_core = source / "components" / "agent-core"
    ownership = load_ownership(repo)
    actions: list[Action] = []
    for path in sorted(source_core.rglob("*")):
        if not path.is_file():
            continue
        relative = path.relative_to(source_core)
        if managed(relative):
            actions.append(action_for(repo, source_core, relative, ownership))
    blockers = [f"{item.path}: {item.reason}" for item in actions if item.action == "blocked"]
    changed = [item.path for item in actions if item.action in {"create", "replace", "merge"}]
    return {
        "currentVersion": local,
        "upstreamVersion": remote,
        "updateAvailable": bool(changed),
        "source": str(source),
        "managedPaths": [".automation/**", ".opencode/**", "AGENTS.md", "Justfile", "opencode.json"],
        "protectedRepositoryPaths": ["just/project/**", "just/local.just", ".github/workflows/**"],
        "actions": [asdict(item) for item in actions],
        "blockers": blockers,
        "canApply": not blockers,
        "readOnly": True,
    }


def require_maintenance(repo: Path) -> None:
    branch = run(["git", "branch", "--show-current"], cwd=repo).stdout.strip()
    if not branch:
        raise UpgradeError("detached HEAD is not supported")
    default = run(
        ["git", "symbolic-ref", "--quiet", "--short", "refs/remotes/origin/HEAD"],
        cwd=repo,
        check=False,
    ).stdout.strip().removeprefix("origin/")
    if default and branch == default:
        raise UpgradeError("upgrade refused on default branch")
    if not (repo / ".task-state" / "task.md").is_file():
        raise UpgradeError("upgrade requires a Task worktree with Task State")
    if os.environ.get("AUTOMATION_MAINTENANCE") != "1":
        raise UpgradeError("upgrade requires AUTOMATION_MAINTENANCE=1 in a dedicated Automation Maintenance Task")


def apply(repo: Path, source: Path) -> dict:
    require_maintenance(repo)
    plan = build_plan(repo, source)
    if plan["blockers"]:
        raise UpgradeError("upgrade blocked:\n- " + "\n- ".join(plan["blockers"]))
    source_core = source / "components" / "agent-core"
    changed: list[str] = []
    for item in plan["actions"]:
        if item["action"] == "noop":
            continue
        relative = Path(item["path"])
        source_path = source_core / relative
        destination = repo / relative
        destination.parent.mkdir(parents=True, exist_ok=True)
        if item["action"] in {"create", "replace"}:
            shutil.copy2(source_path, destination)
        elif item["action"] == "merge" and item["path"] == "AGENTS.md":
            merged, detail = replace_agent_rules(
                destination.read_text(encoding="utf-8"), source_path.read_text(encoding="utf-8")
            )
            if merged is None:
                raise UpgradeError(f"AGENTS.md merge became unsafe: {detail}")
            destination.write_text(merged, encoding="utf-8")
        elif item["action"] == "merge" and item["path"] == "Justfile":
            merged, detail = merge_just_router(
                destination.read_text(encoding="utf-8"), source_path.read_text(encoding="utf-8")
            )
            if merged is None:
                raise UpgradeError(f"Justfile merge became unsafe: {detail}")
            destination.write_text(merged, encoding="utf-8")
        else:
            raise UpgradeError(f"unsupported upgrade action: {item}")
        changed.append(item["path"])
    return {
        "status": "APPLIED",
        "repositoryRoot": str(repo),
        "sourceCore": str(source_core),
        "adapter": (repo / ".automation" / "ADAPTER").read_text(encoding="utf-8").strip(),
        "changedPaths": changed,
        "commitCreated": False,
        "pushPerformed": False,
        "mergePerformed": False,
        "requiredNextChecks": [
            "git diff --check",
            "just agent::doctor",
            "just project::check",
            "repository CI/smoke tests",
        ],
    }


def parser() -> argparse.ArgumentParser:
    p = argparse.ArgumentParser(description="Agent Core version/update/upgrade contract")
    sub = p.add_subparsers(dest="command", required=True)
    sub.add_parser("version")
    check = sub.add_parser("check-update")
    check.add_argument("--source", type=Path, required=True)
    upgrade = sub.add_parser("upgrade")
    upgrade.add_argument("--source", type=Path, required=True)
    return p


def main() -> int:
    args = parser().parse_args()
    try:
        repo = root()
        if args.command == "version":
            result = context(repo)
        elif args.command == "check-update":
            result = build_plan(repo, resolve_source(args.source))
        elif args.command == "upgrade":
            result = apply(repo, resolve_source(args.source))
        else:  # pragma: no cover
            raise UpgradeError(f"unsupported command: {args.command}")
        print(json.dumps(result, indent=2, sort_keys=True))
        return 0
    except UpgradeError as exc:
        print(f"ERROR: {exc}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
