#ifndef TEST_H
#define TEST_H

#define TEST_PROC(name, ...) void name() __attribute__((annotate("test"))) __VA_ARGS__

// no-op EXPECT_FAIL macros to avoid compiling unnecessary code when included in translation units outside the test-suite
#define EXPECT_FAIL(...)

#include "core.h"
#include <setjmp.h>
#include <signal.h>

struct TestResult {
    const char *src; int line;
    const char *cond;
    char *msg;

    TestResult *next;
};

typedef void (*test_proc_t)();

struct TestSuite {
    const char *name;
    test_proc_t proc;

    TestSuite *children = nullptr;
    int child_count = 0;

    bool passed = true;
    TestResult *result = nullptr;
};

// NOTE: exposed so tests can inspect error counts mid-run
extern TestSuite *test_current;

extern jmp_buf test_jmp_[2];
extern int test_jmp_i;
extern volatile sig_atomic_t test_in_progress;
#define test_jmp test_jmp_[test_jmp_i]

void test_sig_handler(int sig);
void report_fail(const char *file, int line, const char *sz_cond, const char *msg, ...);

#define INTEGRATION_TEST_PROC(name) \
    void name() __attribute__((annotate("integration_test")))

int run_integration_tests_(TestSuite *tests, int count);

#define run_integration_tests(MODULE) \
    run_integration_tests_(MODULE##__integration_tests, ARRAY_COUNT(MODULE##__integration_tests))

#define run_integration_test(proc) do { \
    TestSuite t_ = { #proc, proc, nullptr, 0, true }; \
    run_integration_tests_(&t_, 1); \
} while (0)

struct TestStats {
    i32 total;
    i32 passed;
    i32 failed;
};

void test_report_subtest(const char *name, TestResult *before, TestResult *after, TestStats *stats);
void test_print_summary(TestStats *stats);
void test_clear_results();

#endif // TEST_H

#if defined(DO_TESTS) && !defined(TEST_H_DECL)
#define TEST_H_DECL

#include <setjmp.h>
#include "core.h"
#include "hash.h"

#ifndef TEST_TYPE_LOG
#define TEST_TYPE_LOG(...)
#endif

struct TestType {
    static inline int constructor_calls = 0;
    static inline int destructor_calls = 0;
    static inline int copy_constructor_calls = 0;
    static inline int move_constructor_calls = 0;
    static inline int copy_assignment_calls = 0;
    static inline int move_assignment_calls = 0;

    int value;

    TestType(int v = 0) : value(v)
    {
        constructor_calls++;
        TEST_TYPE_LOG("constructor: %d", value);
    }

    ~TestType()
    {
        destructor_calls++;
        TEST_TYPE_LOG("destructor: %d", value);
    }

    TestType(const TestType& other) : value(other.value)
    {
        copy_constructor_calls++;
        TEST_TYPE_LOG("copy_constructor: %d", value);
    }
    TestType(TestType&& other) : value(other.value)
    {
        move_constructor_calls++;
        TEST_TYPE_LOG("move_constructor: %d", value);
    }

    TestType& operator=(const TestType& other) {
        if (this != &other) {
            value = other.value;
            copy_assignment_calls++;
            TEST_TYPE_LOG("copy_assignment: %d", value);
        }
        return *this;
    }

    TestType& operator=(TestType&& other) {
        if (this != &other) {
            value = other.value;
            move_assignment_calls++;
            TEST_TYPE_LOG("move_assignment: %d", value);
        }
        return *this;
    }

    bool operator==(const TestType &other) { return value == other.value; }

    static void reset_counters() {
        constructor_calls = 0;
        destructor_calls = 0;
        copy_constructor_calls = 0;
        move_constructor_calls = 0;
        copy_assignment_calls = 0;
        move_assignment_calls = 0;
    }
};


inline u32 hash32(const TestType &value, u32 seed = 0xdeadbeef)
{
    return hash32(value.value, seed);
}

extern int test_expect_fail;

extern int run_tests(TestSuite *tests, int count, TestStats *stats);

#define RUN_TESTS(suite, stats) run_tests(suite, sizeof(suite)/sizeof(suite[0]), stats)

#define REPORT_FAIL(...) report_fail(__FILE__, __LINE__, __VA_ARGS__)

#undef EXPECT_FAIL
#define EXPECT_FAIL(body) do {\
    test_expect_fail=true;\
    test_jmp_i=1;\
    bool failed = false;\
    switch (setjmp(test_jmp)) {\
    case 0: { body; } break;\
    default: failed=true; break;\
    }\
    if (!failed) REPORT_FAIL(nullptr, "expected failure");\
    test_expect_fail=false;\
    test_jmp_i=0;\
} while(0)

#endif // DO_TESTS && !TEST_H_DECL

#if defined(TEST_H_IMPL) || defined(INTEGRATION_TEST_H_IMPL)

#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>

jmp_buf test_jmp_[2];  int test_jmp_i = 0;
volatile sig_atomic_t test_in_progress = 0;
TestSuite *test_current = nullptr;

void test_sig_handler(int sig) {
    if (test_in_progress) {
        signal(sig, test_sig_handler);
        longjmp(test_jmp, sig);
    }
}

void report_failv(const char *file, int line, const char *sz_cond, const char *fmt, va_list va_args)
{
    int length = 0;
    char msg[2048];
    if (fmt) {
        length = vsnprintf(msg, sizeof msg - 1, fmt, va_args);
        length = length > (int)sizeof msg - 1 ? (int)sizeof msg - 1 : length;
    }
    msg[length] = '\0';

    TestResult *rep = new (malloc(sizeof *rep + length + 1)) TestResult {
        .src = file, .line = line, .cond = sz_cond,
        .next = test_current->result
    };

    if (length > 0) {
        rep->msg = (char*)rep + sizeof *rep;
        strcpy(rep->msg, msg);
    }

    test_current->result = rep;
    test_current->passed = false;
}

void report_fail(const char *file, int line, const char *sz_cond, const char *fmt, ...)
{
    va_list va_args;
    va_start(va_args, fmt);
    report_failv(file, line, sz_cond, fmt, va_args);
    va_end(va_args);
}

#include "string.h"
#include "array.h"

extern FixedArray<sink_proc_t, 10> log_sinks;

static i32 test_print_results(TestResult *from, TestResult *to = nullptr)
{
    i32 count = 0;
    for (TestResult *res = from; res && res != to; res = res->next) {
        String filename = res->src ? filename_of_sz(res->src) : String{};
        int color = 31;
        if (res->cond) {
            if (strcmp(res->cond, "panic") == 0) color = 91;
            else if (strcmp(res->cond, "info") == 0) color = 36;
        }

        if (res->cond && res->msg) {
            printf("\t%.*s:%d \033[%dm%s\033[m: %s\n",
                STRFMT(filename), res->line, color, res->cond, res->msg);
        } else if (res->cond) {
            printf("\t%.*s:%d \033[%dmcheck failed: %s\033[m\n",
                STRFMT(filename), res->line, color, res->cond);
        } else if (res->msg) {
            printf("\t%.*s:%d \033[%dm%s\033[m\n",
                STRFMT(filename), res->line, color, res->msg);
        } else {
            printf("\t%.*s:%d \033[%dmcheck failed\033[m\n",
                STRFMT(filename), res->line, color);
        }
        count++;
    }
    return count;
}

static void test_print_status(
    const char *name, 
    bool passed, 
    TestResult *results,
    TestResult *results_end, 
    int indent, int width)
{
    if (passed) {
        printf("%*s%-*s : \033[32m[OK]\033[m\n",    indent, "", width, name);
    } else {
        printf("%*s%-*s : \033[31m[ERROR]\033[m\n", indent, "", width, name);
        test_print_results(results, results_end);
    }
}

static void test_free_results(TestResult *res)
{
    while (res) {
        auto *next = res->next;
        free(res);
        res = next;
    }
}

void test_report_subtest(const char *name, TestResult *before, TestResult *after, TestStats *stats)
{
    stats->total++;
    bool had_errors = after != before;
    test_print_status(name, !had_errors, after, before, 2, 78);
    if (had_errors) stats->failed++;
    else stats->passed++;
}

void test_clear_results()
{
    if (test_current) {
        test_free_results(test_current->result);
        test_current->result = nullptr;
    }
}


void test_print_summary(TestStats *stats)
{
    printf("Summary: %d/%d passed, %d failed\n", stats->passed, stats->total, stats->failed);
}


#endif // TEST_H_IMPL || INTEGRATION_TEST_H_IMPL

#ifdef TEST_H_IMPL
#undef TEST_H_IMPL
#ifndef DO_TESTS
#define DO_TESTS
#endif

#include "test.h"

int test_expect_fail = 0;

static bool test_assert_handler(const char *file, int line, const char *sz_cond)
{
    if (!test_expect_fail) {
        report_fail(file, line, sz_cond, nullptr);
    } else longjmp(test_jmp, 1);

    return false;
}

static bool test_panic_handler(const char *file, int line, const char *sz_cond, const char *msg, ...)
{
    if (!test_expect_fail) {
        va_list va_args;
        va_start(va_args, msg);
        report_failv(file, line, sz_cond, msg, va_args);
        va_end(va_args);
    } else longjmp(test_jmp, 1);

    return false;
}

static void test_log_sink(const char *src, u32 line, LogType type, const char *msg)
{
    if (type == LOG_TYPE_INFO) return;  // suppress info noise during tests

    // LOG_ERROR and LOG_PANIC: either fail the test or trigger the EXPECT_FAIL path
    if (test_expect_fail) {
        longjmp(test_jmp, 1);
    } else if (test_current) {
        report_fail(src, (int)line, sz_from_enum(type), "%s", msg);
    }
}

static void run_tests_(TestSuite *tests, int count, TestStats *stats, int depth = 0)
{
    for (int i = 0; i < count; i++) {
        test_current = &tests[i];
        tests[i].passed = true;

        if (tests[i].children && tests[i].child_count) {
            printf("/%s\n", tests[i].name);
            run_tests_(tests[i].children, tests[i].child_count, stats, depth+1);
        }

        if (!tests[i].proc) continue;

        stats->total++;

        test_in_progress = 1;
        int sig = setjmp(test_jmp);
        if (sig == 0) {
            tests[i].proc();
        } else {
            report_fail(nullptr, 0, nullptr, "crashed with signal %d", sig);
        }
        test_in_progress = 0;

        int indent = depth * 2;
        test_print_status(tests[i].name, tests[i].passed, tests[i].result, nullptr, indent, 80 - indent);
        if (tests[i].passed) stats->passed++;
        else stats->failed++;
    }
}

int run_tests(TestSuite *tests, int count, TestStats *stats)
{
    auto prev_sigint  = signal(SIGINT,  test_sig_handler);
    auto prev_sigsegv = signal(SIGSEGV, test_sig_handler);
    auto prev_sigabrt = signal(SIGABRT, test_sig_handler);

    extern jl_assert_handler_proc jl_assert_handler;
    extern jl_panic_handler_proc jl_panic_handler;

    jl_assert_handler = test_assert_handler;
    jl_panic_handler  = test_panic_handler;

    auto prev_sinks = log_sinks;
    log_sinks.count = 0;
    array_add(&log_sinks, test_log_sink);

    run_tests_(tests, count, stats);

    log_sinks = prev_sinks;

    signal(SIGINT,  prev_sigint);
    signal(SIGSEGV, prev_sigsegv);
    signal(SIGABRT, prev_sigabrt);

    return stats->failed;
}

#endif // TEST_H_IMPL

#ifdef INTEGRATION_TEST_H_IMPL
#undef INTEGRATION_TEST_H_IMPL

#include "core.h"
#include "memory.h"

static bool itest_defer_log = false;

static void itest_log_sink(const char *src, u32 line, LogType type, const char *msg)
{
    if (test_current && type <= LOG_TYPE_ERROR) {
        report_fail(src, (int)line, sz_from_enum(type), "%s", msg);
    }

    if (type <= LOG_TYPE_ERROR && !itest_defer_log) {
        String filename = filename_of_sz(src);
        const char *type_s = sz_from_enum(type);
        char color = 49;
        switch (type) {
        case LOG_TYPE_ERROR: color = 31; break;
        case LOG_TYPE_INFO:  color = 36; break;
        case LOG_TYPE_PANIC: color = 91; break;
        }
        fprintf(stderr, "%.*s:%d \033[%dm%s\033[m: %s\n", STRFMT(filename), line, color, type_s, msg);
    }
}

static bool itest_assert_handler(const char *src, int line, const char *sz_cond)
{
    report_fail(src, line, sz_cond, nullptr);
    return false;
}

static bool itest_panic_handler(const char *src, int line, const char *sz_cond, const char *msg, ...)
{
    if (msg) {
        va_list va_args;
        va_start(va_args, msg);
        report_failv(src, line, sz_cond, msg, va_args);
        va_end(va_args);
    } else {
        report_fail(src, line, sz_cond, nullptr);
    }

    longjmp(test_jmp, 1);
    return false;
}

int run_integration_tests_(TestSuite *tests, int count)
{
    auto prev_sinks = log_sinks;
    log_sinks.count = 0;
    array_add(&log_sinks, itest_log_sink);

    auto prev_assert = jl_assert_handler;
    auto prev_panic  = jl_panic_handler;
    jl_assert_handler = itest_assert_handler;
    jl_panic_handler  = itest_panic_handler;

    auto prev_sigsegv = signal(SIGSEGV, test_sig_handler);
    auto prev_sigabrt = signal(SIGABRT, test_sig_handler);

    int failed = 0;

    for (int i = 0; i < count; i++) {
        TestSuite *test = &tests[i];
        test->result = nullptr;
        test->passed = true;
        test_current = test;

        printf("/%s\n", test->name);

        test_in_progress = 1;
        int sig = setjmp(test_jmp);
        if (sig == 0) {
            test->proc();
        } else {
            printf("  %-78s : \033[31m[ERROR]\033[m\n", "(crashed)");
            printf("\tsignal %d\n", sig);
            test->passed = false;
        }
        test_in_progress = 0;

        if (!test->passed) {
            test_print_results(test->result);
            failed++;
        }

        test_free_results(test->result);
    }

    test_current = nullptr;

    log_sinks = prev_sinks;
    jl_assert_handler = prev_assert;
    jl_panic_handler  = prev_panic;
    signal(SIGSEGV, prev_sigsegv);
    signal(SIGABRT, prev_sigabrt);

    return failed;
}

#endif // INTEGRATION_TEST_H_IMPL
