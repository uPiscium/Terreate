#!/usr/bin/env python3
from __future__ import annotations

import argparse
import json
import subprocess
import sys
from pathlib import Path
from typing import Any

API_VERSION = "2026-03-10"
RULESET_NAME = "Agent repository policy"
PULL_REQUEST_PARAMETERS = {
    "allowed_merge_methods": ["merge", "squash", "rebase"],
    "dismiss_stale_reviews_on_push": False,
    "require_code_owner_review": False,
    "require_last_push_approval": False,
    "required_approving_review_count": 0,
    "required_review_thread_resolution": False,
}


class RepositoryPolicyError(RuntimeError):
    pass


def repository_root() -> Path:
    result = subprocess.run(
        ["git", "rev-parse", "--show-toplevel"],
        text=True,
        capture_output=True,
    )
    if result.returncode != 0:
        raise RepositoryPolicyError("not inside a Git repository")
    return Path(result.stdout.strip()).resolve()


def load_policy(root: Path) -> dict[str, Any]:
    path = root / ".automation" / "repository-policy.json"
    try:
        data = json.loads(path.read_text(encoding="utf-8"))
    except FileNotFoundError as exc:
        raise RepositoryPolicyError(f"missing repository policy: {path}") from exc
    except json.JSONDecodeError as exc:
        raise RepositoryPolicyError(f"invalid repository policy JSON: {exc}") from exc

    if set(data) != {"version", "default_branch", "ruleset"}:
        raise RepositoryPolicyError(
            "repository policy must contain exactly version, default_branch, ruleset"
        )
    if data["version"] != 1:
        raise RepositoryPolicyError(
            f"unsupported repository policy version: {data['version']!r}"
        )
    if data["default_branch"] != "main":
        raise RepositoryPolicyError("repository policy default_branch must be 'main'")

    expected_ruleset = {
        "name": RULESET_NAME,
        "target": "branch",
        "enforcement": "active",
        "bypass_actors": [],
        "conditions": {
            "ref_name": {
                "include": ["~DEFAULT_BRANCH"],
                "exclude": [],
            }
        },
        "rules": [
            {
                "type": "pull_request",
                "parameters": PULL_REQUEST_PARAMETERS,
            },
            {"type": "deletion"},
            {"type": "non_fast_forward"},
        ],
    }
    if data["ruleset"] != expected_ruleset:
        raise RepositoryPolicyError(
            "repository policy ruleset does not match the supported Agent Core contract"
        )
    return data


def _run_json(
    command: list[str],
    *,
    cwd: Path,
    stdin: dict[str, Any] | None = None,
    allow_not_found: bool = False,
) -> Any:
    result = subprocess.run(
        command,
        cwd=cwd,
        text=True,
        input=None if stdin is None else json.dumps(stdin),
        capture_output=True,
    )
    if result.returncode != 0:
        detail = (result.stderr or result.stdout).strip()
        if allow_not_found and ("HTTP 404" in detail or "Not Found" in detail):
            return None
        raise RepositoryPolicyError(
            f"{' '.join(command)} failed: {detail or f'exit {result.returncode}'}"
        )
    output = result.stdout.strip()
    if not output:
        return None
    try:
        return json.loads(output)
    except json.JSONDecodeError as exc:
        raise RepositoryPolicyError(
            f"{' '.join(command)} returned invalid JSON"
        ) from exc


def gh_api(
    root: Path,
    method: str,
    endpoint: str,
    *,
    body: dict[str, Any] | None = None,
    allow_not_found: bool = False,
) -> Any:
    command = [
        "gh",
        "api",
        "--method",
        method,
        "-H",
        "Accept: application/vnd.github+json",
        "-H",
        f"X-GitHub-Api-Version: {API_VERSION}",
        endpoint,
    ]
    if body is not None:
        command.extend(["--input", "-"])
    return _run_json(
        command,
        cwd=root,
        stdin=body,
        allow_not_found=allow_not_found,
    )


def current_repository(root: Path) -> str:
    value = _run_json(
        ["gh", "repo", "view", "--json", "nameWithOwner"],
        cwd=root,
    )
    repository = value.get("nameWithOwner") if isinstance(value, dict) else None
    if not isinstance(repository, str) or repository.count("/") != 1:
        raise RepositoryPolicyError(
            "unable to resolve the current GitHub repository from this checkout"
        )
    return repository


def task_worktree(root: Path) -> bool:
    return (root / ".task-state" / "task.md").is_file()


def branch_exists(root: Path, repository: str, branch: str) -> bool:
    value = gh_api(
        root,
        "GET",
        f"repos/{repository}/branches/{branch}",
        allow_not_found=True,
    )
    return value is not None


def desired_ruleset(policy: dict[str, Any]) -> dict[str, Any]:
    return policy["ruleset"]


def normalize_ruleset(value: dict[str, Any]) -> dict[str, Any]:
    normalized_rules: list[dict[str, Any]] = []
    for raw_rule in value.get("rules", []):
        if not isinstance(raw_rule, dict):
            continue
        rule_type = raw_rule.get("type")
        if not isinstance(rule_type, str):
            continue
        rule: dict[str, Any] = {"type": rule_type}
        if rule_type == "pull_request":
            parameters = raw_rule.get("parameters")
            if isinstance(parameters, dict):
                rule["parameters"] = {
                    key: parameters.get(key)
                    for key in PULL_REQUEST_PARAMETERS
                }
                methods = rule["parameters"]["allowed_merge_methods"]
                if isinstance(methods, list):
                    rule["parameters"]["allowed_merge_methods"] = sorted(methods)
        normalized_rules.append(rule)
    normalized_rules.sort(key=lambda item: item["type"])

    bypass_actors = value.get("bypass_actors")
    if bypass_actors is None:
        normalized_bypass: Any = "__UNVERIFIED__"
    elif isinstance(bypass_actors, list):
        normalized_bypass = sorted(
            bypass_actors,
            key=lambda item: json.dumps(item, sort_keys=True),
        )
    else:
        normalized_bypass = bypass_actors

    return {
        "name": value.get("name"),
        "target": value.get("target"),
        "enforcement": value.get("enforcement"),
        "bypass_actors": normalized_bypass,
        "conditions": value.get("conditions"),
        "rules": normalized_rules,
    }


def normalized_desired(policy: dict[str, Any]) -> dict[str, Any]:
    result = normalize_ruleset(desired_ruleset(policy))
    for rule in result["rules"]:
        if rule["type"] == "pull_request":
            rule["parameters"]["allowed_merge_methods"] = sorted(
                rule["parameters"]["allowed_merge_methods"]
            )
    return result


def find_managed_ruleset(
    root: Path,
    repository: str,
) -> tuple[int | None, dict[str, Any] | None]:
    values = gh_api(
        root,
        "GET",
        f"repos/{repository}/rulesets?includes_parents=false&per_page=100",
    )
    if not isinstance(values, list):
        raise RepositoryPolicyError("unexpected repository rulesets response")

    matches = [
        item
        for item in values
        if isinstance(item, dict)
        and item.get("name") == RULESET_NAME
        and item.get("source_type") == "Repository"
    ]
    if len(matches) > 1:
        raise RepositoryPolicyError(
            f"multiple repository rulesets named {RULESET_NAME!r} exist"
        )
    if not matches:
        return None, None

    ruleset_id = matches[0].get("id")
    if not isinstance(ruleset_id, int):
        raise RepositoryPolicyError("managed repository ruleset is missing its id")
    detail = gh_api(root, "GET", f"repos/{repository}/rulesets/{ruleset_id}")
    if not isinstance(detail, dict):
        raise RepositoryPolicyError("unexpected managed repository ruleset response")
    return ruleset_id, detail


def inspect(root: Path, policy: dict[str, Any]) -> dict[str, Any]:
    repository = current_repository(root)
    repository_data = gh_api(root, "GET", f"repos/{repository}")
    if not isinstance(repository_data, dict):
        raise RepositoryPolicyError("unexpected repository response")

    expected_branch = policy["default_branch"]
    actual_branch = repository_data.get("default_branch")
    expected_branch_exists = branch_exists(root, repository, expected_branch)

    ruleset_id, actual_ruleset = find_managed_ruleset(root, repository)
    expected_ruleset = normalized_desired(policy)
    normalized_actual = (
        None if actual_ruleset is None else normalize_ruleset(actual_ruleset)
    )

    drift: list[str] = []
    if actual_branch != expected_branch:
        drift.append(
            f"default branch is {actual_branch!r}; expected {expected_branch!r}"
        )
    if actual_branch != expected_branch and not expected_branch_exists:
        drift.append(
            f"required branch {expected_branch!r} does not exist; "
            "create or rename it before applying policy"
        )
    if actual_ruleset is None:
        drift.append(f"missing ruleset {RULESET_NAME!r}")
    elif normalized_actual != expected_ruleset:
        drift.append(f"ruleset {RULESET_NAME!r} differs from policy")

    return {
        "repository": repository,
        "policyVersion": policy["version"],
        "defaultBranch": {
            "expected": expected_branch,
            "actual": actual_branch,
            "expectedBranchExists": expected_branch_exists,
            "match": actual_branch == expected_branch,
        },
        "ruleset": {
            "name": RULESET_NAME,
            "id": ruleset_id,
            "present": actual_ruleset is not None,
            "match": normalized_actual == expected_ruleset,
            "actual": normalized_actual,
            "expected": expected_ruleset,
        },
        "drift": drift,
    }


def command_check(root: Path) -> int:
    policy = load_policy(root)
    result = inspect(root, policy)
    result["status"] = "PASS" if not result["drift"] else "DRIFT"
    print(json.dumps(result, indent=2, sort_keys=True))
    return 0 if not result["drift"] else 1


def command_apply(root: Path) -> int:
    if task_worktree(root):
        raise RepositoryPolicyError(
            "repository policy mutation is forbidden from a Task worktree"
        )

    policy = load_policy(root)
    before = inspect(root, policy)
    repository = before["repository"]
    expected_branch = policy["default_branch"]

    if before["defaultBranch"]["actual"] != expected_branch:
        if not before["defaultBranch"]["expectedBranchExists"]:
            raise RepositoryPolicyError(
                f"cannot set default branch to {expected_branch!r}: "
                f"branch {expected_branch!r} does not exist"
            )
        gh_api(
            root,
            "PATCH",
            f"repos/{repository}",
            body={"default_branch": expected_branch},
        )

    ruleset_id = before["ruleset"]["id"]
    if ruleset_id is None:
        gh_api(
            root,
            "POST",
            f"repos/{repository}/rulesets",
            body=desired_ruleset(policy),
        )
    elif not before["ruleset"]["match"]:
        gh_api(
            root,
            "PUT",
            f"repos/{repository}/rulesets/{ruleset_id}",
            body=desired_ruleset(policy),
        )

    after = inspect(root, policy)
    if after["drift"]:
        raise RepositoryPolicyError(
            "repository policy apply completed but verification still reports drift: "
            + "; ".join(after["drift"])
        )

    after["status"] = "PASS"
    after["changed"] = bool(before["drift"])
    print(json.dumps(after, indent=2, sort_keys=True))
    return 0


def parser() -> argparse.ArgumentParser:
    result = argparse.ArgumentParser(
        description="Check and apply the Agent repository GitHub policy"
    )
    sub = result.add_subparsers(dest="command", required=True)
    sub.add_parser("check")
    sub.add_parser("apply")
    return result


def main() -> int:
    args = parser().parse_args()
    try:
        root = repository_root()
        if args.command == "check":
            return command_check(root)
        if args.command == "apply":
            return command_apply(root)
        raise AssertionError(args.command)
    except RepositoryPolicyError as exc:
        print(f"ERROR: {exc}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
