#include "../eztest/eztest.h"
#include "utils/hook/hook.h"
#include "utils/mem/mem.h"

typedef int (*sum_t)(int, int);
static sum_t orig_caller_;

__attribute__((__naked__)) int sum_naked(int a, int b)
{
    (void)a;
    (void)b;
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("pushl %ebp");
    __asm__("movl %esp,%ebp");
    __asm__("movl 8(%ebp),%eax");
    __asm__("addl 12(%ebp),%eax");
    __asm__("popl %ebp");
    __asm__("ret");
}

int sum_hook(int a, int b)
{
    a += 1;
    b += 2;
    return orig_caller_(a, b);
}

TEST_BEGIN(test_01)
{
    int allocs = mem_debug_get_diff();

    EXPECT(hook_init(), true);
    EXPECT(sum_naked(4, 3), 7);
    orig_caller_ = (sum_t)hook_set(sum_naked, sum_hook, 5);
    EXPECT_NOT_ZERO(orig_caller_);
    EXPECT(sum_naked(4, 3), 10);
    hook_unset(sum_naked);
    EXPECT(sum_naked(4, 3), 7);
    orig_caller_ = (sum_t)hook_set(sum_naked, sum_hook, 5);
    EXPECT_NOT_ZERO(orig_caller_);
    EXPECT(sum_naked(4, 3), 10);
    hook_destroy();
    EXPECT(sum_naked(4, 3), 7);

    EXPECT(mem_debug_get_diff(), allocs);
}
TEST_END

RUN_TESTS(FILENAME, test_01);
