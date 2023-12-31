/**
 ***********************************************************************************************************************
 * @file        cb_evt_handlers.h
 * @author      Diego Martínez García (dmg0345@gmail.com)
 * @date        04-11-2023 21:33:15 (UTC)
 * @version     1.0.0
 * @copyright   github.com/dmg0345/cb/blob/master/LICENSE
 ***********************************************************************************************************************
 */

/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/
#ifndef CB_EVT_HANDLERS_H
#define CB_EVT_HANDLERS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @defgroup cb_evt_handlers_defs Definitions */
/** @defgroup cb_evt_handlers_papi Public API */

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "cb/cb.h"

/* Exported types ----------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup cb_evt_handlers_defs
 * @{
 */

/**
 * @}
 */

/* Exported constants ------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup cb_evt_handlers_defs
 * @{
 */

/**
 * @}
 */

/* Exported macro ----------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup cb_evt_handlers_papi
 * @{
 */

/**
 * @}
 */

/* Exported functions ------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup cb_evt_handlers_papi
 * @{
 */

/** Event handler that performs successfully. */
cb_error_t cb_evt_handler(cb_evt_t * const evt);
/** Event handler that returns error on first write operation. */
cb_error_t cb_evt_handler_write_first_operation_error(cb_evt_t * const evt);
/** Event handler that returns error on first read operation. */
cb_error_t cb_evt_handler_read_first_operation_error(cb_evt_t * const evt);
/** Event handler that returns error on second write operation. */
cb_error_t cb_evt_handler_write_second_operation_error(cb_evt_t * const evt);
/** Event handler that returns error on second read operation. */
cb_error_t cb_evt_handler_read_second_operation_error(cb_evt_t * const evt);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CB_EVT_HANDLERS_H */

/******************************************************************************************************END OF FILE*****/
