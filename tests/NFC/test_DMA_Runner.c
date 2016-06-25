/* AUTOGENERATED FILE. DO NOT EDIT. */
#include "unity.h"
#include <setjmp.h>
#include <stdio.h>

char MessageBuffer[50];

extern void setUp(void);
extern void tearDown(void);

extern void test_init_ShouldWork(void);
extern void test_SEM1_to_DMAW_ShouldWork(void);
extern void test_SEM1_ShouldBlock(void);
extern void test_dmar_to_rsdm1_ShouldWork(void);
extern void test_internal_loopback_ShouldWork(void);
extern void test_external_loopback_ShouldWork(void);

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
  Unity.TestFile = "tests/NFC/test_DMA.c";
  UnityBegin();

  // RUN_TEST calls runTest
  RUN_TEST(test_init_ShouldWork, 31);
  RUN_TEST(test_SEM1_to_DMAW_ShouldWork, 98);
  RUN_TEST(test_SEM1_ShouldBlock, 157);
  RUN_TEST(test_dmar_to_rsdm1_ShouldWork, 213);
  RUN_TEST(test_internal_loopback_ShouldWork, 317);
  RUN_TEST(test_external_loopback_ShouldWork, 399);

  UnityEnd();
  return 0;
}