.ifndef  CREATE_CRSRSPRITES_INC
CREATE_CRSRSPRITES_INC = 1
;====================================
crsr_col:               .byte $0E
;crsr_bgcol:             .byte $10
;==================================================================================
vlineDemo_baseadr:
    stz VERA_ctrl
    lda #$11
    sta VERA_addr_bank
    lda #$FD
    sta VERA_addr_high
    rts
;===================
vlineDemo_switchon:
    jsr vlineDemo_baseadr
    lda #$B6        ; sprite #52
    sta VERA_addr_low
    ;
    lda #$08
    sta VERA_data0  ; z-depth:  $00=initially off; $08=between layer0 and layer 1
    rts
;===================
vlineDemo_switchoff:
    jsr vlineDemo_baseadr
    sta VERA_addr_high
    lda #$B6        ; +6 = z, sprite #52
    ;
    sta VERA_addr_low
    stz VERA_data0
    rts
;=================
set_vlineDemo_n_pos:
    jsr vlineDemo_baseadr
    lda #$B2        ; +2 = pos, sprite #52
    sta VERA_addr_low
    ; position
    lda #$20
    sta VERA_data0  ; xpos lo
    lda #$02
    sta VERA_data0  ; xpos hi
    ; position
    lda #$90
    sta VERA_data0  ; ypos lo
    lda #$01
    sta VERA_data0  ; ypos hi
    rts
;=================
set_vlineDemo_h_pos:
    jsr vlineDemo_baseadr
    lda #$B2        ; sprite #52
    sta VERA_addr_low
    ; position
    lda #$CD
    sta VERA_data0  ; xpos lo
    lda #$00
    sta VERA_data0  ; xpos hi
    ;
    lda #$DC
    sta VERA_data0  ; ypos lo
    lda #$00
    sta VERA_data0  ; ypos hi
    rts
;=========================================== create cursor sprite
;=====================
;.macro WRITE_ncursor    ;cursor_bothhalf
WRITE_ncursor:    ; cursor in A on entry
    ldx #$40    ; = $4 * $10 = d4 * d16= d64
   :sta VERA_data0
    dex
    bne :-
    ;
    ldx #$06    ; 6 !
   :lda crsr_col
    jsr WRITE_8_byte
    jsr WRITE_16z_byte
 ;   lda crsr_col
    jsr WRITE_8_byte
    dex
    bne :-
    rts
;.endmacro
;=======================
.macro create_cursor; 32x8 pix
    stz VERA_ctrl
    lda #$11   ;stride = 1, start addr: $13000
    sta VERA_addr_bank
    lda #$30 ;(>VRAM_cursor)
    sta VERA_addr_high
    stz VERA_addr_low
    ;
    lda #<crsr_colschemes
    sta ZP_PTR_3
    lda #>crsr_colschemes
    sta ZP_PTR_3+1
    ;
    stz ZP_tmp_DATA
    lda ZP_tmp_DATA
@c_loop:
    tay
    lda (ZP_PTR_3),y
    sta crsr_col
;     iny
;     lda (ZP_PTR_3),y  ; no use for crsr bg col so far
;     sta crsr_bgcol
    ;
    jsr WRITE_ncursor
    ;
    inc  ZP_tmp_DATA
    inc  ZP_tmp_DATA
    lda  ZP_tmp_DATA
    cmp #$20        ; 16*2 sprite data sets
    bmi @c_loop
.endmacro
;=======================
.macro  WRITE_hcursor
  ;  lda crsr_col
    jsr WRITE_16_byte
    jsr WRITE_16_byte
    ;
    jsr WRITE_4_byte

    jsr WRITE_4z_byte
    jsr WRITE_4z_byte
    ;
    jsr WRITE_8_byte
    jsr WRITE_4z_byte
    jsr WRITE_4z_byte
    ;
 ;   lda crsr_col
    jsr WRITE_8_byte
    jsr WRITE_4z_byte
    jsr WRITE_4z_byte
  ;  lda crsr_col
    jsr WRITE_4_byte

    ldx #$0C        ; d12
@z_loop:
    jsr WRITE_4z_byte
    dex
    bne @z_loop
.endmacro
;=======================
.macro create_hcursor; 16x8 pix
    stz VERA_ctrl
    lda #$11   ;stride = 1, start addr: $14000
    sta VERA_addr_bank
    lda #$40 ;(>VRAM_hcursor)
    sta VERA_addr_high
    stz VERA_addr_low
    ;
    lda #<crsr_colschemes
    sta ZP_PTR_3
    lda #>crsr_colschemes
    sta ZP_PTR_3+1
    ;
    stz ZP_tmp_DATA
    ;ldy ZP_tmp_DATA
    ldy #$00    ; ZP_tmp_DATA
   :lda (ZP_PTR_3),y
    sta crsr_col
;   iny
;   lda (ZP_PTR_3),y    ; no use for crsr bg col so far
;   sta crsr_bgcol
    ;
    WRITE_hcursor
    ;
    inc  ZP_tmp_DATA
    inc  ZP_tmp_DATA    ; skip the background color, so inc twice
    ldy  ZP_tmp_DATA
    cpy #$20            ; 16*2(bytes) sprite data sets
    bmi :-
    ;
.endmacro
;===============================================================
.macro write_vlineSPR   ; single vline sprite
    ldx #$40            ; 64 pix height
   :stz VERA_data0
    stz VERA_data0
    lda (ZP_PTR_3),y    ; read from crsr_colschemes
    jsr WRITE_4_byte
    stz VERA_data0
    stz VERA_data0
    dex
    bne :-
.endmacro
;=======================
.macro create_verlineSPR
    stz VERA_ctrl
    lda #$11        ; stride = 1, start addr: $10000
    sta VERA_addr_bank
    stz VERA_addr_high
    stz VERA_addr_low
    ;
    lda #<crsr_colschemes
    sta ZP_PTR_3
    lda #>crsr_colschemes
    sta ZP_PTR_3+1
    ;
    ldy #$00
@wvl_loop:
    write_vlineSPR
    iny
    iny
    cpy #$20        ; 16 sprite data sets
    bmi @wvl_loop
    ;
; set sprite attr to v line
    stz VERA_ctrl
    lda #$11
    sta VERA_addr_bank
    lda #$FC
    sta VERA_addr_high
    lda #$10        ; jump over sprite #0 and cursor sprite #1 attr
    sta VERA_addr_low
    ;
    lda #>(VRAM_vlineSPR >> 5)
    ora #$80        ; Bit 7 : 8bpp mode
    sta ZP_tmp_DATA
    ;
    stz ZP_PTR_5
    stz ZP_PTR_5+1
    ;
    ldx #$08
   :stz VERA_data0  ; 8 sprite attr used: #2-#9
    lda ZP_tmp_DATA
    sta VERA_data0  ; sprite data addr hi
; position
    stz VERA_data0  ; xpos lo
    stz VERA_data0  ; xpos hi
    ;
    lda ZP_PTR_5
    sta VERA_data0  ; ypos lo
    lda ZP_PTR_5+1
    sta VERA_data0  ; ypos hi
    ;
    stz VERA_data0  ; z-depth:  $00=initially off; $08=between layer0 and layer 1
    lda #$C0        ; sprite8W,64H=$20, palette offset 0
    sta VERA_data0
    ;
    clc
    lda ZP_PTR_5
    adc #$40        ; next sprite 64 pix down
    sta ZP_PTR_5
    bcc :+
    inc ZP_PTR_5+1
   :dex
    bne :--
    ;
    ;DEMO vline:
    jsr vlineDemo_baseadr
    lda #$B0        ; sprite #52
    sta VERA_addr_low
    ;
    stz VERA_data0
    ;
    lda #>(VRAM_vlineSPR >> 5)
    ora #$80        ; Bit 7 : 8bpp mode
    sta VERA_data0
    ; position
    lda #$20
    sta VERA_data0  ; xpos lo
    lda #$02
    sta VERA_data0  ; xpos hi
    ; position
    lda #$90
    sta VERA_data0  ; ypos lo
    lda #$01
    sta VERA_data0  ; ypos hi
    ;
    lda #$00
    sta VERA_data0  ; z-depth:  $00=initially off; $08=between layer0 and layer 1
    lda #$C0        ; sprite8W,64H=$20, palette offset 0
    sta VERA_data0
    ;
.endmacro
;=================================================================================
.endif
