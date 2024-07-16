;===============================================================================;
;                                                                               ;
; DOT MATRIX for Commander X16                                       Version 9  ;
;                                                                               ;
; turns the X16 screen into a 5x7 dots per character display                    ;
; supports 2 modes with 20*10 or 40*20 characters per screen                    ;
; provides 8 pages of text and a simple sequencer                               ;
; each page may be individually set to different colors and mode                ;
; features some simple pseudo random generators for garbage text and pattern    ;
;                                                                               ;
; requires a 65C02S CPU: PRG makes use of smbx, rmbx, bbsx, bbrx etc. commands  ;
;                                                                               ;
;===============================================================================;
.org $080D
.segment "STARTUP"
.segment "INIT"
.segment "ONCE"
.segment "CODE"
;
    jmp start
;
;===============================================================================;
.include "dot_zp.inc"               ; zero page usage
.include "dotsysdeclarations.inc"   ; usual X16 stuff + some more includes
;====================================== program start ==========================;
start:
    initdotmatrix                   ; initializations + intro
;====================================== main loop ==============================;
main_loop:
    wai
    ;
    jsr GETIN           ; checks if keyboard key was pressed
    sta char_value
    beq main_loop
    jmp(use_key_vector) ; call keyboard routine
    ; keyb routine may return to 'main_loop', 'enter_char', 'output' or 'reset'
    ; coming back from keyboard routine - ...if so
enter_char:             ; insert new character to user text
    jsr cursor_switchon ; turn on cursor  - just in case it was off
    jsr char_2_txtRAM   ; returns 0 if char was written to text RAM
    bne output          ; don't increment cursor, when nothing was written
    jsr cursor_incposx  ; increment cursor pos after char written
    jsr move_cursor     ; and don't forget to reposition
;=======================
output:
    smb0 textrefresh    ; status bit handled by irq
    jmp main_loop
;====================================== exit program ===========================;
; program has no other exit than reset
reset:
    ; don't care restoring irq, we reset anyway...
    ldx #$42  ; did "Deep Thought" count decimal or hex?
    ldy #$02  ; magic location for system reset
    lda #$00  ; magic value for system poweroff
    jmp REBOOT_X16
;===============================================================================;
.include "dotcopyright.inc"
;===============================================================================;
