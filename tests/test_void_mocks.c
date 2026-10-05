#include <moltest.h>
#include <moltest_mock.h>

/* MOCK_VOID_FUNC (spec 001 AC6). */

MOCK_VOID_FUNC(log_line, int, const char *);
MOCK_VOID_FUNC(flush);

static int seen_level;

static void fake_log_line(int level, const char *text) {
    (void)text;
    seen_level = level;
}

DESCRIBE(void_mock_records_and_calls_custom_fake) {
    seen_level = 0;
    log_line_mock.custom_fake = fake_log_line;
    log_line(3, "hello");
    flush();
    EXPECT_EQ(3, seen_level);
    EXPECT_EQ(1, (int)log_line_mock.call_count);
    EXPECT_EQ(3, log_line_mock.arg0_val);
    EXPECT_STREQ("hello", log_line_mock.arg1_history[0]);
    EXPECT_EQ(1, (int)flush_mock.call_count);
}
