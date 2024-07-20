import subprocess
import sys
from random import randint, seed, choice
from time import sleep, time
from os import system

from colored import Fore, Style

# provide --help or -h param for usage
# example: python welcome_art.py --help


class PaletteItem:
    pattern: str

    def __init__(self, color, pattern: str):
        self.color = color
        self.pattern = pattern

    def apply(self, text: str):
        return f"{self.color}{text}{Style.reset}"


palette = list(
    map(
        lambda item: PaletteItem(*item),
        [
            [Fore.blue, "@K&%#*?$"],
            [Fore.magenta, "MW>86RBE"],
            [Fore.green, "=3~xdVN<"],
            [Fore.light_blue, ":;\"'`"],
            [Fore.light_magenta, "qojsei"],
            [Fore.light_green, "/.,|_-"],
        ],
    )
)

bin_path = "ca-1d"

unix_epoch_millis = int(time() * 1000)

# NOTE: in C seed parameter is truncated to UINT_MAX
# while in Python it is not
seed(unix_epoch_millis)

height_range = [16, 22]
height = None
height_factor = 4
scaled_height_range = [height_factor * value for value in height_range]

default_width = "31-80"
width = default_width
use_color = False
help = False
animate = False
animation_height = 1

map_alive = choice([
    "@",
    r"K",
    r"\&",
    r"\%",
    r"\#",
    r"\*",
    r"\?",
    r"\$",
    r"\=",
    "M",
    "W",
    r"\>",
    "8",
    "6",
    "R",
    "B",
    "E",
    "3",
    "~",
    "x",
    "d",
    "V",
    "N",
    r"\<",
])

map_dead = choice([
    r"e",
    r"\ ",
    r"i",
    r"\:",
    r"\;"
    r"\"",
    r"\'",
    r"\`",
    r"\/",
    "\\",
    r"\ ",
    ".",
    r"\,",
    r"\|",
    r"_",
    r"\-",
    r"q",
    r"o",
    r"j",
    r"s",
])

def print_usage():
    print("Usage:")
    print(
        f"    python welcome_art.py [width={default_width}] [height={height_range[0]}-{height_range[1]}|inf] [-c|--color] [-h|--help] [-a|--animate]"
    )


if len(sys.argv) > 1:
    args = sys.argv.copy()

    prev_args_len = len(args)
    args = list(filter(lambda arg: arg != "-c" and arg != "--color", args))
    next_args_len = len(args)

    use_color = prev_args_len != next_args_len

    prev_args_len = next_args_len
    args = list(filter(lambda arg: arg != "-h" and arg != "--help", args))
    next_args_len = len(args)

    help = prev_args_len != next_args_len

    prev_args_len = next_args_len
    args = list(filter(lambda arg: arg != "-a" and arg != "--animate", args))
    next_args_len = len(args)

    animate = prev_args_len != next_args_len

    prev_args_len = next_args_len

    for id, arg in enumerate(args):
        if arg.startswith("-ah=") or arg.startswith("--animation-height="):
            args.pop(id)
            animation_height = max(1, int(arg.split('=')[1]))

    next_args_len = len(args)

    if next_args_len > 1:
        try:
            arg_width = args[1]

            if arg_width:
                width = str(int(arg_width))

        except ValueError:
            print(f"Invalid width value: {args[1]}")
            print_usage()
            sys.exit(1)

    if next_args_len > 2:
        try:
            arg_height = args[2]

            if arg_height and arg_height != "inf":
                height = str(int(arg_height))
            elif arg_height == "inf":
                height = arg_height

        except ValueError:
            print(f"Invalid height value: {args[2]}")
            print_usage()
            sys.exit(1)

if help:
    print_usage()
    sys.exit(0)


command = [
    bin_path,
    "--rule=110,22,108,30,218,45,18,24,54,184,90,126,250,150,66,99,182",
    f"--width={width}",
    (
        f"--height={scaled_height_range[0]}-{scaled_height_range[1]}"
        if height is None
        else f"--height={height}"
    ),
    f"--map_alive={map_alive}",
    f"--map_dead={map_dead}",
    "--seed_mode=r",
    f"--seed={unix_epoch_millis}",
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
    if animate:
        proc = subprocess.Popen(
            [
                "ca-1d",
                "--height=50",
                f"--width={width}",
                "--height=inf",
                f"--map_alive={map_alive}",
                f"--map_dead={map_dead}",
            ],
            encoding="utf-8",
            stdout=subprocess.PIPE,
        )
        stdout_end = False

        buffer = []

        while not stdout_end:
            line = proc.stdout.readline()
            stdout_end = len(line) == 0
            result = ""

            if not stdout_end:
                for ch in line:
                    pal_item = next(filter(lambda pi: ch in pi.pattern, palette), None)
                    if use_color and pal_item:
                        result += f"{Style.reset}{pal_item.color}"

                    result += ch
                result = result.replace("\n", "")

                buffer.append(result)
                system("clear")
                print("\r" + "\n".join(buffer))

                if len(buffer) > animation_height:
                    buffer.pop(0)

                try:
                    sleep(0.1)
                except KeyboardInterrupt:
                    exit(0)
        return

    command_output = subprocess.check_output(
        command, text=True, encoding="utf-8", shell=True
    )
    output_lines = command_output.split("\n")
    slice = get_vertical_slice(output_lines)
    slice = shift_horiz(slice)
    text = "\n".join(slice)

    result = ""
    for ch in text:
        pal_item = next(filter(lambda pi: ch in pi.pattern, palette), None)
        if use_color and pal_item:
            result += f"{Style.reset}{pal_item.color}"

        result += ch

    print(result)


if __name__ == "__main__":
    main()
