""" Combine multiple C++ headers into one file, preserving std includes and namespaces """

import argparse
import pathlib
import re
import sys

INCLUDE_RE = re.compile(r'^\s*#\s*include\s*[<"]([^">]+)[>"]')
PRAGMA_ONCE_RE = re.compile(r'^\s*#\s*pragma\s+once')
IFDEF_GUARD_RE = re.compile(r'^\s*#\s*(ifndef|ifdef|define|endif)\b')
NAMESPACE_OPEN_RE = re.compile(r'^\s*namespace\s+cip\s*{')
NAMESPACE_CLOSE_RE = re.compile(r'^\s*}\s*//\s*namespace\s+cip')


def process_file(path: pathlib.Path,
                 seen_user: set,
                 seen_std: set,
                 std_lines: list):
    """
    Generator that yields lines from `path`, recursively inlining user headers,
    preserving and deduplicating std includes, and stripping guards.
    """
    path = path.resolve()
    if not path.is_file():
        print(f"Warning: include not found: {path}", file=sys.stderr)
        return

    with path.open(encoding='utf-8') as f:
        for line in f:
            if PRAGMA_ONCE_RE.match(line) or IFDEF_GUARD_RE.match(line):
                continue
            if NAMESPACE_OPEN_RE.match(line) or NAMESPACE_CLOSE_RE.match(line):
                continue

            m = INCLUDE_RE.match(line)
            if m:
                filename = m.group(1)
                inc = path.parent / filename
                # Standard library / system include
                if line.strip().startswith('#include <'):
                    if filename not in seen_std:
                        seen_std.add(filename)
                        line.replace('<mdspan/mdspan.hpp>', '"mdspan.hpp"')
                        std_lines.append(line)
                    continue
                # User header include (quotes)
                inc_path = (path.parent / inc).resolve()
                if inc_path not in seen_user:
                    seen_user.add(inc_path)
                    yield from process_file(
                        inc_path, seen_user, seen_std, std_lines
                    )
                continue

            yield line.replace('cip::', '')


def collect_headers(entry: pathlib.Path):
    """Kick off the header collection with fresh include sets."""
    seen_user = set()
    seen_std = set()
    std_lines = []
    body_lines = list(process_file(entry, seen_user, seen_std, std_lines))
    return std_lines, body_lines


def comment_header(args):
    """ Generator that yields comment lines for the single header file. """
    with open(args.license, encoding="utf-8") as f:
        license_lines = f.read()
    yield '/*\n'
    yield 'Cubinterpp: Single header library\n'
    yield '\n'
    yield '\n'
    yield 'SPDX-License-Identifier: MIT\n'
    yield '\n'
    yield '\n'
    for line in license_lines.splitlines():
        yield f'{line}\n'
    yield '\n */\n\n'


def write_single_header(args):
    """ Write the single header file. """
    entry_file = pathlib.Path(args.entry)
    std_lines, body_lines = collect_headers(entry_file)
    output_file = pathlib.Path(args.output)
    output_file.parent.mkdir(parents=True, exist_ok=True)
    with open(output_file, "w", encoding="utf-8") as f:
        f.writelines(' * '.join(comment_header(args)))
        f.writelines(std_lines)
        f.write("\n")
        f.write("namespace cip {")
        f.writelines(body_lines)
        f.write("} // namespace cip\n")


def main():
    p = argparse.ArgumentParser(description="Combine C++ headers into one file")
    p.add_argument("--entry", help="Top‑level header (relative to root)")
    p.add_argument("--output", help="Path to write combined header")
    p.add_argument("--license", help="Path to license file")
    args = p.parse_args()
    write_single_header(args)


if __name__ == "__main__":
    main()
