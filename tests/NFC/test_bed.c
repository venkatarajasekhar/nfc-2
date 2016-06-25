/**
 * @file test_bed.c
 *
 * This testsuit tests if the test bed is robust.
 *
 * @author fox.chao@gmail.com
 */

#include "config.h"
#include "NFC.h"

#include "unity.h"
#include "helpers.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_memory_ShouldBe32KB(void)
{
    volatile unsigned int *buff = (unsigned int *)0x20003C70;

    for (int i = 0; i < 0x20; i++)
	{
        buff[i] = 0x55AA55AA;
//        DBGPRINTF("%08x, ", buff[i]);
	}

    TEST_ASSERT_EQUAL_UINT(0x7f86a95, fletcher32((uint16_t *)buff, 0x20 * 2));
}

void test_ignore_ShouldIgnore(void)
{
    TEST_IGNORE_MESSAGE("ignore");
}

void test_fail_ShouldFail(void)
{
    TEST_ASSERT_EQUAL_UINT(0, 1);
}
