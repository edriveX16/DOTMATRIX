.ifndef CREATE_INFO_SPRITES_INC
CREATE_INFO_SPRITES_INC = 1
;=======================================================================================
; functions to create these small info-sprites, which are displayed from above on occasion
; like e.g. 'page 0'...'page 7' or 'sequence start ' etc.
;=======================================================================================
;======================
; ZP_PTR_1 source text pointer
; ZP_PTR_2 target pointer to VRAM sprite data start to be created
; ZP_PTR_5 target pointer to VRAM sprite data start to be created, but will be messed up after each char
; ZP_PTR_4 source pointer to VRAM existing sys character set in VERA VRAM, but will be messed up after each char
; ZP_PTR_6 sourcepointer for sourcepointer to read from exisitng charset in VERA
; due to PTR_4,5 are messed up during sprite creating routines
;
;======================
VRAM_sysextra     = $1F100 ;
VRAM_sysnumbers   = $1F180 ;
VRAM_syslowcase   = $1F300 ;
VRAM_sysspecial   = $1F500 ;
;
;VRAM_charsprites    = $11000
;VRAM_syschars       = $1F000
;
target_startoffset:     .byte   $00
; sprite texts:
txtpage0SPR:
.byte $00,$80,$08,$38,$28, $00, $00,$00;, $00    ; ' page 0  '
.byte $00,$80,$08,$38,$28, $00, $08,$00;, $00    ; ' page 1  '
;txtpage2SPR:
.byte $00,$80,$08,$38,$28, $00, $10,$00;, $00    ; ' page 2  '
.byte $00,$80,$08,$38,$28, $00, $18,$00;, $00    ; ' page 3  '
;txtpage4SPR:
.byte $00,$80,$08,$38,$28, $00, $20,$00;, $00    ; ' page 4  '
.byte $00,$80,$08,$38,$28, $00, $28,$00;, $00    ; ' page 5  '
;txtpage6SPR:
.byte $00,$80,$08,$38,$28, $00, $30,$00;, $00    ; ' page 6  '
.byte $00,$80,$08,$38,$28, $00, $38,$00;, $00    ; ' page 7  '
txtseqinfoSPR:
.byte $98,$28,$88,$A8,$28, $70, $18,$28;, $00    ; 'sequence'
.byte $00,$98,$A0,$08,$90, $A0, $00,$00;, $00    ; ' start  '
.byte $00,$80,$08,$A8,$98, $28, $00,$00;, $00    ; ' pause  '
.byte $00,$98,$A0,$78,$80, $80, $28,$20;, $00    ; ' stopped'
.byte $00,$18,$78,$70,$A0, $70, $00,$00;, $00    ; ' cont.  '
; ;============================================
set_source_ptr_lower_case:
; adr. bit 16 is hardcoded to 1
    lda #<VRAM_syslowcase ; Source
    sta ZP_PTR_6
    lda #>VRAM_syslowcase ; Source
    sta ZP_PTR_6+1
    rts
;============================================
set_source_ptr_extra:
; adr. bit 16 is hardcoded to 1
    lda #<VRAM_sysextra   ; Source
    sta ZP_PTR_6
    lda #>VRAM_sysextra   ; Source
    sta ZP_PTR_6+1
    rts
;============================================
set_source_ptr_numbers:
; adr. bit 16 is hardcoded to 1
    lda #<VRAM_sysnumbers   ; Source
    sta ZP_PTR_6
    lda #>VRAM_sysnumbers   ; Source
    sta ZP_PTR_6+1
    rts
;============================================
set_source_ptr_special:
; adr. bit 16 is hardcoded to 1
    lda #<VRAM_sysspecial   ; Source
    sta ZP_PTR_6
    lda #>VRAM_sysspecial   ; Source
    sta ZP_PTR_6+1
    rts
;============================================
.macro set_target_pageSPR_data
    lda #<VRAM_charsprites  ; Target
    sta ZP_PTR_2    ;#<VRAM_charsprites  ; Target
    lda #>VRAM_charsprites ; Target
    sta ZP_PTR_2+1  ;#>VRAM_charsprites  ; Target
.endmacro
;============================================
.macro set_target_seqinfoSPR_data
    lda #<VRAM_seqinfoSPR  ; Target
    sta ZP_PTR_2
    lda #>VRAM_seqinfoSPR ; Target
    sta ZP_PTR_2+1
.endmacro
;============================================
.macro write_rawsinglesyscharSPR
    lda #$08
    sta ZP_tmp_DATA2
    ;
@char_loop:
    lda VERA_data1
    sta ZP_tmp_DATA
    iny
    ;
    ldx #$08
;@8bit_pix_line_loop
   :asl ZP_tmp_DATA
    bcc :+          ;----------     <- invertierung möglich
    lda cheat_litecol
    sta VERA_data0
    bra @ccend
   :lda #$10        ; hintergrund farbe
    sta VERA_data0
@ccend:
    dex
    bne :--
    ;
    clc
    lda ZP_PTR_5
    adc #$40
    sta ZP_PTR_5
    bcc :+
    inc ZP_PTR_5+1
    ;
   :stz VERA_ctrl ; Data0 = Target
    lda #$11            ; stride = 1
    sta VERA_addr_bank
    lda ZP_PTR_5+1
    sta VERA_addr_high
    lda ZP_PTR_5
    sta VERA_addr_low
    ;
    dec ZP_tmp_DATA2
    bne @char_loop
    ;
.endmacro
;============================================
page_prepare_nxtChar:
    clc
    lda ZP_PTR_1    ;#<txtpage0SPR
    adc #$08
    sta ZP_PTR_1    ; wird nicht vermasselt
    bcc :+
    inc ZP_PTR_1+1
   :clc
    lda ZP_PTR_2+1
    adc #$02
    sta ZP_PTR_2+1
    ;
    stz target_startoffset  ; gets incremented after each char written
    stz Z_no    ; starts at $0 ends at $7 (<$8)
    rts
;============================================
write_8charlinepageSPR:
    jsr set_source_ptr_special  ;
    jsr write_onesyscharSPR ; ' '
    jsr set_source_ptr_lower_case
    jsr write_onesyscharSPR ; p
    jsr write_onesyscharSPR ; a
    jsr write_onesyscharSPR ; g
    jsr write_onesyscharSPR ; e
    jsr set_source_ptr_special  ;
    jsr write_onesyscharSPR ; ' '
    jsr set_source_ptr_numbers  ;
    jsr write_onesyscharSPR ; 0
    jsr set_source_ptr_special  ;
    jsr write_onesyscharSPR ; ' '
    rts
;============================================
write_onesyscharSPR:
    lda ZP_PTR_6      ; #<VRAM_syschars ; Source
    sta ZP_PTR_4
    lda ZP_PTR_6+1    ; #>VRAM_syschars ; Source
    sta ZP_PTR_4+1
    ;
    lda Z_no
    tay
    ;
    clc
    lda ZP_PTR_4
    adc (ZP_PTR_1),y    ; adc $08= A, $10= B, $18= C; usw.
    sta ZP_PTR_4
    bcc :+
    inc ZP_PTR_4+1
    ;
   :lda #$01            ; Source
    sta VERA_ctrl       ; DATA1
    ;
    lda #$11            ; stride = 1
    sta VERA_addr_bank
    ;
    lda ZP_PTR_4+1
    sta VERA_addr_high
    lda ZP_PTR_4
    sta VERA_addr_low
    ;
    lda ZP_PTR_2    ; #<VRAM_charsprites  ; Target
    sta ZP_PTR_5
    lda ZP_PTR_2+1  ; #>VRAM_charsprites
    sta ZP_PTR_5+1
    ;
    clc
    lda ZP_PTR_5
    adc target_startoffset
    sta ZP_PTR_5
    bcc :+
    inc ZP_PTR_5+1
    ;                   ; Target
   :stz VERA_ctrl       ; DATA0
    lda #$11            ; stride = 1
    sta VERA_addr_bank
    lda ZP_PTR_5+1
    sta VERA_addr_high
    lda ZP_PTR_5
    sta VERA_addr_low
    ;
    write_rawsinglesyscharSPR
    ;
    inc Z_no
    ;
    clc
    lda target_startoffset
    adc #$08
    sta target_startoffset
    rts
    ;
;============================================
loop_w_one_syscharSPR:
    sta ZP_tmp_DATA4
   :jsr write_onesyscharSPR ; s
   ; write_onesyscharSPR ; s
    dec ZP_tmp_DATA4
    bne :-
    rts
;============================================
.macro create_infoSPR
    stz pageSPR_swiperuns
    stz pageSPR_preswipedelay
    stz pageSPR_swipedelay
    ;
    stz seqSPR_swiperuns
    stz seqSPR_preswipedelay
    stz seqSPR_swipedelay
    rmb0 seq_statusbits
    stz seq_show_pgnr
;
; Liest ein System Sprite (1-Bit) hinter VRAM Adresse $1F000 aus
; und generiert daraus ein neues Sprite bei $11000
; Achtung: es werden 2 Data Ports vom VERA verwendet.
;   ;
    ldx #<txtpage0SPR
    ldy #>txtpage0SPR
    jsr set_PTR_1
    ;
    set_target_pageSPR_data
    ;
    lda #$57        ; amber
    sta cheat_litecol
    ;
    stz target_startoffset  ; gets incremented after each char written
    stz Z_no    ; starts at $0 ends at $7 (<$8)
    jsr write_8charlinepageSPR
    ;
    lda #$07
    sta ZP_tmp_DATA3
    ;
@seven_loop:
    jsr page_prepare_nxtChar
    jsr write_8charlinepageSPR
    dec ZP_tmp_DATA3
    bne @seven_loop
    ;
    ldx #<txtseqinfoSPR
    ldy #>txtseqinfoSPR
    jsr set_PTR_1
    ;
    set_target_seqinfoSPR_data
    stz target_startoffset  ; gets incremented after each char written
    stz Z_no    ; starts at $0 ends at $7 (<$8)
    jsr set_source_ptr_lower_case
    lda #$08
    jsr loop_w_one_syscharSPR
;     sta ZP_tmp_DATA4
;    :jsr write_onesyscharSPR ; s
;     dec ZP_tmp_DATA4
;     bne :-
;     jsr write_onesyscharSPR ; e
;     jsr write_onesyscharSPR ; q
;     jsr write_onesyscharSPR ; u
;     jsr write_onesyscharSPR ; e
;     jsr write_onesyscharSPR ; n
;     jsr write_onesyscharSPR ; c
;     jsr write_onesyscharSPR ; e
    ;
    jsr page_prepare_nxtChar
    jsr set_source_ptr_special
    jsr write_onesyscharSPR ;
    jsr set_source_ptr_lower_case
    lda #$05
    jsr loop_w_one_syscharSPR
;     jsr write_onesyscharSPR ; s
;     jsr write_onesyscharSPR ; t
;     jsr write_onesyscharSPR ; a
;     jsr write_onesyscharSPR ; r
;     jsr write_onesyscharSPR ; t
    jsr set_source_ptr_special
    jsr write_onesyscharSPR ;
    jsr write_onesyscharSPR ; ' '
    ;
    jsr page_prepare_nxtChar
    jsr set_source_ptr_special
    jsr write_onesyscharSPR ;
    jsr set_source_ptr_lower_case
    lda #$05
    jsr loop_w_one_syscharSPR
;     jsr write_onesyscharSPR ; p
;     jsr write_onesyscharSPR ; a
;     jsr write_onesyscharSPR ; u
;     jsr write_onesyscharSPR ; s
;     jsr write_onesyscharSPR ; e
    jsr set_source_ptr_special
    jsr write_onesyscharSPR ;
    jsr write_onesyscharSPR ; ' '
    ;
    jsr page_prepare_nxtChar
    jsr set_source_ptr_special
    jsr write_onesyscharSPR ;
    jsr set_source_ptr_lower_case
    lda #$07
    jsr loop_w_one_syscharSPR
;     sta ZP_tmp_DATA4
;    :jsr write_onesyscharSPR ; s
;     dec ZP_tmp_DATA4
;     bne :-
;     jsr write_onesyscharSPR ; t
;     jsr write_onesyscharSPR ; o
;     jsr write_onesyscharSPR ; p
;     jsr write_onesyscharSPR ; p
;     jsr write_onesyscharSPR ; e
;     jsr write_onesyscharSPR ; d
    ;
    jsr page_prepare_nxtChar
    jsr set_source_ptr_special
    jsr write_onesyscharSPR ;
    jsr set_source_ptr_lower_case
    lda #$04
    jsr loop_w_one_syscharSPR
;     jsr write_onesyscharSPR ; c
;     jsr write_onesyscharSPR ; o
;     jsr write_onesyscharSPR ; n
;     jsr write_onesyscharSPR ; t
    jsr set_source_ptr_extra
    jsr write_onesyscharSPR ; .
    jsr set_source_ptr_special
    jsr write_onesyscharSPR ;
    jsr write_onesyscharSPR ;
;========================================================
; now that the sprite data are created,
; take care on the sprite attributes:
    stz VERA_ctrl   ; set sprite attr to cursor sprite #1
    lda #$11        ;stride = 1
    sta VERA_addr_bank
    lda #$FD
    sta VERA_addr_high
    lda #$10
    sta VERA_addr_low
    ;
    lda #<(VRAM_charsprites >> 5)
    sta VERA_data0
    lda #>(VRAM_charsprites >> 5)
    ora #$80        ; Bit 7 : 8bpp mode
    sta VERA_data0
; position  ; 120
    lda #$40
    sta VERA_data0  ; xpos lo
    lda #$02
    sta VERA_data0  ; xpos hi
    ;
    stz VERA_data0  ; ypos lo
    stz VERA_data0  ; ypos hi
    ;
    ;lda #$00        ; initially off ; lda #$0C      ; initially on
    stz VERA_data0  ; z-depth
    lda #$30        ; sprite64W,8H=$30, palette offset 0
    sta VERA_data0
    ;
    ; sequence
    stz VERA_ctrl   ; set sprite attr to cursor sprite #1
    lda #$11        ;stride = 1
    sta VERA_addr_bank
    lda #$FD
    sta VERA_addr_high
    lda #$60
    sta VERA_addr_low
    ;
    lda #<(VRAM_seqinfoSPR >> 5)
    sta VERA_data0
    lda #>(VRAM_seqinfoSPR >> 5)
    ora #$80        ; Bit 7 : 8bpp mode
    sta VERA_data0
; position
    stz VERA_data0  ; xpos lo
    lda #$01
    sta VERA_data0  ; xpos hi
    ;
    stz VERA_data0  ; ypos lo
    stz VERA_data0  ; ypos hi
    ;
    ;lda #$00        ; initially off ; lda #$0C      ; initially on
    stz VERA_data0  ; z-depth
    lda #$30        ; sprite64W,8H=$30, palette offset 0
    sta VERA_data0
    ;
    ; next sprite
    lda #$90        ; hardcoded lo adr
    sta VERA_data0
    lda #>(VRAM_seqinfoSPR >> 5)
    ora #$80        ; Bit 7 : 8bpp mode
    sta VERA_data0
; position
    lda #$40
    sta VERA_data0  ; xpos lo
    lda #$01
    sta VERA_data0  ; xpos hi
    ;
    stz VERA_data0  ; ypos lo
    stz VERA_data0  ; ypos hi
    ;
    ;lda #$00        ; initially off ; lda #$0C      ; initially on
    stz VERA_data0  ; z-depth
    lda #$30        ; sprite64W,8H=$30, palette offset 0
    sta VERA_data0

.endmacro
;===================================================================
.endif
