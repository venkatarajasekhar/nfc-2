/* AUTOGENERATED FILE. DO NOT EDIT. */
#include "unity.h"
#include <setjmp.h>
#include <stdio.h>

char MessageBuffer[50];

extern void setUp(void);
extern void tearDown(void);

extern void test_ONFI_init_ShouldBeOK(void);
extern void test_ONFI_Reset_ShouldWork(void);
extern void test_ONFI_ReadID_ShouldRead_ONFI(void);
extern void test_ONFI_ReadParameterPage_ShouldRead_ONFI(void);
extern void test_ONFI_Features_CouldBeChange(void);
extern void test_ONFI_set_speed_ShouldWork(void);
extern void test_ONFI_EraseBlock_ShouldWork(void);
extern void test_ONFI_ProgramPage_ShouldWork(void);
extern void test_ONFI_ReadPage_ShouldWork(void);

static void runTest(UnityTestFunction test)
{
  if (TEST_PROTECT())
  {
      setUp();
      test();
  }
  if (TEST_PROTECT() && !TEST_IS_IGNORED)
  {
    tearDown();
  }
}
void resetTest()
{
  tearDown();
  setUp();
}


int main(void)
{
  Unity.TestFile = "tests/NFC/test_as8.c";
  UnityBegin();

  // RUN_TEST calls runTest
  RUN_TEST(test_ONFI_init_ShouldBeOK, 25);
  RUN_TEST(test_ONFI_Reset_ShouldWork, 30);
  RUN_TEST(test_ONFI_ReadID_ShouldRead_ONFI, 36);
  RUN_TEST(test_ONFI_ReadParameterPage_ShouldRead_ONFI, 49);
  RUN_TEST(test_ONFI_Features_CouldBeChange, 62);
  RUN_TEST(test_ONFI_set_speed_ShouldWork, 81);
  RUN_TEST(test_ONFI_EraseBlock_ShouldWork, 88);
  RUN_TEST(test_ONFI_ProgramPage_ShouldWork, 163);
  RUN_TEST(test_ONFI_ReadPage_ShouldWork, 217);

  UnityEnd();
  return 0;
}
