# libemu

libemu is a simple IPC library for controlling emulators through
client programs.

It supports messages for controlling the flow of execution (like
setting breakpoints) and inspecting the emulators' internal state.

A sample client, implementing a monitor/debugger for the RealBoy
emulator, is found at https://github.com/sergio-gdr/realboy-mon.

Currently it is meant to be used by clients and servers running on
the same host machine.

This library is intended to be used alongside [RealBoy] and
[RealBoy Monitor] as a teaching device for introducing low-level and
systems-level programming.

Take a look [here] for more details on the purpose and goals of the
project.

## Installation

### Compiling from Source

Dependencies:

* meson (for building)

Run these commands:

    meson setup build/
    ninja -C build/
    sudo ninja -C build/ install


[RealBoy]: https://github.com/sergio-gdr/realboy
[RealBoy Monitor]: https://github.com/sergio-gdr/realboy-mon
[here]: https://raw.githubusercontent.com/sergio-gdr/realboy-book/refs/heads/main/book.txt
