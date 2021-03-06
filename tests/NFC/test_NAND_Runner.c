/* AUTOGENERATED FILE. DO NOT EDIT. */
#include "unity.h"
#include <setjmp.h>
#include <stdio.h>

char MessageBuffer[50];

extern void setUp(void);
extern void tearDown(void);

extern void test_NAND_init_ShouldBeOK(void);

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
  Unity.TestFile = "tests/NFC/test_NAND.c";
  UnityBegin();

  // RUN_TEST calls runTest
  RUN_TEST(test_NAND_init_ShouldBeOK, 22);

  UnityEnd();
  return 0;
}
