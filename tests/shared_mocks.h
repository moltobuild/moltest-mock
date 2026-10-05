#ifndef SHARED_MOCKS_H
#define SHARED_MOCKS_H

#include <moltest_mock.h>

/* A mock two test files use: declared here, defined in test_shared_a.c. */
MOCK_DECLARE_VALUE_FUNC(int, shared_lookup, int);
MOCK_DECLARE_VOID_FUNC(shared_notify, const char *);

#endif
