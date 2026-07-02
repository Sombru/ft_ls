
*This project has been created as part of the 42 advanced curriculum by pkostura*

# ft_ls

A 42-style reimplementation of `ls`, written in C and backed by a custom `libft`.

The goal is to reproduce the common `ls` listing modes while keeping the code explicit: entries are read with `opendir`/`readdir`, metadata is collected with `lstat`, output is formatted manually, and recursive traversal is handled in the project code.

## Build

```sh
make
```

This builds:

```text
./ft_ls
```

Clean targets:

```sh
make clean
make fclean
make re
```

## Usage

```sh
./ft_ls [options] [file ...]
```

If no path is provided, `.` is listed.

Examples:

```sh
./ft_ls
./ft_ls -la
./ft_ls -R src include
./ft_ls -d *
./ft_ls -lt /tmp
```

## Supported Options

```text
-1    one entry per line
-C    force column output
-a    include hidden entries
-d    list directory arguments as entries, do not expand contents
-f    do not sort, imply -a
-g    long listing without owner
-l    long listing
-R    recurse into subdirectories
-r    reverse sort order
-t    sort by modification time
-u    use access time for sorting when combined with -t
-U    keep directory order, do not sort
```

## Implemented Behavior

- Regular files passed as arguments are displayed directly.
- Mixed operands are handled like `ls`: file arguments are printed first, then directory contents.
- `-d` treats all operands as entries and does not expand directories.
- Recursive listing prints directory headers and separates entry lists with blank lines.
- Long listing computes aligned columns for links, owner, group, and size.
- Symlink targets are printed in long mode.
- Column mode adapts to terminal width.
- Output includes simple color support for directories, symlinks, executables, FIFOs, sockets, and device files.
- Hidden-name sorting ignores a leading `.` for comparison, while preserving special handling for `.` and `..`.

## Tests

Two helper scripts live in `tests/`.

### Compare Against System ls

```sh
tests/compare_ls.sh
```

The script creates a deterministic fixture in `/tmp`, runs `./ft_ls` and `/bin/ls --color=never`, strips ANSI color codes, and compares stdout, stderr, and exit status.

Known current differences:

- Missing paths currently return exit status `0` in `ft_ls`, while `/bin/ls` returns `2`.
- Hidden-name sorting intentionally follows the project rule of ignoring a leading `.`, which may differ from the system `ls` on this machine.

### Benchmark

```sh
tests/bench_ls.sh
```

The benchmark creates a larger fixture and compares repeated runs of `/bin/ls` and `./ft_ls`.

Set the number of repetitions with:

```sh
REPEAT=1000 tests/bench_ls.sh
```

## Notes

This project is still intentionally small and direct. Some areas that can be improved later:

- Match system `ls` error status and error message formatting more closely.
- Add ACL/security-context indicator support in long mode.
- Reduce repeated lookups in long listing for better performance.
- Expand comparison tests for edge cases such as permissions, broken symlinks, and special files.
