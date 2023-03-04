#include "../eztest/eztest.h"
#include <windows.h>

extern BOOL WINAPI DllMain(HMODULE hinst, DWORD reason, LPVOID reserved);

TEST_BEGIN(test_01)
{
    EXPECT(DllMain(0, ~DLL_PROCESS_ATTACH, 0), TRUE);
}
TEST_END

RUN_TESTS(FILENAME, test_01);
