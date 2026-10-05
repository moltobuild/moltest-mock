#include <moltest.h>
#include <moltest_mock.h>

/* MOCK_VALUE_FUNC (spec 001 AC1-AC5, AC7). */

MOCK_VALUE_FUNC(int, read_file, const char *);
MOCK_VALUE_FUNC(int, now);
MOCK_VALUE_FUNC(long, six, int, int, int, int, int, long);
MOCK_VALUE_FUNC(int, seven, int, int, int, int, int, int, int);
MOCK_VALUE_FUNC(long, twelve, int, int, int, int, int, int, int, int, int, int, int, long);

/* The code under test: calls read_file across a boundary it does not own. */
static int load_config(const char *path) {
    return read_file(path) == 0 ? 1 : -1;
}

DESCRIBE(returns_zero_until_a_test_says_otherwise) {
    EXPECT_EQ(0, read_file("a"));
    EXPECT_EQ(0, now());
}

DESCRIBE(returns_return_val) {
    read_file_mock.return_val = -2;
    EXPECT_EQ(-1, load_config("x.toml"));
    EXPECT_EQ(-2, read_file("y"));
}

DESCRIBE(records_the_arguments_of_every_call) {
    load_config("a.toml");
    load_config("b.toml");
    EXPECT_EQ(2, (int)read_file_mock.call_count);
    EXPECT_STREQ("b.toml", read_file_mock.arg0_val);
    EXPECT_STREQ("a.toml", read_file_mock.arg0_history[0]);
    EXPECT_STREQ("b.toml", read_file_mock.arg0_history[1]);
    EXPECT_EQ(0, (int)read_file_mock.history_dropped);
}

DESCRIBE(calls_past_the_history_are_counted_but_not_kept) {
    for(int i = 0; i < MOLTEST_MOCK_HISTORY + 3; i++)
        (void)read_file(i == MOLTEST_MOCK_HISTORY + 2 ? "last" : "early");
    EXPECT_EQ(MOLTEST_MOCK_HISTORY + 3, (int)read_file_mock.call_count);
    EXPECT_EQ(3, (int)read_file_mock.history_dropped);
    EXPECT_STREQ("last", read_file_mock.arg0_val);
    EXPECT_STREQ("early", read_file_mock.arg0_history[MOLTEST_MOCK_HISTORY - 1]);
}

DESCRIBE(return_seq_gives_one_value_per_call_then_repeats_the_last) {
    int values[] = {10, 20, 30};
    now_mock.return_seq = values;
    now_mock.return_seq_len = 3;
    now_mock.return_val = 99;
    EXPECT_EQ(10, now());
    EXPECT_EQ(20, now());
    EXPECT_EQ(30, now());
    EXPECT_EQ(30, now());
}

static int fake_read_file(const char *path) {
    return path[0] == 'o' ? 0 : 7;
}

DESCRIBE(custom_fake_wins_over_return_values) {
    int values[] = {5};
    read_file_mock.return_seq = values;
    read_file_mock.return_seq_len = 1;
    read_file_mock.return_val = 5;
    read_file_mock.custom_fake = fake_read_file;
    EXPECT_EQ(0, read_file("ok"));
    EXPECT_EQ(7, read_file("no"));
    EXPECT_EQ(2, (int)read_file_mock.call_count);
    EXPECT_STREQ("no", read_file_mock.arg0_val);
}

DESCRIBE(mocks_take_zero_to_six_arguments) {
    now_mock.return_val = 42;
    six_mock.return_val = 6;
    EXPECT_EQ(42, now());
    EXPECT_EQ(6, (int)six(1, 2, 3, 4, 5, 60L));
    EXPECT_EQ(1, (int)now_mock.call_count);
    EXPECT_EQ(1, six_mock.arg0_val);
    EXPECT_EQ(5, six_mock.arg4_val);
    EXPECT_EQ(60, (int)six_mock.arg5_history[0]);
}

DESCRIBE(value_mocks_take_up_to_twelve_arguments) {
    /* spec 002 AC1, AC4: source_fetch in molto takes eight. */
    EXPECT_EQ(12, MOLTEST_MOCK_ARGS_MAX);
    seven_mock.return_val = 7;
    twelve_mock.return_val = 12;
    EXPECT_EQ(7, seven(1, 2, 3, 4, 5, 6, 70));
    EXPECT_EQ(12, (int)twelve(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 120L));
    EXPECT_EQ(70, seven_mock.arg6_val);
    EXPECT_EQ(7, twelve_mock.arg6_val);
    EXPECT_EQ(9, twelve_mock.arg8_val);
    EXPECT_EQ(11, twelve_mock.arg10_val);
    EXPECT_EQ(120, (int)twelve_mock.arg11_val);
    EXPECT_EQ(120, (int)twelve_mock.arg11_history[0]);
    EXPECT_EQ(1, (int)twelve_mock.call_count);
}
