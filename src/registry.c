#include "registry.h"

bool mock_registry_add(mock_registry *registry, const char *name, mock_reset_fn reset) {
    if(registry->count >= MOLTEST_MOCK_MAX) {
        registry->dropped++;
        return false;
    }
    registry->entries[registry->count++] = (mock_entry){.name = name, .reset = reset};
    return true;
}

void mock_registry_reset(const mock_registry *registry) {
    for(size_t i = 0; i < registry->count; i++)
        registry->entries[i].reset();
}
