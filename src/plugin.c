#include "moltest_api.h"
#include "registry.h"

#include <stdio.h>

/*
 * The plugin: a moltest reporter registered when the package is linked
 * (ADR 0001). Before each test, and so before its BEFORE_EACH, every mock goes
 * back to zero: a test never sees the calls or the return values of another.
 *
 * Every mock registers itself from a constructor, which references this file,
 * so linking a single mock is enough to bring the reporter in.
 */

static mock_registry registry;

void moltest_mock_register(const char *name, void (*reset)(void)) {
    (void)mock_registry_add(&registry, name, reset);
}

void moltest_mock_reset_all(void) { mock_registry_reset(&registry); }

/* A mock that is not reset leaks state between tests; a red run that says why
   is better than tests that pass or fail depending on their order. */
static void on_run_start(size_t files, size_t tests, void *ctx) {
    (void)files, (void)tests, (void)ctx;
    if(registry.dropped == 0)
        return;
    char reason[256];
    (void)snprintf(reason, sizeof reason,
                   "moltest_mock: %zu mocks over the limit of %d (MOLTEST_MOCK_MAX) would not be "
                   "reset between tests",
                   registry.dropped, MOLTEST_MOCK_MAX);
    moltest_fail_run(reason);
}

static void on_test_start(const char *file, const char *name, void *ctx) {
    (void)file, (void)name, (void)ctx;
    mock_registry_reset(&registry);
}

static const mock_moltest_reporter reporter = {
    .api_version = MOCK_MOLTEST_REPORTER_API,
    .name = "moltest_mock",
    .on_run_start = on_run_start,
    .on_test_start = on_test_start,
};

__attribute__((constructor)) static void register_plugin(void) { moltest_add_reporter(&reporter); }
