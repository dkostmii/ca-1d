# ca-1d

Elementary cellular automata CLI, written in C.

Control display by providing parameters:

```bash
Usage:
        ./build/ca-1d --help
        ./build/ca-1d --rule=[0-255] --width=[1-1000] --height=[1-1000] --map=CC...
        ./build/ca-1d --rule=[0-255,;|...] --width=[1-1000,;|...] --height=[1-1000,;|...] --map=CC...
        ./build/ca-1d --rule=r --width=r --height=r --map=CC...
```

You can provide range of numeric values or list of values to choose (but not both) for `rule`, `width` and `height`.
`map` option accepts two or more symbols for displaying CA state. If more than two given, choosing two different random characters.

You can also provide `r` value for options `rule`, `width` and `height` to randomize value with default range (`0-255` or `1-1000`).

## Building

```bash
make clean && make
```

Then run:

```bash
./build/ca-1d ...your options
```
