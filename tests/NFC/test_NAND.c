/**
 * @file test_NAND.c
 *
 * This testsuit tests if NAND module is working or not.
 *
 * @author fox.chao@gmail.com
 */

#include "NFC.h"

#include "unity.h"
#include "helpers.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_NAND_init_ShouldBeOK(void)
{
    NAND_init();
}
