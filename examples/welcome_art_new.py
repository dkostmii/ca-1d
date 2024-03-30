from time import time
from random import randint, seed
import sys
import subprocess

bin_path = "~/Projects/ca-1d/build/release/ca-1d"

unix_epoch_millis = int(time() * 1000)

# Note: in C seed parameter is truncated to UINT_MAX
# while in Python it is not
seed(unix_epoch_millis)

height_range = [16, 22]
height_factor = 4
scaled_height_range = [ height_factor * value for value in height_range ]

width = "31-80"

if len(sys.argv) > 1:
    try:
        width = str(int(sys.argv[1]))
    except ValueError:
        pass

command = [
    bin_path,
    "--rule=110,22,108,30,218,45,18,24,54,184,90,126,250,150,66,99,182",
    f"--width={width}",
    f"--height={scaled_height_range[0]}-{scaled_height_range[1]}",
    r"--map_alive=@K\&\%\#\*\?\$\=MW\>86RBE3~xdVN\<",
    r"--map_dead=e\ i\:\;\"\'\`\/\\\ .\,\|_-qojs",
    "--map_frequency=r",
    "--seed_mode=r",
    f"--seed={unix_epoch_millis}"
]

command = " ".join(command)

def rotate(string: str, amount: int) -> str:
    return string[-amount:] + string[:-amount]


def shift_horiz(lines: list[str]) -> list[str]:
    width = len(lines[0])
    rotation_range = [int(-width / 2), int(width / 2)]
    rotation_amount = randint(rotation_range[0], rotation_range[1])

    return [rotate(line, rotation_amount) for line in lines]


def get_vertical_slice(lines: list[str]) -> list[str]:
    height = len(lines)

    slice_height = int(height / height_factor)

    start = randint(0, height - slice_height - 1)
    end = start + slice_height

    return lines[start:end]


def main():
    command_output = subprocess.check_output(command, text=True, encoding='utf-8', shell=True)
    output_lines = command_output.split('\n')
    slice = get_vertical_slice(output_lines)
    slice = shift_horiz(slice)

    for line in slice:
        print(line)


if __name__ == "__main__":
    main()
