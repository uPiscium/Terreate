root := justfile_directory() / ".." / ".."
script := root / ".automation" / "bin" / "automation_upgrade.py"

version:
    python3 '{{script}}' version

check-update source:
    python3 '{{script}}' check-update --source '{{source}}'

upgrade source:
    python3 '{{script}}' upgrade --source '{{source}}'
