#include "shared_mocks.h"

#include <moltest.h>

/* The other half of spec 001 AC9: only the declaration is seen here. */

DESCRIBE(a_shared_mock_works_where_it_is_only_declared) {
    shared_lookup_mock.return_val = 9;
    EXPECT_EQ(9, shared_lookup(2));
    EXPECT_EQ(2, shared_lookup_mock.arg0_val);
    shared_notify("b");
    EXPECT_STREQ("b", shared_notify_mock.arg0_val);
}
