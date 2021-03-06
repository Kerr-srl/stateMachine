# stateMachine

A feature-rich, yet simple finite state machine (FSM) implementation in C.

For when a simple switch statement just isn't enough.
[Documentation](http://misje.github.io/stateMachine).

## Build (CMake)

```bash
$ mkdir build && cd build && cmake ..
```

### Optional build parameters

- `STATE_MACHINE_DOCS`: Whether to generate doxygen documentation

  - Type: BOOLEAN
  - Default value: `TRUE` if invoking cmake from this repository, `FALSE`
    otherwise (e.g. when you're using this repository as submodule)

- `STATE_MACHINE_EXAMPLE`: Whether to build examples

  - Type: BOOLEAN
  - Default value: same as `STATE_MACHINE_DOCS`

- `STATE_MACHINE_TEST`: Whether to build tests

  - Type: BOOLEAN
  - Default value: same as `STATE_MACHINE_DOCS`

- `STATE_MACHINE_COVERAGE`: Whether to generate test coverage report
  - Type: BOOLEAN
  - Default value: same as `STATE_MACHINE_DOCS`

### How to include this library

Just include this repository using `add_subdirectory`.

You can use the static library target `state-machine::state-machine` in your
CMake project.

Before using `add_subdirectory`, you can define a interface target called

- `state-machine::config`: A INTERFACE target that can contain compile
  definitions to override the configuration in `src/state_machine_config.h`
