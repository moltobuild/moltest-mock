#!/bin/sh
#
# moltest-mock as a user meets it: a library from `molto new` (C17, molto's
# default), this checkout as a development dependency, and code in src/ that
# calls two functions of "another library" which only the mocks define.
#
#   1. the suite passes: return values, recorded arguments, and a reset
#      between tests whatever their order                  (spec 001 AC1, AC2, AC6, AC8)
#   2. a wrong expectation on a mock fails the run         (the mock reaches moltest)
#
# Also the regression test of KI-2: before the fix this did not compile in C17.
#
#   e2e.sh <moltest-mock checkout>
set -eu

checkout=$1
fail() { echo "e2e: $*" >&2; exit 1; }

cd "$(mktemp -d)"
molto new e2e_lib
cd e2e_lib
molto add moltest_mock --dev --path "$checkout"
grep -q 'std = "c17"' Project.toml || fail "molto new no longer defaults to C17; update this script"

cat >> include/e2e_lib.h <<'C'

/* Defined by another library, not this one: the tests mock them. */
int e2e_clock_now(void);
void e2e_log(int level, const char *text);

/* Seconds since `start`, logged. */
int e2e_lib_elapsed(int start);
C
cat >> src/e2e_lib.c <<'C'

int e2e_lib_elapsed(int start) {
    const int elapsed = e2e_clock_now() - start;
    e2e_log(1, "elapsed");
    return elapsed;
}
C
cat > tests/test_mock.c <<'C'
#include <moltest.h>
#include <moltest_mock.h>
#include <e2e_lib.h>

MOCK_VALUE_FUNC(int, e2e_clock_now);
MOCK_VOID_FUNC(e2e_log, int, const char *);

/* The same test twice: whichever runs second needs the reset. */
DESCRIBE(elapsed_reads_the_clock_and_logs_a) {
    EXPECT_EQ(0, (int)e2e_clock_now_mock.call_count);
    e2e_clock_now_mock.return_val = 100;
    EXPECT_EQ(40, e2e_lib_elapsed(60));
    EXPECT_EQ(1, e2e_log_mock.arg0_val);
    EXPECT_STREQ("elapsed", e2e_log_mock.arg1_val);
}

DESCRIBE(elapsed_reads_the_clock_and_logs_b) {
    EXPECT_EQ(0, (int)e2e_clock_now_mock.call_count);
    e2e_clock_now_mock.return_val = 100;
    EXPECT_EQ(40, e2e_lib_elapsed(60));
    EXPECT_EQ(1, (int)e2e_log_mock.call_count);
}
C

echo "--- 1. the suite passes"
molto test > run1.txt 2>&1 || { cat run1.txt; fail "the suite with mocks failed"; }
cat run1.txt
grep -q "3 passed" run1.txt || fail "expected 3 passing tests"

echo "--- 2. a wrong expectation fails the run"
cat >> tests/test_mock.c <<'C'

DESCRIBE(a_wrong_expectation) {
    e2e_clock_now_mock.return_val = 5;
    EXPECT_EQ(2, (int)e2e_clock_now_mock.call_count);
    (void)e2e_lib_elapsed(0);
}
C
if molto test > run2.txt 2>&1; then cat run2.txt; fail "a failing mock expectation passed"; fi
grep -q "e2e_clock_now_mock.call_count" run2.txt || { cat run2.txt; fail "the failure does not name the mock"; }

echo "e2e: ok"
