root := justfile_directory()
script := root / ".automation/bin/agent_core.py"

[working-directory: root]
check pr:
    python3 {{quote(script)}} integrate check {{quote(pr)}}

[working-directory: root]
merge pr:
    python3 {{quote(script)}} integrate merge {{quote(pr)}}
