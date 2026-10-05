#include "shared_mocks.h"

#include <moltest.h>

/* MOCK_DECLARE_* / MOCK_DEFINE_* share a mock across files (spec 001 AC9). */

MOCK_DEFINE_VALUE_FUNC(int, shared_lookup, int);
MOCK_DEFINE_VOID_FUNC(shared_notify, const char *);

DESCRIBE(a_shared_mock_works_where_it_is_defined) {
    shared_lookup_mock.return_val = 4;
    EXPECT_EQ(4, shared_lookup(1));
    shared_notify("a");
    EXPECT_EQ(1, (int)shared_notify_mock.call_count);
}
