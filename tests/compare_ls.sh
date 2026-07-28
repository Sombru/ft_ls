#!/usr/bin/env bash
set -u

ROOT_DIR="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
FT_LS="$ROOT_DIR/ft_ls"
TMP_DIR="${TMPDIR:-/tmp}/ft_ls_compare.$$"
PASS_COUNT=0
FAIL_COUNT=0

cleanup()
{
	rm -rf "$TMP_DIR"
}

strip_ansi()
{
	sed -E 's/\x1B\[[0-9;]*[[:alpha:]]//g'
}

run_cmd()
{
	local out="$1"
	local err="$2"
	shift 2

	"$@" >"$out.raw" 2>"$err.raw"
	local status=$?
	strip_ansi <"$out.raw" >"$out"
	strip_ansi <"$err.raw" >"$err"
	return "$status"
}

set_times()
{
	local path="$1"
	local mtime="$2"
	local atime="$3"

	touch -h -t "$mtime" "$path" 2>/dev/null || touch -t "$mtime" "$path"
	touch -a -t "$atime" "$path"
}

make_fixture()
{
	mkdir -p "$TMP_DIR/root/a_dir" "$TMP_DIR/root/c_dir" "$TMP_DIR/root/empty"
	printf 'alpha\n' > "$TMP_DIR/root/b_file"
	printf 'zulu\n' > "$TMP_DIR/root/z_file"
	printf 'early\n' > "$TMP_DIR/root/early_file"
	printf 'middle\n' > "$TMP_DIR/root/middle_file"
	printf 'late\n' > "$TMP_DIR/root/late_file"
	printf 'hidden\n' > "$TMP_DIR/root/.hidden"
	mkdir -p "$TMP_DIR/root/.hidden_dir"
	printf 'secret\n' > "$TMP_DIR/root/.hidden_dir/secret"
	printf 'inside\n' > "$TMP_DIR/root/a_dir/inside_a"
	printf 'inside\n' > "$TMP_DIR/root/c_dir/inside_c"
	chmod +x "$TMP_DIR/root/z_file"
	ln -s ../b_file "$TMP_DIR/root/a_dir/link_to_b" 2>/dev/null || true
	set_times "$TMP_DIR/root/early_file" 202607010101 202607270101
	set_times "$TMP_DIR/root/middle_file" 202607020202 202607270202
	set_times "$TMP_DIR/root/late_file" 202607030303 202607270303
	set_times "$TMP_DIR/root/a_dir/link_to_b" 202607041212 202607270404
	set_times "$TMP_DIR/root/.hidden_dir/secret" 202607211111 202607270505
	set_times "$TMP_DIR/root/a_dir/inside_a" 202607231111 202607270707
	set_times "$TMP_DIR/root/c_dir/inside_c" 202607251111 202607270909
	set_times "$TMP_DIR/root/.hidden_dir" 202607201010 202607270505
	set_times "$TMP_DIR/root/.hidden" 202607211010 202607270606
	set_times "$TMP_DIR/root/a_dir" 202607221010 202607270707
	set_times "$TMP_DIR/root/b_file" 202607231010 202607270808
	set_times "$TMP_DIR/root/c_dir" 202607241010 202607270909
	set_times "$TMP_DIR/root/empty" 202607251010 202607271010
	set_times "$TMP_DIR/root/z_file" 202607261010 202607271111
	set_times "$TMP_DIR/root" 202607281212 202607271212
}

record_failure()
{
	local name="$1"
	local ft_status="$2"
	local ls_status="$3"
	local ft_out="$4"
	local ls_out="$5"
	local ft_err="$6"
	local ls_err="$7"

	FAIL_COUNT=$((FAIL_COUNT + 1))
	printf 'FAIL %s\n' "$name"
	if [ "$ft_status" -ne "$ls_status" ]; then
		printf '  exit status: ft_ls=%s ls=%s\n' "$ft_status" "$ls_status"
	fi
	if ! cmp -s "$ft_out" "$ls_out"; then
		printf '  stdout diff:\n'
		diff -u "$ls_out" "$ft_out" | sed 's/^/    /'
	fi
	if ! cmp -s "$ft_err" "$ls_err"; then
		printf '  stderr diff:\n'
		diff -u "$ls_err" "$ft_err" | sed 's/^/    /'
	fi
}

run_case()
{
	local name="$1"
	shift
	local case_dir="$TMP_DIR/case_$((PASS_COUNT + FAIL_COUNT + 1))"
	local ft_status
	local ls_status

	mkdir -p "$case_dir"
	run_cmd "$case_dir/ft.out" "$case_dir/ft.err" "$FT_LS" "$@"
	ft_status=$?
	run_cmd "$case_dir/ls.out" "$case_dir/ls.err" /bin/ls --color=never "$@"
	ls_status=$?
	if [ "$ft_status" -eq "$ls_status" ] \
		&& cmp -s "$case_dir/ft.out" "$case_dir/ls.out" \
		&& cmp -s "$case_dir/ft.err" "$case_dir/ls.err"; then
		PASS_COUNT=$((PASS_COUNT + 1))
		printf 'PASS %s\n' "$name"
	else
		record_failure "$name" "$ft_status" "$ls_status" \
			"$case_dir/ft.out" "$case_dir/ls.out" \
			"$case_dir/ft.err" "$case_dir/ls.err"
	fi
}

trap cleanup EXIT
make -C "$ROOT_DIR" >/dev/null || exit 1
rm -rf "$TMP_DIR"
mkdir -p "$TMP_DIR"
make_fixture

run_case "one per line directory" -1 "$TMP_DIR/root"
run_case "column output" -C "$TMP_DIR/root"
run_case "all entries" -1a "$TMP_DIR/root"
run_case "directory as entry" -1d "$TMP_DIR/root/a_dir"
run_case "all directory as entry" -1ad "$TMP_DIR/root/.hidden_dir"
run_case "file argument" -1 "$TMP_DIR/root/b_file"
run_case "hidden file argument" -1 "$TMP_DIR/root/.hidden"
run_case "mixed operands files before directories" -1 \
	"$TMP_DIR/root/a_dir" "$TMP_DIR/root/b_file" \
	"$TMP_DIR/root/c_dir" "$TMP_DIR/root/z_file"
run_case "reverse sort" -1r "$TMP_DIR/root"
run_case "time sort" -1t "$TMP_DIR/root"
run_case "time reverse sort" -1tr "$TMP_DIR/root"
run_case "access time sort" -1tu "$TMP_DIR/root"
run_case "access time reverse sort" -1tur "$TMP_DIR/root"
run_case "unsorted" -1U "$TMP_DIR/root"
run_case "unsorted reverse ignored" -1Ur "$TMP_DIR/root"
run_case "force unsorted all entries" -1f "$TMP_DIR/root"
run_case "recursive" -1R "$TMP_DIR/root/a_dir" "$TMP_DIR/root/c_dir"
run_case "recursive skips hidden dirs" -1R "$TMP_DIR/root"
run_case "recursive all entries" -1aR "$TMP_DIR/root"
run_case "long listing" -l "$TMP_DIR/root"
run_case "long listing all" -la "$TMP_DIR/root"
run_case "long listing symlink argument" -l "$TMP_DIR/root/a_dir/link_to_b"
run_case "group long listing" -g "$TMP_DIR/root"
run_case "group long listing all" -ga "$TMP_DIR/root"
run_case "combined flags" -1alRrt "$TMP_DIR/root"
run_case "missing path" -1 "$TMP_DIR/root/missing"

printf '\n%d passed, %d failed\n' "$PASS_COUNT" "$FAIL_COUNT"
if [ "$FAIL_COUNT" -ne 0 ]; then
	exit 1
fi
