 .ifndef LOGO_INC
LOGO_INC  = 1
;===================================================
; the X16 logo is only available in 20x10 mode
; it consists of 20 sprites, with the 10 on the right half using the same data as the left half, only mirrored horizontally.
;===================================================
logo_SPR_Z_flip: .byte $00
logo_on:         .byte $00
;
; logo SPR attribute look up array:
;               VRAM_adr_lo, xpos_lo left, right, ypos_lo, ypos_hi, width/height
logo_attr0:     .byte $00,      $00,        $58,    $C0,    $00,        $00
logo_attr1:     .byte $02,      $06,        $52,    $C8,    $00,        $00
logo_attr2:     .byte $04,      $06,        $4A,    $D0,    $00,        $50
logo_attr3:     .byte $0C,      $16,        $42,    $D0,    $00,        $40
logo_attr4:     .byte $10,      $0E,        $42,    $E0,    $00,        $10
logo_attr5:     .byte $14,      $1E,        $32,    $DC,    $00,        $50
logo_attr6:     .byte $1C,      $1E,        $32,    $F4,    $00,        $50
logo_attr7:     .byte $24,      $0E,        $42,    $FA,    $00,        $50
logo_attr8:     .byte $2C,      $0C,        $4C,    $0A,    $01,        $00
logo_attr9:     .byte $2E,      $06,        $52,    $10,    $01,        $00
;
;===================================================
logo_half:
    ldx #$0A         ; 1st 10 left logo sprites
    lda logo_SPR_Z_flip
   :sta VERA_data0   ; sprite Z
    dex
    bne :-
    rts
;=======================
setlogo:
; turns logo  on or off
    lda X16logo_on
    bne :+
    jmp logo_switchoff
   :jmp logo_switchon
;=======================
logo_switchoff:
; turns off all 20 logo sprites by setting the Z value
; sprites 10 - 20 will also be flipped horizontally
    stz VERA_ctrl
    lda #$41   ;stride = 8
    sta VERA_addr_bank
    lda #$FC
    sta VERA_addr_high
    lda #$56        ; $50+6
    sta VERA_addr_low
    ;
    stz logo_SPR_Z_flip
    ;
    jsr logo_half
    ;
    jsr logo_half
    ;
    stz logo_on
    rts
    ;
;=====================================
logo_switchon:
    stz VERA_ctrl
    lda #$41   ;stride = 8
    sta VERA_addr_bank
    lda #$FC
    sta VERA_addr_high
    lda #$56
    sta VERA_addr_low
    ;
    lda #$08
    sta logo_SPR_Z_flip
    ;
    jsr logo_half
    ;
    lda logo_SPR_Z_flip
    ora #$01
    sta logo_SPR_Z_flip
    ;
    jsr logo_half
    ;
    lda #$01
    sta logo_on
    rts
;=========================================
; creates sprites data for logo
WRITE_4z_4_8_byte:
    jsr WRITE_4z_byte
    jsr WRITE_4_byte
    jsr WRITE_8_byte
    jsr WRITE_4z_byte
    jsr WRITE_4_byte
    jsr WRITE_8_byte
    jsr WRITE_4z_byte
    jsr WRITE_4_byte
    jsr WRITE_8_byte
    jsr WRITE_4z_byte
    jsr WRITE_4_byte
    jsr WRITE_8_byte
    rts
    ;
;====================
.macro create_logoSPR
    stz VERA_ctrl
    lda #$11   ;stride = 1, start addr: $12000
    sta VERA_addr_bank
    lda #$20
    sta VERA_addr_high
    stz VERA_addr_low
; logoSPR_0 8x8:
    lda #$FC    ; dark pink
    jsr WRITE_64_byte
; logoSPR_1 8x8:
    lda #$F8    ; magenta
    jsr WRITE_64_byte
; logoSPR_2 16x16:
    lda #$BF    ; dark blue
    jsr WRITE_64_byte
    jsr WRITE_64_byte
    ; ob. haelfte
    lda #$C0    ; bright blue
    jsr WRITE_64_byte
    lda #$A4    ; torquiose blue
    jsr WRITE_64_byte
    ; unt. haelfte
; logoSPR_3 8x16:
    lda #$BF    ; dark blue
    jsr WRITE_4_byte
    jsr WRITE_4z_byte
    jsr WRITE_4_byte
    jsr WRITE_4z_byte
    jsr WRITE_4_byte
    jsr WRITE_4z_byte
    jsr WRITE_4_byte
    jsr WRITE_4z_byte
    jsr WRITE_16_byte
    jsr WRITE_16_byte
    ;
    lda #$C0    ; bright blue
    jsr WRITE_16_byte
    lda #$A4    ; turquoise blue
    jsr WRITE_16_byte
    jsr WRITE_16_byte
    jsr WRITE_16_byte
    ;
; logoSPR_4 16x8:
;   lda #$A4    ;turquoise blue
    jsr WRITE_4z_4_8_byte
    jsr WRITE_4z_4_8_byte
    ;
; logoSPR_5 16x16:
    ;lda #$A4    ; turquoise blue
    jsr WRITE_8_byte
    jsr WRITE_4z_byte
    jsr WRITE_4z_byte
    jsr WRITE_8_byte
    jsr WRITE_4z_byte
    jsr WRITE_4z_byte
    jsr WRITE_8_byte
    jsr WRITE_4z_byte
    jsr WRITE_4z_byte
    jsr WRITE_8_byte
    jsr WRITE_4z_byte
    jsr WRITE_4z_byte
    ;
    jsr WRITE_64_byte
    ;
    lda #$72    ; moos gruen
    jsr WRITE_64_byte
    jsr WRITE_64_byte
; logoSPR_6 16x16:
    lda #$6C    ; "LCD green"
    jsr WRITE_64_byte
    jsr WRITE_64_byte
    ;
    lda #$50    ; okker
    jsr WRITE_64_byte
    jsr WRITE_64_byte
; logoSPR_7 16x16:
    lda #$49    ; bright okker
    jsr WRITE_4z_4_8_byte
    jsr WRITE_64_byte
    ;
    lda #$08    ; pseudo bright orange
    jsr WRITE_64_byte
    jsr WRITE_64_byte
; logoSPR_8 8x8:
    lda #$34    ; fire red
    jsr WRITE_64_byte
    ;
; logoSPR_9 8x8:
    lda #$32    ; dark red
    jsr WRITE_64_byte
.endmacro
;=========================================
; set sprite attributes
set_logoSPR_half:
    stz ZP_tmp_DATA2
    ldy #$00
    ldx #$0A
    ;
@sl_loop:
    lda (ZP_PTR_4),y    ; #<(VRAM_logo >> 5)
    sta VERA_data0      ; VRAM addr. lo
    lda #$09            ; #>(VRAM_logo >> 5) allways $09
    ora #$80            ; Bit 7 : 8bpp mode
    sta VERA_data0      ; VRAM addr. hi
    iny
; position:
    lda ZP_tmp_DATA
    bne :+
; do sprites of left logo half
    lda (ZP_PTR_4),y    ; xpos lo left sprites
    sta VERA_data0      ; xpos
    iny
    bra :++
; do sprites of right logo half
   :iny
    lda (ZP_PTR_4),y    ; xpos lo right sprites
    sta VERA_data0      ; xpos
    lda #$01
    sta ZP_tmp_DATA2
   :lda #$01            ; d256
    sta VERA_data0      ; xpos hi allways $01
    ;
    iny
    lda (ZP_PTR_4),y
    sta VERA_data0      ; ypos lo
    iny
    lda (ZP_PTR_4),y
    sta VERA_data0      ; ypos hi
    ;
    lda ZP_tmp_DATA2
    sta VERA_data0      ; z-depth: $00 = disabled, $08 = between layer0 and layer 1
    ;
    iny
    lda (ZP_PTR_4),y
    sta VERA_data0      ; sprite8W,8H=$00, palette offset 0
    ;
    iny                 ; for the next in the loop
    dex
    bne @sl_loop
    ;
    rts
;========================
; set SPR attrb. for the 20 X16 logo sprites
; logo spr attrb. registers start @ 1FC50
.macro set_logoSPR  ; run only once at program init
    stz ZP_tmp_DATA ; selects left (0..9) or right half (10..19) logo sprites
    lda #<logo_attr0
    sta ZP_PTR_4
    lda #>logo_attr0
    sta ZP_PTR_4+1  ; provide pointer to look up array
    ;
    stz VERA_ctrl
    lda #$11        ;stride = 1
    sta VERA_addr_bank
    lda #$FC
    sta VERA_addr_high
    lda #$50        ; +$50 = sprite 10, logoSPR_0
    sta VERA_addr_low
    ;
    jsr set_logoSPR_half
    inc ZP_tmp_DATA ; now right half selected
    jsr set_logoSPR_half
    ;
.endmacro
;=========================================
.endif
