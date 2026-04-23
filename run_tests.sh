#!/usr/bin/env bash
# Pipex test runner — mirrors projects/42_Pipex/42_Pipex_test_instructions.md
# Usage: ./tests/run_tests.sh            # run all tests
#        ./tests/run_tests.sh T6 T13     # run a subset
#
# For every case: captures stdout, stderr, exit code, and (where applicable)
# diffs against the shell reference. Prints a PASS/FAIL summary at the end.

set -u

PIPEX_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$PIPEX_DIR"

PIPEX="./pipex"
PASS=0
FAIL=0
RESULTS=()

OS="$(uname -s)"

# ---------- helpers ----------

run_case() {
	# $1 = test id, $2 = human label, rest = command (as a single string via eval)
	local id="$1" label="$2"; shift 2
	local cmd="$*"
	echo
	echo "=============================="
	echo "[$id] $label"
	echo "=============================="
	echo "\$ $cmd"
	eval "$cmd"
	local rc=$?
	echo "exit=$rc"
	RESULTS+=("$id exit=$rc  $label")
	return $rc
}

expect() {
	# $1 = test id, $2 = condition (as string, evaluated with eval), $3 = description
	local id="$1" cond="$2" desc="$3"
	if eval "$cond"; then
		echo "  [PASS] $id — $desc"
		PASS=$((PASS+1))
	else
		echo "  [FAIL] $id — $desc"
		FAIL=$((FAIL+1))
	fi
}

should_run() {
	local id="$1"
	if [ $# -lt 1 ] || [ ${#SELECTED[@]} -eq 0 ]; then return 0; fi
	for s in "${SELECTED[@]}"; do
		[ "$s" = "$id" ] && return 0
	done
	return 1
}

# ---------- CLI ----------

SELECTED=("$@")

# ---------- Build ----------

echo "[setup] make fclean && make"
# make fclean >/dev/null 2>&1
# make >/dev/null || { echo "[setup] build failed"; exit 2; }

# ---------- Fixtures ----------

echo "[setup] creating fixtures"
printf "hello world\nfoo bar\nbaz\n" > /tmp/px_in
printf "line1\nline2\n"              > /tmp/px_in_small
: > /tmp/px_empty
rm -f /tmp/px_noread
printf "x" > /tmp/px_noread && chmod 000 /tmp/px_noread
rm -f /tmp/px_out /tmp/px_ref

cleanup() {
	chmod 644 /tmp/px_noread 2>/dev/null
	chmod 755 /tmp/px_out_nodir_ro 2>/dev/null
	chmod 644 /tmp/px_in_w /tmp/px_in_none 2>/dev/null
	rm -f  /tmp/px_in /tmp/px_in_small /tmp/px_empty /tmp/px_noread \
	       /tmp/px_out /tmp/px_ref /tmp/px_err_pipex /tmp/px_err_shell \
	       /tmp/px_in_big /tmp/px_out_rw /tmp/px_out_r
	rm -rf /tmp/px_in_rw /tmp/px_in_r /tmp/px_in_w /tmp/px_in_none /tmp/px_in_dir \
	       /tmp/px_out_dir /tmp/px_out_nodir_ro
}
trap cleanup EXIT

# ============================================================
# T1 — Happy path: cat | wc -w
# ============================================================
if should_run T1; then
	run_case T1 "cat | wc -w" "$PIPEX /tmp/px_in \"cat\" \"wc -w\" /tmp/px_out"
	expect T1 '[ "$(tr -d " " < /tmp/px_out)" = "5" ]' "outfile == 5"
fi

# ============================================================
# T2 — grep a | wc -l (subject example)
# ============================================================
if should_run T2; then
	run_case T2 "grep a | wc -l" "$PIPEX /tmp/px_in \"grep a\" \"wc -l\" /tmp/px_out"
	expect T2 '[ "$(tr -d " " < /tmp/px_out)" = "2" ]' "outfile == 2"
fi

# ============================================================
# T3 — Absolute path commands
# ============================================================
if should_run T3; then
	run_case T3 "/bin/cat | /usr/bin/wc -c" "$PIPEX /tmp/px_in \"/bin/cat\" \"/usr/bin/wc -c\" /tmp/px_out"
	bash -c '< /tmp/px_in /bin/cat | /usr/bin/wc -c > /tmp/px_ref'
	expect T3 'diff -q /tmp/px_out /tmp/px_ref >/dev/null' "byte-identical to shell"
fi

# ============================================================
# T4 — Nonexistent infile
# ============================================================
if should_run T4; then
	run_case T4 "nonexistent infile" "$PIPEX /tmp/does_not_exist \"cat\" \"wc -l\" /tmp/px_out 2>/tmp/px_err_pipex"
	expect T4 'grep -q "No such file" /tmp/px_err_pipex' "stderr reports No such file"
fi

# ============================================================
# T5 — Infile no-read
# ============================================================
if should_run T5; then
	run_case T5 "infile no-read" "$PIPEX /tmp/px_noread \"cat\" \"wc -l\" /tmp/px_out 2>/tmp/px_err_pipex"
	expect T5 'grep -q "Permission denied" /tmp/px_err_pipex' "stderr reports Permission denied"
fi

# ============================================================
# T6 — Outfile in non-writable dir
# ============================================================
if should_run T6; then
	run_case T6 "outfile bad dir" "$PIPEX /tmp/px_in \"cat\" \"wc -l\" /root/nope/px_out 2>/tmp/px_err_pipex"
	expect T6 'grep -Eq "Permission denied|No such file" /tmp/px_err_pipex' "stderr class OK"
fi

# ============================================================
# T7 — cmd1 not found
# ============================================================
if should_run T7; then
	run_case T7 "cmd1 not found" "$PIPEX /tmp/px_in \"nonexistentcmd\" \"wc -l\" /tmp/px_out 2>/tmp/px_err_pipex"
	expect T7 'grep -Eq "command not found|No such file" /tmp/px_err_pipex' "stderr says not found"
fi

# ============================================================
# T8 — cmd2 not found
# ============================================================
if should_run T8; then
	run_case T8 "cmd2 not found" "$PIPEX /tmp/px_in \"cat\" \"nonexistentcmd\" /tmp/px_out 2>/tmp/px_err_pipex; echo \$? >/tmp/px_rc"
	expect T8 '[ "$(cat /tmp/px_rc)" = "127" ]' "exit == 127"
fi

# ============================================================
# T9 — cmd2 found but not executable
# ============================================================
if should_run T9; then
	run_case T9 "cmd2 not executable" "$PIPEX /tmp/px_in \"cat\" \"/etc/hosts\" /tmp/px_out 2>/tmp/px_err_pipex; echo \$? >/tmp/px_rc"
	expect T9 '[ "$(cat /tmp/px_rc)" = "126" ]' "exit == 126"
fi

# ============================================================
# T10 — Empty cmd1
# ============================================================
if should_run T10; then
	run_case T10 "empty cmd1" "$PIPEX /tmp/px_in \"\" \"wc -l\" /tmp/px_out 2>/tmp/px_err_pipex"
	expect T10 '! grep -q "execve child" /tmp/px_err_pipex' "no internal label leaked"
fi

# ============================================================
# T11 — Empty cmd2
# ============================================================
if should_run T11; then
	run_case T11 "empty cmd2" "$PIPEX /tmp/px_in \"cat\" \"\" /tmp/px_out 2>/tmp/px_err_pipex; echo \$? >/tmp/px_rc"
	expect T11 '[ "$(cat /tmp/px_rc)" = "127" ]' "exit == 127"
fi

# ============================================================
# T12 — Space-only cmd
# ============================================================
if should_run T12; then
	run_case T12 "space-only cmd" "$PIPEX /tmp/px_in \" \" \"wc -l\" /tmp/px_out 2>/tmp/px_err_pipex"
	expect T12 '! grep -q "execve child" /tmp/px_err_pipex' "no internal label leaked"
fi

# ============================================================
# T13 — PATH unset (env -i)
# ============================================================
if should_run T13; then
	run_case T13 "PATH unset (env -i)" "env -i $PIPEX /tmp/px_in \"ls\" \"wc -l\" /tmp/px_out 2>/tmp/px_err_pipex; echo \$? >/tmp/px_rc"
	expect T13 '! grep -q "Envp not found" /tmp/px_err_pipex' "no Envp internal label"
	expect T13 '[ "$(cat /tmp/px_rc)" = "127" ]' "exit == 127"
fi

# ============================================================
# T14 — Empty PATH
# ============================================================
if should_run T14; then
	run_case T14 "empty PATH" "PATH= $PIPEX /tmp/px_in \"ls\" \"wc -l\" /tmp/px_out 2>/tmp/px_err_pipex; echo \$? >/tmp/px_rc"
	expect T14 '! grep -q "Invalid argument" /tmp/px_err_pipex' "no stale errno leak"
fi

# ============================================================
# T15 — /dev/null infile
# ============================================================
if should_run T15; then
	run_case T15 "/dev/null infile" "$PIPEX /dev/null \"cat\" \"wc -c\" /tmp/px_out"
	expect T15 '[ "$(tr -d " " < /tmp/px_out)" = "0" ]' "outfile == 0"
fi

# ============================================================
# T16 — Outfile truncation
# ============================================================
if should_run T16; then
	printf "old content that is longer\n" > /tmp/px_out
	run_case T16 "outfile truncation" "$PIPEX /tmp/px_in \"cat\" \"wc -c\" /tmp/px_out"
	expect T16 '[ "$(wc -c < /tmp/px_out | tr -d " ")" -lt 10 ]' "outfile truncated"
fi

# ============================================================
# T17 — Wrong argc (0 / 3 / 5)
# ============================================================
if should_run T17; then
	for argv in '' 'a b c' 'a b c d e'; do
		run_case T17 "argc variant: [$argv]" "$PIPEX $argv 2>/tmp/px_err_pipex; echo \$? >/tmp/px_rc"
		expect T17 '[ "$(cat /tmp/px_rc)" = "1" ]' "exit == 1 for [$argv]"
		expect T17 '! grep -q "Undefined error" /tmp/px_err_pipex' "no garbage errno for [$argv]"
	done
fi

# ============================================================
# T18 — Memory leaks
# ============================================================
if should_run T18; then
	if [ "$OS" = "Darwin" ]; then
		run_case T18 "leaks --atExit (macOS)" "MallocStackLogging=1 leaks --atExit -- $PIPEX /tmp/px_in \"cat\" \"wc -l\" /tmp/px_out 2>&1 | tail -20 | tee /tmp/px_err_pipex"
		expect T18 'grep -Eq "0 leaks for 0 total leaked bytes|0 bytes" /tmp/px_err_pipex' "no leaked bytes"
	else
		run_case T18 "valgrind (Linux)" "valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes --track-fds=yes --error-exitcode=99 $PIPEX /tmp/px_in \"cat\" \"wc -l\" /tmp/px_out 2>/tmp/px_err_pipex; echo \$? >/tmp/px_rc"
		expect T18 '[ "$(cat /tmp/px_rc)" != "99" ]' "valgrind clean"
	fi
fi

# ============================================================
# T19 — Shell parity diff (happy path)
# ============================================================
if should_run T19; then
	run_case T19 "shell parity diff" "$PIPEX /tmp/px_in \"grep o\" \"wc -c\" /tmp/px_out"
	bash -c '< /tmp/px_in grep o | wc -c > /tmp/px_ref'
	expect T19 'diff -q /tmp/px_out /tmp/px_ref >/dev/null' "byte-identical to shell"
fi

# ============================================================
# T20 — Stress (100k lines)
# ============================================================
if should_run T20; then
	yes "the quick brown fox" | head -n 100000 > /tmp/px_in_big
	run_case T20 "100k lines stress" "$PIPEX /tmp/px_in_big \"cat\" \"wc -l\" /tmp/px_out"
	expect T20 '[ "$(tr -d " " < /tmp/px_out)" = "100000" ]' "outfile == 100000"
fi

# ============================================================
# T21 — Infile permission matrix
# ============================================================
if should_run T21; then
	printf "content\n" > /tmp/px_in_rw   && chmod 644 /tmp/px_in_rw
	printf "content\n" > /tmp/px_in_r    && chmod 444 /tmp/px_in_r
	printf "content\n" > /tmp/px_in_w    && chmod 200 /tmp/px_in_w
	printf "content\n" > /tmp/px_in_none && chmod 000 /tmp/px_in_none
	mkdir -p /tmp/px_in_dir
	for f in /tmp/px_in_rw /tmp/px_in_r /tmp/px_in_w /tmp/px_in_none /tmp/px_in_dir; do
		run_case T21 "infile=$f" "$PIPEX \"$f\" \"cat\" \"wc -l\" /tmp/px_out 2>/tmp/px_err_pipex; echo \$? >/tmp/px_rc"
		bash -c "< '$f' cat | wc -l > /tmp/px_ref" 2>/tmp/px_err_shell
		shell_rc=$?
		expect T21 "[ \"\$(cat /tmp/px_rc)\" = \"$shell_rc\" ]" "exit parity for $f"
		expect T21 '! grep -q "^infile:" /tmp/px_err_pipex' "no internal label for $f"
	done
	chmod 644 /tmp/px_in_w /tmp/px_in_none 2>/dev/null
fi

# ============================================================
# T22 — Outfile permission matrix
# ============================================================
if should_run T22; then
	printf "" > /tmp/px_out_rw && chmod 644 /tmp/px_out_rw
	printf "" > /tmp/px_out_r  && chmod 444 /tmp/px_out_r
	mkdir -p /tmp/px_out_dir
	mkdir -p /tmp/px_out_nodir_ro && chmod 555 /tmp/px_out_nodir_ro
	for f in /tmp/px_out_rw /tmp/px_out_r /tmp/px_out_dir /tmp/px_out_nodir_ro/newfile; do
		run_case T22 "outfile=$f" "$PIPEX /tmp/px_in \"cat\" \"wc -l\" \"$f\" 2>/tmp/px_err_pipex; echo \$? >/tmp/px_rc"
		bash -c "< /tmp/px_in cat | wc -l > '$f'" 2>/tmp/px_err_shell
		shell_rc=$?
		expect T22 "[ \"\$(cat /tmp/px_rc)\" = \"$shell_rc\" ]" "exit parity for $f"
		expect T22 '! grep -q "Bad file descriptor" /tmp/px_err_pipex' "no EBADF leak for $f"
	done
	chmod 755 /tmp/px_out_nodir_ro 2>/dev/null
fi

# ============================================================
# T23 — Wrong-args error parity with shell
# ============================================================
if should_run T23; then
	# argc=0..3
	for argv in '' 'a' 'a b' 'a b c'; do
		run_case T23 "argc:[$argv]" "$PIPEX $argv 2>/tmp/px_err_pipex; echo \$? >/tmp/px_rc"
		expect T23 '[ "$(cat /tmp/px_rc)" = "1" ]' "exit==1 for [$argv]"
		expect T23 '! grep -q "Undefined error" /tmp/px_err_pipex' "no garbage errno for [$argv]"
	done
	# argc=5 edge-case argv slots
	run_case T23 'empty infile path'  "$PIPEX \"\" \"cat\" \"wc\" /tmp/px_out 2>/tmp/px_err_pipex"
	run_case T23 'empty outfile path' "$PIPEX /tmp/px_in \"cat\" \"wc\" \"\" 2>/tmp/px_err_pipex"
	run_case T23 'empty cmd1'         "$PIPEX /tmp/px_in \"\" \"wc -l\" /tmp/px_out 2>/tmp/px_err_pipex"
	run_case T23 'empty cmd2'         "$PIPEX /tmp/px_in \"cat\" \"\" /tmp/px_out 2>/tmp/px_err_pipex"
	run_case T23 'whitespace cmds'    "$PIPEX /tmp/px_in \"   \" \"   \" /tmp/px_out 2>/tmp/px_err_pipex"
	expect T23 '! grep -Eq "execve child|cmd alloc fail|split failed|Envp not found" /tmp/px_err_pipex' "no internal labels on last case"
fi

# ---------- Summary ----------

echo
echo "=============================="
echo "SUMMARY"
echo "=============================="
for r in "${RESULTS[@]}"; do echo "  $r"; done
echo "------------------------------"
echo "  PASS: $PASS"
echo "  FAIL: $FAIL"
[ "$FAIL" -eq 0 ]
