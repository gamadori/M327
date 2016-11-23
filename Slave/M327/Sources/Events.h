/* ###################################################################
**     Filename    : Events.h
**     Project     : M327
**     Processor   : MC56F84789VLL
**     Component   : Events
**     Version     : Driver 01.03
**     Compiler    : CodeWarrior DSP C Compiler
**     Date/Time   : 2016-06-12, 15:52, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file Events.h
** @version 01.03
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "AS1.h"
#include "SM1.h"
#include "CSInput.h"
#include "Out0.h"
#include "Out1.h"
#include "Out2.h"
#include "Out3.h"
#include "Out4.h"
#include "Out5.h"
#include "Out6.h"
#include "Out7.h"
#include "Out8.h"
#include "Out9.h"
#include "Out10.h"
#include "Out11.h"
#include "Out12.h"
#include "Out13.h"
#include "Out14.h"
#include "Out15.h"
#include "Inp16.h"
#include "Inp1.h"
#include "TI1.h"
#include "PWMA.h"
#include "SDA.h"
#include "SCL.h"
#include "CSPot.h"
#include "CSFlash.h"
#include "CAN1.h"
#include "AD1.h"
#include "MCBLDC.h"
#include "IFsh1.h"
#include "TMRA0.h"
#include "TMRA1.h"
#include "TMRA2.h"
#include "PWMC1.h"
#include "Enc1.h"
#include "Enc2.h"


void SM1_OnRxChar(void);
/*
** ===================================================================
**     Event       :  SM1_OnRxChar (module Events)
**
**     Component   :  SM1 [SynchroMaster]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**         [ Version specific information for Freescale 56800
**         derivatives ] 
**         DMA mode:
**         If DMA controller is available on the selected CPU and the
**         receiver is configured to use DMA controller then this event
**         is disabled. Only OnFullRxBuf method can be used in DMA mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void SM1_OnTxChar(void);
/*
** ===================================================================
**     Event       :  SM1_OnTxChar (module Events)
**
**     Component   :  SM1 [SynchroMaster]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void SM1_OnFullRxBuf(void);
/*
** ===================================================================
**     Event       :  SM1_OnFullRxBuf (module Events)
**
**     Component   :  SM1 [SynchroMaster]
**     Description :
**         This event is called when the input buffer is full, i.e.
**         after reception of the last character that was successfully
**         placed into input buffer.
**         This event is available only when the <Interrupt
**         service/event> property is enabled and the <Input buffer
**         size> property is set to non-zero value.
**         [ Version specific information for Freescale 56800
**         derivatives ] 
**         DMA mode:
**         If DMA controller is available on the selected CPU and the
**         receiver is configured to use DMA controller then this event
**         is enabled and can be used to detect the end of DMA transfer.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void SM1_OnFreeTxBuf(void);
/*
** ===================================================================
**     Event       :  SM1_OnFreeTxBuf (module Events)
**
**     Component   :  SM1 [SynchroMaster]
**     Description :
**         This event is called after the last character in output
**         buffer is transmitted.
**         This event is available only when the <Interrupt
**         service/event> property is enabled and the <Output buffer
**         size> property is set to non-zero value.
**         [ Version specific information for Freescale 56800
**         derivatives ] 
**         DMA mode:
**         If DMA controller is available on the selected CPU and the
**         transmitter is configured to use DMA controller then this
**         event is enabled and can be used to detect the end of DMA
**         transfer.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void SM1_OnError(void);
/*
** ===================================================================
**     Event       :  SM1_OnError (module Events)
**
**     Component   :  SM1 [SynchroMaster]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be read
**         using <GetError> method.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void AS1_OnError(void);
/*
** ===================================================================
**     Event       :  AS1_OnError (module Events)
**
**     Component   :  AS1 [AsynchroSerial]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be read
**         using <GetError> method.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void AS1_OnRxChar(void);
/*
** ===================================================================
**     Event       :  AS1_OnRxChar (module Events)
**
**     Component   :  AS1 [AsynchroSerial]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled and either the <Receiver>
**         property is enabled or the <SCI output mode> property (if
**         supported) is set to Single-wire mode.
**         Version specific information for Freescale 56800 derivatives
**         DMA mode:
**         If DMA controller is available on the selected CPU and the
**         receiver is configured to use DMA controller then this event
**         is disabled. Only OnFullRxBuf method can be used in DMA mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void AS1_OnTxChar(void);
/*
** ===================================================================
**     Event       :  AS1_OnTxChar (module Events)
**
**     Component   :  AS1 [AsynchroSerial]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void AS1_OnFullRxBuf(void);
/*
** ===================================================================
**     Event       :  AS1_OnFullRxBuf (module Events)
**
**     Component   :  AS1 [AsynchroSerial]
**     Description :
**         This event is called when the input buffer is full;
**         i.e. after reception of the last character 
**         that was successfully placed into input buffer.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void AS1_OnFreeTxBuf(void);
/*
** ===================================================================
**     Event       :  AS1_OnFreeTxBuf (module Events)
**
**     Component   :  AS1 [AsynchroSerial]
**     Description :
**         This event is called after the last character in output
**         buffer is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void TI1_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  TI1_OnInterrupt (module Events)
**
**     Component   :  TI1 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void EC83_OnEnd(void);
/*
** ===================================================================
**     Event       :  EC83_OnEnd (module Events)
**
**     Component   :  EC83 [EventCntr8]
**     Description :
**         This event is called when the specified number of iterations
**         is generated. This event is available only if a <interrupt
**         service/event> is enabled, the component is in <Repetitive mode>
**         and is enabled - <Enable> method and the events are
**         enabled - <EnableEvent> method.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void EC82_OnEnd(void);
/*
** ===================================================================
**     Event       :  EC82_OnEnd (module Events)
**
**     Component   :  EC82 [EventCntr8]
**     Description :
**         This event is called when the specified number of iterations
**         is generated. This event is available only if a <interrupt
**         service/event> is enabled, the component is in <Repetitive mode>
**         and is enabled - <Enable> method and the events are
**         enabled - <EnableEvent> method.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void EC81_OnEnd(void);
/*
** ===================================================================
**     Event       :  EC81_OnEnd (module Events)
**
**     Component   :  EC81 [EventCntr8]
**     Description :
**         This event is called when the specified number of iterations
**         is generated. This event is available only if a <interrupt
**         service/event> is enabled, the component is in <Repetitive mode>
**         and is enabled - <Enable> method and the events are
**         enabled - <EnableEvent> method.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void IFsh1_OnWriteEnd(void);
/*
** ===================================================================
**     Event       :  IFsh1_OnWriteEnd (module Events)
**
**     Component   :  IFsh1 [IntFLASH]
*/
/*!
**     @brief
**         Event is called after a write operation to FLASH memory is
**         finished (except [SetPage]). This event is available only if
**         an [Interrupt service/event] is selected.
*/
/* ===================================================================*/

/*!
**     @brief
**         Event is called when an protection violation occurred.
*/
/* ===================================================================*/

void PWMC1_OnFault0(void);
/*
** ===================================================================
**     Event       :  PWMC1_OnFault0 (module Events)
**
**     Component   :  PWMC1 [PWMMC]
**     Description :
**         This event is called when fault 0 occurs. Only when the
**         component is enabled - <Enable> and the events are enabled -
**         <EnableEvent>. This event is enabled only if <Interrupt
**         service/event> is enabled and property <Fault protection> is
**         set to controlled by this component. The event clears Fault
**         flag only when the Fault is set to the automatic clearing
**         mode. When the Fault is set to the manual clearing mode, the
**         Fault flag must be cleared by the user using ClearFaultFlag()
**         method.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void CAN1_OnFullRxBuffer(void);
/*
** ===================================================================
**     Event       :  CAN1_OnFullRxBuffer (module Events)
**
**     Component   :  CAN1 [FreescaleCAN]
**     Description :
**         This event is called when the receive buffer is full after a
**         successful reception of a message. The event is available
**         only if Interrupt service/event is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void AD1_OnEnd(void);
/*
** ===================================================================
**     Event       :  AD1_OnEnd (module Events)
**
**     Component   :  AD1 [ADC]
**     Description :
**         This event is called after the measurement (which consists
**         of <1 or more conversions>) is/are finished.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void CAN1_OnFreeTxBuffer(word BufferMask);
/*
** ===================================================================
**     Event       :  CAN1_OnFreeTxBuffer (module Events)
**
**     Component   :  CAN1 [FreescaleCAN]
**     Description :
**         This event is called after a successful transmission of a
**         message. The event is available only if Interrupt
**         service/event is enabled.
**     Parameters  :
**         NAME            - DESCRIPTION
**         BufferMask      - Transmit buffer mask. The
**                           mask can be used to check what message
**                           buffer caused the transmit interrupt.
**     Returns     : Nothing
** ===================================================================
*/

void CAN1_OnBusOff(void);
/*
** ===================================================================
**     Event       :  CAN1_OnBusOff (module Events)
**
**     Component   :  CAN1 [FreescaleCAN]
**     Description :
**         This event is called when the node status becomes bus-off.
**         The event is available only if Interrupt service/event is
**         enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void CAN1_OnError(void);
/*
** ===================================================================
**     Event       :  CAN1_OnError (module Events)
**
**     Component   :  CAN1 [FreescaleCAN]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. These errors can be read
**         using <GetError> method. The event is available only if
**         Interrupt service/event is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/* END Events */
#endif /* __Events_H*/

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
