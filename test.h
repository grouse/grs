#ifndef TEST_H
#define TEST_H

#define CATEGORY(cat) __attribute__((annotate("category:" #cat)))
#define TEST_PROC(name, ...) void CAT3(name, _test, __COUNTER__)() __attribute__((annotate("test"))) __VA_ARGS__

// no-op EXPECT_FAIL macros to avoid compiling unnecessary code when included in translation units outside the test-suite
#define EXPECT_FAIL(...)

#endif // TEST_H

#ifdef TEST_H_IMPL
#undef TEST_H_IMPL

#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>

#include "core.h"

typedef void (*test_proc_t)();

struct TestResult {
    const char *src; int line;
    const char *cond;
    char *msg;

    struct TestResult *next;
};

struct TestSuite {
    const char *name;
    test_proc_t proc;

    TestSuite *children;
    int child_count;

    TestResult *result = nullptr;
};

extern TestSuite *test_current;
extern int test_expect_fail;

extern jmp_buf test_jmp_[2];
extern int test_jmp_i;
#define test_jmp test_jmp_[test_jmp_i]

extern int run_tests(TestSuite *tests, int count);
extern void report_fail(const char *file, int line, const char *sz_cond, const char *msg, ...);
extern void report_failv(const char *file, int line, const char *sz_cond, const char *msg, va_list va_args);


#define REPORT_FAIL(...) report_fail(__FILE__, __LINE__, __VA_ARGS__)

#undef EXPECT_FAIL
#define EXPECT_FAIL(body) do {\
    test_expect_fail=true;\
    test_jmp_i=1;\
    bool failed = false;\
    switch (setjmp(test_jmp)) {\
    case 1: failed=true; break;\
    case 0: { body; } break;\
    }\
    if (!failed) REPORT_FAIL(nullptr, "expected failure");\
    test_expect_fail=false;\
    test_jmp_i=0;\
} while(0)

jmp_buf test_jmp_[2];  int test_jmp_i = 0;

TestSuite *test_current = nullptr;
int test_expect_fail = 0;

void test_sig_handler(int sig) {
    signal(sig, test_sig_handler);
    longjmp(test_jmp, 1);
}

bool test_assert_handler(
    const char *file, int line,
    const char *sz_cond)
{
    if (!test_expect_fail) {
        report_fail(file, line, sz_cond, nullptr);
    } else longjmp(test_jmp, 1);

    return false;
}

bool test_panic_handler(
    const char *file, int line,
    const char *sz_cond,
    const char *msg, ...)
{
    if (!test_expect_fail) {
        va_list va_args;
        va_start(va_args, msg);
        report_failv(file, line, sz_cond, msg, va_args);
        va_end(va_args);
    } else longjmp(test_jmp, 1);

    return false;
}

void report_fail(const char *file, int line, const char *sz_cond, const char *msg, ...)
{
    va_list va_args;
    va_start(va_args, msg);
    report_failv(file, line, sz_cond, msg, va_args);
    va_end(va_args);
}

void report_failv(const char *file, int line, const char *sz_cond, const char *fmt, va_list va_args)
{
    int length = 0;
    char msg[2048];

    if (fmt) {
        length = vsnprintf(msg, sizeof msg-1, fmt, va_args);
        length = length > sizeof msg-1 ? sizeof msg-1 : length;
    };
    msg[length] = '\0';

    TestResult *rep = new (malloc(sizeof *rep + length+1)) TestResult {
        .src = file, .line = line, .cond = sz_cond,
        .next = test_current->result
    };

    if (length > 0) {
        rep->msg = (char*)rep + sizeof *rep;
        strcpy(rep->msg, msg);
    }

    test_current->result = rep;
}

static int run_tests_int(TestSuite *tests, int count, int depth = 0)
{
    int failed_tests = 0;
    for (int i = 0; i < count; i++) {
        test_current = &tests[i];

        if (tests[i].children && tests[i].child_count) {
            printf("/%s\n", tests[i].name);
            failed_tests += run_tests_int(tests[i].children, tests[i].child_count, depth+1);
        }

        if (!tests[i].proc) continue;

        if (setjmp(test_jmp) == 0) tests[i].proc();
        bool result = tests[i].result == nullptr;
        printf("%*s%-*s : [%s]\n", depth*2, "", 80-depth*2, tests[i].name, result ? "OK" : "ERROR");

        if (!result) {
            for (auto *res = tests[i].result; res; res = res->next) {
                if (res->cond && res->msg) {
                    printf("\t%s:%d check failed: %s: %s\n", res->src, res->line, res->cond, res->msg);
                } else if (res->cond) {
                    printf("\t%s:%d check failed: %s\n", res->src, res->line, res->cond);
                } else if (res->msg) {
                    printf("\t%s:%d test failed: %s\n", res->src, res->line, res->msg);
                } else {
                    printf("\t%s:%d test failed\n", res->src, res->line);
                }
            }
        }
    }

    return failed_tests;
}

int run_tests(TestSuite *tests, int count)
{
    signal(SIGINT, test_sig_handler);
    signal(SIGSEGV, test_sig_handler);

    extern assert_handler_t assert_handler;
    extern panic_handler_t panic_handler;

    assert_handler = test_assert_handler;
    panic_handler = test_panic_handler;

    printf("\nRunning %d test procedures\n", count);
    int failed_tests = run_tests_int(tests, count);
    return failed_tests;
}

#endif // TEST_H_IMPL
