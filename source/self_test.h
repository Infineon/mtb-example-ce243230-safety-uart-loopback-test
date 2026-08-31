/******************************************************************************
 * File Name:   self_test.h
 *
 * Description: This file is the public interface of self_test.c source file.
 *
 * Related Document: See README.md
 *
 *
 *******************************************************************************
 * $ Copyright 2026 Infineon Technologies AG $
 *******************************************************************************/

#ifndef SELF_TEST_H_
#define SELF_TEST_H_

/*******************************************************************************
* Header Files
*******************************************************************************/
#include "cybsp.h"
#include "cy_pdl.h"
#include "SelfTest.h"

/*******************************************************************************
* Macros
*******************************************************************************/
/* State used by main() after the UART self-test passes to run the
 * user application. Must be different from all PASS/ERROR status codes
 * defined in SelfTest_common.h (1-6). */
#define EXE_CUSTOM_APP  (7u)

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
void    uart_self_test_init(void);
uint8_t uart_self_test(void);

#endif /* SELF_TEST_H_ */

/* [] END OF FILE */
