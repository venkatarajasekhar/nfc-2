/**
 * @file test_ONFI.c
 *
 * This testsuit tests asynchronous Flash compatibility to ONFI
 *
 * @author fox.chao@gmail.com
 */

#include "ONFI.h"
#include "NFC.h"
#include "crypto_ext.h"

#include "unity.h"
#include "helpers.h"

void setUp(void)
{
    DBGCMDFF(0);
}

void tearDown(void)
{
}

void test_ONFI_init_ShouldBeOK(void)
{
    TEST_ASSERT_EQUAL_INT(0, ONFI_init(NFC_MODE_D16));
}

void test_ONFI_Reset_ShouldWork(void)
{
    ONFI_Reset();
    TEST_ASSERT_EQUAL_HEX16(0xe0e0, ONFI_ReadStatus());
}
