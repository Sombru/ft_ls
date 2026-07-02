#!/usr/bin/env bash
set -u

ROOT_DIR="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
FT_LS="$ROOT_DIR/ft_ls"
TMP_DIR="${TMPDIR:-/tmp}/ft_ls_bench.$$"
REPEAT="${REPEAT:-200}"

cleanup()
{
	rm -rf "$TMP_DIR"
}

now_ns()
{
	date +%s%N
}

elapsed_ms()
{
	local start="$1"
	local end="$2"

	printf '%s' $(((end - start) / 1000000))
}

make_fixture()
{
	local i

	mkdir -p "$TMP_DIR/root"
	i=0
	while [ "$i" -lt 500 ]; do
		printf 'file %04d\n' "$i" > "$TMP_DIR/root/file_$i"
		i=$((i + 1))
	done
	i=0
	while [ "$i" -lt 50 ]; do
		mkdir -p "$TMP_DIR/root/dir_$i/sub"
		printf 'nested %04d\n' "$i" > "$TMP_DIR/root/dir_$i/sub/nested_$i"
		i=$((i + 1))
	done
}

bench()
{
	local label="$1"
	shift
	local start
	local end
	local i

	i=0
	start=$(now_ns)
	while [ "$i" -lt "$REPEAT" ]; do
		"$@" >/dev/null 2>/dev/null
		i=$((i + 1))
	done
	end=$(now_ns)
	printf '%-34s %8s ms total  %8s ms/run\n' \
		"$label" "$(elapsed_ms "$start" "$end")" \
		"$(((end - start) / 1000000 / REPEAT))"
}

bench_pair()
{
	local name="$1"
	shift

	printf '\n%s (%d runs)\n' "$name" "$REPEAT"
	bench "system ls" /bin/ls --color=never "$@"
	bench "ft_ls" "$FT_LS" "$@"
}

trap cleanup EXIT
make -C "$ROOT_DIR" >/dev/null || exit 1
rm -rf "$TMP_DIR"
mkdir -p "$TMP_DIR"
make_fixture

printf 'Benchmark fixture: %s\n' "$TMP_DIR/root"
printf 'Override repetitions with: REPEAT=1000 %s\n' "$0"

bench_pair "one per line" -1 "$TMP_DIR/root"
bench_pair "all entries" -1a "$TMP_DIR/root"
bench_pair "long listing" -l "$TMP_DIR/root"
bench_pair "mixed operands" -1 \
	"$TMP_DIR/root/dir_1" "$TMP_DIR/root/file_10" \
	"$TMP_DIR/root/dir_2" "$TMP_DIR/root/file_20"
bench_pair "recursive" -1R "$TMP_DIR/root/dir_1" "$TMP_DIR/root/dir_2"
