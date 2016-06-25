/**
 * @file test_NFC.c
 *
 * This testsuit tests if NFC module is active or not.
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

void test_NFC_init_ShouldBeOK(void)
{
    NFC_init();
}

void test_NFC_DMAWFF_ShouldBeOK(void)
{
    TEST_IGNORE();

	// Read DMAWFF_SLOT
    int tp_count = 2 * get_slot(NFC_REG[NFC_DMAWFF_SLOT]);
    TEST_ASSERT_EQUAL_INT(128, tp_count);

	for (int tp_i=0; tp_i<=tp_count-1; tp_i++)
    {
        NFC_DMAWFF[0] = tp_i + 5;
    }
	TEST_ASSERT_EQUAL_INT(tp_count, 2 * get_count(NFC_REG[NFC_DMAWFF_SLOT]));

	tp_count = get_count(NFC_REG[NFC_DMAWFF_SLOT]);
	for (int tp_i=0; tp_i<=tp_count-1; tp_i++)
	{
        NFC_DMAWFFRD[0] = 2;  // pop
        NFC_DMAWFFRD[0] = 0;  // lower DOWRD
		TEST_ASSERT_EQUAL_INT((2 * tp_i) + 5, NFC_DMAWFFRD[0]);
		NFC_DMAWFFRD[0] = 1;  // upper DWORD
		TEST_ASSERT_EQUAL_INT((2 * tp_i) + 6, NFC_DMAWFFRD[0]);
	}
}

void test_NFC_DMARFF_ShouldBeOK(void)
{
    TEST_IGNORE();

	// Read DMARFF_SLOT
    int tp_count = 2 * get_slot(NFC_REG[NFC_DMARFF_SLOT]);
    TEST_ASSERT_EQUAL_INT(128, tp_count);

	for (int tp_i=0; tp_i<=tp_count-1; tp_i++)
    {
        NFC_DMARFF[0] = tp_i + 95;
    }
	TEST_ASSERT_EQUAL_INT(tp_count, 2 * get_count(NFC_REG[NFC_DMARFF_SLOT]));

    tp_count = get_count(NFC_REG[NFC_DMARFF_SLOT]);
	for (int tp_i=0; tp_i<=tp_count-1; tp_i++)
	{
        NFC_DMARFFRD[0] = 2;  // pop
        NFC_DMARFFRD[0] = 0;  // lower DOWRD
		TEST_ASSERT_EQUAL_INT((2 * tp_i) + 95, NFC_DMARFFRD[0]);
		NFC_DMARFFRD[0] = 1;  // upper DWORD
		TEST_ASSERT_EQUAL_INT((2 * tp_i) + 96, NFC_DMARFFRD[0]);
	}
}
