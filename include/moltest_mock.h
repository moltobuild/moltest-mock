#ifndef MOLTEST_MOCK_H
#define MOLTEST_MOCK_H

/*
 * moltest-mock: fake functions for C suites run by moltest (ADR 0002).
 *
 *     MOCK_VALUE_FUNC(int, read_file, const char *);
 *
 *     DESCRIBE(loads_config) {
 *         read_file_mock.return_val = 0;
 *         load_config("x.toml");
 *         EXPECT_EQ(1, (int)read_file_mock.call_count);
 *         EXPECT_STREQ("x.toml", read_file_mock.arg0_val);
 *     }
 *
 * MOCK_VALUE_FUNC defines read_file() itself, in the test binary, and a
 * `read_file_mock` struct that records every call and says what the next one
 * returns. The real read_file() must therefore not be linked into the same
 * test binary: mock what the code under test calls across a boundary (another
 * library, the system), not a function of the same object file.
 *
 * Every mock is reset to zero before each test, before its BEFORE_EACH, by the
 * reporter this package registers with moltest (ADR 0001). A test can also
 * call `read_file_mock_reset()` itself.
 *
 * What a mock records and returns, in order of precedence:
 *   custom_fake     a function with the same signature; called with the
 *                   arguments, and what it returns is returned
 *   return_seq      an array of `return_seq_len` values: call 1 returns
 *                   return_seq[0], call 2 return_seq[1], ... and the last one
 *                   is repeated once the array runs out
 *   return_val      returned otherwise; zero until a test sets it
 * and for each argument i (0-based):
 *   argI_val        the value of the last call
 *   argI_history[]  the value of each of the first MOLTEST_MOCK_HISTORY calls
 *   call_count      every call, including those past the history
 *   history_dropped how many calls did not fit in the history
 *
 * Arguments are stored as they are passed: a pointer is kept, not what it
 * points to. A type with a comma or a declarator around the name (a function
 * pointer, an array) needs a typedef first. Up to MOLTEST_MOCK_ARGS_MAX
 * arguments; variadic functions cannot be mocked.
 *
 * C11 or later. Under -Wpedantic before C23, a mock with no arguments,
 * MOCK_VALUE_FUNC(int, now), warns: C17 wants at least one argument for a
 * macro's `...`.
 *
 * A mock used by several test files is declared in a shared header with
 * MOCK_DECLARE_VALUE_FUNC / MOCK_DECLARE_VOID_FUNC and defined in one of them
 * with MOCK_DEFINE_VALUE_FUNC / MOCK_DEFINE_VOID_FUNC; MOCK_VALUE_FUNC and
 * MOCK_VOID_FUNC are both at once.
 */

#include <stddef.h>
#include <string.h>

/* Calls whose arguments a mock keeps. Define it for the whole test binary
   (in the build flags), never per file: every file must agree on the layout. */
#ifndef MOLTEST_MOCK_HISTORY
#define MOLTEST_MOCK_HISTORY 50
#endif

/* The most arguments a mocked function can take. */
#define MOLTEST_MOCK_ARGS_MAX 6

/* The most mocks a test binary can have; past it, the run fails (spec 001). */
#define MOLTEST_MOCK_MAX 512

#ifdef __cplusplus
extern "C" {
#endif

/* Called by every mock's constructor: from then on it is reset before each
   test. Not meant to be called by hand. */
void moltest_mock_register(const char *name, void (*reset)(void));

/* Reset every registered mock to zero. The plugin does this before each test. */
void moltest_mock_reset_all(void);

#ifdef __cplusplus
}
#endif

/* ------------------------------------------------------------------ */
/* Public macros                                                        */
/* ------------------------------------------------------------------ */

#define MOCK_VALUE_FUNC(ret, name, ...)                                                            \
    MOCK_DECLARE_VALUE_FUNC(ret, name __VA_OPT__(, ) __VA_ARGS__);                                 \
    MOCK_DEFINE_VALUE_FUNC(ret, name __VA_OPT__(, ) __VA_ARGS__)

#define MOCK_VOID_FUNC(name, ...)                                                                  \
    MOCK_DECLARE_VOID_FUNC(name __VA_OPT__(, ) __VA_ARGS__);                                       \
    MOCK_DEFINE_VOID_FUNC(name __VA_OPT__(, ) __VA_ARGS__)

#define MOCK_DECLARE_VALUE_FUNC(ret, name, ...)                                                    \
    MOLTEST_MOCK_APPLY_(MOLTEST_MOCK_DECLARE_VALUE_, MOLTEST_MOCK_NARG_(__VA_ARGS__), ret,         \
                        name __VA_OPT__(, ) __VA_ARGS__)

#define MOCK_DEFINE_VALUE_FUNC(ret, name, ...)                                                     \
    MOLTEST_MOCK_APPLY_(MOLTEST_MOCK_DEFINE_VALUE_, MOLTEST_MOCK_NARG_(__VA_ARGS__), ret,          \
                        name __VA_OPT__(, ) __VA_ARGS__)

#define MOCK_DECLARE_VOID_FUNC(name, ...)                                                          \
    MOLTEST_MOCK_APPLY_(MOLTEST_MOCK_DECLARE_VOID_, MOLTEST_MOCK_NARG_(__VA_ARGS__),               \
                        name __VA_OPT__(, ) __VA_ARGS__)

#define MOCK_DEFINE_VOID_FUNC(name, ...)                                                           \
    MOLTEST_MOCK_APPLY_(MOLTEST_MOCK_DEFINE_VOID_, MOLTEST_MOCK_NARG_(__VA_ARGS__),                \
                        name __VA_OPT__(, ) __VA_ARGS__)

/* ------------------------------------------------------------------ */
/* Plumbing                                                             */
/* ------------------------------------------------------------------ */

/* How many arguments, 0 to 6. `macro` is applied once `n` is a literal, so
   that it can paste it onto the per-arity helpers below. */
#define MOLTEST_MOCK_NARG_(...) MOLTEST_MOCK_NTH_(__VA_OPT__(__VA_ARGS__, ) 6, 5, 4, 3, 2, 1, 0)
#define MOLTEST_MOCK_NTH_(_1, _2, _3, _4, _5, _6, n, ...) n
#define MOLTEST_MOCK_APPLY_(macro, n, ...) macro(n, __VA_ARGS__)

/* The parameter list of the fake: `t0 moltest_mock_a0, t1 moltest_mock_a1` */
#define MOLTEST_MOCK_PARAMS_0() void
#define MOLTEST_MOCK_PARAMS_1(t0) t0 moltest_mock_a0
#define MOLTEST_MOCK_PARAMS_2(t0, t1) MOLTEST_MOCK_PARAMS_1(t0), t1 moltest_mock_a1
#define MOLTEST_MOCK_PARAMS_3(t0, t1, t2) MOLTEST_MOCK_PARAMS_2(t0, t1), t2 moltest_mock_a2
#define MOLTEST_MOCK_PARAMS_4(t0, t1, t2, t3) MOLTEST_MOCK_PARAMS_3(t0, t1, t2), t3 moltest_mock_a3
#define MOLTEST_MOCK_PARAMS_5(t0, t1, t2, t3, t4)                                                  \
    MOLTEST_MOCK_PARAMS_4(t0, t1, t2, t3), t4 moltest_mock_a4
#define MOLTEST_MOCK_PARAMS_6(t0, t1, t2, t3, t4, t5)                                              \
    MOLTEST_MOCK_PARAMS_5(t0, t1, t2, t3, t4), t5 moltest_mock_a5

/* The types alone, for custom_fake's signature. */
#define MOLTEST_MOCK_TYPES_0() void
#define MOLTEST_MOCK_TYPES_1(...) __VA_ARGS__
#define MOLTEST_MOCK_TYPES_2(...) __VA_ARGS__
#define MOLTEST_MOCK_TYPES_3(...) __VA_ARGS__
#define MOLTEST_MOCK_TYPES_4(...) __VA_ARGS__
#define MOLTEST_MOCK_TYPES_5(...) __VA_ARGS__
#define MOLTEST_MOCK_TYPES_6(...) __VA_ARGS__

/* The names alone, to pass the arguments on to custom_fake. */
#define MOLTEST_MOCK_ARGS_0()
#define MOLTEST_MOCK_ARGS_1() moltest_mock_a0
#define MOLTEST_MOCK_ARGS_2() MOLTEST_MOCK_ARGS_1(), moltest_mock_a1
#define MOLTEST_MOCK_ARGS_3() MOLTEST_MOCK_ARGS_2(), moltest_mock_a2
#define MOLTEST_MOCK_ARGS_4() MOLTEST_MOCK_ARGS_3(), moltest_mock_a3
#define MOLTEST_MOCK_ARGS_5() MOLTEST_MOCK_ARGS_4(), moltest_mock_a4
#define MOLTEST_MOCK_ARGS_6() MOLTEST_MOCK_ARGS_5(), moltest_mock_a5

/* The struct fields that record argument i. */
#define MOLTEST_MOCK_FIELD_(t, i)                                                                  \
    t arg##i##_val;                                                                                \
    t arg##i##_history[MOLTEST_MOCK_HISTORY];
#define MOLTEST_MOCK_FIELDS_0()
#define MOLTEST_MOCK_FIELDS_1(t0) MOLTEST_MOCK_FIELD_(t0, 0)
#define MOLTEST_MOCK_FIELDS_2(t0, t1) MOLTEST_MOCK_FIELDS_1(t0) MOLTEST_MOCK_FIELD_(t1, 1)
#define MOLTEST_MOCK_FIELDS_3(t0, t1, t2) MOLTEST_MOCK_FIELDS_2(t0, t1) MOLTEST_MOCK_FIELD_(t2, 2)
#define MOLTEST_MOCK_FIELDS_4(t0, t1, t2, t3)                                                      \
    MOLTEST_MOCK_FIELDS_3(t0, t1, t2) MOLTEST_MOCK_FIELD_(t3, 3)
#define MOLTEST_MOCK_FIELDS_5(t0, t1, t2, t3, t4)                                                  \
    MOLTEST_MOCK_FIELDS_4(t0, t1, t2, t3) MOLTEST_MOCK_FIELD_(t4, 4)
#define MOLTEST_MOCK_FIELDS_6(t0, t1, t2, t3, t4, t5)                                              \
    MOLTEST_MOCK_FIELDS_5(t0, t1, t2, t3, t4) MOLTEST_MOCK_FIELD_(t5, 5)

/* Store argument i of the current call into mock `m`. */
#define MOLTEST_MOCK_SAVE_(m, i)                                                                   \
    (m).arg##i##_val = moltest_mock_a##i;                                                          \
    if((m).call_count < MOLTEST_MOCK_HISTORY)                                                      \
        (m).arg##i##_history[(m).call_count] = moltest_mock_a##i;
#define MOLTEST_MOCK_SAVE_0(m)
#define MOLTEST_MOCK_SAVE_1(m) MOLTEST_MOCK_SAVE_(m, 0)
#define MOLTEST_MOCK_SAVE_2(m) MOLTEST_MOCK_SAVE_1(m) MOLTEST_MOCK_SAVE_(m, 1)
#define MOLTEST_MOCK_SAVE_3(m) MOLTEST_MOCK_SAVE_2(m) MOLTEST_MOCK_SAVE_(m, 2)
#define MOLTEST_MOCK_SAVE_4(m) MOLTEST_MOCK_SAVE_3(m) MOLTEST_MOCK_SAVE_(m, 3)
#define MOLTEST_MOCK_SAVE_5(m) MOLTEST_MOCK_SAVE_4(m) MOLTEST_MOCK_SAVE_(m, 4)
#define MOLTEST_MOCK_SAVE_6(m) MOLTEST_MOCK_SAVE_5(m) MOLTEST_MOCK_SAVE_(m, 5)

/* What every call does before it returns: record the arguments, count. */
#define MOLTEST_MOCK_RECORD_(n, name)                                                              \
    MOLTEST_MOCK_SAVE_##n(name##_mock) if(name##_mock.call_count >= MOLTEST_MOCK_HISTORY)          \
        name##_mock.history_dropped++;                                                             \
    name##_mock.call_count++;

/* A declaration to end a definition macro on, so that it takes the caller's
   semicolon. _Static_assert is C11 and needs no header; C23 and C++ spell it
   static_assert, which C17 only has through <assert.h>. */
#ifdef __cplusplus
#define MOLTEST_MOCK_END_(name) static_assert(1, #name)
#else
#define MOLTEST_MOCK_END_(name) _Static_assert(1, #name)
#endif

/* The reset function, and a constructor that registers it with the plugin. */
#define MOLTEST_MOCK_RESET_(name)                                                                  \
    void name##_mock_reset(void) { memset(&name##_mock, 0, sizeof name##_mock); }                  \
    __attribute__((constructor)) static void moltest_mock_register_##name(void) {                  \
        moltest_mock_register(#name, name##_mock_reset);                                           \
    }

#define MOLTEST_MOCK_DECLARE_VALUE_(n, ret, name, ...)                                             \
    typedef struct {                                                                               \
        size_t call_count;                                                                         \
        size_t history_dropped;                                                                    \
        MOLTEST_MOCK_FIELDS_##n(__VA_ARGS__) ret return_val;                                       \
        ret *return_seq;                                                                           \
        size_t return_seq_len;                                                                     \
        ret (*custom_fake)(MOLTEST_MOCK_TYPES_##n(__VA_ARGS__));                                   \
    } name##_mock_type;                                                                            \
    extern name##_mock_type name##_mock;                                                           \
    void name##_mock_reset(void);                                                                  \
    ret name(MOLTEST_MOCK_PARAMS_##n(__VA_ARGS__))

#define MOLTEST_MOCK_DEFINE_VALUE_(n, ret, name, ...)                                              \
    name##_mock_type name##_mock;                                                                  \
    MOLTEST_MOCK_RESET_(name)                                                                      \
    ret name(MOLTEST_MOCK_PARAMS_##n(__VA_ARGS__)) {                                               \
        MOLTEST_MOCK_RECORD_(n, name)                                                              \
        if(name##_mock.custom_fake != NULL)                                                        \
            return name##_mock.custom_fake(MOLTEST_MOCK_ARGS_##n());                               \
        if(name##_mock.return_seq != NULL && name##_mock.return_seq_len > 0) {                     \
            size_t moltest_mock_i = name##_mock.call_count - 1;                                    \
            if(moltest_mock_i >= name##_mock.return_seq_len)                                       \
                moltest_mock_i = name##_mock.return_seq_len - 1;                                   \
            return name##_mock.return_seq[moltest_mock_i];                                         \
        }                                                                                          \
        return name##_mock.return_val;                                                             \
    }                                                                                              \
    MOLTEST_MOCK_END_(name)

#define MOLTEST_MOCK_DECLARE_VOID_(n, name, ...)                                                   \
    typedef struct {                                                                               \
        size_t call_count;                                                                         \
        size_t history_dropped;                                                                    \
        MOLTEST_MOCK_FIELDS_##n(__VA_ARGS__) void (*custom_fake)(                                  \
            MOLTEST_MOCK_TYPES_##n(__VA_ARGS__));                                                  \
    } name##_mock_type;                                                                            \
    extern name##_mock_type name##_mock;                                                           \
    void name##_mock_reset(void);                                                                  \
    void name(MOLTEST_MOCK_PARAMS_##n(__VA_ARGS__))

#define MOLTEST_MOCK_DEFINE_VOID_(n, name, ...)                                                    \
    name##_mock_type name##_mock;                                                                  \
    MOLTEST_MOCK_RESET_(name)                                                                      \
    void name(MOLTEST_MOCK_PARAMS_##n(__VA_ARGS__)) {                                              \
        MOLTEST_MOCK_RECORD_(n, name)                                                              \
        if(name##_mock.custom_fake != NULL)                                                        \
            name##_mock.custom_fake(MOLTEST_MOCK_ARGS_##n());                                      \
    }                                                                                              \
    MOLTEST_MOCK_END_(name)

#endif /* MOLTEST_MOCK_H */
