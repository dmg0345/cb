/**
 ***********************************************************************************************************************
 * @file        test_types.h
 * @author      Diego Martínez García (dmg0345@gmail.com)
 * @date        04-11-2023 21:33:15 (UTC)
 * @version     1.0.0
 * @copyright   github.com/dmg0345/cb/blob/master/LICENSE
 ***********************************************************************************************************************
 */

/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/
#ifndef TEST_TYPES_H
#define TEST_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @defgroup test_types_defs Definitions */
/** @defgroup test_types_papi Public API */

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ----------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup test_types_defs
 * @{
 */

#if defined(USE_UINT16_T)
typedef uint16_t test_type_t; /**< Use 16bit unsigned integer for the tests. */
#elif defined(USE_UINT32_T)
typedef uint32_t test_type_t; /**< Use 32bit unsigned integer for the tests. */
#elif defined(USE_UINT64_T)
typedef uint64_t test_type_t; /**< Use 64bit unsigned integer for the tests. */
#else
typedef uint8_t test_type_t; /**< Use 8bit unsigned integer for the tests, the default. */
#endif

/**
 * @}
 */

/* Exported constants ------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup test_types_defs
 * @{
 */

#if defined(USE_UINT16_T)
#define TEST_CLEAR_VALUE ((test_type_t)0xFFFFU) /**< Clear value for 16bit unsigned integers, the default. */
#elif defined(USE_UINT32_T)
#define TEST_CLEAR_VALUE ((test_type_t)0xFFFFFFFFU) /**< Clear value for 32bit unsigned integers, the default. */
#elif defined(USE_UINT64_T)
#define TEST_CLEAR_VALUE ((test_type_t)0xFFFFFFFFFFFFFFFFU) /**< Clear value for 64bit unsigned integers. */
#else
#define TEST_CLEAR_VALUE ((test_type_t)0xFFU) /**< Clear value for 8bit unsigned integers, the default. */
#endif

/**
 * @}
 */

/* Exported macro ----------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup test_types_papi
 * @{
 */

/**
 * @}
 */

/* Exported functions ------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup test_types_papi
 * @{
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TEST_TYPES_H */

/******************************************************************************************************END OF FILE*****/
