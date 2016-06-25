/* AUTOGENERATED FILE. DO NOT EDIT. */
#include "unity.h"
#include <setjmp.h>
#include <stdio.h>

char MessageBuffer[50];

extern void setUp(void);
extern void tearDown(void);

extern void test_rhm_init_ShouldWork(void);
extern void test_rhm_cpu_ShouldWork(void);

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
  Unity.TestFile = "tests/CRYPTO/test_RHM.c";
  UnityBegin();

  // RUN_TEST calls runTest
  RUN_TEST(test_rhm_init_ShouldWork, 48);
  RUN_TEST(test_rhm_cpu_ShouldWork, 61);

  UnityEnd();
  return 0;
}
