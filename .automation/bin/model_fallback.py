#!/usr/bin/env python3
from __future__ import annotations

import argparse
import json
import re
import sys
import tomllib
from pathlib import Path


class FallbackError(RuntimeError):
    pass


def repository_root() -> Path:
    return Path(__file__).resolve().parents[2]


def policy(root: Path) -> dict:
    path = root / ".automation" / "model-fallback.toml"
    try:
        return tomllib.loads(path.read_text(encoding="utf-8"))
    except FileNotFoundError as exc:
        raise FallbackError(f"missing fallback policy: {path}") from exc


def classify(text: str, status: int | None, cfg: dict) -> dict:
    lower = text.lower()
    non_fallback = cfg["classification"].get("non_fallback_markers", [])
    for marker in non_fallback:
        if marker.lower() in lower:
            return {"fallback": False, "reason": f"non-fallback marker: {marker}"}

    if status in cfg["classification"].get("retryable_http_status", []):
        return {"fallback": True, "reason": f"HTTP {status}"}

    for marker in cfg["classification"].get("usage_markers", []):
        if marker.lower() in lower:
            return {"fallback": True, "reason": marker}

    return {"fallback": False, "reason": "unclassified error"}


def role_chain(role: str, cfg: dict) -> dict:
    roles = cfg.get("roles", {})
    if role not in roles:
        raise FallbackError(f"unknown fallback role: {role}")
    entry = roles[role]
    agents = [entry["primary_agent"], *entry.get("fallback_agents", [])]
    models = [entry["primary_model"], *entry.get("fallback_models", [])]
    if len(agents) != len(models):
        raise FallbackError(f"invalid fallback chain for {role}: agent/model length mismatch")
    if len(set(models)) != len(models):
        raise FallbackError(f"invalid fallback chain for {role}: duplicate model")
    return {
        "role": role,
        "automatic": bool(entry.get("automatic", False)),
        "agents": agents,
        "models": models,
    }


def next_fallback(role: str, failed_agent: str, cfg: dict) -> dict:
    chain = role_chain(role, cfg)
    if not chain["automatic"]:
        return {"available": False, "reason": "automatic fallback disabled", **chain}
    try:
        index = chain["agents"].index(failed_agent)
    except ValueError as exc:
        raise FallbackError(f"agent {failed_agent!r} is not in fallback chain for {role}") from exc
    next_index = index + 1
    if next_index >= len(chain["agents"]):
        return {"available": False, "reason": "fallback chain exhausted", **chain}
    return {
        "available": True,
        "agent": chain["agents"][next_index],
        "model": chain["models"][next_index],
        **chain,
    }


def append_evidence(root: Path, role: str, failed_model: str, fallback_model: str, reason: str, outcome: str) -> None:
    state = root / ".task-state" / "task.md"
    if not state.is_file():
        raise FallbackError("Task State is required to record automatic fallback evidence")
    text = state.read_text(encoding="utf-8")
    heading = "### Model fallback"
    entry = (
        f"\n- Role: {role}; failed model: {failed_model}; fallback model: {fallback_model}; "
        f"reason: {reason}; outcome: {outcome}\n"
    )
    if heading in text:
        text = text.replace(heading, heading + entry, 1)
    elif "## Evidence" in text:
        text = text.replace("## Evidence", "## Evidence\n\n" + heading + entry, 1)
    else:
        text += "\n## Evidence\n\n" + heading + entry
    state.write_text(text, encoding="utf-8")


def parser() -> argparse.ArgumentParser:
    p = argparse.ArgumentParser(description="Role-scoped model fallback policy helper")
    sub = p.add_subparsers(dest="command", required=True)
    c = sub.add_parser("classify")
    c.add_argument("text")
    c.add_argument("--status", type=int)
    n = sub.add_parser("next")
    n.add_argument("role")
    n.add_argument("failed_agent")
    r = sub.add_parser("record")
    r.add_argument("role")
    r.add_argument("failed_model")
    r.add_argument("fallback_model")
    r.add_argument("reason")
    r.add_argument("outcome", choices=["retrying", "succeeded", "failed", "blocked"])
    return p


def main() -> int:
    args = parser().parse_args()
    root = repository_root()
    cfg = policy(root)
    try:
        if args.command == "classify":
            print(json.dumps(classify(args.text, args.status, cfg)))
        elif args.command == "next":
            print(json.dumps(next_fallback(args.role, args.failed_agent, cfg)))
        elif args.command == "record":
            append_evidence(root, args.role, args.failed_model, args.fallback_model, args.reason, args.outcome)
            print(json.dumps({"recorded": True}))
    except FallbackError as exc:
        print(f"ERROR: {exc}", file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
