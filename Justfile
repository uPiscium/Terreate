set minimum-version := "1.55.0"

mod agent '.automation/just/agent.just'
mod automation '.automation/just/automation.just'
mod integrate '.automation/just/integrate.just'
mod repository '.automation/just/repository.just'
mod project 'just/project/mod.just'
mod? local 'just/local.just'

default:
    @just --list
