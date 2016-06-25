/* AUTOGENERATED FILE. DO NOT EDIT. */
#include "unity.h"
#include "CException.h"

extern void setUp(void);
extern void tearDown(void);

extern void test_BasicTryDoesNothingIfNoThrow(void);
extern void test_BasicThrowAndCatch(void);
extern void test_BasicThrowAndCatch_WithMiniSyntax(void);
extern void test_VerifyVolatilesSurviveThrowAndCatch(void);
extern void test_ThrowFromASubFunctionAndCatchInRootFunc(void);
extern void test_ThrowAndCatchFromASubFunctionAndRethrowToCatchInRootFunc(void);
extern void test_ThrowAndCatchFromASubFunctionAndNoRethrowToCatchInRootFunc(void);
extern void test_CanHaveMultipleTryBlocksInASingleFunction(void);
extern void test_CanHaveNestedTryBlocksInASingleFunction_ThrowInside(void);
extern void test_CanHaveNestedTryBlocksInASingleFunction_ThrowOutside(void);
extern void test_ThrowAnErrorThenEnterATryBlockFromWithinCatch_VerifyThisDoesntCorruptExceptionId(void);
extern void test_ThrowAnErrorThenEnterATryBlockFromWithinCatch_VerifyThatEachExceptionIdIndependent(void);

static void runTest(UnityTestFunction test)
{
  CEXCEPTION_T e;
  if (TEST_PROTECT())
  {
    setUp();
    Try
    {
      test();
    }
    Catch(e)
    {
      TEST_FAIL("Unexpected exception!")
    }
  }
  tearDown();
}


int main(void)
{
  Unity.TestFile = __FILE__;
  UnityBegin();

  // RUN_TEST calls runTest
  RUN_TEST(test_BasicTryDoesNothingIfNoThrow, 12);
  RUN_TEST(test_BasicThrowAndCatch, 30);
  RUN_TEST(test_BasicThrowAndCatch_WithMiniSyntax, 49);
  RUN_TEST(test_VerifyVolatilesSurviveThrowAndCatch, 69);
  RUN_TEST(test_ThrowFromASubFunctionAndCatchInRootFunc, 98);
  RUN_TEST(test_ThrowAndCatchFromASubFunctionAndRethrowToCatchInRootFunc, 139);
  RUN_TEST(test_ThrowAndCatchFromASubFunctionAndNoRethrowToCatchInRootFunc, 158);
  RUN_TEST(test_ThrowAnErrorThenEnterATryBlockFromWithinCatch_VerifyThisDoesntCorruptExceptionId, 175);
  RUN_TEST(test_ThrowAnErrorThenEnterATryBlockFromWithinCatch_VerifyThatEachExceptionIdIndependent, 193);
  RUN_TEST(test_CanHaveMultipleTryBlocksInASingleFunction, 220);
  RUN_TEST(test_CanHaveNestedTryBlocksInASingleFunction_ThrowInside, 245);
  RUN_TEST(test_CanHaveNestedTryBlocksInASingleFunction_ThrowOutside, 269);

  UnityEnd();
  
  return 0;
}
