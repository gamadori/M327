/* ###################################################################
**     Filename    : Events.c
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
** @file Events.c
** @version 01.03
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"
#include "Timer.h"
#include "bldcUser.h"
#include "Encoder.h"
#include "Drive.h"
#include "Current.h"
#include "Can.h"

/* User includes (#include below this line is not maintained by Processor Expert) */

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
/* Comment following line if the appropriate 'Interrupt preserve registers' property */
/* is set to 'yes' (#pragma interrupt saveall is generated before the ISR)           */
#pragma interrupt called
void SM1_OnRxChar(void)
{
  /* Write your code here ... */
}

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
/* Comment following line if the appropriate 'Interrupt preserve registers' property */
/* is set to 'yes' (#pragma interrupt saveall is generated before the ISR)           */
#pragma interrupt called
void SM1_OnTxChar(void)
{
  /* Write your code here ... */
}

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
/* Comment following line if the appropriate 'Interrupt preserve registers' property */
/* is set to 'yes' (#pragma interrupt saveall is generated before the ISR)           */
#pragma interrupt called
void SM1_OnFullRxBuf(void)
{
  /* Write your code here ... */
}

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
/* Comment following line if the appropriate 'Interrupt preserve registers' property */
/* is set to 'yes' (#pragma interrupt saveall is generated before the ISR)           */
#pragma interrupt called
void SM1_OnFreeTxBuf(void)
{
  /* Write your code here ... */
}

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
/* Comment following line if the appropriate 'Interrupt preserve registers' property */
/* is set to 'yes' (#pragma interrupt saveall is generated before the ISR)           */
#pragma interrupt called
void SM1_OnError(void)
{
  /* Write your code here ... */
}

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
/* Comment following line if the appropriate 'Interrupt preserve registers' property */
/* is set to 'yes' (#pragma interrupt saveall is generated before the ISR)           */
#pragma interrupt called
void AS1_OnError(void)
{
  /* Write your code here ... */
}

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
/* Comment following line if the appropriate 'Interrupt preserve registers' property */
/* is set to 'yes' (#pragma interrupt saveall is generated before the ISR)           */
#pragma interrupt called
void AS1_OnRxChar(void)
{
  /* Write your code here ... */
}

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
/* Comment following line if the appropriate 'Interrupt preserve registers' property */
/* is set to 'yes' (#pragma interrupt saveall is generated before the ISR)           */
#pragma interrupt called
void AS1_OnTxChar(void)
{
  /* Write your code here ... */
}

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
/* Comment following line if the appropriate 'Interrupt preserve registers' property */
/* is set to 'yes' (#pragma interrupt saveall is generated before the ISR)           */
#pragma interrupt called
void AS1_OnFullRxBuf(void)
{
  /* Write your code here ... */
}

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
/* Comment following line if the appropriate 'Interrupt preserve registers' property */
/* is set to 'yes' (#pragma interrupt saveall is generated before the ISR)           */
#pragma interrupt called
void AS1_OnFreeTxBuf(void)
{
  /* Write your code here ... */
}

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
/* Comment following line if the appropriate 'Interrupt preserve registers' property */
/* is set to 'yes' (#pragma interrupt saveall is generated before the ISR)           */
#pragma interrupt called
void TI1_OnInterrupt(void)
{
	TimerServer();
	GetEncoders();
	DriveServer();
}



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
/* Comment following line if the appropriate 'Interrupt preserve registers' property */
/* is set to 'yes' (#pragma interrupt saveall is generated before the ISR)           */
#pragma interrupt called
void IFsh1_OnWriteEnd(void)
{
  /* Write your code here ... */
}

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
/* Comment following line if the appropriate 'Interrupt preserve registers' property */
/* is set to 'yes' (#pragma interrupt saveall is generated before the ISR)           */
#pragma interrupt called
void PWMC1_OnFault0(void)
{
	faultOccors = TRUE;
}

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
/* Comment following line if the appropriate 'Interrupt preserve registers' property */
/* is set to 'yes' (#pragma interrupt saveall is generated before the ISR)           */
#pragma interrupt called
void CAN1_OnFullRxBuffer(void)
{
	short i;
	dword ID;
	byte type, len;
	byte buff[8];
	word bufferMask;
	byte frameFormat;
	word state;

	
	state = CAN1_GetStateRX();
	
	// Check what message have received data from CANBUS
	for (i = 0; i < CAN_FIFO_NUM_CHANNEL; i++)
	{
		if ((state & CanFifoMbMask[i]) == CanFifoMbMask[i])
		{			
			CAN1_ReadFrame(i, &ID, &type, &frameFormat, &len, buff);
			CanReceivedPush(i, ID, len, buff);
		}
	}
}

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
/* Comment following line if the appropriate 'Interrupt preserve registers' property */
/* is set to 'yes' (#pragma interrupt saveall is generated before the ISR)           */
#pragma interrupt called
void AD1_OnEnd(void)
{
  /* Write your code here ... */
}

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
/* Comment following line if the appropriate 'Interrupt preserve registers' property */
/* is set to 'yes' (#pragma interrupt saveall is generated before the ISR)           */
#pragma interrupt called
void CAN1_OnFreeTxBuffer(word BufferMask)
{
  /* Write your code here ... */
}

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
/* Comment following line if the appropriate 'Interrupt preserve registers' property */
/* is set to 'yes' (#pragma interrupt saveall is generated before the ISR)           */
#pragma interrupt called
void CAN1_OnBusOff(void)
{
  /* Write your code here ... */
}

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
/* Comment following line if the appropriate 'Interrupt preserve registers' property */
/* is set to 'yes' (#pragma interrupt saveall is generated before the ISR)           */
#pragma interrupt called
void CAN1_OnError(void)
{
  /* Write your code here ... */
}

/* END Events */

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
