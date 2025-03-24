USAGE = """
Execute a curl command which downloads from multiple URLs to multiple files.
URL-filename pairs are provided from stdin, with the following format:
    <URL1> <filename1>
    <URL2> <filename2>
    ...

Downloaded files are written to `dist_dir/filename` where `dist_dir` is specified in argv.
`dist_dir` is set to `.` when there is no arguments.
"""

from dataclasses import dataclass
import argparse
import subprocess


@dataclass(frozen=True, slots=True)
class Entry:
    url: str
    file: str

    def to_options(self, dist_dir):
        return [self.url, "-o", f"{dist_dir}/{self.file}"]

    @classmethod
    def from_line(cls, line):
        url, file = line.split()
        return cls(url, file)


@dataclass(frozen=True, slots=True)
class Args:
    dist_dir: str

    @staticmethod
    def parser():
        parser = argparse.ArgumentParser(usage=USAGE)
        parser.add_argument("dist_dir", nargs="?", default=".")
        return parser

    @classmethod
    def from_any(cls, o):
        dist_dir = getattr(o, "dist_dir", None)
        assert isinstance(dist_dir, str)
        return cls(dist_dir)


def main():
    from itertools import chain
    from sys import stdin, stderr

    parser = Args.parser()
    args = parser.parse_args()
    args = Args.from_any(args)
    lines = map(str.strip, stdin)
    entries = (Entry.from_line(l) for l in lines if l)
    parallel_opts = (e.to_options(args.dist_dir) for e in entries)
    parallel_opts = chain.from_iterable(parallel_opts)
    command = ["curl", "-fLZ", "--progress-bar", *parallel_opts]
    subprocess.run(
        command,
        check=True,
        stdout=subprocess.DEVNULL,
        stderr=stderr,
    )


if __name__ == "__main__":
    main()
