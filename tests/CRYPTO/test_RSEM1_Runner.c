/* AUTOGENERATED FILE. DO NOT EDIT. */
#include "unity.h"
#include <setjmp.h>
#include <stdio.h>

char MessageBuffer[50];

extern void setUp(void);
extern void tearDown(void);

extern void test_rsem1_init_ShouldWork(void);
extern void test_rsem1_block_ShouldWork(void);
extern void test_rsem1_cpu_ShouldWork(void);
extern void test_rsem1_encrypt_xfer_cpu_ShouldWork(void);

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
  Unity.TestFile = "tests/CRYPTO/test_RSEM1.c";
  UnityBegin();

  // RUN_TEST calls runTest
  RUN_TEST(test_rsem1_init_ShouldWork, 49);
  RUN_TEST(test_rsem1_block_ShouldWork, 107);
  RUN_TEST(test_rsem1_cpu_ShouldWork, 132);
  RUN_TEST(test_rsem1_encrypt_xfer_cpu_ShouldWork, 182);

  UnityEnd();
  return 0;
}