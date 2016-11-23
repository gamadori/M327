/** ###################################################################
**
**     (c) Freescale Semiconductor, Inc.
**     2004 All Rights Reserved
**
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

/* File: prototype.h */

#ifndef __PROTOTYPE_H
#define __PROTOTYPE_H

#include "port.h"
#include "arch.h"

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************/
/* Name mappings for DSP code portability             */
/******************************************************/

/* void setnostat (void); */
#define setnosat   archSetNoSat

/* void setstat32 (void); */
#define setsat32   archSetSat32

/* void Stop (void); */
#define Stop        archStop

/* void Trap (void); */
#define Trap        archTrap

/* void Wait (void); */
#define Wait        archWait

/* void EnableInt (void); */
#define EnableInt   archEnableInt

/* void DisableInt (void); */
#define DisableInt  archDisableInt

#define MAX_32 (Word32)0x7fffffffL
#define MIN_32 (Word32)0x80000000L

#define MAX_16 (Word16)0x7fff
#define MIN_16 (Word16)0x8000


#ifdef ITU_INTRINSICS

  /*******************************************************

     Predefined basic intrinsics.

     The intrinsic functions are defined in the compiler
     defined functions name space. They are redefined here
     according to the ETSI naming convention.

   ******************************************************/

  #include "intrinsics_56800E.h"

#else

  Word16 div_ls  (Word32 L_var1, Word16 L_var2);
  Word32 L_mult_ls (Word32 L_var1, Word16 L_var2);

  /* Use ITU C code */
  #include "basic_op.h"

#endif


#ifdef __cplusplus
}
#endif

#endif
