#ifndef TEST_H
#define TEST_H

#include <setjmp.h>

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
    TestResult *result = nullptr;

    bool expect_fail;
    int checks_count;
};

extern TestSuite *current_test;

extern bool assert_handler(const char *src, int line, const char *sz_cond, bool cond, const char *fmt, ...);

extern void test_assert_handler(const char *src, int line, const char *condition);
extern void test_panic_handler(
    const char *src, int line,
    const char *condition,
    const char *fmt, ...);

extern int run_tests(TestSuite *tests, int count);

#define REPORT_ASSERT(cond) test_assert_handler(__FILE__, __LINE__, cond)
#define REPORT_PANIC(cond, ...) test_panic_handler(__FILE__, __LINE__, cond, __VA_ARGS__)


#define ASSERT(cond) do {\
    if (current_test) current_test->checks_count++;\
    if (!(cond)) {\
        if (!current_test->expect_fail) REPORT_ASSERT(#cond);\
        else { extern jmp_buf test_jmp; longjmp(test_jmp, 1); }\
    }\
} while(0)

#define PANIC(...) do {\
    if (current_test) current_test->checks_count++;\
    if (!current_test->expect_fail) REPORT_PANIC(nullptr, __VA_ARGS__);\
    else { extern jmp_buf test_jmp; longjmp(test_jmp, 1); }\
} while(0)

#define PANIC_IF(cond, ...) do {\
    if (current_test) current_test->checks_count++;\
    if ((cond)) {\
        if (!current_test->expect_fail) REPORT_PANIC(#cond, __VA_ARGS__);\
        else { extern jmp_buf test_jmp; longjmp(test_jmp, 1); }\
    }\
} while(0)

#define EXPECT_FAIL\
    for (int i = ((current_test->expect_fail=true), setjmp(test_jmp));\
         i == 0;\
         i = ((current_test->expect_fail=false),((i != 1) ? (REPORT_PANIC("expected failure", nullptr), -1) : 1)))

#endif // TEST_H


#ifdef TEST_H_IMPL
#undef TEST_H_IMPL

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <signal.h>
#include <setjmp.h>

#include <new>

jmp_buf test_jmp;
TestSuite *current_test = nullptr;

void test_assert_handler(const char *src, int line, const char *condition)
{
    TestResult *rep = new (malloc(sizeof *rep)) TestResult {
        .src = src, .line = line, .cond = condition,
        .next = current_test->result
    };

    current_test->result = rep;
}

void test_panic_handler(
    const char *src, int line,
    const char *condition,
    const char *fmt, ...)
{
    int length = 0;
    char msg[2048];

    if (fmt) {
        va_list args;
        va_start(args, fmt);

        length = vsnprintf(msg, sizeof msg-1, fmt, args);
        length = length > sizeof msg-1 ? sizeof msg-1 : length;

        va_end(args);
    };

    msg[length] = '\0';

    TestResult *rep = new (malloc(sizeof *rep + length+1)) TestResult {
        .src = src, .line = line, .cond = condition,
        .next = current_test->result
    };
    if (length > 0) {
        rep->msg = (char*)rep + sizeof *rep;
        strcpy(rep->msg, msg);
    }

    current_test->result = rep;
}

void test_sig_handler(int sig) {
    signal(sig, test_sig_handler);
    longjmp(test_jmp, 1);
}

int run_tests(TestSuite *tests, int count)
{
    signal(SIGINT, test_sig_handler);
    signal(SIGSEGV, test_sig_handler);

    int failed_tests = 0;
    for (int i = 0; i < count; i++) {
        current_test = &tests[i];

        if (setjmp(test_jmp) == 0) tests[i].proc();
        bool result = tests[i].result == nullptr;
        printf("%-80s : [%s] (%d checks)\n",\
               tests[i].name, \
               result ? "OK" : "ERROR",\
               tests[i].checks_count);

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

#endif // TEST_H_IMPL
