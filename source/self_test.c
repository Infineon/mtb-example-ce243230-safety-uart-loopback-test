/******************************************************************************
 * File Name:   self_test.c
 *
 * Description: Provides UART (SCB6) initialization and the Class-B UART
 *              loopback self-test wrapper.
 *
 *              The SCB hardware loopback is enabled for the duration of each
 *              SelfTest_UART_SCB() call and then cleared, so no external
 *              wiring is required.
 *
 * Related Document: See README.md
 *
 *
 *******************************************************************************
 * $ Copyright 2026 Infineon Technologies AG $
 *******************************************************************************/

/*******************************************************************************
* Header Files
*******************************************************************************/
#include "cy_retarget_io.h"
#include "self_test.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
static cy_stc_scb_uart_context_t CYBSP_DUT_UART_context;

/*******************************************************************************
* Macros
*******************************************************************************/
/* Microsecond delay after each test iteration to allow the FIFO to fully
 * drain before the next call. */
#define BUFF_CLEAR_DELAY_US  (100u)

/*******************************************************************************
* Function Name: uart_self_test_init
********************************************************************************
* Summary:
*  Initializes the DUT UART peripheral (SCB6) and clears both FIFOs.
*  Call once before entering the test loop.
*
*  SCB6 is configured in the design.modus file with pins P22_0 (RX) and
*  P22_1 (TX). The hardware internal loopback mode (TX routed back to RX
*  inside the SCB block) is used during the test, so no external wiring
*  between TX and RX is required.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void uart_self_test_init(void)
{
    cy_en_scb_uart_status_t init_status;

    init_status = Cy_SCB_UART_Init(CYBSP_DUT_UART_HW, &CYBSP_DUT_UART_config,
                                    &CYBSP_DUT_UART_context);
    if (init_status != CY_SCB_UART_SUCCESS)
    {
        CY_ASSERT(0);
    }

    Cy_SCB_UART_Enable(CYBSP_DUT_UART_HW);

    Cy_SCB_UART_ClearRxFifo(CYBSP_DUT_UART_HW);
    Cy_SCB_UART_ClearTxFifo(CYBSP_DUT_UART_HW);

    printf("UART (SCB6) initialized - hardware loopback test ready.\r\n\n");
}

/*******************************************************************************
* Function Name: uart_self_test
********************************************************************************
* Summary:
*  Runs one iteration of the UART loopback self-test.
*
*  Enables the SCB6 hardware internal loopback (TX routed back to RX inside
*  the SCB block) before calling SelfTest_UART_SCB() and disables it
*  afterward. The caller repeats this function while PASS_STILL_TESTING_STATUS
*  is returned; SelfTest_UART_SCB() advances an internal byte counter on each
*  call and returns PASS_COMPLETE_STATUS once all 256 bytes have been
*  verified.
*
* Parameters:
*  void
*
* Return:
*  PASS_STILL_TESTING_STATUS — test in progress, call again
*  PASS_COMPLETE_STATUS      — all 256 bytes verified, test passed
*  ERROR_STATUS or other     — test failure (see SelfTest_common.h)
*
*******************************************************************************/
uint8_t uart_self_test(void)
{
    uint8_t ret;

    /* Enable hardware internal loopback: TX is routed back to RX inside the SCB */
    SCB_UART_CTRL(CYBSP_DUT_UART_HW) |= SCB_UART_CTRL_LOOPBACK_Msk;

    /* Clear FIFOs before each test call */
    Cy_SCB_UART_ClearRxFifo(CYBSP_DUT_UART_HW);
    Cy_SCB_UART_ClearTxFifo(CYBSP_DUT_UART_HW);

    ret = SelfTest_UART_SCB(CYBSP_DUT_UART_HW);

    /* Disable hardware loopback - restore normal external TX/RX routing */
    SCB_UART_CTRL(CYBSP_DUT_UART_HW) &= ~SCB_UART_CTRL_LOOPBACK_Msk;

    /* Short delay so any in-flight bytes fully drain from the FIFO */
    Cy_SysLib_DelayUs(BUFF_CLEAR_DELAY_US);
    Cy_SCB_UART_ClearRxFifo(CYBSP_DUT_UART_HW);
    Cy_SCB_UART_ClearTxFifo(CYBSP_DUT_UART_HW);

    return ret;
}

/* [] END OF FILE */
