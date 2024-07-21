import subprocess
import sys
from random import seed, choice, choices
from time import sleep, time
from os import system

from colored import Fore, Style


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
            [Fore.light_blue, ":;\\\"'`"],
            [Fore.light_magenta, "qojsei"],
            [Fore.light_green, "/.,|_-"],
        ],
    )
)


unix_epoch_millis = int(time() * 1000)
seed(unix_epoch_millis)

rule = 90
width = 21
help = False
animate = False
animation_height = 10
animation_freq_seconds = 0.1


def indent_content(content: list[str | list], indent: str = "    ", level: int = 0) -> list[str]:
    result = []

    for line_or_block in content:
        if isinstance(line_or_block, list):
            block = line_or_block
            indented_block = indent_content(block, indent, level + 1)
            # INFO: Flatten in place
            result.extend(indented_block)

        elif isinstance(line_or_block, str):
            line = line_or_block
            line = (indent * level) + line
            result.append(line)

        else:
            raise Exception("Invalid content item type.")

    return result


def print_usage():
    print(
       "\n".join(
            indent_content(
                content=[
                    "ca-1d CLI example Python script",
                    "",
                    "Usage:"
                    "",
                    [
                        "python welcome_art.py"
                    ],
                    "",
                    "Parameters:",
                    [
                        f"--width Defines a width of resulting image. Defaults to {width}.",
                        f"--animation-height Defines a maximum height of resulting image. Defaults to {animation_height}.",
                        f"--animation-freq-seconds Defines, how fast is animation. Defaults to {animation_freq_seconds} seconds.",
                        f"--rule Defines an elementary cellular automaton rule to use. Must be in range 0-255. Defaults to {rule} rule."
                    ],
                ]
            )
       )
    )


def parse_int(value: str) -> int | None:
    try:
        return int(value)
    except ValueError:
        return None

def parse_float(value: str) -> float | None:
    try:
        return float(value)
    except ValueError:
        return None


if len(sys.argv) > 0:
    for arg in sys.argv:
        if arg == "--help":
            print_usage()
            exit(0)

        elif arg.startswith("--animation-height="):
            _, value = arg.split("=")

            value = parse_int(value)

            if value is None:
                print(f"Invalid animation height value: {value}.")
                print_usage()
                exit(1)

            animation_height = value

        elif arg.startswith("--width="):
            _, value = arg.split("=")

            value = parse_int(value)

            if value is None:
                print(f"Invalid width value: {value}.")
                print_usage()
                exit(1)

            width = value

        elif arg.startswith("--anim-freq-seconds="):
            _, value = arg.split("=")

            value = parse_float(value)

            if value is None:
                print(f"Invalid anim-freq-seconds value: {value}.")
                print_usage()
                exit(1)

            animation_freq_seconds = value

        elif arg.startswith("--rule"):
            _, value = arg.split("=")

            value = parse_int(value)

            if value is None:
                print(f"Invalid rule value: {value}.")
                print_usage()
                exit(1)


            if 0 <= value <= 255:
                rule = value
            else:
                print(f"Rule value out of range 0-255: {value}.")
                print_usage()
                exit(1)


map_alive = "@K&%#*?$=MW>86RBE3~xdVN<"
map_dead = "e i:;\"\'`/\\ .,|_-qojs"
seed_state = choice(
    [
        None,
        "".join(choices(['0', '1'], [0.5, 0.5], k=width))
    ]
)

bin_path = "/home/dkostmii/Projects/ca-1d/build/release/ca-1d"
command = [
    bin_path,
    f"--rule={rule}",
    f"--width={width}",
    f"--height=inf",
]


def main():
    if seed_state is not None:
        seed_proc = subprocess.Popen(
            ["echo", seed_state],
            stdout=subprocess.PIPE,
        )

        proc = subprocess.Popen(
            command,
            encoding="utf-8",
            stdout=subprocess.PIPE,
            stdin=seed_proc.stdout,
        )
    else:
        proc = subprocess.Popen(
            command,
            encoding="utf-8",
            stdout=subprocess.PIPE,
        )

    stdout_end = False

    buffer = []

    program_char_dead = choice(map_dead)
    program_char_alive = choice(map_alive)

    while not stdout_end:
        line = proc.stdout.readline()
        stdout_end = len(line) == 0
        result = ""

        line_char_dead = choice(map_dead)
        line_char_alive = choice(map_alive)
        rnd_method = choice(["program", "line", "cell"])

        if not stdout_end:
            for ch in line:
                if rnd_method == "cell":
                    ch = choice(map_dead if ch == '0' else map_alive)
                elif rnd_method == "line":
                    ch = line_char_dead if ch == '0' else line_char_alive
                elif rnd_method == "program":
                    ch = program_char_dead if ch == '0' else program_char_alive

                pal_item = next(filter(lambda pi: ch in pi.pattern, palette), None)
                if pal_item:
                    result += f"{Style.reset}{pal_item.color}"

                result += ch
            result = result.replace("\n", "")

            buffer.append(result)
            system("clear")
            print("\r" + "\n".join(buffer))

            if len(buffer) >= animation_height:
                buffer.pop(0)

            try:
                sleep(animation_freq_seconds)
            except KeyboardInterrupt:
                exit(0)


if __name__ == "__main__":
    main()
