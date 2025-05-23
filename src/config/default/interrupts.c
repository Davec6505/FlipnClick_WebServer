/*******************************************************************************
 System Interrupts File

  Company:
    Microchip Technology Inc.

  File Name:
    interrupt.c

  Summary:
    Interrupt vectors mapping

  Description:
    This file maps all the interrupt vectors to their corresponding
    implementations. If a particular module interrupt is used, then its ISR
    definition can be found in corresponding PLIB source file. If a module
    interrupt is not used, then its ISR implementation is mapped to dummy
    handler.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2025 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "configuration.h"
#include "interrupts.h"
#include "definitions.h"



// *****************************************************************************
// *****************************************************************************
// Section: System Interrupt Vector Functions
// *****************************************************************************
// *****************************************************************************


/* All the handlers are defined here.  Each will call its PLIB-specific function. */
// *****************************************************************************
// *****************************************************************************
// Section: System Interrupt Vector declarations
// *****************************************************************************
// *****************************************************************************
void CORE_TIMER_Handler (void);
void SPI1_RX_Handler (void);
void SPI1_TX_Handler (void);
void DMA0_Handler (void);
void DMA1_Handler (void);
void DMA2_Handler (void);
void DMA3_Handler (void);
void SPI2_RX_Handler (void);
void SPI2_TX_Handler (void);
void FLASH_CONTROL_Handler (void);
void UART5_FAULT_Handler (void);
void UART5_RX_Handler (void);
void UART5_TX_Handler (void);


// *****************************************************************************
// *****************************************************************************
// Section: System Interrupt Vector definitions
// *****************************************************************************
// *****************************************************************************
void __attribute__((used)) __ISR(_CORE_TIMER_VECTOR, ipl7SRS) CORE_TIMER_Handler (void)
{
    CORE_TIMER_InterruptHandler();
}

void __attribute__((used)) __ISR(_SPI1_RX_VECTOR, ipl6SRS) SPI1_RX_Handler (void)
{
    SPI1_RX_InterruptHandler();
}

void __attribute__((used)) __ISR(_SPI1_TX_VECTOR, ipl6SRS) SPI1_TX_Handler (void)
{
    SPI1_TX_InterruptHandler();
}

void __attribute__((used)) __ISR(_DMA0_VECTOR, ipl4SRS) DMA0_Handler (void)
{
    DMA0_InterruptHandler();
}

void __attribute__((used)) __ISR(_DMA1_VECTOR, ipl4SRS) DMA1_Handler (void)
{
    DMA1_InterruptHandler();
}

void __attribute__((used)) __ISR(_DMA2_VECTOR, ipl5SRS) DMA2_Handler (void)
{
    DMA2_InterruptHandler();
}

void __attribute__((used)) __ISR(_DMA3_VECTOR, ipl5SRS) DMA3_Handler (void)
{
    DMA3_InterruptHandler();
}

void __attribute__((used)) __ISR(_SPI2_RX_VECTOR, ipl5SRS) SPI2_RX_Handler (void)
{
    SPI2_RX_InterruptHandler();
}

void __attribute__((used)) __ISR(_SPI2_TX_VECTOR, ipl5SRS) SPI2_TX_Handler (void)
{
    SPI2_TX_InterruptHandler();
}

void __attribute__((used)) __ISR(_FLASH_CONTROL_VECTOR, ipl1SRS) FLASH_CONTROL_Handler (void)
{
    NVM_InterruptHandler();
}

void __attribute__((used)) __ISR(_UART5_FAULT_VECTOR, ipl1SRS) UART5_FAULT_Handler (void)
{
    UART5_FAULT_InterruptHandler();
}

void __attribute__((used)) __ISR(_UART5_RX_VECTOR, ipl1SRS) UART5_RX_Handler (void)
{
    UART5_RX_InterruptHandler();
}

void __attribute__((used)) __ISR(_UART5_TX_VECTOR, ipl1SRS) UART5_TX_Handler (void)
{
    UART5_TX_InterruptHandler();
}




/*******************************************************************************
 End of File
*/
