#include <moltest.h>
#include <moltest_mock.h>

#include <stddef.h>
#include <string.h>

/* The README's dependency injection example, so that it keeps compiling and
   keeps meaning what it says. Production code and test are in one file here;
   in a project they are include/, src/ and tests/. */

typedef struct {
    int (*read_file)(const char *path, char *out, size_t size);
    int (*write_file)(const char *path, const char *text);
} fs_ops;

typedef struct {
    int verbose;
} config;

static int config_load(const fs_ops *fs, const char *path, config *out) {
    char text[64];
    if(fs->read_file(path, text, sizeof text) != 0)
        return -1;
    out->verbose = strcmp(text, "verbose") == 0;
    return 0;
}

/* Under names of their own: a real read_file may exist in the same binary. */
MOCK_VALUE_FUNC(int, fake_read_file, const char *, char *, size_t);
MOCK_VALUE_FUNC(int, fake_write_file, const char *, const char *);

static const fs_ops fake_fs = {.read_file = fake_read_file, .write_file = fake_write_file};

DESCRIBE(config_load_fails_when_the_file_cannot_be_read) {
    fake_read_file_mock.return_val = -1;
    config cfg;
    EXPECT_EQ(-1, config_load(&fake_fs, "app.toml", &cfg));
    EXPECT_STREQ("app.toml", fake_read_file_mock.arg0_val);
    EXPECT_EQ(0, (int)fake_write_file_mock.call_count);
}

static int read_verbose(const char *path, char *out, size_t size) {
    (void)path;
    strncpy(out, "verbose", size);
    return 0;
}

DESCRIBE(custom_fake_fills_the_buffer_a_real_read_would) {
    fake_read_file_mock.custom_fake = read_verbose;
    config cfg = {0};
    EXPECT_EQ(0, config_load(&fake_fs, "app.toml", &cfg));
    EXPECT_EQ(1, cfg.verbose);
    EXPECT_EQ(1, (int)fake_read_file_mock.call_count);
}
