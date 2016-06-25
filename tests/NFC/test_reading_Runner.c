/* AUTOGENERATED FILE. DO NOT EDIT. */
#include "unity.h"
#include <setjmp.h>
#include <stdio.h>

char MessageBuffer[50];

extern void setUp(void);
extern void tearDown(void);

extern void test_ONFI_init_ShouldBeOK(void);
extern void test_ONFI_Reset_ShouldWork(void);
extern void test_ONFI_Features_ShouldBeChanged(void);
extern void test_ONFI_onfi_speed_ShouldWork(void);
extern void test_ReadPage_via_FMICFF_ShouldWork(void);
extern void test_ReadPage_via_DMARFF_ShouldWork(void);
extern void test_ReadPage_via_RSDM1_ShouldWork(void);

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
  Unity.TestFile = "tests/NFC/test_reading.c";
  UnityBegin();

  // RUN_TEST calls runTest
  RUN_TEST(test_ONFI_init_ShouldBeOK, 25);
  RUN_TEST(test_ONFI_Reset_ShouldWork, 30);
  RUN_TEST(test_ONFI_Features_ShouldBeChanged, 36);
  RUN_TEST(test_ONFI_onfi_speed_ShouldWork, 55);
  RUN_TEST(test_ReadPage_via_FMICFF_ShouldWork, 101);
  RUN_TEST(test_ReadPage_via_DMARFF_ShouldWork, 148);
  RUN_TEST(test_ReadPage_via_RSDM1_ShouldWork, 183);

  UnityEnd();
  return 0;
}
