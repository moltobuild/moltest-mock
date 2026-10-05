#include <moltest.h>
#include <moltest_mock.h>

/* Every mock is back to zero before each test, before BEFORE_EACH (spec 001 AC8).
   The first two tests do the same thing: whichever runs second fails unless
   the mock was reset in between, whatever order moltest picks. */

MOCK_VALUE_FUNC(int, open_socket, int);
MOCK_VALUE_FUNC(int, configured);

BEFORE_EACH() {
    configured_mock.return_val = 7;
}

DESCRIBE(state_from_another_test_is_gone_a) {
    EXPECT_EQ(0, (int)open_socket_mock.call_count);
    EXPECT_EQ(0, open_socket_mock.return_val);
    open_socket_mock.return_val = 3;
    EXPECT_EQ(3, open_socket(80));
}

DESCRIBE(state_from_another_test_is_gone_b) {
    EXPECT_EQ(0, (int)open_socket_mock.call_count);
    EXPECT_EQ(0, open_socket_mock.return_val);
    open_socket_mock.return_val = 3;
    EXPECT_EQ(3, open_socket(80));
}

DESCRIBE(before_each_runs_after_the_reset) {
    EXPECT_EQ(7, configured());
}

DESCRIBE(a_test_can_reset_one_mock_or_all_of_them) {
    (void)open_socket(1);
    open_socket_mock_reset();
    EXPECT_EQ(0, (int)open_socket_mock.call_count);
    (void)open_socket(1);
    moltest_mock_reset_all();
    EXPECT_EQ(0, (int)open_socket_mock.call_count);
    EXPECT_EQ(0, configured());
}
