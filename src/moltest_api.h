#ifndef MOLTEST_MOCK_MOLTEST_API_H
#define MOLTEST_MOCK_MOLTEST_API_H

/*
 * moltest's reporter API v1, as moltest 0.3.0's moltest.h defines it,
 * declared here instead of included.
 *
 * The plugin links against the consumer's own moltest: molto allows one
 * version of a package per build, so this package cannot bring its own, and
 * its recipe has no way to put the consumer's moltest.h on its include path.
 * If moltest's layout moves on, the copy below still says 1 and moltest
 * refuses the plugin by name before any test runs.
 *
 * tests/test_moltest_api.c checks every field against the real header. The
 * names are prefixed so that both can be seen in one translation unit; the
 * functions are declared only when moltest.h was not included first.
 */

#include <stddef.h>

#define MOCK_MOLTEST_REPORTER_API 1

typedef struct {
    size_t files;
    size_t tests;
    size_t passed;
    size_t failed;
    size_t skipped;
    size_t warned;
    size_t assertions;
    size_t warnings;
    double seconds;
} mock_moltest_summary;

typedef struct {
    int api_version;
    const char *name;
    void (*on_run_start)(size_t files, size_t tests, void *ctx);
    void (*on_file_start)(const char *file, void *ctx);
    void (*on_test_start)(const char *file, const char *name, void *ctx);
    void (*on_test_end)(const char *file, const char *name, int status, double seconds, void *ctx);
    void (*on_file_end)(const char *file, size_t done, size_t total, void *ctx);
    void (*on_run_end)(const mock_moltest_summary *summary, void *ctx);
    void *ctx;
} mock_moltest_reporter;

#ifndef MOLTEST_H
void moltest_add_reporter(const mock_moltest_reporter *reporter);
void moltest_fail_run(const char *reason);
#endif

#endif /* MOLTEST_MOCK_MOLTEST_API_H */
