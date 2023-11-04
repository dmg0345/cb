/**
 ***********************************************************************************************************************
 * @file        cb.c
 * @author      ${PROJECT_AUTHOR} (${PROJECT_CONTACT})
 * @date        ${PROJECT_BUILD_TIMESTAMP_UTC}
 * @version     ${PROJECT_VERSION}
 * @copyright   ${PROJECT_COPYRIGHT}
 ***********************************************************************************************************************
 */

/** @defgroup cb_iapi_impl Internal API implementation */
/** @defgroup cb_papi_impl Public API implementation */

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "cb/cb.h"
#include <string.h>

/* Private types -----------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup cb_iapi_impl
 * @{
 */

/**
 * @}
 */

/* Private define ----------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup cb_iapi_impl
 * @{
 */

/**
 * @}
 */

/* Private macro -----------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup cb_iapi_impl
 * @{
 */

#ifdef CB_USE_STDATOMIC
/** Critical variable assignment, load and store operations, with atomic support. */
/** @{ */
#define CB_CRIT_VAR_INIT(variable, value)  (atomic_init(&(variable), (value)))
#define CB_CRIT_VAR_LOAD(variable)         (atomic_load(&(variable)))
#define CB_CRIT_VAR_STORE(variable, value) (atomic_store(&(variable), (value)))
/** @} */
#else
/** Critical variable assignment, load and store operations, without atomic support. */
/** @{ */
#define CB_CRIT_VAR_INIT(variable, value)  (variable) = (value)
#define CB_CRIT_VAR_LOAD(variable)         (variable)
#define CB_CRIT_VAR_STORE(variable, value) (variable) = (value)
/** @} */
#endif

/** 
 * @brief Casts a pointer to void to pointer to char for pointer arithmetic in units of one.
 * @param[in] ptr The pointer to void to cast.
 * @return The pointer to void to cast.
 */
#define CB_CAST(ptr) ((char *)(ptr))

/** 
 * @brief Casts a pointer to void to pointer to const char for pointer arithmetic in units of one.
 * @param[in] ptr The pointer to void to cast.
 * @return The pointer to void to cast.
 */
#define CB_CONST_CAST(ptr) ((const char *)(ptr))

/** 
 * @brief Checks if a circular buffer is subscribed to an event or multiple events specified.
 * @param[in] cb The circular buffer.
 * @param[in] evt The event or events to check if subscribed.
 * @return @c true if subscribed to all the events specified and @c false if not.
 */
#define CB_IS_SUB(cb, evt) (((cb)->evt_sub & (evt)) == (evt))

/**
 * @}
 */

/* Private variables -------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup cb_iapi_impl
 * @{
 */

/**
 * @}
 */

/* Private function prototypes ---------------------------------------------------------------------------------------*/
/**
 * @addtogroup cb_iapi_impl
 * @{
 */

/**
 * @brief Triggers a ::cb_evt_id_read event, or falls back to the internal implementation if not subscribed.
 * @param[in] cb Circular buffer context.
 * @param[in] read_ptr The read pointer where to read the data from.
 * @param[in] bytes The number of bytes to read from @p read_ptr and write to @p buffer.
 * @param[in] buffer The buffer where to write data to.
 * @retval ::cb_error_ok Success.
 * @retval ::cb_error_evt The handling of the event resulted in error.
 */
static cb_error_t
    cb_evt_read(const cb_t * const cb, const void * const read_ptr, const size_t bytes, void * const buffer);

/**
 * @brief Triggers a ::cb_evt_id_write event, or falls back to the internal implementation if not subscribed.
 * @param[in] cb Circular buffer context.
 * @param[in] buffer The buffer where to read the data from.
 * @param[in] bytes The number of bytes to read from @p buffer and write to @p write_ptr.
 * @param[in] write_ptr The write pointer where to write the data to.
 * @retval ::cb_error_ok Success.
 * @retval ::cb_error_evt The handling of the event resulted in error.
 */
static cb_error_t
    cb_evt_write(const cb_t * const cb, const void * const buffer, const size_t bytes, void * const write_ptr);

/**
 * @brief Triggers a ::cb_evt_id_lock event, or falls back to the internal implementation if not subscribed.
 * @param[in] cb Circular buffer context.
 */
static void cb_evt_lock(const cb_t * const cb);

/**
 * @brief Triggers a ::cb_evt_id_unlock event, or falls back to the internal implementation if not subscribed.
 * @param[in] cb Circular buffer context.
 */
static void cb_evt_unlock(const cb_t * const cb);

/**
 * @brief Obtains the number of filled slots in the circular buffer.
 * @param[in] cb Circular buffer context.
 * @param[out] felems The number of filled elements from the read index to first write or end index, can be @c NULL.
 * @param[out] selems The number of filled elements from the start index to write index, can be @c NULL.
 * @return The number of filled slots.
 */
static size_t cb_int_get_filled(const cb_t * const cb, size_t * const felems, size_t * const selems);

/**
 * @brief Obtains the number of unfilled slots in the circular buffer.
 * @param[in] cb Circular buffer context.
 * @param[out] felems The number of unfilled elements from the write index to first read or end index, can be @c NULL.
 * @param[out] selems The number of unfilled elements from the start index to read index, can be @c NULL.
 * @return The number of unfilled slots.
 */
static size_t cb_int_get_unfilled(const cb_t * const cb, size_t * const felems, size_t * const selems);

/**
 * @}
 */

/* Private functions -------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup cb_iapi_impl
 * @{
 */

/*--------------------------------------------------------------------------------------------------------------------*/
static cb_error_t
    cb_evt_read(const cb_t * const cb, const void * const read_ptr, const size_t bytes, void * const buffer)
{
    // Check if subscribed to event, and call event handler if so.
    if (CB_IS_SUB(cb, cb_evt_id_read))
    {
        cb_evt_t evt = {
            .cb = cb,
            .user_data = cb->evt_user_data,
            .id = cb_evt_id_read,
            .data.read = {.read_ptr = read_ptr, .bytes = bytes, .buffer = buffer},
        };
        return cb->evt_handler(&evt);
    }

    // Otherwise, use built-in implementation.
    (void)memcpy(buffer, read_ptr, bytes);

    return cb_error_ok;
}

/*--------------------------------------------------------------------------------------------------------------------*/
static cb_error_t
    cb_evt_write(const cb_t * const cb, const void * const buffer, const size_t bytes, void * const write_ptr)
{
    // Check if subscribed to event, and call event handler if so.
    if (CB_IS_SUB(cb, cb_evt_id_write))
    {
        cb_evt_t evt = {
            .cb = cb,
            .user_data = cb->evt_user_data,
            .id = cb_evt_id_write,
            .data.write = {.buffer = buffer, .bytes = bytes, .write_ptr = write_ptr},
        };
        return cb->evt_handler(&evt);
    }

    // Otherwise, use built-in implementation.
    (void)memcpy(write_ptr, buffer, bytes);

    return cb_error_ok;
}

/*--------------------------------------------------------------------------------------------------------------------*/
static void cb_evt_lock(const cb_t * const cb)
{
    if (CB_IS_SUB(cb, cb_evt_id_lock))
    {
        cb_evt_t evt = {
            .cb = cb,
            .user_data = cb->evt_user_data,
            .id = cb_evt_id_lock,
            .data.lock.unused = 0U,
        };
        (void)cb->evt_handler(&evt);
    }

    // Internal implementation assumes no locking mechanisms.
}

/*--------------------------------------------------------------------------------------------------------------------*/
static void cb_evt_unlock(const cb_t * const cb)
{
    if (CB_IS_SUB(cb, cb_evt_id_unlock))
    {
        cb_evt_t evt = {
            .cb = cb,
            .user_data = cb->evt_user_data,
            .id = cb_evt_id_unlock,
            .data.unlock.unused = 0U,
        };
        (void)cb->evt_handler(&evt);
    }

    // Internal implementation assumes no locking mechanisms.
}

/*--------------------------------------------------------------------------------------------------------------------*/
static size_t cb_int_get_filled(const cb_t * const cb, size_t * const felems, size_t * const selems)
{
    // Read critical variables to local ones and perform operation.
    const size_t write_idx = CB_CRIT_VAR_LOAD(cb->write_idx);
    const size_t read_idx = CB_CRIT_VAR_LOAD(cb->read_idx);

    // If empty, then nothing is filled.
    if (write_idx == read_idx)
    {
        *felems = 0U;
        *selems = 0U;
    }
    // If full, everything is filled.
    else if (((write_idx == (cb->buffer_length - 1U)) ? (0U) : (write_idx + 1U)) == read_idx)
    {
        *felems = (read_idx < write_idx) ? ((write_idx) - (read_idx)) : ((cb->buffer_length) - (read_idx));
        *selems = (read_idx > write_idx) ? (write_idx) : (0U);
    }
    // Check if the elements from read index to end index and start index to write index are filled.
    else if (write_idx < read_idx)
    {
        *felems = cb->buffer_length - read_idx;
        *selems = write_idx;
    }
    // Otherwise, the elements between read index and write index are filled.
    else
    {
        *felems = write_idx - read_idx;
        *selems = 0U;
    }

    return *felems + *selems;
}

/*--------------------------------------------------------------------------------------------------------------------*/
static size_t cb_int_get_unfilled(const cb_t * const cb, size_t * const felems, size_t * const selems)
{
    // Read critical variables to local ones and perform operation.
    const size_t write_idx = CB_CRIT_VAR_LOAD(cb->write_idx);
    const size_t read_idx = CB_CRIT_VAR_LOAD(cb->read_idx);
    const size_t read_idx_lim = (read_idx == 0U) ? (cb->buffer_length - 1U) : (read_idx - 1U);

    // If empty, then everything is unfilled.
    if (write_idx == read_idx)
    {
        *felems = (read_idx_lim < write_idx) ? (cb->buffer_length - write_idx) : (read_idx_lim - write_idx);
        *selems = (write_idx > read_idx_lim) ? (read_idx_lim) : (0U);
    }
    // If full, nothing is unfilled.
    else if (write_idx == read_idx_lim)
    {
        *felems = 0U;
        *selems = 0U;
    }
    // Check if the elements from write index to end index and start index to read limit index are filled.
    else if (read_idx_lim < write_idx)
    {
        *felems = cb->buffer_length - write_idx;
        *selems = read_idx_lim;
    }
    // Otherwise, the elements between read limit index and write index are filled.
    else
    {
        *felems = read_idx_lim - write_idx;
        *selems = 0U;
    }

    return *felems + *selems;
}

/**
 * @}
 */

/* Exported functions ------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup cb_papi_impl
 * @{
 */

/*--------------------------------------------------------------------------------------------------------------------*/
cb_error_t cb_init(cb_t * const cb,
                   void * const buffer,
                   const size_t buffer_length,
                   const size_t elem_size,
                   const cb_evt_handler_t evt_handler,
                   const cb_evt_id_t evt_sub,
                   void * const evt_user_data)
{
    // Sanity check on arguments.
    if ((cb == NULL) || (buffer == NULL) || (buffer_length <= 1U) || (elem_size == 0U) ||
        ((evt_sub == cb_evt_id_none) && (evt_handler != NULL)) ||
        ((evt_sub != cb_evt_id_none) && (evt_handler == NULL)))
    {
        return cb_error_invalid_args;
    }

    // Initialize.
    cb->buffer = buffer;
    cb->buffer_length = buffer_length;
    cb->elem_size = elem_size;
    CB_CRIT_VAR_INIT(cb->read_idx, 0U);
    CB_CRIT_VAR_INIT(cb->write_idx, 0U);
    cb->evt_handler = evt_handler;
    cb->evt_sub = evt_sub;
    cb->evt_user_data = evt_user_data;

    return cb_error_ok;
}

/*--------------------------------------------------------------------------------------------------------------------*/
cb_error_t cb_write(cb_t * const cb, const void * const buffer, const size_t count)
{
    // Sanity check for arguments.
    if ((cb == NULL) || (buffer == NULL) || (count == 0U))
    {
        return cb_error_invalid_args;
    }

    // Lock buffer for writing, in a single-producer single-consumer scenario nothing else can be writing by design
    // thus a user provided lock is not necessary, in other scenarios, the user needs to provide a lock to guarantee
    // that multiple threads are not writing at the same time. With this in consideration, we can guarantee that
    // here onwards there will only be a single thread executing the write process.
    cb_evt_lock(cb);

    // Get number of unfilled slots and check if requested amount fits in the buffer, if a read is performed at the
    // same time, this means that more space would become available but has no direct implication on the write as
    // in any case case we are guaranteeing the amount of elements requested for write fit.
    size_t fe = 0U;
    size_t se = 0U;
    size_t we = cb_int_get_unfilled(cb, &fe, &se);
    if (count > we)
    {
        cb_evt_unlock(cb);
        return cb_error_full;
    }
    size_t write_idx = CB_CRIT_VAR_LOAD(cb->write_idx);
    we = count;

    // Perform first write.
    fe = (we > fe) ? (fe) : (we);
    we -= fe;
    fe *= cb->elem_size;
    cb_error_t error = cb_evt_write(cb, buffer, fe, CB_CAST(cb->buffer) + (write_idx * cb->elem_size));
    if (error != cb_error_ok)
    {
        cb_evt_unlock(cb);
        return error;
    }

    // Perform second write if any.
    if (we > 0U)
    {
        // Perform second write from the start index till the read index at most.
        se = we * cb->elem_size;
        error = cb_evt_write(cb, CB_CONST_CAST(buffer) + fe, se, cb->buffer);
        if (error != cb_error_ok)
        {
            cb_evt_unlock(cb);
            return error;
        }
    }

    // Update write index.
    write_idx += count;
    write_idx = (write_idx >= cb->buffer_length) ? (write_idx - cb->buffer_length) : (write_idx);

    // Update buffer details.
    CB_CRIT_VAR_STORE(cb->write_idx, write_idx);

    // Unlock buffer after writing and updating variables.
    cb_evt_unlock(cb);

    return cb_error_ok;
}

/*--------------------------------------------------------------------------------------------------------------------*/
cb_error_t cb_read(cb_t * const cb, void * const buffer, const size_t count)
{
    // Sanity check for arguments.
    if ((cb == NULL) || (buffer == NULL) || (count == 0U))
    {
        return cb_error_invalid_args;
    }

    // Lock buffer for reading, in a single-producer single-consumer scenario nothing else can be reading by design
    // thus a user provided lock is not necessary, in other scenarios, the user needs to provide a lock to guarantee
    // that multiple threads are not reading at the same time. With this in consideration, we can guarantee that
    // here onwards there will only be a single thread executing the read process.
    cb_evt_lock(cb);

    // Get number of filled slots and check if requested amount fits in the buffer, if a write is performed at the
    // same time, this means that more data would become available but has no direct implication on the read as
    // in any case case we are guaranteeing the amount of elements requested for read exist.
    size_t fe = 0U;
    size_t se = 0U;
    size_t re = cb_int_get_filled(cb, &fe, &se);
    if (count > re)
    {
        cb_evt_unlock(cb);
        return cb_error_empty;
    }
    size_t read_idx = CB_CRIT_VAR_LOAD(cb->read_idx);
    re = count;

    // Perform first read.
    fe = (re > fe) ? (fe) : (re);
    re -= fe;
    fe *= cb->elem_size;
    cb_error_t error = cb_evt_read(cb, CB_CAST(cb->buffer) + (read_idx * cb->elem_size), fe, buffer);
    if (error != cb_error_ok)
    {
        cb_evt_unlock(cb);
        return error;
    }

    // Perform second read if any.
    if (re > 0U)
    {
        se = re * cb->elem_size;
        error = cb_evt_read(cb, cb->buffer, se, CB_CAST(buffer) + fe);
        if (error != cb_error_ok)
        {
            cb_evt_unlock(cb);
            return error;
        }
    }

    // Update write index.
    read_idx += count;
    read_idx = (read_idx >= cb->buffer_length) ? (read_idx - cb->buffer_length) : (read_idx);

    // Update buffer details.
    CB_CRIT_VAR_STORE(cb->read_idx, read_idx);

    // Unlock buffer after writing and updating variables.
    cb_evt_unlock(cb);

    return cb_error_ok;
}

/*--------------------------------------------------------------------------------------------------------------------*/
cb_error_t cb_get_unfilled(cb_t * const cb, size_t * const count)
{
    // Sanity check on arguments.
    if ((cb == NULL) || (count == NULL))
    {
        return cb_error_invalid_args;
    }

    // Lock.
    cb_evt_lock(cb);
    // Get number of unfilled slots.
    size_t felems = 0U;
    size_t selems = 0U;
    *count = cb_int_get_unfilled(cb, &felems, &selems);
    // Unlock.
    cb_evt_unlock(cb);

    return cb_error_ok;
}

/*--------------------------------------------------------------------------------------------------------------------*/
cb_error_t cb_get_filled(cb_t * const cb, size_t * const count)
{
    // Sanity check on arguments.
    if ((cb == NULL) || (count == NULL))
    {
        return cb_error_invalid_args;
    }

    // Lock.
    cb_evt_lock(cb);
    // Get number of filled slots.
    size_t felems = 0U;
    size_t selems = 0U;
    *count = cb_int_get_filled(cb, &felems, &selems);
    // Unlock.
    cb_evt_unlock(cb);

    return cb_error_ok;
}

/*--------------------------------------------------------------------------------------------------------------------*/
cb_error_t cb_is_empty(cb_t * const cb, bool * const is_empty)
{
    // Sanity check on arguments.
    if ((cb == NULL) || (is_empty == NULL))
    {
        return cb_error_invalid_args;
    }

    // Lock.
    cb_evt_lock(cb);
    // Check if number of filled slots is zero to determine empty.
    size_t felems = 0U;
    size_t selems = 0U;
    *is_empty = (cb_int_get_filled(cb, &felems, &selems) == 0U);
    // Unlock.
    cb_evt_unlock(cb);

    return cb_error_ok;
}

/*--------------------------------------------------------------------------------------------------------------------*/
cb_error_t cb_is_full(cb_t * const cb, bool * const is_full)
{
    // Sanity check on arguments.
    if ((cb == NULL) || (is_full == NULL))
    {
        return cb_error_invalid_args;
    }

    // Lock.
    cb_evt_lock(cb);
    // Check if number of unfilled slots is zero to determine full.
    size_t felems = 0U;
    size_t selems = 0U;
    *is_full = (cb_int_get_unfilled(cb, &felems, &selems) == 0U);
    // Unlock.
    cb_evt_unlock(cb);

    return cb_error_ok;
}

/*--------------------------------------------------------------------------------------------------------------------*/
cb_error_t cb_deinit(cb_t * const cb)
{
    // Sanity check for arguments.
    if (cb == NULL)
    {
        return cb_error_invalid_args;
    }

    // Deinitialize.
    cb->buffer = NULL;
    cb->buffer_length = 0U;
    cb->elem_size = 0U;
    CB_CRIT_VAR_STORE(cb->read_idx, 0U);
    CB_CRIT_VAR_STORE(cb->write_idx, 0U);
    cb->evt_handler = NULL;
    cb->evt_sub = cb_evt_id_none;
    cb->evt_user_data = NULL;

    return cb_error_ok;
}

/**
 * @}
 */

/******************************************************************************************************END OF FILE*****/
