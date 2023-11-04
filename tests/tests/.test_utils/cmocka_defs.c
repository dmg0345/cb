/**
 ***********************************************************************************************************************
 * @file        cmocka_defs.c
 * @author      Diego Martínez García (dmg0345@gmail.com)
 * @date        04-11-2023 19:37:00 (UTC)
 * @version     1.0.0
 * @copyright   github.com/dmg0345/cb/blob/master/LICENSE
 ***********************************************************************************************************************
 */

/** @defgroup cmocka_defs_iapi_impl Internal API implementation */
/** @defgroup cmocka_defs_papi_impl Public API implementation */

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "cmocka_defs.h"

/* Private types -----------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup cmocka_defs_iapi_impl
 * @{
 */

/**
 * @}
 */

/* Private define ----------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup cmocka_defs_iapi_impl
 * @{
 */

/**
 * @}
 */

/* Private macro -----------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup cmocka_defs_iapi_impl
 * @{
 */

/**
 * @}
 */

/* Private variables -------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup cmocka_defs_iapi_impl
 * @{
 */

/**
 * @}
 */

/* Private function prototypes ---------------------------------------------------------------------------------------*/
/**
 * @addtogroup cmocka_defs_iapi_impl
 * @{
 */

/**
 * @}
 */

/* Private functions -------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup cmocka_defs_iapi_impl
 * @{
 */

/**
 * @}
 */

/* Exported functions ------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup cmocka_defs_papi_impl
 * @{
 */

void cmocka_init(void)
{
#if defined(CMOCKA_JUNIT_XML_OUTPUT)
    cmocka_set_message_output(CM_OUTPUT_XML);
#else
    cmocka_set_message_output(CM_OUTPUT_STDOUT);
#endif
}

/**
 * @}
 */

/******************************************************************************************************END OF FILE*****/
