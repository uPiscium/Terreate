root := justfile_directory()
script := root / ".automation/bin/agent_core.py"
lifecycle := root / ".automation/bin/task_lifecycle.py"
fallback := root / ".automation/bin/model_fallback.py"
init := root / ".automation/bin/init_context.py"

[working-directory: root]
doctor:
    python3 {{quote(init)}} doctor

[working-directory: root]
context:
    python3 {{quote(init)}} context

[working-directory: root]
task-start task slug:
    python3 {{quote(lifecycle)}} start {{quote(task)}} {{quote(slug)}}

[working-directory: root]
status task:
    python3 {{quote(lifecycle)}} status {{quote(task)}}

[working-directory: root]
state-set task status:
    python3 {{quote(lifecycle)}} state-set {{quote(task)}} {{quote(status)}}

[working-directory: root]
batch-plan +tasks:
    python3 {{quote(lifecycle)}} batch-plan {{tasks}}

[working-directory: root]
fallback-record role failed_model fallback_model reason outcome:
    python3 {{quote(fallback)}} record {{quote(role)}} {{quote(failed_model)}} {{quote(fallback_model)}} {{quote(reason)}} {{quote(outcome)}}

[working-directory: root]
verify task:
    python3 {{quote(script)}} agent verify {{quote(task)}}

[working-directory: root]
commit task message='':
    python3 {{quote(script)}} agent commit {{quote(task)}} {{quote(message)}}

[working-directory: root]
push task:
    python3 {{quote(script)}} agent push {{quote(task)}}

[working-directory: root]
pr-create task:
    python3 {{quote(script)}} agent pr-create {{quote(task)}}

[working-directory: root]
pr-edit task:
    python3 {{quote(script)}} agent pr-edit {{quote(task)}}

[working-directory: root]
pr-ready task:
    python3 {{quote(script)}} agent pr-ready {{quote(task)}}

[working-directory: root]
cleanup task:
    python3 {{quote(lifecycle)}} cleanup {{quote(task)}}
