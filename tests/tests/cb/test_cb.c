/**
 ***********************************************************************************************************************
 * @file        test_cb.c
 * @author      Diego Martínez García (dmg0345@gmail.com)
 * @date        04-11-2023 21:33:15 (UTC)
 * @version     1.0.0
 * @copyright   github.com/dmg0345/cb/blob/master/LICENSE
 ***********************************************************************************************************************
 */

/** @defgroup cb_tests Tests */

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "cmocka_defs.h"
#include "test_types.h"
#include "cb_evt_handlers/cb_evt_handlers.h"
#include "cb/cb.h"

/* Private types -----------------------------------------------------------------------------------------------------*/
/* Private define ----------------------------------------------------------------------------------------------------*/
/* Private macro -----------------------------------------------------------------------------------------------------*/
/* Private variables -------------------------------------------------------------------------------------------------*/
/** Underlying linear buffer for the circular buffer, with extra element first and last. */
static test_type_t lcbuf[12U + 1U];
/** Destination buffer, to be used for read operations in the circular buffer, with extra element first and last. */
static test_type_t ldbuf[12U];
/** Source buffer, to be used for write operations in the circular buffer. */
static const test_type_t lsbuf[10U] = {0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U, 0x09U, 0x0AU};
/** Circular buffer. */
static cb_t cbuf;

/* Private function prototypes ---------------------------------------------------------------------------------------*/
/** Suite setup function. */
static int setup(void ** state);
/** Suite teardown function. */
static int teardown(void ** state);

/**
 * @addtogroup cb_tests
 * @{
 */

/** Tests for invalid arguments. */
static void test_cb_invalid_arguments(void ** state);
/** Tests for write and read with multiple block sizes. */
static void test_cb_write_read_blocks(void ** state);
/** Tests for write and read on some edge cases. */
static void test_cb_write_read_edge_cases(void ** state);
/** Tests for write and read errors on the event handlers. */
static void test_cb_write_read_evt_handler_errors(void ** state);
/** Tests for write and read errors on full and empty conditions. */
static void test_cb_write_read_full_empty_errors(void ** state);

/**
 * @}
 */

/* Private functions -------------------------------------------------------------------------------------------------*/
static int setup(void ** state)
{
    // Initialize linear buffers and assert their sizes.
    (void)memset(lcbuf, 0xFFU, sizeof(lcbuf));
    (void)memset(ldbuf, 0xFFU, sizeof(ldbuf));
    assert_int_equal(ARRAY_DIM(lsbuf) + 2U, ARRAY_DIM(ldbuf));
    assert_int_equal(ARRAY_DIM(lsbuf) + 2U, ARRAY_DIM(lcbuf) - 1U);

    // Initialize circular buffer, leave one element first and at the end for checking out of bounds writes.
    assert_int_equal(cb_init(&cbuf, lcbuf + 1U, ARRAY_DIM(lcbuf) - 2U, sizeof(*lcbuf), NULL, cb_evt_id_none, NULL),
                     cb_error_ok);

    // Assign circular buffer to tests.
    *state = &cbuf;

    return CMOCKA_OK;
}

/*--------------------------------------------------------------------------------------------------------------------*/
static int teardown(void ** state)
{
    cb_t * const cb = (cb_t * const)*state;

    // Deinitialize circular buffer.
    assert_int_equal(cb_deinit(cb), cb_error_ok);

    // Deinitialize linear buffers.
    (void)memset(lcbuf, 0xFFU, sizeof(lcbuf));
    (void)memset(ldbuf, 0xFFU, sizeof(ldbuf));

    // Clear state.
    *state = NULL;

    return CMOCKA_OK;
}

/*--------------------------------------------------------------------------------------------------------------------*/
static void test_cb_invalid_arguments(void ** state)
{
    cb_t * const cb = (cb_t * const)*state;
    size_t count = 0U;
    bool is_state = false;

    // Check invalid arguments on 'cb_init'.
    assert_int_equal(cb_init(NULL, lcbuf, ARRAY_DIM(lcbuf), sizeof(*lcbuf), NULL, cb_evt_id_none, NULL),
                     cb_error_invalid_args);
    assert_int_equal(cb_init(cb, NULL, ARRAY_DIM(lcbuf), sizeof(*lcbuf), NULL, cb_evt_id_none, NULL),
                     cb_error_invalid_args);
    assert_int_equal(cb_init(cb, lcbuf, 0U, sizeof(*lcbuf), NULL, cb_evt_id_none, NULL), cb_error_invalid_args);
    assert_int_equal(cb_init(cb, lcbuf, 1U, sizeof(*lcbuf), NULL, cb_evt_id_none, NULL), cb_error_invalid_args);
    assert_int_equal(cb_init(cb, lcbuf, ARRAY_DIM(lcbuf), 0U, NULL, cb_evt_id_none, NULL), cb_error_invalid_args);
    assert_int_equal(cb_init(cb, lcbuf, ARRAY_DIM(lcbuf), sizeof(*lcbuf), cb_evt_handler, cb_evt_id_none, NULL),
                     cb_error_invalid_args);
    assert_int_equal(cb_init(cb, lcbuf, ARRAY_DIM(lcbuf), sizeof(*lcbuf), NULL, cb_evt_id_lock, NULL),
                     cb_error_invalid_args);

    // Check invalid arguments on 'cb_write'.
    assert_int_equal(cb_write(NULL, lsbuf, ARRAY_DIM(lsbuf)), cb_error_invalid_args);
    assert_int_equal(cb_write(cb, NULL, ARRAY_DIM(lsbuf)), cb_error_invalid_args);
    assert_int_equal(cb_write(cb, lsbuf, 0U), cb_error_invalid_args);

    // Check invalid arguments on 'cb_write'.
    assert_int_equal(cb_read(NULL, ldbuf, ARRAY_DIM(ldbuf)), cb_error_invalid_args);
    assert_int_equal(cb_read(cb, NULL, ARRAY_DIM(ldbuf)), cb_error_invalid_args);
    assert_int_equal(cb_read(cb, ldbuf, 0U), cb_error_invalid_args);

    // Check invalid arguments on 'cb_get_unfilled'.
    assert_int_equal(cb_get_unfilled(cb, NULL), cb_error_invalid_args);
    assert_int_equal(cb_get_unfilled(NULL, &count), cb_error_invalid_args);

    // Check invalid arguments on 'cb_get_filled'.
    assert_int_equal(cb_get_filled(cb, NULL), cb_error_invalid_args);
    assert_int_equal(cb_get_filled(NULL, &count), cb_error_invalid_args);

    // Check invalid arguments on 'cb_is_empty'.
    assert_int_equal(cb_is_empty(cb, NULL), cb_error_invalid_args);
    assert_int_equal(cb_is_empty(NULL, &is_state), cb_error_invalid_args);

    // Check invalid arguments on 'cb_is_full'.
    assert_int_equal(cb_is_full(cb, NULL), cb_error_invalid_args);
    assert_int_equal(cb_is_full(NULL, &is_state), cb_error_invalid_args);

    // Check invalid arguments on 'cb_deinit'.
    assert_int_equal(cb_deinit(NULL), cb_error_invalid_args);
}

/*--------------------------------------------------------------------------------------------------------------------*/
static void test_cb_write_read_blocks(void ** state)
{
    cb_t * const cb = (cb_t * const)*state;
    size_t count = 0U;

    // Perform the following passes:
    //     - On first pass, use the built-in read and write functions, the default in the tests.
    //     - On second pass, use the custom read and write functions.
    for (size_t pass = 0U; pass < 2U; pass++)
    {
        if (pass == 1U)
        {
            // Modify event handler directly for simplicity.
            cb->evt_handler = cb_evt_handler;
            cb->evt_sub = cb_evt_id_read | cb_evt_id_write;
        }

        // Write and read data from buffer.
        for (size_t block_size = 1U; block_size <= ARRAY_DIM(lsbuf); block_size++)
        {
            // Perform multiple writes and reads to exercise multiple scenarios with the pointers.
            for (size_t run = 0U; run < ARRAY_DIM(lcbuf); run++)
            {
                // Write buffer.
                assert_int_equal(cb_write(cb, lsbuf, block_size), cb_error_ok);
                assert_int_equal(cb_get_filled(cb, &count), cb_error_ok);
                assert_int_equal(count, block_size);
                assert_int_equal(cb_get_unfilled(cb, &count), cb_error_ok);
                assert_int_equal(count, ARRAY_DIM(lsbuf) - block_size);
                // Read buffer.
                assert_int_equal(cb_read(cb, &ldbuf[1U], block_size), cb_error_ok);
                assert_int_equal(cb_get_filled(cb, &count), cb_error_ok);
                assert_int_equal(count, 0U);
                assert_int_equal(cb_get_unfilled(cb, &count), cb_error_ok);
                assert_int_equal(count, ARRAY_DIM(lsbuf));

                // Check underlying linear buffer of the circular buffer has not been overrun.
                assert_true(lcbuf[0U] == TEST_CLEAR_VALUE);
                assert_true(lcbuf[ARRAY_DIM(lcbuf) - 1U] == TEST_CLEAR_VALUE);

                // Check destination buffer has been read correctly and it has not been overrun.
                assert_true(ldbuf[0U] == TEST_CLEAR_VALUE);
                assert_memory_equal(&ldbuf[1U], lsbuf, block_size * sizeof(*ldbuf));
                assert_true(ldbuf[ARRAY_DIM(ldbuf) - 1U] == TEST_CLEAR_VALUE);

                // Clear underlying linear buffer and destination buffer for next run.
                (void)memset(lcbuf, 0xFFU, sizeof(lcbuf));
                (void)memset(ldbuf, 0xFFU, sizeof(ldbuf));
            }
        }
    }
}

/*--------------------------------------------------------------------------------------------------------------------*/
static void test_cb_write_read_edge_cases(void ** state)
{
    cb_t * const cb = (cb_t * const)*state;
    size_t count = 0U;
    bool is_empty = false;
    bool is_full = false;

    // Fill the buffer to 4/5 capacity.
    assert_int_equal(cb_write(cb, lsbuf, (ARRAY_DIM(lsbuf) / 5U) * 4U), cb_error_ok);
    assert_int_equal(cb_get_filled(cb, &count), cb_error_ok);
    assert_int_equal(count, (ARRAY_DIM(lsbuf) / 5U) * 4U);
    assert_int_equal(cb_get_unfilled(cb, &count), cb_error_ok);
    assert_int_equal(count, (ARRAY_DIM(lsbuf) / 5U) * 1U);
    assert_int_equal(cb_is_full(cb, &is_full), cb_error_ok);
    assert_false(is_full);
    assert_int_equal(cb_is_empty(cb, &is_empty), cb_error_ok);
    assert_false(is_empty);

    // Read 1/5 capacity of the buffer.
    assert_int_equal(cb_read(cb, &ldbuf[1U], (ARRAY_DIM(lsbuf) / 5U) * 1U), cb_error_ok);
    assert_int_equal(cb_get_filled(cb, &count), cb_error_ok);
    assert_int_equal(count, (ARRAY_DIM(lsbuf) / 5U) * 3U);
    assert_int_equal(cb_get_unfilled(cb, &count), cb_error_ok);
    assert_int_equal(count, (ARRAY_DIM(lsbuf) / 5U) * 2U);
    assert_int_equal(cb_is_full(cb, &is_full), cb_error_ok);
    assert_false(is_full);
    assert_int_equal(cb_is_empty(cb, &is_empty), cb_error_ok);
    assert_false(is_empty);

    // Write 2/5 capacity, making it full.
    assert_int_equal(cb_write(cb, lsbuf, (ARRAY_DIM(lsbuf) / 5U) * 2U), cb_error_ok);
    assert_int_equal(cb_get_filled(cb, &count), cb_error_ok);
    assert_int_equal(count, (ARRAY_DIM(lsbuf) / 5U) * 5U);
    assert_int_equal(cb_get_unfilled(cb, &count), cb_error_ok);
    assert_int_equal(count, (ARRAY_DIM(lsbuf) / 5U) * 0U);
    assert_int_equal(cb_is_full(cb, &is_full), cb_error_ok);
    assert_true(is_full);
    assert_int_equal(cb_is_empty(cb, &is_empty), cb_error_ok);
    assert_false(is_empty);

    // Read the buffer in se parate operations till empty.
    assert_int_equal(cb_read(cb, &ldbuf[1U], (ARRAY_DIM(lsbuf) / 5U) * 2U), cb_error_ok);
    assert_int_equal(cb_read(cb, &ldbuf[1U], (ARRAY_DIM(lsbuf) / 5U) * 1U), cb_error_ok);
    assert_int_equal(cb_read(cb, &ldbuf[1U], (ARRAY_DIM(lsbuf) / 5U) * 2U), cb_error_ok);
    assert_int_equal(cb_get_filled(cb, &count), cb_error_ok);
    assert_int_equal(count, (ARRAY_DIM(lsbuf) / 5U) * 0U);
    assert_int_equal(cb_get_unfilled(cb, &count), cb_error_ok);
    assert_int_equal(count, (ARRAY_DIM(lsbuf) / 5U) * 5U);
    assert_int_equal(cb_is_full(cb, &is_full), cb_error_ok);
    assert_false(is_full);
    assert_int_equal(cb_is_empty(cb, &is_empty), cb_error_ok);
    assert_true(is_empty);

    // Check no overflow / underflow on underlying and destination buffers.
    assert_int_equal(lcbuf[0U], TEST_CLEAR_VALUE);
    assert_int_equal(lcbuf[ARRAY_DIM(lcbuf) - 1U], TEST_CLEAR_VALUE);
    assert_int_equal(ldbuf[0U], TEST_CLEAR_VALUE);
    assert_int_equal(ldbuf[ARRAY_DIM(ldbuf) - 1U], TEST_CLEAR_VALUE);
}

/*--------------------------------------------------------------------------------------------------------------------*/
static void test_cb_write_read_evt_handler_errors(void ** state)
{
    cb_t * const cb = (cb_t * const)*state;

    // Attempt to write in a single operation, trigerring an error.
    cb->evt_handler = cb_evt_handler_write_first_operation_error;
    cb->evt_sub = cb_evt_id_write;
    assert_int_equal(cb_write(cb, lsbuf, ARRAY_DIM(lsbuf) / 2U), cb_error_evt);

    // Write half the buffer, to have something to read next.
    cb->evt_handler = NULL;
    cb->evt_sub = cb_evt_id_none;
    assert_int_equal(cb_write(cb, lsbuf, ARRAY_DIM(lsbuf) / 2U), cb_error_ok);

    // Attempt to read in a single operation, triggering an error.
    cb->evt_handler = cb_evt_handler_read_first_operation_error;
    cb->evt_sub = cb_evt_id_read;
    assert_int_equal(cb_read(cb, &ldbuf[1U], ARRAY_DIM(lsbuf) / 2U), cb_error_evt);

    // Read half the buffer to have both the read and write pointers in the middle.
    cb->evt_handler = NULL;
    cb->evt_sub = cb_evt_id_none;
    assert_int_equal(cb_read(cb, &ldbuf[1U], ARRAY_DIM(lsbuf) / 2U), cb_error_ok);

    // Attempt to write full buffer, trigerring two write operations, failing on second operation.
    cb->evt_handler = cb_evt_handler_write_second_operation_error;
    cb->evt_sub = cb_evt_id_write;
    assert_int_equal(cb_write(cb, lsbuf, ARRAY_DIM(lsbuf)), cb_error_evt);

    // Perform full write of buffer.
    cb->evt_handler = NULL;
    cb->evt_sub = cb_evt_id_none;
    assert_int_equal(cb_write(cb, lsbuf, ARRAY_DIM(lsbuf)), cb_error_ok);

    // Attempt to read full buffer, trigerring two reads, failing on second operation.
    cb->evt_handler = cb_evt_handler_read_second_operation_error;
    cb->evt_sub = cb_evt_id_read;
    assert_int_equal(cb_read(cb, &ldbuf[1U], ARRAY_DIM(lsbuf)), cb_error_evt);
}

/*--------------------------------------------------------------------------------------------------------------------*/
static void test_cb_write_read_full_empty_errors(void ** state)
{
    cb_t * const cb = (cb_t * const)*state;

    // Attempt to read buffer when empty.
    assert_int_equal(cb_read(cb, &ldbuf[1U], 1U), cb_error_empty);
    // Fill the buffer till full.
    assert_int_equal(cb_write(cb, lsbuf, ARRAY_DIM(lsbuf)), cb_error_ok);
    // Attempt to write buffer when full.
    assert_int_equal(cb_write(cb, lsbuf, 1U), cb_error_full);
    // Read half the buffer.
    assert_int_equal(cb_read(cb, &ldbuf[1U], ARRAY_DIM(lsbuf) / 2U), cb_error_ok);
    // Attempt to write more data that can fit.
    assert_int_equal(cb_write(cb, lsbuf, ARRAY_DIM(lsbuf)), cb_error_full);
    // Attempt to read more data that exists.
    assert_int_equal(cb_read(cb, &ldbuf[1U], ARRAY_DIM(lsbuf)), cb_error_empty);
}

/* Exported functions ------------------------------------------------------------------------------------------------*/
/**
 * @brief Test runner for this suite of tests.
 * @return The result of the test runner.
 */
int main(void)
{
    // Initialize CMocka.
    cmocka_init();

    // The table with the tests.
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_cb_invalid_arguments, setup, teardown),
        cmocka_unit_test_setup_teardown(test_cb_write_read_blocks, setup, teardown),
        cmocka_unit_test_setup_teardown(test_cb_write_read_edge_cases, setup, teardown),
        cmocka_unit_test_setup_teardown(test_cb_write_read_evt_handler_errors, setup, teardown),
        cmocka_unit_test_setup_teardown(test_cb_write_read_full_empty_errors, setup, teardown),
    };

    // Execute the test runner.
    return cmocka_run_group_tests_name("cb", tests, NULL, NULL);
}

/******************************************************************************************************END OF FILE*****/
