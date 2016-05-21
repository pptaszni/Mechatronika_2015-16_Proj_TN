
/*
 * MMCSD_HD.s
 *
 * Created: 2016-05-02 08:18:55
 *  Author: Mati
 *	Przeóbka biblioterki BASCOM  
 */ 
/*
 Copyright = Mcs Electronics/Vögel Franz Josef
Www = http://members.aon.at/voegel
Comment = MMC/SD/SDHC Driver For Avr-dos Compiler Library
Libversion = 3.04 test release
Date = 27 January 2009
Statement = No Source Code From The Library May Be Distributed In Any Form Unless You Have Permision From The Author
Statement = For support please check http://members.aon.at/voegel for contact address
History = No Known Bugs.
history=3.03, saved r23, xmega supported
history=3.04  removed card deselect for arduino shields that drive a buffer with CS
*/

[_MMC]

; Card commands
.equ _mmcsd_Cmd0 = &H40 + 0
.equ _mmcsd_Cmd1 = &H40 + 1
.equ _mmcsd_Cmd8 = &H40 + 8
.equ _mmcsd_Cmd9 = &H40 + 9
.equ _mmcsd_Cmd10 = &H40 + 10
.equ _mmcsd_Cmd16 = &H40 + 16
.equ _mmcsd_Cmd17 = &H40 + 17
.equ _mmcsd_Cmd24 = &H40 + 24
.equ _mmcsd_Cmd55 = &H40 + 55
.equ _mmcsd_Cmd58 = &H40 + 58
.equ _mmcsd_ACmd13 = &HC0 + 13
.equ _mmcsd_ACmd41 = &HC0 + 41

; this variables are needed anyway by the driver, so Dim it here, if they
; are not DIMmed in BASIC-Part

*#IF NOT VarExist("MMCSD_CardType")
   *BASIC: Dim MMCSD_CardType As Byte
*#ENDIF

*#IF NOT VarExist("_mmcsd_Timer1")
   *BASIC: Dim _mmcsd_Timer1 As Word
*#ENDIF

*#IF NOT VarExist("_mmcsd_Timer2")
   *BASIC: Dim _mmcsd_Timer2 As Word
*#ENDIF

;-------------------------------------------------------------------------------
; This routine builds a rudimentary Timer capability for the MMC/SD driver
; without using a hardware-timer of the AVR.
; It makes a delay of appr. 50µsec and decreases one or two counter
; Timer/Counter1 is for complete function (Init(Read/write)
; Timer/Counter2 is for wait-ready of card to accet new command
; Call of _mmcsd_wait50us decreases timer-1
; call of _mmcsd_wait50uswr decreases Timer-1 and Timer-2 (Wait-Ready Timer)
; If timer1 or 2 reaches 0, the Zero-Flag will be set.
; If the counter reaches 0, it will be decremented any more
;-------------------------------------------------------------------------------

_mmcsd_wait50uswr:
   set
   rjmp _mmcsd_wait50us1
_mmcsd_wait50us:
   clt
_mmcsd_wait50us1:
   push r24
   push r25
*#IF _xtal > 1000000
 * ldi r25, ( ( _XTAL * 50 ) / 5000000 ) - 9      ; 1 loop needs 5 cycles, 44 Cycles overhead
_mmcsd_wait50us2:
   NOP
   NOP
   dec r25
   brne _mmcsd_wait50us2
*#ENDIF
   clz
; Now timer2
   lds r24, {_mmcsd_Timer2}
   lds r25, {_mmcsd_Timer2 + 1}
   brtc _mmcsd_wait50us3                          ; only Timer1 ?
   sbiw r24, 1
   brcc _mmcsd_wait50us3
   adiw r24, 1
_mmcsd_wait50us3:
   sts  {_mmcsd_Timer2}, r24
   sts  {_mmcsd_Timer2 + 1}, r25
   breq _mmcsd_wait50us5
; now timer1
   lds r24, {_mmcsd_Timer1}
   lds r25, {_mmcsd_Timer1 + 1}
   sbiw r24, 1
   brcc _mmcsd_wait50us4
   adiw r24, 1
_mmcsd_wait50us4:
   sts  {_mmcsd_Timer1}, r24
   sts  {_mmcsd_Timer1 + 1}, r25
_mmcsd_wait50us5:
   pop r25
   pop r24
   ret



;-------------------------------------------------------------------------------
; Send Command 0 to Card
;-------------------------------------------------------------------------------
_mmcsd_Sendcmd0:
   ldi r16, _mmcsd_Cmd0                           ; load CMD0
_mmcsd_Sendcmdx4:
   LDI r20, 0                                     ; no command argument
_mmcsd_Sendcmdx3:
   ldi r21, 0
_mmcsd_Sendcmdx2:
   ldi r22, 0
_mmcsd_Sendcmdx1:
   ldi r23, 0
   rJmp _mmcsd_Send_Command


;-------------------------------------------------------------------------------
; Send Command 8 to Card
;-------------------------------------------------------------------------------
_mmcsd_Sendcmd8:
   ldi r16, _mmcsd_Cmd8                           ; Send CMD8
   LDI r20, &HAA                                  ; Pattern
   ldi r21, &H01                                  ; voltage range 2.7-3.6V
   rJmp _mmcsd_Sendcmdx2


;-------------------------------------------------------------------------------
; Send Command 9 to Card
;-------------------------------------------------------------------------------
_mmcsd_SendCmd9:
   ldi r16, _mmcsd_Cmd9
   rjmp _mmcsd_Send_Command



;-------------------------------------------------------------------------------
; Send Command 10 to Card
;-------------------------------------------------------------------------------
_mmcsd_SendCmd10:
   ldi r16, _mmcsd_Cmd10
   rjmp _mmcsd_Send_Command



;-------------------------------------------------------------------------------
; Send Command 16 to Card
;-------------------------------------------------------------------------------
_mmcsd_Sendcmd16:
   ldi r16, _mmcsd_Cmd16                          ; Send CMD16
   LDI r20, 0                                     ; Block size 512 Bytes
   ldi r21, 2
   rjmp _mmcsd_SendCMDx2


;-------------------------------------------------------------------------------
; Send Command 17 to Card
;-------------------------------------------------------------------------------
_mmcsd_SendCmd17:
   ldi r16, _mmcsd_Cmd17
   rjmp _mmcsd_Send_Command


;-------------------------------------------------------------------------------
; Send Command 24 to Card
;-------------------------------------------------------------------------------
_mmcsd_SendCmd24:
   ldi r16, _mmcsd_Cmd24
   rjmp _mmcsd_Send_Command


;-------------------------------------------------------------------------------
; Send Command 58 to Card
;-------------------------------------------------------------------------------
_mmcsd_Sendcmd58:
   ldi r16, _mmcsd_Cmd58                          ; Send CMD58
   rjmp _mmcsd_SendCMDx4

;-------------------------------------------------------------------------------
; Send Command A13
;-------------------------------------------------------------------------------
_mmcsd_Sendacmd13:
   ldi r16, _mmcsd_ACmd13                         ; Send ACMD13
   rjmp _mmcsd_SendCMDx4

;-------------------------------------------------------------------------------
; Send Command A41 with HSC-Bit cleared to Card
;-------------------------------------------------------------------------------
_mmcsd_Sendacmd41_0:
   ldi r16, _mmcsd_ACmd41                         ; Send ACMD41
   rjmp _mmcsd_SendCMDx4                          ; no command argument

;-------------------------------------------------------------------------------
; Send Command A41 with HSC-Bit set to Card
;-------------------------------------------------------------------------------
_mmcsd_Sendacmd41_1:
   ldi r16, _mmcsd_ACmd41                         ; Send ACMD41
   LDI r20, 0
   ldi r21, 0
   ldi r22, 0
   ldi r23, &H40                                  ; set HSC-Bit
   rjmp _mmcsd_Send_Command


;-------------------------------------------------------------------------------
; Get Card Response R3 4Bytes + CRC-Byte
;-------------------------------------------------------------------------------
_mmcsd_Get5Responses:
   rcall _mmcsd_Byte2SPI_FF                      ; MSB-Byte
   mov r23, r16
   rcall _mmcsd_Byte2SPI_FF
   mov r22, r16
   rcall _mmcsd_Byte2SPI_FF
   mov r21, r16
   rcall _mmcsd_Byte2SPI_FF                      ; LSB-Byte
   mov r20, r16
   rjmp _mmcsd_Byte2SPI_FF                       ; and CRC to r16




;-------------------------------------------------------------------------------
; Init MMC/SD and SDHC Card
;-------------------------------------------------------------------------------
[_DriveInit]
_DriveInit:
_mmcsd_Init:
*#IF Cmmc_soft = 0  and _XMEGA=0
   *BASIC: Spiinit
*#ENDIF
  clr r0
_mmcsd_Init1:
   rcall _mmcsd_Timer1_1000ms                     ; wait 1000ms for timeout

*#IF VarExist("MMCSD_Try")
   clr r16
   sts {MMCSD_Try}, r16
*#ENDIF

_mmcsd_Init2:
   clr r16
   sts {MMCSD_CardType}, r16
*#IF VarExist("MMCSD_Try")
   lds r16, {MMCSD_Try}
   inc r16
   sts {MMCSD_Try}, r16
*#ENDIF
;   rcall _mmcsd_Deselect
   rcall _mmcsd_select         ; mod by MCS for arduino shields that use the CS to drive a buffer




; Wait 50msec for power-on stabilize and wait between retries

   ldi r24, lbyte(1000)                           ; 1000 * 50µsec = 50msec
   ldi r25, hbyte(1000)

_mmcsd_Init3:
   rcall _mmcsd_wait50us                          ; Timeout
   brne _mmcsd_Init4
 * ldi r25, cpErrDriveInit1
   rjmp _mmcsd_Exit_Error

_mmcsd_Init4:
   sbiw r24, 1
   brne _mmcsd_Init3

_mmcsd_Init5:
   ldi r16, 10                                    ; send 80 dummy clocks

_mmcsd_Init6:
   push r16
   rcall _mmcsd_Byte2SPI_FF
   pop r16
   dec r16
   brne _mmcsd_Init6                              ; next MMCSD_Try

_mmcsd_Init7:
; Send Command0 and expect 1
   rcall _mmcsd_SendCmd0

   cpi r16, 1                                     ; leave cmd0 with idle-state
;   brne _mmcsd_Init2
   breq _mmcsd_Init8  ; OK

   ;now try with sending code for a normal card
   ldi r16,64  ; cmd0
   rcall _mmcsd_Byte2SPI                          ; send CMD
   clr r16
   rcall _mmcsd_Byte2SPI                          ; send Argument(4)
   clr r16
   rcall _mmcsd_Byte2SPI                          ; send Argument(3)
   clr r16
   rcall _mmcsd_Byte2SPI                          ; send Argument(2)
   clr r16
   rcall _mmcsd_Byte2SPI                          ; send Argument(1)
   ldi r16,149
   rcall _mmcsd_Byte2SPI                          ; send CRC

   rjmp _mmcsd_Init2

_mmcsd_Init8:
; Send Command8 and expect 1
   rcall _mmcsd_SendCmd8
   cpi r16, 1
   breq _mmcsd_Init9
   rjmp _mmcsd_Init14
_mmcsd_Init9:
; get result of Command8 XX XX 01 AA
   rcall _mmcsd_Get5Responses
   cpi r21, &H01
   brne _mmcsd_Init10
   cpi r20, &HAA
   breq _mmcsd_Init11
_mmcsd_Init10:
 * ldi r25, cpErrDriveInit2                       ; Voltage range not OK
   rjmp _mmcsd_Exit_Error

_mmcsd_Init11:
; Send ACommand41 (40 00 00 00) until result = 0 or timerout)
   rcall _mmcsd_SendACmd41_1
   cpi r16, 0
   breq _mmcsd_Init12
   rcall _mmcsd_wait50us
   brne  _mmcsd_Init11
 * ldi r25, cpErrDriveInit3
   rjmp _mmcsd_Exit_Error

_mmcsd_Init12:
; send Command58 and expect
   rcall _mmcsd_SendCmd58
   cpi r16, 0
   breq  _mmcsd_Init13
   rcall _mmcsd_wait50us
   brne _mmcsd_Init12
 * ldi r25, cpErrDriveInit4
   rjmp _mmcsd_Exit_Error

_mmcsd_Init13:
   rcall _mmcsd_Get5Responses
   ldi r24, 4                                     ; Card-MMCSD_CardType 4
   sts {MMCSD_CardType}, r24
   sbrs r23, 6                                    ; first byte from response
   rjmp _mmcsd_Init18
   ldi r24, 12                                    ; Card-MMCSD_CardType 12 (4 + 8)
   sts {MMCSD_CardType}, r24
   rjmp _mmcsd_Init18

_mmcsd_Init14:
   ; SDSC or MMC
   rcall _mmcsd_SendACmd41_0
   cpi r16, 2
   brcs _mmcsd_Init15
   ldi r24,1                                      ; Result > 1
   sts {MMCSD_CardType}, r24                      ; Card-Type = 1
   ldi r16, _mmcsd_Cmd1                           ; Send Cmd1
   rjmp _mmcsd_Init16
_mmcsd_Init15:
   ldi r24,2                                      ; Result <= 1
   sts {MMCSD_CardType}, r24                      ; Card-Type = 2
   ldi r16, _mmcsd_ACmd41                         ; Send ACmd41
_mmcsd_Init16:
   push r16
   rcall _mmcsd_Sendcmdx4                         ; fill 4 Arg-Byts with 0
   tst r16                                        ; result = 0?
   pop r16
   breq _mmcsd_Init17
   rcall _mmcsd_wait50us
   brne _mmcsd_Init16
 * ldi r25, cpErrDriveInit5
   rjmp _mmcsd_Exit_error

_mmcsd_Init17:
   rcall _mmcsd_Sendcmd16
   tst r16
   breq _mmcsd_Init18
   rjmp _mmcsd_Init2                              ; try again
_mmcsd_Init18:
   rjmp _mmcsd_Exit_Standard





;-------------------------------------------------------------------------------
; Send a Command to the Card
;
; Input with:
; r16    Command
; r20    Argument(0)
; r21    Argument(1)
; r22    Argument(2)
; r23    Argument(3)
;
; Return with:
; ZERO-Flag set      Command accepted
; ZERO-Flag cleared  Command not accepted
; r16       return value of Card
;-------------------------------------------------------------------------------

_mmcsd_Send_Command:
   ldi r17, 1                                     ; default CRC
   cpi r16, _mmcsd_Cmd0
   brne _mmcsd_Send_Command1
   ldi r17, &H95
_mmcsd_Send_Command1:
   CPI r16, _mmcsd_Cmd8
   brne _mmcsd_Send_Command2
   ldi r17, &H87
_mmcsd_Send_Command2:
   sbrs r16, 7                                    ; check for ACMD
   rjmp _mmcsd_Send_Command3
   Push r16
   push r17
   push r20
   push r21
   push r22
   push r23
   ldi r16, _mmcsd_Cmd55
   rcall _mmcsd_Sendcmdx4                         ; Send Command with 4 Arg-Byts = 0
   pop r23
   pop r22
   pop r21
   pop r20
   pop r17
   mov r24, r16                                   ; save last return (error-code)
   pop r16
   ANDI r16, &H7F                                 ; clear Bit 7 (ACMD-Bit)
   cpi r24, 2                                     ; res > 1 => Error
   brcs _mmcsd_Send_Command3                      ; no Error
   mov r16, r24                                   ; Error-code for output
   rjmp _mmcsd_Send_Command8
_mmcsd_Send_Command3:
*#IF VarExist("gbDriveLastCommand")
   sts {gbDriveLastCommand}, r16                  ; Save Command for debugging
*#ENDIF
   rcall _mmcsd_Deselect                          ; be sure, that card is deselected before starting
   RCALL _mmcsd_wait50us
   RCALL _mmcsd_Select                            ; Select card
   RCALL _mmcsd_Wait_Ready
   breq _mmcsd_Send_Command4                      ; is Card Ready?                   for wait-ready timeout
   ldi r16, &HE8                                  ; return-Error-Value
   rjmp _mmcsd_Send_Command8                      ; virt. Code for timeout at wait ready
_mmcsd_Send_Command4:
   rcall _mmcsd_Byte2SPI                          ; send CMD
   mov r16, r23
   rcall _mmcsd_Byte2SPI                          ; send Argument(4)
   mov r16, r22
   rcall _mmcsd_Byte2SPI                          ; send Argument(3)
   mov r16, r21
   rcall _mmcsd_Byte2SPI                          ; send Argument(2)
   mov r16, r20
   rcall _mmcsd_Byte2SPI                          ; send Argument(1)
   mov r16, r17
   rcall _mmcsd_Byte2SPI                          ; send CRC
; --- insert here check for Cmd12
 ;  rcall _mmcsd_Byte2SPI_FF                      ; send additional &HFF  modde by mcs
   Push r18
   ldi r18, 100                                    ; 10 attempts to wait for valid response
_mmcsd_Send_Command5:
   rcall _mmcsd_Byte2SPI_FF
   sez                                            ; Set OK sign
   sbrs r16, 7
   rjmp _mmcsd_Send_Command7
   rcall _mmcsd_wait50us
   breq _mmcsd_Send_Command6
   dec r18
   brne _mmcsd_Send_Command5
_mmcsd_Send_Command6:
   clz
_mmcsd_Send_Command7:
   pop r18
_mmcsd_Send_Command8:
*#IF VarExist("gbDriveStatusReg")
   sts {gbDriveStatusReg}, r16                    ; Save Card-Response for debugging
*#ENDIF
   RET



;-------------------------------------------------------------------------------
; Wait for SD/MMC Card to be ready
;
; Return with:
; ZERO-Flag set for Ready
; ZERO-Flag cleared for not Ready
;-------------------------------------------------------------------------------

_mmcsd_Wait_ready:
   push r16
   Push r24
   push r25                                       ; save used register
   ldi r24, lBYte(10000)                          ; 500mSec = 10000 * 50µs
   ldi r25, hByte(10000)
   sts {_mmcsd_Timer2}, r24                       ; Init "MMCSD Timer2"
   sts {_mmcsd_Timer2 + 1}, r25
_mmcsd_Wait_Ready1:
   rcall _mmcsd_Byte2SPI_FF                      ; send &HFF
   cpi r16, &HFF                                  ; get &HFF ?
   breq _mmcsd_Wait_Ready2                        ; leave with Zero-Flag set
   rcall _mmcsd_wait50uswr                        ; Delay of appr. 50µSec
   brne _mmcsd_Wait_Ready1
   clz                                            ; clear Zero-Flag for timeOut
_mmcsd_Wait_Ready2:
   pop r25
   pop r24
   pop r16
   ret



;-------------------------------------------------------------------------------
; Select Card
;-------------------------------------------------------------------------------

_mmcsd_Select:
#IF _XMEGA = 1
  push r23
#ENDIF
*BASIC: Reset Mmc_cs
#IF _XMEGA = 1
  pop r23
#ENDIF
ret



;-------------------------------------------------------------------------------
; DeSelect Card
;-------------------------------------------------------------------------------

_mmcsd_Deselect:
#IF _XMEGA = 1
  push r23
#ENDIF
*BASIC: Set Mmc_cs
#IF _XMEGA = 1
  pop r23
#ENDIF
ret



;-------------------------------------------------------------------------------
; send and receive a byte to the SPI-Interface
; In and out Byte in r16
;-------------------------------------------------------------------------------

*#IF Cmmc_soft = 1


_mmcsd_Byte2SPI_FF:
   ser r16
   rjmp _mmcsd_Byte2SPI
_mmcsd_Byte2SPI_00:
   clr r16
_mmcsd_Byte2SPI:
   push r23
   push r19                                       ; scratch register
   push r18                                       ; bit loop counter
   push r17                                       ; income Byte
   clr r17
   ldi r18, 8                                     ; 8 Bits to receive
_mmcsd_Byte2SPI_Loop:
   lsl r17
   lsl r16                                        ; byte to send
   brcs _mmcsd_Byte2SPI_1
  cbi MMC_PortMOSI, bMMC_MOSI
   rjmp _mmcsd_Byte2SPI_Clock
_mmcsd_Byte2SPI_1:
 * sbi MMC_PortMOSI, bMMC_MOSI
_mmcsd_Byte2SPI_Clock:
   in r19, MMC_PortMISO
  sbi MMC_PortSCK, bMMC_SCK
   nop                                            ; pulse width
   nop
   nop
  cbi MMC_PortSCK, bMMC_SCK
;   nop                                           ; following code have enough delay
;   nop
;   nop
  sbrc r19, bMMC_MISO
   inc r17
   dec r18
   brne _mmcsd_Byte2SPI_Loop
   mov r16, r17                                   ; return value
   pop r17
   pop r18
   pop r19
   pop r23
   ret

*#ELSE

*#IF _XMEGA = 1
 *  .equ SPI_DATA = _MMC_SPI+3
 *  .equ SPI_STATUS = _MMC_SPI+2

_mmcsd_Byte2SPI_FF:
   ser r16                                        ; send &HFF
   rjmp _mmcsd_Byte2SPI
_mmcsd_Byte2SPI_00:
   Clr r16                                        ; send 0
_mmcsd_Byte2SPI:
* STS SPI_DATA , R16
_mmcsd_Byte2SPI2:
 * lds R16 , SPI_STATUS
   rol r16
   brcc _mmcsd_Byte2SPI2
* lds R16 , SPI_DATA
   ret
 *#ELSE
 *  .equ SPI_DATA=Spdr
 *  .equ SPI_STATUS=Spsr

_mmcsd_Byte2SPI_FF:
   ser r16                                        ; send &HFF
   rjmp _mmcsd_Byte2SPI
_mmcsd_Byte2SPI_00:
   Clr r16                                        ; send 0
_mmcsd_Byte2SPI:
 * Out SPDR , R16
_mmcsd_Byte2SPI2:
 * Sbis SPSR , 7 ; Wait For Spif
 rjmp  _mmcsd_Byte2SPI2
 * In R16 , SPDR
   ret
*#ENDIF
*#ENDIF




;---[_MMC_ReadSector]-----------------------------------------------------------
; Read one Sector from MMC/SD-Card to SRAM
; Entry with following parameters set:
; Register X: Pointer to Sectornumber (LONG)
; Register Z: SRAM-Address, where the data to transfer
;-------------------------------------------------------------------------------
[_DriveReadSector]
_DriveReadSector:
_mmcsd_ReadSector:
   rcall _mmcsd_CheckInitialized
   breq _mmcsd_ReadSector1
   rcall _mmcsd_Timer1_1000ms
   rcall _mmcsd_setAddress
   rcall _mmcsd_SendCmd17
   tst r16                                        ; response = 0?
   breq _mmcsd_ReadSector2
 * Ldi R25 , CperrdriveRead1
_mmcsd_ReadSector1:
   rjmp _mmcsd_Exit_Error

_mmcsd_ReadSector2:
   rcall _mmcsd_Byte2SPI_FF
   cpi r16, &HFE                                  ; get data response byte
   breq _mmcsd_ReadSector3
   Rcall _mmcsd_wait50us
   brne _mmcsd_ReadSector2
 * Ldi R25 , Cperrdriveread2
   rjmp _mmcsd_Exit_Error

_mmcsd_ReadSector3:
   ldi r25,2                                      ; 512 bytes to read
   clr r24
_mmcsd_ReadSector4:
   rcall _mmcsd_Byte2SPI_FF
   st z+,r16
   sbiw r24,1
   brne _mmcsd_ReadSector4
   rcall _mmcsd_Byte2SPI_FF                       ; read first crc byte
   rcall _mmcsd_Byte2SPI_FF                       ; read second crc byte
   rjmp _mmcsd_Exit_Standard




;-------------------------------------------------------------------------------
; write one Sector from SRAM to MMC/SD-Card
; Entry with following parameter set:
; Register X: Pointer to Sectornumber (LONG)
; Register Z: SRAM-Address, at which Data to transfer starts
;-------------------------------------------------------------------------------
[_DriveWriteSector]
_DriveWriteSector:
_mmcsd_WriteSector:
   rcall _mmcsd_CheckInitialized
   breq _mmcsd_WriteSector1
   rcall _mmcsd_Timer1_1000ms
   rcall _mmcsd_setAddress
   rcall _mmcsd_SendCmd24
   tst r16                                        ; Command not accepted?
   breq _mmcsd_WriteSector2                       ; Response = 0?
 * Ldi R25 , Cperrdrivewrite1
_mmcsd_WriteSector1:
   rjmp _mmcsd_Exit_Error

_mmcsd_WriteSector2:
   rcall _mmcsd_Wait_ready
   breq _mmcsd_WriteSector3
 * ldi r25, cpErrdrivewrite2
   rjmp _mmcsd_Exit_Error
_mmcsd_WriteSector3:
   ldi r16, &HFE
   rcall _mmcsd_Byte2SPI
   cpi r16, &HFD
   brne _mmcsd_WriteSector4
 * ldi r25, cpErrdrivewrite3
   rjmp _mmcsd_Exit_Error
_mmcsd_WriteSector4:
   ldi r25,2                                      ; counter for 512 bytes
   clr r24
_mmcsd_WriteSector5:
   ld r16,z+
   rcall _mmcsd_Byte2SPI
   Sbiw r24,1
   brne _mmcsd_WriteSector5
   rcall _mmcsd_Byte2SPI_FF                       ; dummy crc bytes
   rcall _mmcsd_Byte2SPI_FF
_mmcsd_WriteSector6:
   rcall _mmcsd_Byte2SPI_FF                       ; read Data response byte (=xxx00101b)
   andi r16, &H1F
*#IF VarExist("gbDriveDebug")
   sts {gbDriveDebug}, r16                        ; save Data response byte for debug
*#ENDIF
   cpi r16, &H05
   breq _mmcsd_WriteSector7
*#IF VarExist("gbDriveStatusReg")
   sts {gbDriveStatusReg}, r16
*#ENDIF
   lsr r16
 * ori r16, cpErrDriveWrite4
   rjmp _mmcsd_Exit_Error
_mmcsd_WriteSector7:
   rjmp _mmcsd_Exit_Standard



;-------------------------------------------------------------------------------

_mmcsd_SetAddress:
   lds r20, {MMCSD_CardType}
   sbrc r20, 3                                    ; SDHC-Bit Set
   rjmp _mmcsd_setAddress2
   clr r20                                        ; Convert sector# to Byte#
   ld r21,x+   ;LSB
   ld r22,x+
   ld r23,x+
   lsl r21
   rol r22
   rol r23
   ret
_mmcsd_SetAddress2:
   ld r20, X+                                     ; load sector-address for SDHC
   ld r21, X+
   ld r22, X+
   ld r23, X+
   ret


_mmcsd_Timer1_1000ms:
   push r24
   push r25
   ldi r24, lByte(20000)                          ; Init MMCSD Timer1 for 1 Second
   ldi r25, hByte(20000)
   sts {_mmcsd_Timer1}, r24
   sts {_mmcsd_Timer1 + 1}, r25
   pop r25
   pop r24
   ret


_mmcsd_Exit_standard:
   rcall _mmcsd_Deselect
   rcall _mmcsd_Byte2SPI_FF
   clr r25                                        ; clear error byte
   clc                                            ; clear error flag
_mmcsd_Exit_standard1:
*#IF VarExist("gbDriveError")
   sts {gbDriveError}, r25
*#ENDIF
   ret


_mmcsd_Exit_error:
   push r25
   rcall _mmcsd_Deselect
   rcall _mmcsd_Byte2SPI_FF
   pop r25
   sec                                            ; set error flag
   rjmp _mmcsd_Exit_standard1


;-------------------------------------------------------------------------------
; check whether card is initialized
; Variable MMCSD_CardType should have value > 0
; leave with Zero-Flag set if no card is initialized and Error-Code in r25
;-------------------------------------------------------------------------------
_mmcsd_CheckInitialized:
   LDS r25, {MMCSD_CardType}
   tst r25
 * ldi r25, cpErrDriveNotInitialized
   ret


[_DriveReset]
_DriveReset:
   rjmp _mmcsd_Exit_standard


[_DriveGetIdentity]
_DriveGetIdentity:
 * ldi r25, cpErrDriveNotSupported
   rjmp _mmcsd_Exit_error

[_DriveCheck]
_DriveCheck:
 * ldi r25, cpErrDriveNotSupported
   rjmp _mmcsd_Exit_error

[end]

[mmcsd_GetSize]
$EXTERNAL _MMC
mmcsd_GetSize:
_mmcsd_GetSize:
   rcall _mmcsd_CheckInitialized
   breq _mmcsd_GetSize1
   rcall _mmcsd_Timer1_1000ms
   rcall _mmcsd_SendCmd9
   tst r16                                        ; response = 0?
   breq _mmcsd_GetSize2
 * Ldi R25 , cpErrDriveCmdNotAccepted
_mmcsd_GetSize1:
   jmp _mmcsd_Exit_Error

_mmcsd_GetSize2:
   loadadr ___strbuf , Z
   rcall _mmcsd_Byte2SPI_FF
   cpi r16, &HFE                                  ; get data response byte
   breq _mmcsd_GetSize3
   rcall _mmcsd_wait50us
   brne _mmcsd_GetSize2
 * Ldi R25 , cpErrDriveNoData
   jmp _mmcsd_Exit_Error

_mmcsd_GetSize3:
   ldi r25,0                                      ; 16 bytes to read
   ldi r24, 16
_mmcsd_GetSize4:
   rcall _mmcsd_Byte2SPI_FF
   st z+,r16
   sbiw r24,1
   brne _mmcsd_GetSize4
   rcall _mmcsd_Byte2SPI_FF                       ; read first crc byte
   rcall _mmcsd_Byte2SPI_FF                       ; read second crc byte
   rcall _mmcsd_Exit_Standard
   loadadr ___strbuf , Z
   LDS r25, {MMCSD_CardType}
   cpi r25, 1
   breq _mmcsd_GetSize5
   ldd r24, Z+0
   andi r24, &B11000000
   cpi r24, &H40
   brne _mmcsd_GetSize5

   ldd r19, Z+7
   andi r19, &B00111111
   ldd r18, Z+8
   ldd r17, Z+9
   clr r16
   subi r17, -1
   sbci r18, -1
   sbci r19, -1
   lsl r17
   rol r18
   rol r19
   rjmp _mmcsd_GetSize9
_mmcsd_GetSize5:
   clr r19
   ldd r18, Z+6
   andi r18, &B00000011
   ldd r17, Z+7
   ldd r16, Z+8
   andi r16, &B11000000
   subi r16, -64
   sbci r17, -1
   sbci r18, -1
   ldd r24, Z+9
   andi r24, &B00000011
   ldd r25, Z+10
   lsl r25
   rol r24
   breq _mmcsd_GetSize7
_mmcsd_GetSize6:
   rcall _mmcsd_GetSize10
   brne _mmcsd_GetSize6
_mmcsd_GetSize7:
   ldd r24, Z+5
   andi r24, &B00001111
   subi r24, 6
_mmcsd_GetSize8:
   rcall _mmcsd_GetSize10
   brne _mmcsd_GetSize8
   mov r16, r17
   mov r17, r18
   mov r18, r19
   clr r19

_mmcsd_GetSize9:
   ldd zl, Y+0
   ldd zh, Y+1
   st Z+, r16
   st Z+, r17
   st Z+, r18
   st Z+, r19
   ret

_mmcsd_GetSize10:
   lsl r16
   rol r17
   rol r18
   rol r19
   dec r24
   ret

[End]


[mmcsd_GetCSD]
$EXTERNAL _MMC
mmcsd_GetCSD:
_mmcsd_GetCSD:
   rcall _mmcsd_CheckInitialized
   breq _mmcsd_GetCSD1
   rcall _mmcsd_Timer1_1000ms
   rcall _mmcsd_SendCmd9
   tst r16                                        ; response = 0?
   breq _mmcsd_GetCSD2
 * Ldi R25 , cpErrDriveCmdNotAccepted
_mmcsd_GetCSD1:
   jmp _mmcsd_Exit_Error

_mmcsd_GetCSD2:
   ldd zl, Y+0
   ldd zh, Y+1
   rcall _mmcsd_Byte2SPI_FF
   cpi r16, &HFE                                  ; get data response byte
   breq _mmcsd_GetCSD3
   rcall _mmcsd_wait50us
   brne _mmcsd_GetCSD2
 * Ldi R25 , cpErrDriveNoData
   jmp _mmcsd_Exit_Error

_mmcsd_GetCSD3:
   ldi r25,0                                      ; 16 bytes to read
   ldi r24, 16
_mmcsd_GetCSD4:
   rcall _mmcsd_Byte2SPI_FF
   st z+,r16
   sbiw r24,1
   brne _mmcsd_GetCSD4
   rcall _mmcsd_Byte2SPI_FF                       ; read first crc byte
   rcall _mmcsd_Byte2SPI_FF                       ; read second crc byte
   rjmp _mmcsd_Exit_Standard

[End]


[mmcsd_GetCID]
$EXTERNAL _MMC
mmcsd_GetCID:
_mmcsd_GetCID:
   rcall _mmcsd_CheckInitialized
   breq _mmcsd_GetCID1
   rcall _mmcsd_Timer1_1000ms
   rcall _mmcsd_SendCmd10
   tst r16                                        ; response = 0?
   breq _mmcsd_GetCID2
 * Ldi R25 , cpErrDriveCmdNotAccepted
_mmcsd_GetCID1:
   jmp _mmcsd_Exit_Error

_mmcsd_GetCID2:
   ldd zl, Y+0
   ldd zh, Y+1
   rcall _mmcsd_Byte2SPI_FF
   cpi r16, &HFE                                  ; get data response byte
   breq _mmcsd_GetCID3
   rcall _mmcsd_wait50us
   brne _mmcsd_GetCID2
 * Ldi R25 , cpErrDriveNoData
   jmp _mmcsd_Exit_Error

_mmcsd_GetCID3:
   ldi r25,0                                      ; 16 bytes to read
   ldi r24, 16
_mmcsd_GetCID4:
   rcall _mmcsd_Byte2SPI_FF
   st z+,r16
   sbiw r24,1
   brne _mmcsd_GetCID4
   rcall _mmcsd_Byte2SPI_FF                       ; read first crc byte
   rcall _mmcsd_Byte2SPI_FF                       ; read second crc byte
   rjmp _mmcsd_Exit_Standard

[End]




[mmcsd_GetOCR]
$EXTERNAL _MMC
mmcsd_GetOCR:
_mmcsd_GetOCR:
   rcall _mmcsd_CheckInitialized
   breq _mmcsd_GetOCR1
   rcall _mmcsd_Timer1_1000ms
   rcall _mmcsd_SendCmd58
   tst r16                                        ; response = 0?
   breq _mmcsd_GetOCR2
 * Ldi R25 , cpErrDriveCmdNotAccepted
_mmcsd_GetOCR1:
   jmp _mmcsd_Exit_Error

_mmcsd_GetOCR2:
   rcall _mmcsd_Get5Responses
   ldd zl, Y+0
   ldd zh, Y+1
   st Z+, r23
   st Z+, r22
   St Z+, r21
   St Z+, r20
   rjmp _mmcsd_Exit_Standard

[End]



[sd_GetSD_Status]
$EXTERNAL _MMC
sd_GetSD_Status:
_sd_GetSD_Status:
   rcall _mmcsd_CheckInitialized
   breq _sd_GetSD_Status1
   rcall _mmcsd_Timer1_1000ms
   rcall _mmcsd_SendACmd13
   tst r16                                        ; response = 0?
   breq _sd_GetSD_Status2
 * Ldi R25 , cpErrDriveCmdNotAccepted
_sd_GetSD_Status1:
   jmp _mmcsd_Exit_Error

_sd_GetSD_Status2:
   ldd zl, Y+0
   ldd zh, Y+1
   rcall _mmcsd_Byte2SPI_FF
   rcall _mmcsd_Byte2SPI_FF
   cpi r16, &HFE                                  ; get data response byte
   breq _sd_GetSD_Status3
   rcall _mmcsd_wait50us
   brne _sd_GetSD_Status2
 * Ldi R25 , cpErrDriveNoData
   jmp _mmcsd_Exit_Error

_sd_GetSD_Status3:
   ldi r25,0                                      ; 16 bytes to read
   ldi r24, 64
_sd_GetSD_Status4:
   rcall _mmcsd_Byte2SPI_FF
   st z+,r16
   sbiw r24,1
   brne _sd_GetSD_Status4
   rcall _mmcsd_Byte2SPI_FF                       ; read first crc byte
   rcall _mmcsd_Byte2SPI_FF                       ; read second crc byte
   rjmp _mmcsd_Exit_Standard

[End]