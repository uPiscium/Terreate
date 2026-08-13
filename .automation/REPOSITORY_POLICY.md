# Repository Policy

Agent-ready repositories carry a declarative GitHub repository policy in `.automation/repository-policy.json`.

The expected default branch is `main`. The managed repository ruleset targets `~DEFAULT_BRANCH` and requires all changes to reach the default branch through a pull request. The ruleset requires zero approving reviews, so a pull request is mandatory while self-merge remains possible. Default-branch deletion and force pushes are also blocked. No bypass actor is configured.

Repository policy is separate from project bootstrap and session initialization:

```text
project::bootstrap
  -> project files only

repository::policy-check
  -> read-only GitHub policy comparison

repository::policy-apply
  -> explicit GitHub repository mutation

/init
  -> read-only session validation
```

Check the current repository without changing it:

```sh
just repository::policy-check
```

Apply the policy explicitly:

```sh
just repository::policy-apply
```

`policy-apply` acts only on the GitHub repository resolved from the current checkout. It does not accept an arbitrary repository target. It requires GitHub Administration write permission because it may update the repository default branch and create or update a repository ruleset.

If the current default branch is not `main`, `policy-apply` changes it only when a `main` branch already exists. It does not create or rename branches automatically. Create or rename `main` explicitly first when adopting an existing repository with another branch layout.

Repository policy mutation is refused from a Task worktree. In OpenCode, `repository::policy-check` is read-only and allowed; `repository::policy-apply` requires Ask.
