/* AUTOGENERATED FILE. DO NOT EDIT. */
#include "unity.h"
#include <setjmp.h>
#include <stdio.h>

char MessageBuffer[50];

extern void setUp(void);
extern void tearDown(void);

extern void test_init_ShouldWork(void);
extern void test_FTLSIZE_ShouldWork(void);
extern void test_DMAW_loopback_DMAR_ShouldWork(void);
extern void test_AUX_loopback_DMAR_ShouldWork(void);
extern void test_FTL_loopback_DMAR_ShouldWork(void);
extern void test_DMAW_loopback_FTL_ShouldWork(void);

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
  Unity.TestFile = "tests/NFC/test_FTL.c";
  UnityBegin();

  // RUN_TEST calls runTest
  RUN_TEST(test_init_ShouldWork, 32);
  RUN_TEST(test_FTLSIZE_ShouldWork, 59);
  RUN_TEST(test_DMAW_loopback_DMAR_ShouldWork, 127);
  RUN_TEST(test_AUX_loopback_DMAR_ShouldWork, 168);
  RUN_TEST(test_FTL_loopback_DMAR_ShouldWork, 215);
  RUN_TEST(test_DMAW_loopback_FTL_ShouldWork, 261);

  UnityEnd();
  return 0;
}