#!/bin/sh
#
# moltest-mock as a user meets it: a library from `molto new` (C17, molto's
# default), this checkout as a development dependency, and code in src/ that
# calls two functions of "another library" which only the mocks define.
#
#   1. the suite passes: return values, recorded arguments, and a reset
#      between tests whatever their order                  (spec 001 AC1, AC2, AC6, AC8)
#   2. a wrong expectation on a mock fails the run         (the mock reaches moltest)
#   3. a mock of a function a real dependency defines: a duplicate symbol on
#      its own, a passing run once the test declares what it replaces (KI-3)
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
# Use the same released runner as the plugin dependency.
molto add "git+https://github.com/moltobuild/moltest#${MOLTEST_REF:-v0.4.0}" --dev
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

echo "--- 3. a mock of a real dependency (KI-3)"
# Steps 1 and 2 mock functions nobody defines. A real [deps] entry is compiled
# from source into the test binary, so its definition and the mock collide
# until the test says it replaces that source: molto then links it alone,
# without it (RFC-0021, molto 0.52.0).
sed -i.bak '/^DESCRIBE(a_wrong_expectation)/,$d' tests/test_mock.c && rm -f tests/test_mock.c.bak
mkdir -p ../e2e_clock/src ../e2e_clock/include
cat > ../e2e_clock/include/e2e_clock.h <<'C'
int e2e_clock_real(void);
C
cat > ../e2e_clock/src/e2e_clock.c <<'C'
int e2e_clock_real(void) { return 1; }
C
cat > ../e2e_clock/recipe.toml <<'T'
schema = 1
form = "source"
kind = "package"
name = "e2e_clock"
version = "0.1.0"
target = "any"

[artifacts]
type = "source"
std = "c17"
sources = ["src/e2e_clock.c"]
include = ["include"]
T
molto add e2e_clock --path ../e2e_clock
cat >> src/e2e_lib.c <<'C'

#include <e2e_clock.h>
int e2e_lib_real(void) { return e2e_clock_real() + 1; }
C
cat > tests/test_real_dep.c <<'C'
#include <moltest.h>
#include <moltest_mock.h>

int e2e_lib_real(void);
MOCK_VALUE_FUNC(int, e2e_clock_real);
/* Never called here, and needed all the same: this test links alone, and the
   linker takes src/e2e_lib.c whole, whose e2e_lib_elapsed() calls these two.
   In the shared suite tests/test_mock.c defines them; this binary does not
   have that file. */
MOCK_VALUE_FUNC(int, e2e_clock_now);
MOCK_VOID_FUNC(e2e_log, int, const char *);

DESCRIBE(a_dependency_function_is_mocked) {
    e2e_clock_real_mock.return_val = 41;
    EXPECT_EQ(42, e2e_lib_real());
}
C
if molto test > run3.txt 2>&1; then
    cat run3.txt
    fail "a mock of a real dependency linked without [[test.isolated]]"
fi
grep -qE "duplicate symbol|multiple definition" run3.txt ||
    { cat run3.txt; fail "the run failed, but not with a duplicate symbol"; }

cat >> Project.toml <<'TOML'

[[test.isolated]]
file = "tests/test_real_dep.c"
replaces = ["e2e_clock:src/e2e_clock.c"]
TOML
molto test > run3b.txt 2>&1 || { cat run3b.txt; fail "a test that replaces the dependency's source failed (KI-3)"; }
cat run3b.txt
grep -qE "test_real_dep(\.exe)? \.\.\. ok" run3b.txt || fail "the isolated test did not run on its own"

echo "e2e: ok"
