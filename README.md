# indigo

Indigo: A simple learning OS for Raspberry Pi 4 B+

## Development

A CMake toolchain file for the ARM Embedded (GNU) toolchain is provided, if you want to use a toolchain besides that one
you're on your own.

GCC 11.2 is required at a minimum, as this project (and its dependencies) use many C++20 features in both the library
and language that older versions of GCC/libstdc++ do not provide.

## Configuration

Little configuration is needed beyond the toolchain prefix.

- `TOOLCHAIN_PREFIX`: Path to the root of your embedded toolchain (e.g `/home/you/opt/gcc-embedded-11.2`).

## License

All code in this project is licensed under the license found in [LICENSE](./LICENSE). Any dependencies may have their
own licensing requirements.