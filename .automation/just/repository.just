root := justfile_directory()
policy := root / ".automation/bin/repository_policy.py"

[working-directory: root]
policy-check:
    python3 {{quote(policy)}} check

[working-directory: root]
policy-apply:
    python3 {{quote(policy)}} apply
