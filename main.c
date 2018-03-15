/*
 * The Clear BSD License
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_dac.h"

#include "fsl_common.h"
#include "clock_config.h"
#include "pin_mux.h"

#include "signal.h"
#include "fsl_pit.h"
#include "SignalValues.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_DAC_BASEADDR DAC0

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
uint32_t LDVAL_trigger;
bool g_stop_song = false;

void PIT0_IRQHandler(void)
{
    if(g_stop_song)
    {
        DAC_SetBufferValue(DEMO_DAC_BASEADDR, 0U, 0);
    }
    else if(END!=LDVAL_trigger)
    {
        if((SILENCE!=LDVAL_trigger))
        {
            uint32_t dacValue = nextDACValue();
            DAC_SetBufferValue(DEMO_DAC_BASEADDR, 0U, dacValue);
            PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, LDVAL_trigger);//cycles
            PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, PIT_TFLG_TIF_MASK);
            PIT_StartTimer(PIT, kPIT_Chnl_0);
        }
        else
        {
            DAC_SetBufferValue(DEMO_DAC_BASEADDR, 0U, 0);
            PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, LDVAL_trigger_SEC);//cycles
            PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, PIT_TFLG_TIF_MASK);
            PIT_StartTimer(PIT, kPIT_Chnl_0);
        }
    }
}

void PIT1_IRQHandler(void)
{
    if(!g_stop_song)
    {
        LDVAL_trigger = nextSongNote();
        if(SILENCE==LDVAL_trigger)
        {
            PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, LDVAL_trigger_SEC);//cycles
            PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, PIT_TFLG_TIF_MASK);
            PIT_StartTimer(PIT, kPIT_Chnl_1);
        }
        else if(END!=LDVAL_trigger)
        {
            PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, LDVAL_trigger);//cycles
            PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, PIT_TFLG_TIF_MASK);
            PIT_StartTimer(PIT, kPIT_Chnl_0);
            PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, LDVAL_trigger_SEC);//cycles
            PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, PIT_TFLG_TIF_MASK);
            PIT_StartTimer(PIT, kPIT_Chnl_1);
        }
        else if(END==LDVAL_trigger)
        {
            DAC_SetBufferValue(DEMO_DAC_BASEADDR, 0U, 0);
            LDVAL_trigger = END;
            g_stop_song = true;
            PIT_StopTimer(PIT, kPIT_Chnl_0);
        }
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_EnableClock(kCLOCK_Pit0);

    /* Configure the DAC. */
    /*
     * dacConfigStruct.referenceVoltageSource = kDAC_ReferenceVoltageSourceVref2;
     * dacConfigStruct.enableLowPowerMode = false;
     */
    dac_config_t dacConfigStruct;
    DAC_GetDefaultConfig(&dacConfigStruct);
    DAC_Init(DEMO_DAC_BASEADDR, &dacConfigStruct);
    DAC_Enable(DEMO_DAC_BASEADDR, true);             /* Enable output. */
    DAC_SetBufferReadPointer(DEMO_DAC_BASEADDR, 0U); /* Make sure the read pointer to the start. */
                                                     /*
                                                     * The buffer is not enabled, so the read pointer can not move automatically. However, the buffer's read pointer
                                                     * and itemss can be written manually by user.
                                                     */
    LDVAL_trigger = LDVAL_trigger_C;
    //PIT enable debug
    pit_config_t pit_config;
    PIT_GetDefaultConfig(&pit_config);
    //PIT configuration
    PIT_Init(PIT, &pit_config);
    ///cycles
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, (uint32_t) LDVAL_trigger);
    EnableIRQ(PIT0_IRQn);
    PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, PIT_TFLG_TIF_MASK);
    PIT_StartTimer(PIT, kPIT_Chnl_0);

    PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, (uint32_t) LDVAL_trigger_SEC);
    EnableIRQ(PIT1_IRQn);
    PIT_EnableInterrupts(PIT, kPIT_Chnl_1, kPIT_TimerInterruptEnable);
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, PIT_TFLG_TIF_MASK);
    PIT_StartTimer(PIT, kPIT_Chnl_1);

    while (1)
    {

    }
}
