# PROG20002 Graphics Programming

## Commands

Build project:

```sh
mkdir -p build && cd build && cmake .. && make && cd ..
```

Create `compile_commands.json` for `ccls`:

```sh
mkdir -p build && cd build && cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1 && cp compile_commands.json ../compile_commands.json && cd ..
```
