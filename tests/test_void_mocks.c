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

MOCK_VOID_FUNC(report12, int, int, int, int, int, int, int, int, int, int, int, const char *);

static int sum_seen;
static const char *last_seen;

static void sum_twelve(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k,
                       const char *text) {
    sum_seen = a + b + c + d + e + f + g + h + i + j + k;
    last_seen = text;
}

DESCRIBE(void_mocks_take_twelve_arguments_into_custom_fake) {
    /* spec 002 AC2 */
    report12_mock.custom_fake = sum_twelve;
    report12(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, "done");
    EXPECT_EQ(66, sum_seen);
    EXPECT_STREQ("done", last_seen);
    EXPECT_EQ(1, (int)report12_mock.call_count);
    EXPECT_EQ(8, report12_mock.arg7_val);
    EXPECT_STREQ("done", report12_mock.arg11_val);
}
