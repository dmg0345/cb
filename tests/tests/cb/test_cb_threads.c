/**
 ***********************************************************************************************************************
 * @file        test_cb_threads.c
 * @author      Diego Martínez García (dmg0345@gmail.com)
 * @date        04-11-2023 19:37:00 (UTC)
 * @version     1.0.0
 * @copyright   github.com/dmg0345/cb/blob/master/LICENSE
 ***********************************************************************************************************************
 */

/** @defgroup cb_threads_tests Tests */

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include <pthread.h>
#include "cmocka_defs.h"
#include "test_types.h"
#include "cb/cb.h"

/* Private types -----------------------------------------------------------------------------------------------------*/
/* Private define ----------------------------------------------------------------------------------------------------*/
/** Number of loops to perform on the producer and consumer threads. */
#define THREAD_LOOPS        ((size_t)(10000U))
/** Number of elements to write and read in producer and consumer threads. */
#define THREAD_ELEM_COUNT   ((size_t)(2U))
/** Total checksum value for first producer-consumer thread pair. */
#define FIRST_CHECKSUM_VAL  ((size_t)(THREAD_LOOPS * 0x37U))
/** Total check sum value for second producer-consumer thread pair. */
#define SECOND_CHECKSUM_VAL ((size_t)(THREAD_LOOPS * 0xD7U))

/* Private macro -----------------------------------------------------------------------------------------------------*/
/* Private variables -------------------------------------------------------------------------------------------------*/
/** Underlying linear buffer for the circular buffer, with extra element first and last. */
static test_type_t lcbuf[12U + 1U];
/** Destination buffer, to be used for read operations in the circular buffer for consumer threads. */
/** @{ */
static test_type_t ldbuf_one[12U];
static test_type_t ldbuf_two[12U];
/** @} */
/** Source buffer, to be used for write operations in the circular buffer for producer threads. */
/** @{ */
static const test_type_t lsbuf_one[10U] = {0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U, 0x09U, 0x0AU};
static const test_type_t lsbuf_two[10U] = {0x11U, 0x12U, 0x13U, 0x14U, 0x15U, 0x16U, 0x17U, 0x18U, 0x19U, 0x1AU};
/** @} */
/** Producer thread identifiers. */
/** @{ */
static pthread_t pt_id_one;
static pthread_t pt_id_two;
/** @} */
/** Consumer thread identifiers. */
/** @{ */
static pthread_t ct_id_one;
static pthread_t ct_id_two;
/** @} */
/** Number of producer and consumer threads. */
/** @{ */
static uint8_t pt_num;
static uint8_t ct_num;
/** @} */
/* Checksum values for the consumer threads. */
/** @{ */
static size_t ct_checksum_one = 0U;
static size_t ct_checksum_two = 0U;
/** @} */
/** Circular buffer mutex. */
static pthread_mutex_t cb_mutex;
/** Circular buffer. */
static cb_t cbuf;

/* Private function prototypes ---------------------------------------------------------------------------------------*/
/** Suite setup function. */
static int setup(void ** state);
/** Suite teardown function. */
static int teardown(void ** state);
/** Circular buffer event handler. */
static cb_error_t cb_evt_handler(cb_evt_t * const evt);
/** Functions for the producer threads. */
/** @{ */
static void * pt_func_one(void * ptr);
static void * pt_func_two(void * ptr);
/** @} */
/** Function for consumer threads. */
/** @{ */
static void * ct_func_one(void * ptr);
static void * ct_func_two(void * ptr);
/** @} */

/**
 * @addtogroup cb_threads_tests
 * @{
 */

/** Tests the circular buffer with a single producer and single consumer, without mutex, relies on atomic ops. */
static void test_cb_threads_onep_onec_atomic(void ** state);
/** Tests the circular buffer with a single producer and single consumer. */
static void test_cb_threads_onep_onec(void ** state);
/** Tests the circular buffer with a single producer and two consumers. */
static void test_cb_threads_onep_twoc(void ** state);
/** Tests the circular buffer with two producers and a single consumer. */
static void test_cb_threads_twop_onec(void ** state);
/** Tests the circular buffer with a two producers and two consumers. */
static void test_cb_threads_twop_twoc(void ** state);

/**
 * @}
 */

/* Private functions -------------------------------------------------------------------------------------------------*/
static int setup(void ** state)
{
    // Initialize mutexes.
    pthread_mutex_init(&cb_mutex, NULL);

    // Initialize thread data.
    pt_id_one = 0U;
    pt_id_two = 0U;
    ct_id_one = 0U;
    ct_id_two = 0U;
    ct_checksum_one = 0U;
    ct_checksum_two = 0U;
    pt_num = 0U;
    ct_num = 0U;

    // Initialize linear buffers and assert their sizes.
    (void)memset(lcbuf, 0xFFU, sizeof(lcbuf));
    (void)memset(ldbuf_one, 0xFFU, sizeof(ldbuf_one));
    (void)memset(ldbuf_two, 0xFFU, sizeof(ldbuf_two));
    assert_int_equal(ARRAY_DIM(lsbuf_one) + 2U, ARRAY_DIM(ldbuf_one));
    assert_int_equal(ARRAY_DIM(lsbuf_one) + 2U, ARRAY_DIM(ldbuf_two));
    assert_int_equal(ARRAY_DIM(lsbuf_two) + 2U, ARRAY_DIM(ldbuf_one));
    assert_int_equal(ARRAY_DIM(lsbuf_two) + 2U, ARRAY_DIM(ldbuf_two));
    assert_int_equal(ARRAY_DIM(lsbuf_one) + 2U, ARRAY_DIM(lcbuf) - 1U);
    assert_int_equal(ARRAY_DIM(lsbuf_two) + 2U, ARRAY_DIM(lcbuf) - 1U);
    assert_int_equal(ARRAY_DIM(lsbuf_one) % THREAD_ELEM_COUNT, 0U);
    assert_int_equal(ARRAY_DIM(lsbuf_two) % THREAD_ELEM_COUNT, 0U);

    // Initialize circular buffer, leave one element first and at the end for checking out of bounds writes.
    assert_int_equal(cb_init(&cbuf,
                             lcbuf + 1U,
                             ARRAY_DIM(lcbuf) - 2U,
                             sizeof(*lcbuf),
                             cb_evt_handler,
                             cb_evt_id_lock | cb_evt_id_unlock,
                             NULL),
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
    (void)memset(ldbuf_one, 0xFFU, sizeof(ldbuf_one));
    (void)memset(ldbuf_two, 0xFFU, sizeof(ldbuf_two));

    // Deinitialize thread data.
    pt_id_one = 0U;
    pt_id_two = 0U;
    ct_id_one = 0U;
    ct_id_two = 0U;
    ct_checksum_one = 0U;
    ct_checksum_two = 0U;
    pt_num = 0U;
    ct_num = 0U;

    // Deinitialize mutex.
    pthread_mutex_destroy(&cb_mutex);

    // Clear state.
    *state = NULL;

    return CMOCKA_OK;
}

/*--------------------------------------------------------------------------------------------------------------------*/
static cb_error_t cb_evt_handler(cb_evt_t * const evt)
{
    switch (evt->id)
    {
        case cb_evt_id_lock:
        {
            pthread_mutex_lock(&cb_mutex);
        }
        break;

        case cb_evt_id_unlock:
        {
            pthread_mutex_unlock(&cb_mutex);
        }
        break;

        default:
        {
            return cb_error_evt;
        }
        break;
    }

    return cb_error_ok;
}

/*--------------------------------------------------------------------------------------------------------------------*/
static void * pt_func_one(void * ptr)
{
    cb_t * const cb = (cb_t * const)ptr;

    // If the number of producer threads is one and consumer threads is two, then write twice the values.
    size_t pt_mult = 1U;
    if ((pt_num == 1U) && (ct_num == 2U))
    {
        pt_mult = 2U;
    }

    // Write the entire source buffer in each loop.
    for (size_t loop = 0U; loop < (THREAD_LOOPS * pt_mult); loop++)
    {
        // Write buffer in the specified steps.
        size_t item = 0U;
        while (item < ARRAY_DIM(lsbuf_one))
        {
            // If write is successful, then increase items for next run.
            if (cb_write(cb, &lsbuf_one[item], THREAD_ELEM_COUNT) == cb_error_ok)
            {
                // Ensure there was no out of bounds write in the circular buffer.
                assert_int_equal(lcbuf[0U], TEST_CLEAR_VALUE);
                assert_int_equal(lcbuf[ARRAY_DIM(lcbuf) - 1U], TEST_CLEAR_VALUE);
                // Increase count.
                item += THREAD_ELEM_COUNT;
            }
        }
    }

    return NULL;
}

/*--------------------------------------------------------------------------------------------------------------------*/
static void * pt_func_two(void * ptr)
{
    cb_t * const cb = (cb_t * const)ptr;

    // If the number of producer threads is one and consumer threads is two, then write twice the values.
    size_t pt_mult = 1U;
    if ((pt_num == 1U) && (ct_num == 2U))
    {
        pt_mult = 2U;
    }

    // Write the entire source buffer in each loop.
    for (size_t loop = 0U; loop < (THREAD_LOOPS * pt_mult); loop++)
    {
        // Write buffer in the specified steps.
        size_t item = 0U;
        while (item < ARRAY_DIM(lsbuf_two))
        {
            // If write is successful, then increase items for next run.
            if (cb_write(cb, &lsbuf_two[item], THREAD_ELEM_COUNT) == cb_error_ok)
            {
                // Ensure there was no out of bounds write in the circular buffer.
                assert_int_equal(lcbuf[0U], TEST_CLEAR_VALUE);
                assert_int_equal(lcbuf[ARRAY_DIM(lcbuf) - 1U], TEST_CLEAR_VALUE);
                // Increase count.
                item += THREAD_ELEM_COUNT;
            }
        }
    }

    return NULL;
}

/*--------------------------------------------------------------------------------------------------------------------*/
static void * ct_func_one(void * ptr)
{
    cb_t * const cb = (cb_t * const)ptr;

    // If the number of producer threads is two and consumer threads is one, then read twice the values.
    size_t ct_mult = 1U;
    if ((pt_num == 2U) && (ct_num == 1U))
    {
        ct_mult = 2U;
    }

    // Read the entire source buffer in each loop.
    for (size_t loop = 0U; loop < (THREAD_LOOPS * ct_mult); loop++)
    {
        // Read buffer in the specified steps.
        size_t item = 0U;
        while (item < ARRAY_DIM(lsbuf_one))
        {
            // If read is successful, then increase items for next run.
            if (cb_read(cb, &ldbuf_one[item + 1U], THREAD_ELEM_COUNT) == cb_error_ok)
            {
                // Ensure no out of bounds write in the circular buffer.
                assert_int_equal(ldbuf_one[0U], TEST_CLEAR_VALUE);
                assert_int_equal(ldbuf_one[ARRAY_DIM(ldbuf_one) - 1U], TEST_CLEAR_VALUE);
                // Increment checksum value for consumer thread.
                for (size_t i = 0U; i < THREAD_ELEM_COUNT; i++)
                {
                    ct_checksum_one += ldbuf_one[item + 1U + i];
                }
                // Increase count.
                item += THREAD_ELEM_COUNT;
            }
        }
        // Clear buffer for next run.
        (void)memset(ldbuf_one, 0xFFU, sizeof(ldbuf_one));
    }

    return NULL;
}

/*--------------------------------------------------------------------------------------------------------------------*/
static void * ct_func_two(void * ptr)
{
    cb_t * const cb = (cb_t * const)ptr;

    // If the number of producer threads is two and consumer threads is one, then read twice the values.
    size_t ct_mult = 1U;
    if ((pt_num == 2U) && (ct_num == 1U))
    {
        ct_mult = 2U;
    }

    // Read the entire source buffer in each loop.
    for (size_t loop = 0U; loop < (THREAD_LOOPS * ct_mult); loop++)
    {
        // Read buffer in the specified steps.
        size_t item = 0U;
        while (item < ARRAY_DIM(lsbuf_two))
        {
            // If read is successful, then increase items for next run.
            if (cb_read(cb, &ldbuf_two[item + 1U], THREAD_ELEM_COUNT) == cb_error_ok)
            {
                // Ensure no out of bounds write in the circular buffer.
                assert_int_equal(ldbuf_two[0U], TEST_CLEAR_VALUE);
                assert_int_equal(ldbuf_two[ARRAY_DIM(ldbuf_two) - 1U], TEST_CLEAR_VALUE);
                // Increment checksum value for consumer thread.
                for (size_t i = 0U; i < THREAD_ELEM_COUNT; i++)
                {
                    ct_checksum_two += ldbuf_two[item + 1U + i];
                }
                // Increase count.
                item += THREAD_ELEM_COUNT;
            }
        }
        // Clear buffer for next run.
        (void)memset(ldbuf_two, 0xFFU, sizeof(ldbuf_two));
    }

    return NULL;
}

/*--------------------------------------------------------------------------------------------------------------------*/
static void test_cb_threads_onep_onec_atomic(void ** state)
{
    cb_t * const cb = (cb_t * const)*state;

    // Unsubscribe from lock and unlock events, and instead rely on atomic operations.
    cb->evt_handler = NULL;
    cb->evt_sub = cb_evt_id_none;
    // Set number of threads.
    ct_num = 1U;
    pt_num = 1U;
    // Create consumer threads first.
    assert_int_equal(pthread_create(&ct_id_one, NULL, ct_func_one, cb), 0U);
    // Create producer threads second
    assert_int_equal(pthread_create(&pt_id_one, NULL, pt_func_one, cb), 0U);

    // Wait for the threads to finish.
    pthread_join(pt_id_one, NULL);
    pthread_join(ct_id_one, NULL);

    // Check checksum value.
    const size_t act_checksum_val = ct_checksum_one;
    const size_t exp_checksum_val = FIRST_CHECKSUM_VAL;
    assert_int_equal(act_checksum_val, exp_checksum_val);
}

/*--------------------------------------------------------------------------------------------------------------------*/
static void test_cb_threads_onep_onec(void ** state)
{
    cb_t * const cb = (cb_t * const)*state;

    // Set number of threads.
    ct_num = 1U;
    pt_num = 1U;
    // Create consumer threads first.
    assert_int_equal(pthread_create(&ct_id_one, NULL, ct_func_one, cb), 0U);
    // Create producer threads second
    assert_int_equal(pthread_create(&pt_id_one, NULL, pt_func_one, cb), 0U);

    // Wait for the threads to finish.
    pthread_join(pt_id_one, NULL);
    pthread_join(ct_id_one, NULL);

    // Check checksum value.
    const size_t act_checksum_val = ct_checksum_one;
    const size_t exp_checksum_val = FIRST_CHECKSUM_VAL;
    assert_int_equal(act_checksum_val, exp_checksum_val);
}

/*--------------------------------------------------------------------------------------------------------------------*/
static void test_cb_threads_onep_twoc(void ** state)
{
    cb_t * const cb = (cb_t * const)*state;

    // Set number of threads.
    ct_num = 2U;
    pt_num = 1U;
    // Create consumer threads first.
    assert_int_equal(pthread_create(&ct_id_one, NULL, ct_func_one, cb), 0U);
    assert_int_equal(pthread_create(&ct_id_two, NULL, ct_func_two, cb), 0U);
    // Create producer threads second
    assert_int_equal(pthread_create(&pt_id_one, NULL, pt_func_one, cb), 0U);

    // Wait for the threads to finish.
    pthread_join(pt_id_one, NULL);
    pthread_join(ct_id_one, NULL);
    pthread_join(ct_id_two, NULL);

    // Check checksum value.
    const size_t act_checksum_val = ct_checksum_one + ct_checksum_two;
    const size_t exp_checksum_val = FIRST_CHECKSUM_VAL + FIRST_CHECKSUM_VAL;
    assert_int_equal(act_checksum_val, exp_checksum_val);
}

/*--------------------------------------------------------------------------------------------------------------------*/
static void test_cb_threads_twop_onec(void ** state)
{
    cb_t * const cb = (cb_t * const)*state;

    // Set number of threads.
    ct_num = 1U;
    pt_num = 2U;
    // Create consumer threads first.
    assert_int_equal(pthread_create(&ct_id_one, NULL, ct_func_one, cb), 0U);
    // Create producer threads second
    assert_int_equal(pthread_create(&pt_id_one, NULL, pt_func_one, cb), 0U);
    assert_int_equal(pthread_create(&pt_id_two, NULL, pt_func_two, cb), 0U);

    // Wait for the threads to finish.
    pthread_join(pt_id_one, NULL);
    pthread_join(pt_id_two, NULL);
    pthread_join(ct_id_one, NULL);

    // Check checksum value.
    const size_t act_checksum_val = ct_checksum_one;
    const size_t exp_checksum_val = FIRST_CHECKSUM_VAL + SECOND_CHECKSUM_VAL;
    assert_int_equal(act_checksum_val, exp_checksum_val);
}

/*--------------------------------------------------------------------------------------------------------------------*/
static void test_cb_threads_twop_twoc(void ** state)
{
    cb_t * const cb = (cb_t * const)*state;

    // Set number of threads.
    ct_num = 2U;
    pt_num = 2U;
    // Create consumer threads first.
    assert_int_equal(pthread_create(&ct_id_one, NULL, ct_func_one, cb), 0U);
    assert_int_equal(pthread_create(&ct_id_two, NULL, ct_func_two, cb), 0U);
    // Create producer threads second
    assert_int_equal(pthread_create(&pt_id_one, NULL, pt_func_one, cb), 0U);
    assert_int_equal(pthread_create(&pt_id_two, NULL, pt_func_two, cb), 0U);

    // Wait for the threads to finish.
    pthread_join(pt_id_one, NULL);
    pthread_join(pt_id_two, NULL);
    pthread_join(ct_id_one, NULL);
    pthread_join(ct_id_two, NULL);

    // Check checksum value.
    const size_t act_checksum_val = ct_checksum_one + ct_checksum_two;
    const size_t exp_checksum_val = FIRST_CHECKSUM_VAL + SECOND_CHECKSUM_VAL;
    assert_int_equal(act_checksum_val, exp_checksum_val);
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
        cmocka_unit_test_setup_teardown(test_cb_threads_onep_onec_atomic, setup, teardown),
        cmocka_unit_test_setup_teardown(test_cb_threads_onep_onec, setup, teardown),
        cmocka_unit_test_setup_teardown(test_cb_threads_onep_twoc, setup, teardown),
        cmocka_unit_test_setup_teardown(test_cb_threads_twop_onec, setup, teardown),
        cmocka_unit_test_setup_teardown(test_cb_threads_twop_twoc, setup, teardown),
    };

    // Execute the test runner.
    return cmocka_run_group_tests_name("cb_threads", tests, NULL, NULL);
}

/******************************************************************************************************END OF FILE*****/
