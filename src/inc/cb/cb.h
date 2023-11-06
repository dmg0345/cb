/**
 ***********************************************************************************************************************
 * @file        cb.h
 * @author      ${PROJECT_AUTHOR} (${PROJECT_CONTACT})
 * @date        ${PROJECT_BUILD_TIMESTAMP_UTC}
 * @version     ${PROJECT_VERSION}
 * @copyright   ${PROJECT_COPYRIGHT}
 ***********************************************************************************************************************
 */

/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/
#ifndef CB_H
#define CB_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup cb Circular Buffer library
 *
 * Provides a circular buffer with read and write pointers that can accomodate types of multiple sizes.
 *
 * @{
 */

/** @defgroup cb_defs Definitions */
/** @defgroup cb_papi Public API */
/** @defgroup cb_iapi_impl Internal API implementation */
/** @defgroup cb_papi_impl Public API implementation */

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "cb/other/version.h"
#include <stddef.h>
#include <stdbool.h>

// if __STDC_NO_ATOMICS__ is defined, then stdatomic is not supported.
// If using clangd or clang-tidy, do not enable atomics, as they raise clang-diagnostic-error which can't be suppressed.
#if defined(__STDC_NO_ATOMICS__) || defined(__clang__)
#else
#include <stdatomic.h>
#define CB_USE_STDATOMIC
#endif

/* Exported types ----------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup cb_defs
 * @{
 */

/** Errors returned by this library. */
typedef enum
{
    cb_error_ok = 0U, /**< Success, no error. */
    cb_error_invalid_args, /**< At least one of the arguments provided is invalid. */
    cb_error_full, /**< The circular buffer is full or the specified number of elements do not fit in it. */
    cb_error_empty, /**< The circular buffer is empty of the specified number of elements do not exist in it. */
    cb_error_evt, /**< The handling of an event in the user provided event handler resulted in error. */

    cb_error_count /**< Number of errors. */
} cb_error_t;

/** Event identifier. */
typedef enum
{
    cb_evt_id_none = 0x00000000U, /**< No event, used to subscribe to no events. */
    cb_evt_id_read = 0x00000001U, /**< Request to read underlying linear buffer of the circular buffer. */
    cb_evt_id_write = 0x00000002U, /**< Request to write underlying linear buffer of the circular buffer. */
    cb_evt_id_lock = 0x00000004U, /**< Request to lock circular buffer, this event can't return ::cb_error_evt. */
    cb_evt_id_unlock = 0x00000008U /**< Request to unlock circular buffer, this event can't return ::cb_error_evt. */
} cb_evt_id_t;

/** Unused event data, used for events that do not have any data. */
typedef struct
{
    size_t unused; /**< The event does not have any data. */
} cb_evt_data_unused_t;

/** Event data for ::cb_evt_id_read event. */
typedef struct
{
    const void * read_ptr; /**< The read pointer where to read the data from. */
    size_t bytes; /**< The number of bytes to read from @c read_ptr and write to @c buffer. */
    void * buffer; /**< The buffer where to write data to. */
} cb_evt_data_read_t;

/** Event data for ::cb_evt_id_write event. */
typedef struct
{
    const void * buffer; /**< Buffer The buffer where to read the data from. */
    size_t bytes; /**< The number of bytes to read from @c buffer and write to @c write_ptr. */
    void * write_ptr; /**< The write pointer where to write the data to. */
} cb_evt_data_write_t;

/** Event data for ::cb_evt_id_lock event. */
typedef cb_evt_data_unused_t cb_evt_data_lock_t;

/** Event data for ::cb_evt_id_unlock event. */
typedef cb_evt_data_unused_t cb_evt_data_unlock_t;

/** Event data. */
typedef union
{
    cb_evt_data_read_t read; /**< Event data for ::cb_evt_id_read event. */
    cb_evt_data_write_t write; /**< Event data for ::cb_evt_id_write event. */
    cb_evt_data_lock_t lock; /**< Event data for ::cb_evt_id_lock event. */
    cb_evt_data_unlock_t unlock; /**< Event data for ::cb_evt_id_unlock event. */
} cb_evt_data_t;

/** Event. */
typedef struct
{
    const struct cb_s * cb; /**< The circular buffer that trigerred the event. */
    void * user_data; /**< The user data passed during initialization of the circular buffer. */
    cb_evt_id_t id; /** The event identifier. */
    cb_evt_data_t data; /** The event data, depends on @c id event identifier. */
} cb_evt_t;

/**
 * @brief Event handler for the circular buffer.
 * @param[in] evt The event.
 * @retval ::cb_error_ok Success.
 * @retval ::cb_error_evt An error ocurred in the event handler processing an event.
 */
typedef cb_error_t (*cb_evt_handler_t)(cb_evt_t * const evt);

/**
 * @brief Circular buffer context.
 *
 * The underlying linear buffer must have an additional element more than the intended size to determine if
 * full or empty, in other words, an additional element is used instead of a full or empty flag.
 *
 * The buffer is determined to be full when the write or head index is one slot behind the read or tail index.
 * The buffer is determined to be empty when the write or head index is at the same slot as the read or tail index.
 *
 * User should not modify nor access the members of this structure directly, only through the API in this library.
 */
typedef struct cb_s
{
    void * buffer; /**< The underlying linear buffer on which the circular buffer operates. */
    size_t buffer_length; /**< The size of @c buffer in number of elements of size @c elem_size. */
    size_t elem_size; /**< The size of each element in @c buffer. */
#ifdef CB_USE_STDATOMIC
    atomic_size_t read_idx; /**< The atomic read or tail index, goes from 0 to <tt>buffer_length - 1</tt>. */
    atomic_size_t write_idx; /**< The atomic write or head index, goes from 0 to <tt>buffer_length - 1</tt>. */
#else
    size_t read_idx; /**< The read or tail index, goes from 0 to <tt>buffer_length - 1</tt>. */
    size_t write_idx; /**< The write or head index, goes from 0 to <tt>buffer_length - 1</tt>. */
#endif
    cb_evt_handler_t evt_handler; /**< Event handler, can be @c NULL if not suscribed to events. */
    cb_evt_id_t evt_sub; /**< Suscribed events, OR combination of ::cb_evt_id_t or ::cb_evt_id_none. */
    void * evt_user_data; /**< Event handler user data, will be passed to @c evt_handler when trigerred. */
} cb_t;

/**
 * @}
 */

/* Exported constants ------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup cb_defs
 * @{
 */

/**
 * @}
 */

/* Exported macro ----------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup cb_papi
 * @{
 */

/**
 * @}
 */

/* Exported functions ------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup cb_papi
 * @{
 */

/**
 * @brief Initializes a circular buffer.
 * @param[in] cb The circular buffer context to initialize.
 * @param[in] buffer The underlying linear buffer for the circular buffer, with one extra element than intended size.
 * @param[in] buffer_length The size of @c buffer in number of elements of size @c elem_size.
 * @param[in] elem_size The size of each element in @c buffer.
 * @param[in] evt_handler Event handler, can be @c NULL if not suscribed to events.
 * @param[in] evt_sub Suscribed events, OR combination of ::cb_evt_id_t or ::cb_evt_id_none.
 * @param[in] evt_user_data Event handler user data, will be passed to @c evt_handler when trigerred.
 * @retval ::cb_error_ok Success.
 * @retval ::cb_error_invalid_args At least one of the arguments provided is invalid.
 */
cb_error_t cb_init(cb_t * const cb,
                   void * const buffer,
                   const size_t buffer_length,
                   const size_t elem_size,
                   const cb_evt_handler_t evt_handler,
                   const cb_evt_id_t evt_sub,
                   void * const evt_user_data);

/**
 * @brief Writes the specified number of elements to the circular buffer.
 *
 * If all the elements do not fit, nothing is written to the circular buffer.
 * @param[in] cb The initialized circular buffer context.
 * @param[in] buffer The buffer with the elements to write to @p cb.
 * @param[in] count The number of elements in @p buffer.
 * @retval ::cb_error_ok Success.
 * @retval ::cb_error_invalid_args At least one of the arguments provided is invalid.
 * @retval ::cb_error_full The circular buffer is full or can't fit @p count elements.
 * @retval ::cb_error_evt An error ocurred in the event handler.
 */
cb_error_t cb_write(cb_t * const cb, const void * const buffer, const size_t count);

/**
 * @brief Reads the specified number of elements from the circular buffer.
 *
 * If the circular buffer does not have the specified number of elements, nothing is read from it.
 * @param[in] cb The initialized circular buffer context.
 * @param[in] buffer The buffer where the elements read from @p cb will be written to.
 * @param[in] count The number of elements to read from @p cb.
 * @retval ::cb_error_ok Success.
 * @retval ::cb_error_invalid_args At least one of the arguments provided is invalid.
 * @retval ::cb_error_empty The circular buffer is empty or does not have @p count elements.
 * @retval ::cb_error_evt An error ocurred in the event handler.
 */
cb_error_t cb_read(cb_t * const cb, void * const buffer, const size_t count);

/**
 * @brief Gets the number of elements that can be written to the buffer before it becomes full.
 * @param[in] cb The initialized circular buffer context.
 * @param[out] count The number of unfilled slots in @p cb.
 * @retval ::cb_error_ok Success.
 * @retval ::cb_error_invalid_args At least one of the arguments provided is invalid.
 */
cb_error_t cb_get_unfilled(cb_t * const cb, size_t * const count);

/**
 * @brief Gets the number of elements that can be read from the buffer before it becomes empty.
 * @param[in] cb The initialized circular buffer context.
 * @param[out] count The number of filled slots in @p cb.
 * @retval ::cb_error_ok Success.
 * @retval ::cb_error_invalid_args At least one of the arguments provided is invalid.
 */
cb_error_t cb_get_filled(cb_t * const cb, size_t * const count);

/**
 * @brief Determines if a buffer is empty and no more data can be written to it.
 * @param[in] cb The initialized circular buffer context.
 * @param[out] is_empty On success, @c true if empty and @c false if full.
 * @retval ::cb_error_ok Success.
 * @retval ::cb_error_invalid_args At least one of the arguments provided is invalid.
 */
cb_error_t cb_is_empty(cb_t * const cb, bool * const is_empty);

/**
 * @brief Determines if a buffer is full and no more data can be written to it.
 * @param[in] cb The initialized circular buffer context.
 * @param[out] is_full On success, @c true if full and @c false if empty.
 * @retval ::cb_error_ok Success.
 * @retval ::cb_error_invalid_args At least one of the arguments provided is invalid.
 */
cb_error_t cb_is_full(cb_t * const cb, bool * const is_full);

/**
 * @brief Deinitializes a circular buffer.
 * @param[in] cb The circular buffer context to initialize.
 * @retval ::cb_error_ok Success.
 * @retval ::cb_error_invalid_args At least one of the arguments provided is invalid.
 */
cb_error_t cb_deinit(cb_t * const cb);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CB_H */

/******************************************************************************************************END OF FILE*****/
