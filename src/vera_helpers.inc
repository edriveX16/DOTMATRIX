.ifndef VERA_HELPERS_INC
VERA_HELPERS_INC  = 1
;==================================
VERA_addr_to_both_cursorSPR:    ; sets VRAM data pointer 0 to sprite #60 attr low address
    stz VERA_ctrl
    lda #$01        ; stride = 0
    sta VERA_addr_bank
    lda #$FD
    sta VERA_addr_high
    lda #$F0
    sta VERA_addr_low
    ; 2nd cursor (sprite #61) used only in 20x10 mode
    lda #$01                    ; sets VRAM data pointer 1 to sprite #61 attr low address
    sta VERA_ctrl
    lda #$01        ; stride = 0
    sta VERA_addr_bank
    lda #$FD
    sta VERA_addr_high
    lda #$F8
    sta VERA_addr_low
    rts
    ;
;====================================
VERA_addr_to_cursorZ:
    stz VERA_ctrl
    lda #$01  ; stride =0
    sta VERA_addr_bank
    lda #$FD
    sta VERA_addr_high
    lda #$F6  ;+6 = +6
    sta VERA_addr_low
    ;
    lda #$01
    sta VERA_ctrl
    lda #$01  ; stride =0
    sta VERA_addr_bank
    lda #$FD
    sta VERA_addr_high
    lda #$FE  ;+6 = +14
    sta VERA_addr_low
    rts
    ;
;====================================
VERA_addr_to_vlineSPR:  ; sets VRAM data pointer 0 to sprite #2 attr low address
    stz VERA_ctrl
    lda #$41        ; stride = 8
    sta VERA_addr_bank
    lda #$FC
    sta VERA_addr_high
    lda #$10
    sta VERA_addr_low
    rts
    ;
VERA_addr_to_vlineDEMOSPR:  ; sets VRAM data pointer 1 to sprite #52 attr low address
    stz VERA_ctrl
    lda #$01        ; stride = 0
    sta VERA_addr_bank
    lda #$FD
    sta VERA_addr_high
    lda #$B0
    sta VERA_addr_low
    rts
    ;
;====================================
VERA_addr_to_pageZ:
    stz VERA_ctrl
    lda #$01  ; stride =0
    sta VERA_addr_bank
    lda #$FD
    sta VERA_addr_high
    lda #$16  ; +6 = Z
    sta VERA_addr_low
    rts
;====================================
set_VERA_pageadr:
    stz VERA_ctrl
    lda #$01  ; stride =0
    sta VERA_addr_bank
    lda #$FD
    sta VERA_addr_high
    lda #$10  ; +0 = (lo adr >>5)
    sta VERA_addr_low
    ;
    lda current_usrtxthi
    and #$1C
    eor #$10
    asl
    asl
    ora #$80
    sta VERA_data0
    rts
;====================================
;====================================
WRITE_4_byte:
    sta VERA_data0
    sta VERA_data0
    sta VERA_data0
    sta VERA_data0
    rts
    ;
WRITE_4z_byte:
    stz VERA_data0
    stz VERA_data0
    stz VERA_data0
    stz VERA_data0
    rts
    ;
WRITE_8_byte:
    sta VERA_data0
    sta VERA_data0
    sta VERA_data0
    sta VERA_data0
    sta VERA_data0
    sta VERA_data0
    sta VERA_data0
    sta VERA_data0
    rts
    ;
;==========================
WRITE_8alternate_byte:  ; (decoder)
    sta VERA_data0
    stz VERA_data0
    sta VERA_data0
    stz VERA_data0
    sta VERA_data0
    stz VERA_data0
    sta VERA_data0
    stz VERA_data0
    rts
    ;
;==========================
WRITE_16_byte:
    ldy #$10
   :sta VERA_data0  ; write acc2VERA
    dey
    bne :-
    rts
    ;
WRITE_64_byte:      ; messes up .Y and .X
    ldx #$04
   :ldy #$10
   :sta VERA_data0  ; write acc2VERA
    dey
    bne :-
    dex
    bne :--
    rts
    ;
WRITE_16z_byte:
    ldy #$10            ; 16 times
   :stz VERA_data0      ; write zero
    dey
    bne :-
    rts
    ;
;=====================================
WRITE_hframe_tile_line:
    sta VERA_data0
    stz VERA_data0
    stz VERA_data0
    sta VERA_data0
    ;
    stz VERA_data0
    stz VERA_data0
    sta VERA_data0
    stz VERA_data0
    rts
    ;
;==========================================
.endif
