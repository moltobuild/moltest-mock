#ifndef MOLTEST_MOCK_REGISTRY_H
#define MOLTEST_MOCK_REGISTRY_H

#include <moltest_mock.h>

#include <stdbool.h>
#include <stddef.h>

/* The mocks of a test binary, so that the plugin can reset them all before
   each test (ADR 0001). Pure: the plugin owns the one instance. */

typedef void (*mock_reset_fn)(void);

typedef struct {
    const char *name;
    mock_reset_fn reset;
} mock_entry;

typedef struct {
    mock_entry entries[MOLTEST_MOCK_MAX];
    size_t count;
    size_t dropped; /* mocks refused because the registry was full */
} mock_registry;

/* Add a mock; false, and `dropped` counts it, when the registry is full. */
bool mock_registry_add(mock_registry *registry, const char *name, mock_reset_fn reset);

/* Call the reset function of every mock added. */
void mock_registry_reset(const mock_registry *registry);

#endif /* MOLTEST_MOCK_REGISTRY_H */
