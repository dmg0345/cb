/**
 ***********************************************************************************************************************
 * @file        cmocka_defs.c
 * @author      ${PROJECT_AUTHOR} (${PROJECT_CONTACT})
 * @date        ${PROJECT_BUILD_TIMESTAMP_UTC}
 * @version     ${PROJECT_VERSION}
 * @copyright   ${PROJECT_COPYRIGHT}
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
