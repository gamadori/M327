/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : CAN1.h
**     Project     : M327
**     Processor   : MC56F84789VLL
**     Component   : FreescaleCAN
**     Version     : Component 02.366, Driver 01.03, CPU db: 3.50.001
**     Compiler    : CodeWarrior DSP C Compiler
**     Date/Time   : 2016-11-17, 22:35, # CodeGen: 172
**     Abstract    :
**         This component "FreescaleCAN" implements a CAN serial channel.
**     Settings    :
**         CAN channel                                     : CAN
**
**           Interrupt service/event                       : Enabled
**             Message buffers interrupt
**               Vector name                               : INT_MB_OR
**               Priority                                  : 1
**             Error interrupt
**               Vector name                               : INT_ERROR
**               Priority                                  : 1
**             Wakeup interrupt
**               Vector name                               : INT_WAKEUP
**               Priority                                  : 1
**             Bus Off interrupt
**               Vector name                               : INT_BUS_OFF
**               Priority                                  : 1
**             Tx warning interrupt
**               Vector name                               : INT_TX_WARN
**               Priority                                  : 1
**             Rx warning interrupt
**               Vector name                               : INT_RX_WARN
**               Priority                                  : 1
**
**          Settings                                       : 
**                Rx pin                                   : GPIOC12/CANRX/SDA1/RXD1
**                Rx pin signal                            : 
**                Tx pin                                   : GPIOC11/CANTX/SCL1/TXD1
**                Tx pin Signal                            : 
**
**            Global Acceptance Mask                       : yes
**            Message buffers                              : 16
**              Buffer0                                    : 
**                Buffer type                              : Receive
**                  Accept frames                          : Standard
**                  Message ID                             : 0
**                  Invidual Acceptance Mask               : Disabled
**              Buffer1                                    : 
**                Buffer type                              : Receive
**                  Accept frames                          : Standard
**                  Message ID                             : 80
**                  Invidual Acceptance Mask               : Disabled
**              Buffer2                                    : 
**                Buffer type                              : Receive
**                  Accept frames                          : Standard
**                  Message ID                             : 100
**                  Invidual Acceptance Mask               : Disabled
**              Buffer3                                    : 
**                Buffer type                              : Transmit
**              Buffer4                                    : 
**                Buffer type                              : Transmit
**              Buffer5                                    : 
**                Buffer type                              : Receive
**                  Accept frames                          : Standard
**                  Message ID                             : 200
**                  Invidual Acceptance Mask               : Disabled
**              Buffer6                                    : 
**                Buffer type                              : Transmit
**              Buffer7                                    : 
**                Buffer type                              : Receive
**                  Accept frames                          : Standard
**                  Message ID                             : 300
**                  Invidual Acceptance Mask               : Disabled
**              Buffer8                                    : 
**                Buffer type                              : Transmit
**              Buffer9                                    : 
**                Buffer type                              : Receive
**                  Accept frames                          : Standard
**                  Message ID                             : 400
**                  Invidual Acceptance Mask               : Disabled
**              Buffer10                                   : 
**                Buffer type                              : Transmit
**              Buffer11                                   : 
**                Buffer type                              : Receive
**                  Accept frames                          : Standard
**                  Message ID                             : 500
**                  Invidual Acceptance Mask               : Disabled
**              Buffer12                                   : 
**                Buffer type                              : Transmit
**              Buffer13                                   : 
**                Buffer type                              : Receive
**                  Accept frames                          : Standard
**                  Message ID                             : 600
**                  Invidual Acceptance Mask               : Disabled
**              Buffer14                                   : 
**                Buffer type                              : Transmit
**              Buffer15                                   : 
**                Buffer type                              : Transmit
**
**            Timer synchronization                        : Disabled
**            Lowest buffer transmitted first              : Lowest ID
**            Debug mode                                   : no
**            Abort Transmission Mode                      : no
**            Remote Request Storing                       : Remote Request Frame is stored
**            Entire Frame Arbitration Field Comparison    : no
**            Self ReceptionEn                             : Disabled
**            Loop Mode                                    : no
**            Bus-Off Recovery                             : Auto
**            Listen Only Mode                             : no
**            Wakeup                                       : Disabled
**
**         Protocol
**             Propagation segment                         : 0
**             Time segment 1                              : 0
**             Time segment 2                              : 0
**             RSJ                                         : 0
**             Samples per bit                             : Three samples
**             Time quanta per bit                         : 4
**             Init baud rate                              : 1000 kbit/s
**
**         Initialization
**             Enabled in init. code                       : no
**             Events enabled in init.                     : yes
**
**         CPU clock/speed selection
**             High speed mode                             : This component enabled
**             Low speed mode                              : This component disabled
**             Slow speed mode                             : This component disabled
**
**     Contents    :
**         Enable                - byte CAN1_Enable(void);
**         SetAcceptanceCode     - byte CAN1_SetAcceptanceCode(byte BufferNum, dword ID);
**         SetAcceptanceMask     - byte CAN1_SetAcceptanceMask(dword Mask);
**         SendFrame             - byte CAN1_SendFrame(byte BufferNum, dword MessageID, byte FrameType, byte...
**         ReadFrame             - byte CAN1_ReadFrame(byte BufferNum, dword *MessageID, byte *FrameType, byte...
**         GetStateTX            - word CAN1_GetStateTX(void);
**         GetStateRX            - word CAN1_GetStateRX(void);
**         GetError              - byte CAN1_GetError(CAN1_TError *Err);
**         SetBufferCode         - byte CAN1_SetBufferCode(byte BufferNum, byte BufferCode);
**
**     Copyright : 1997 - 2014 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file CAN1.h
** @version 01.03
** @brief
**         This component "FreescaleCAN" implements a CAN serial channel.
*/         
/*!
**  @addtogroup CAN1_module CAN1 module documentation
**  @{
*/         

#ifndef __CAN1
#define __CAN1

#include "Cpu.h"

/*Include shared modules, which are used for whole project*/
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* MODULE CAN1. */

#ifndef __BWUserType_CAN1_TError
#define __BWUserType_CAN1_TError
  typedef union {
    word err;
    struct {
      /*lint -save  -e46 Disable MISRA rule (6.4) checking. */
      bool OverRun : 1;                /* Overrun error flag */
      bool RxBufOvf : 1;               /* Rx buffer full error flag */
      bool Idle : 1;                   /* Idle character detect */
      bool BusOff : 1;                 /* Bus-off state */
      bool TxPassive : 1;              /* Transmitter error passive */
      bool RxPassive : 1;              /* Reciver error passive */
      bool TxWarning : 1;              /* Transmitter warning */
      bool RxWarning : 1;              /* Receiver warning */
      bool Ack : 1;                    /* Acknowledge error */
      bool Crc : 1;                    /* CRC error */
      bool Form : 1;                   /* Form error */
      bool Stuff : 1;                  /* Stuff bit error */
      bool Bit0 : 1;                   /* Bit 0 error */
      bool Bit1 : 1;                   /* Bit 1 error */
      /*lint -restore Enable MISRA rule (6.4) checking. */
    }errName;
  } CAN1_TError;                       /* Error flags. For languages which don't support bit access is byte access only to error flags possible. */
#endif

/* Message buffer masks. These mask can be used for message buffer identification in methods
   GetStateTx(), GetStateRx and events OnFreeTxBuffer(), OnFullRxBuffer().
*/
#define CAN_MB0_MASK                    1U                 /* Bit mask for message buffer 0 */
#define CAN_MB1_MASK                    2U                 /* Bit mask for message buffer 1 */
#define CAN_MB2_MASK                    4U                 /* Bit mask for message buffer 2 */
#define CAN_MB3_MASK                    8U                 /* Bit mask for message buffer 3 */
#define CAN_MB4_MASK                    16U                /* Bit mask for message buffer 4 */
#define CAN_MB5_MASK                    32U                /* Bit mask for message buffer 5 */
#define CAN_MB6_MASK                    64U                /* Bit mask for message buffer 6 */
#define CAN_MB7_MASK                    128U               /* Bit mask for message buffer 7 */
#define CAN_MB8_MASK                    256U               /* Bit mask for message buffer 8 */
#define CAN_MB9_MASK                    512U               /* Bit mask for message buffer 9 */
#define CAN_MB10_MASK                   1024U              /* Bit mask for message buffer 10 */
#define CAN_MB11_MASK                   2048U              /* Bit mask for message buffer 11 */
#define CAN_MB12_MASK                   4096U              /* Bit mask for message buffer 12 */
#define CAN_MB13_MASK                   8192U              /* Bit mask for message buffer 13 */
#define CAN_MB14_MASK                   16384U             /* Bit mask for message buffer 14 */
#define CAN_MB15_MASK                   32768U             /* Bit mask for message buffer 15 */

/* RX message buffer codes */
#define MB_RX_NOT_ACTIVE                0x0000U
#define MB_RX_FULL                           2U
#define MB_RX_FULL_MASK                 0x0200U
#define MB_RX_EMPTY                          4U
#define MB_RX_EMPTY_MASK                0x0400U
#define MB_RX_OVERRUN                        6U
#define MB_RX_OVERRUN_MASK              0x0600U
#define MB_RX_BUSY                           1U
#define MB_RX_BUSY_MASK                 0x0100U

/* TX message buffer codes */
#define MB_TX_NOT_ACTIVE                     8U
#define MB_TX_NOT_ACTIVE_MASK           0x0800U
#define MB_TX_DATA_FRAME                    12U
#define MB_TX_DATA_FRAME_MASK           0x0C00U
#define MB_TX_REMOTE_FRAME                  12U
#define MB_TX_REMOTE_FRAME_MASK         0x0C00U
#define MB_TX_RESPONSE_FRAME                10U
#define MB_TX_RESPONSE_FRAME_MASK       0x0A00U

/* Frame formats */
#define STANDARD_FORMAT                 (byte)0U
#define EXTENDED_FORMAT                 (byte)1U

/* Frame types   */
#define DATA_FRAME                      (byte)0U
#define REMOTE_FRAME                    (byte)1U

/* Device status */
#define FC_READY                        (byte)1U
#define FC_STOP                         (byte)2U
#define FC_DEBUG                        (byte)3U

/* Extended message ID identifier.  This constant can be used to specify an extended message ID
 * anywhere the message ID is required.
 */
#define CAN_EXTENDED_FRAME_ID           0x80000000UL       /* Extended message ID identifier */

byte CAN1_SetAcceptanceCode(byte BufferNum,dword ID);
/*
** ===================================================================
**     Method      :  CAN1_SetAcceptanceCode (component FreescaleCAN)
**     Description :
**         Sets the message ID of the given message buffer. Buffer ID
**         is used for message filtering of incoming frames.
**     Parameters  :
**         NAME            - DESCRIPTION
**         BufferNum       - Number of the buffer.
**         ID              - Mask could be 11-bit for standard frame or
**                           29-bit for extended frame
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_VALUE - Some parameter is out of
**                           possible range
** ===================================================================
*/

byte CAN1_SetBufferCode(byte BufferNum,byte BufferCode);
/*
** ===================================================================
**     Method      :  CAN1_SetBufferCode (component FreescaleCAN)
**     Description :
**         Sets up the message buffer code for the appropriate message
**         buffer. This method is always enabled because it is used
**         internally.
**     Parameters  :
**         NAME            - DESCRIPTION
**         BufferNum       - Number of the buffer.
**         BufferCode      - Message buffer code for the
**                           appropriate message buffer.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_VALUE - Some parameter is out of
**                           possible range
** ===================================================================
*/

word CAN1_GetStateRX(void);
/*
** ===================================================================
**     Method      :  CAN1_GetStateRX (component FreescaleCAN)
**     Description :
**         Returns the value of the reception complete flags (register
**         FCIFLAG1).
**     Parameters  : None
**     Returns     :
**         ---             - Content of the receiver complete flag
**                           register.
** ===================================================================
*/

byte CAN1_GetError(CAN1_TError *Err);
/*
** ===================================================================
**     Method      :  CAN1_GetError (component FreescaleCAN)
**     Description :
**         Returns the content of the receiver flag register.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Err             - Pointer to the returned set of errors
**     Returns     :
**         ---             - Error code (if GetError did not succeed),
**                           possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/

byte CAN1_SendFrame(byte BufferNum,dword MessageID,byte FrameType,byte Length,void* Data,bool waitForRTR);
/*
** ===================================================================
**     Method      :  CAN1_SendFrame (component FreescaleCAN)
**     Description :
**         Sends a frame via the CAN device. Using this method the user
**         can send own message to the CAN bus. This method allows to
**         specify CAN buffer number, message ID, data to be sent,
**         frame type (DATA_FRAME/REMOTE_FRAME) and whether the message
**         will be sent after the request comes.
**     Parameters  :
**         NAME            - DESCRIPTION
**         BufferNum       - Number of the buffer.
**         MessageID       - Identification of the message
**                           - ID. Message ID can be specified in the
**                           STANDARD format (default) or the EXTENDED
**                           format. Extended message ID is specified as
**                           (ID | CAN_EXTENDED_FRAME_ID).
**         FrameType       - Type of frame
**                           DATA_FRAME - data frame
**                           REMOTE_FRAME - remote frame
**         Length          - The length of the frame in bytes (0..
**                           8)
**       * Data            - Pointer to data
**         waitForRTR      - Waits for a remote frame. If
**                           this parameter is <true>, only data from
**                           the message buffer will be transmitted as a
**                           response to a remote frame. If this
**                           parameter is <false>, the buffer transmit
**                           data immediately.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED - This component is disabled
**                           by user
**                           ERR_VALUE - Some parameter is out of
**                           possible range
**                           ERR_BUSY - Device is busy. The frame can't
**                           be transmitted.
** ===================================================================
*/

byte CAN1_ReadFrame(byte BufferNum,dword *MessageID,byte *FrameType,byte *FrameFormat,byte *Length,void* Data);
/*
** ===================================================================
**     Method      :  CAN1_ReadFrame (component FreescaleCAN)
**     Description :
**         Reads a frame from the CAN device. The user is informed
**         about CAN reception through OnFullRxBuffer event or
**         GetStateRX method.
**     Parameters  :
**         NAME            - DESCRIPTION
**         BufferNum       - Number of the buffer.
**       * MessageID       - Identification of message
**       * FrameType       - Type of frame
**                           DATA_FRAME - data frame
**                           REMOTE_FRAME - remote frame
**       * FrameFormat     - Format of frame
**                           STANDARD_FORMAT - standard frame 11-bits
**                           EXTENDED_FORMAT - extended frame 29-bits. 
**                           Note: This parameter is obsolete and will
**                           be removed in future releases.
**       * Length          - The length of the frame
**       * Data            - The buffer for received data
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED - This component is disabled
**                           by user
**                           ERR_BUSY - The receive buffer is busy
** ===================================================================
*/

void CAN1_Init(void);
/*
** ===================================================================
**     Method      :  CAN1_Init (component FreescaleCAN)
**
**     Description :
**         Initializes the associated peripheral(s) and the beans 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

word CAN1_GetStateTX(void);
/*
** ===================================================================
**     Method      :  CAN1_GetStateTX (component FreescaleCAN)
**     Description :
**         Returns the value of the transmission complete flags
**         (register FCIFLAG1).
**     Parameters  : None
**     Returns     :
**         ---             - Content of the transmitter complete flag
**                           register.
** ===================================================================
*/

byte CAN1_Enable(void);
/*
** ===================================================================
**     Method      :  CAN1_Enable (component FreescaleCAN)
**     Description :
**         Enables the component - it turns on the send and receive
**         functions. Events may be generated
**         ("DisableEvent"/"EnableEvent").
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/

byte CAN1_SetAcceptanceMask(dword Mask);
/*
** ===================================================================
**     Method      :  CAN1_SetAcceptanceMask (component FreescaleCAN)
**     Description :
**         Sets the acceptance mask registers. This method writes an
**         acceptance mask directly to the acceptance mask registers.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Mask            - 11-bit Mask could be selected for a
**                           standard frame or 29-bit for an extended
**                           frame
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED - This component is disabled
**                           by user
** ===================================================================
*/

void CAN1_InterruptRxTx(void);
/*
** ===================================================================
**     Method      :  CAN1_InterruptRxTx (component FreescaleCAN)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and eventually invokes the beans event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

void CAN1_InterruptError(void);
/*
** ===================================================================
**     Method      :  CAN1_InterruptError (component FreescaleCAN)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and eventually invokes the beans event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

void CAN1_InterruptBusOff(void);
/*
** ===================================================================
**     Method      :  CAN1_InterruptBusOff (component FreescaleCAN)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and eventually invokes the beans event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

void CAN1_InterruptTxWarning(void);
/*
** ===================================================================
**     Method      :  CAN1_InterruptTxWarning (component FreescaleCAN)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and eventually invokes the beans event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

void CAN1_InterruptRxWarning(void);
/*
** ===================================================================
**     Method      :  CAN1_InterruptRxWarning (component FreescaleCAN)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and eventually invokes the beans event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

/* END CAN1. */

#endif /* ifndef __CAN1 */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale 56800 series of microcontrollers.
**
** ###################################################################
*/