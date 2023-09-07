# ca-1d

Elementary cellular automata CLI, written in C.

## Idea for usage

I use this Vim welcome screen art (using sophisticated vim script from [here](https://vi.stackexchange.com/questions/627/how-can-i-change-vims-start-or-intro-screen)):

![Idea for usage](./img/idea.png)

To conveniently run `ca-1d`, I wrapped invocation with all parameters in bash script:

```bash
#!/bin/bash

~/Projects/ca-1d/build/release/ca-1d \
        --rule=110,22,108,30,218,45,18,24,54,184,90,126,250,150,66,99,182 \
        --width=31-80 \
        --height=16-22 \
        --map_alive=@\K\&\%\#\*\?\$\=MW\>86RBE3~xdVN\< \
        --map_dead=e\ i\:\;\"\'\`\/\\\ .\,\|_\-qojs \
        --map_frequency=r \
        --seed_mode=r \
        --seed=$(date +%s%3N)
```

## Usage

Control display by providing parameters:

```text
Usage:
        ./build/release/ca-1d --help
        ./build/release/ca-1d --rule=[0-255] --width=[1-1000] --height=[1-1000] --map_alive=C... --map_dead=C... --map_frequency=program|line|cell -stdin_char_alive=1 --stdin_char_dead=0 --seed_mode=pulse|random --seed=0|[1-4294967295]
        ./build/release/ca-1d --rule=[0-255,;|...] --width=[1-1000,;|...] --height=[1-1000,;|...] --map_alive=C... --map_dead=C... --map_frequency=program|line|cell --stdin_char_alive=1 --stdin_char_dead=0 --seed_mode=pulse|random --seed=0|[1-4294967295]
        ./build/release/ca-1d --rule=r --width=r --height=r --map_alive=C... --map_dead=C... --map_frequency=r --stdin_char_alive=1 --stdin_char_dead=0 --seed_mode=r --seed=0|[1-4294967295]
```

## Standard input

The standard input of program accepts a string of any length.

If non-empty, the string is used to seed cellular automata
(write to first line, displayed in result). Therefore, `seed_mode`
parameter will have no effect.

If the length of accepted string is shorter, than the final `width` value
(which is chosen from list or range, if such provided), the rest of cells
will be dead.

Valid characters for alive and dead cells are specified in
`stdin_char_alive` and `stdin_char_dead` parameters.

## Parameters

General syntax for parameters is `--param=value`.

|Parameter         |Value type|Possible values           |Examples|Default value|
|------------------|----------|--------------------------------|----------------------------------------------------------------------------|---------------------------------------------|
|`rule`            |number    |single, range, list or `r`      |`--rule=45`<br>`--rule=30-45`<br>`--rule=30,45;90\|110`<br>`--rule=r`       |`90`                                         |
|`width`           |number    |single, range, list or `r`      |`--width=20`<br>`--width=20-40`<br>`--width=20,30;40\|50`<br>`--width=r`    |`11`                                         |
|`height`          |number    |single, range, list or `r`      |`--height=10`<br>`--height=10-20`<br>`--height=10,20;30\|40`<br>`--height=r`|`10`                                         |
|`map_alive`       |string    |single                          |`--map_alive=X`<br>`--map_alive=XYZ`                                        |`X`                                          |
|`map_dead`        |string    |single                          |`--map_dead=\ `<br>`--map_dead=\ _.`                                        |Space character                              |
|`map_frequency`   |string    |`program`, `line`, `cell` or `r`|`--map_frequency=line`<br>`--map_frequency=r`                         |`program`                                    |
|`stdin_char_alive`|char      |single                          |`--stdin_char_alive=X`                                                      |`1`                                          |
|`stdin_char_dead` |char      |single                          |`--stdin_char_dead=_`                                                      |`0`                                          |
|`seed_mode`       |string    |`pulse`, `random` or `r`        |`--seed_mode=random`<br>`--seed_mode=r`                                     |`pulse`                                      |
|`seed`            |number    |single                          |`--seed=0`<br>`--seed=1234`                                                 |`0` is replaced by `time(NULL)` from `time.h`|
|`help`            |&ndash;   |&ndash;                         |`--help`                                                                    |&ndash;                                      |

### Notes on parameters

- values for parameters containing range are chosen at the program startup
- `[0-255]` or `[1-1000]` means single value in that range, or range of values
with bounds not exceeding that range
- `[0-255,;|...]` or `[1-1000,;|...]` means a list of values in that range,
separated with either `,`, `;` or `|`. Separators can be combined.

    > **Note**
    > Separators, which have special meaning in your shell
    > must be escaped: `\;` or `\|`

- `C...` means non-empty string
- `r` value indicates, that value is chosen from default range or from list of
possible values for string type parameters. Parameters that accept this value
are listed in [Usage](#usage)
- upper bound for `seed` parameter may differ, depending on your system and
compiler implementation (it is `UINT_MAX` value from `limits.h`).
Always run `ca-1d --help` to check `seed` range
- `seed` parameter value is truncated to its range, this means it will not
return error for greater values, but **it will error on negative value.**
This can be used to give more entropy on short intervals, for example:

    ```bash
    ca-1d --seed=$(date +%s%3N)
    ```

    which gets amount of milliseconds since UNIX epoch, so that
    seed does not repeat until approximately 50 days.

## Building

This builds in `release` mode:

```bash
make clean && make
```

Then run:

```bash
./build/release/ca-1d ...your options
```

To build in `debug` mode:

```bash
make clean && make debug
```

Then debug, using GDB:

```bash
gdb --args ./build/debug/ca-1d ...your options
```

## Development

If encounter bugs or want to improve the project,
feel free to post an issue or submit PR :sunglasses:
