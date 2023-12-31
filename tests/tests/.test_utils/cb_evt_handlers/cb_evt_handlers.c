/**
 ***********************************************************************************************************************
 * @file        cb_evt_handlers.c
 * @author      Diego Martínez García (dmg0345@gmail.com)
 * @date        04-11-2023 21:33:15 (UTC)
 * @version     1.0.0
 * @copyright   github.com/dmg0345/cb/blob/master/LICENSE
 ***********************************************************************************************************************
 */

/** @defgroup cb_evt_handlers_iapi_impl Internal API implementation */
/** @defgroup cb_evt_handlers_papi_impl Public API implementation */

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "cmocka_defs.h"
#include "cb_evt_handlers/cb_evt_handlers.h"

/* Private types -----------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup cb_evt_handlers_iapi_impl
 * @{
 */

/**
 * @}
 */

/* Private define ----------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup cb_evt_handlers_iapi_impl
 * @{
 */

/**
 * @}
 */

/* Private macro -----------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup cb_evt_handlers_iapi_impl
 * @{
 */

/**
 * @}
 */

/* Private variables -------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup cb_evt_handlers_iapi_impl
 * @{
 */

/**
 * @}
 */

/* Private function prototypes ---------------------------------------------------------------------------------------*/
/**
 * @addtogroup cb_evt_handlers_iapi_impl
 * @{
 */

/**
 * @}
 */

/* Private functions -------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup cb_evt_handlers_iapi_impl
 * @{
 */

/**
 * @}
 */

/* Exported functions ------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup cb_evt_handlers_papi_impl
 * @{
 */

/*--------------------------------------------------------------------------------------------------------------------*/
cb_error_t cb_evt_handler(cb_evt_t * const evt)
{
    switch (evt->id)
    {
        case cb_evt_id_read:
        {
            (void)memcpy(evt->data.read.buffer, evt->data.read.read_ptr, evt->data.read.bytes);
        }
        break;

        case cb_evt_id_write:
        {
            (void)memcpy(evt->data.write.write_ptr, evt->data.write.buffer, evt->data.write.bytes);
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
cb_error_t cb_evt_handler_write_first_operation_error(cb_evt_t * const evt)
{
    assert_int_equal(evt->id, cb_evt_id_write);

    return cb_error_evt;
}

/*--------------------------------------------------------------------------------------------------------------------*/
cb_error_t cb_evt_handler_read_first_operation_error(cb_evt_t * const evt)
{
    assert_int_equal(evt->id, cb_evt_id_read);

    return cb_error_evt;
}

/*--------------------------------------------------------------------------------------------------------------------*/
cb_error_t cb_evt_handler_write_second_operation_error(cb_evt_t * const evt)
{
    assert_int_equal(evt->id, cb_evt_id_write);

    static size_t write_fn_call_count = 0U;

    // On second operation, return error.
    if ((++write_fn_call_count) >= 2U)
    {
        write_fn_call_count = 0U;
        return cb_error_evt;
    }

    // On first operation, perform it and return success.
    (void)memcpy(evt->data.write.write_ptr, evt->data.write.buffer, evt->data.write.bytes);

    return cb_error_ok;
}

/*--------------------------------------------------------------------------------------------------------------------*/
cb_error_t cb_evt_handler_read_second_operation_error(cb_evt_t * const evt)
{
    assert_int_equal(evt->id, cb_evt_id_read);

    static size_t read_fn_call_count = 0U;

    // On second operation, return error.
    if ((++read_fn_call_count) >= 2U)
    {
        read_fn_call_count = 0U;
        return cb_error_evt;
    }

    // On first operation, perform it and return success.
    (void)memcpy(evt->data.read.buffer, evt->data.read.read_ptr, evt->data.read.bytes);

    return cb_error_ok;
}

/**
 * @}
 */

/******************************************************************************************************END OF FILE*****/
