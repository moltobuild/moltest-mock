#include <moltest.h>

#include "../src/registry.h"

/* The registry behind the reset (spec 001 AC10). */

static int resets;

static void count_reset(void) {
    resets++;
}

static mock_registry registry;

DESCRIBE(registry_resets_every_mock_added) {
    registry = (mock_registry){0};
    resets = 0;
    ASSERT_TRUE(mock_registry_add(&registry, "a", count_reset));
    ASSERT_TRUE(mock_registry_add(&registry, "b", count_reset));
    mock_registry_reset(&registry);
    EXPECT_EQ(2, resets);
}

DESCRIBE(registry_refuses_mocks_past_the_limit) {
    registry = (mock_registry){0};
    for(int i = 0; i < MOLTEST_MOCK_MAX; i++)
        ASSERT_TRUE(mock_registry_add(&registry, "m", count_reset));
    EXPECT_FALSE(mock_registry_add(&registry, "over", count_reset));
    EXPECT_EQ(MOLTEST_MOCK_MAX, (int)registry.count);
    EXPECT_EQ(1, (int)registry.dropped);
}
