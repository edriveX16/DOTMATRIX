.ifndef POSTINSERT_INC
POSTINSERT_INC =1
;================================================
;     jsr copy_onofftxt2systxt          lda display_onoff   ->post_ins bit7
;     jsr ins_pagenr2seqmenu            lda pgnr_onoff      ->post_ins bit6
;     jsr ins_secnd2systxt              lda secs_onoff      ->post_ins bit5
;     jsr insert_insert                 lda ii_on           ->post_ins bit4
;================================================
chrset_inc:             .byte $02   ; normal mode, half mode= $08
single_line_indent:     .byte $00
secs_line_indent:       .byte $00
onoff_indent_lo:        .byte $25
onoff_indent_hi:        .byte $25
;
pagenr_chrs:            .byte $30
;
lubA: .byte $80,$A0,$C8         ;  d128,d160,d200
lubB: .byte $00
lubC: .byte $00
;==================================
pagetxt0:       ; "page"
.byte $70,$61,$67,$65,$20,$00
.byte $04   ; EOF
;==================================
sectxt0:
.byte $00,$00,$00,$00,$04
;
ontxt0:  ; "on "
.byte $20,$6F,$6E,$20
offtxt0:  ; "off"
.byte $20,$6F,$66,$66
autotxt0:  ; "auto"
.byte $61,$75,$74,$6F
insert0:
.byte $6F,$76,$65,$72,$77,$72,$69,$74,$65
insert1:
.byte $69,$6E,$73, $65,$72,$74,$20, $20,$20
;time0:
;.byte $32,$34,$3A, $30,$30,$3A,$34, $32,$20
;========================
ins_onofftxt2systxt:
    bbs7 post_ins, :+
    rts
   ;
   :lda select_onoff
    beq @off
    cmp #$02
    beq @auto
;@on:
    ldx #<ontxt0
    ldy #>ontxt0
    bra @cont
@auto:
    ldx #<autotxt0
    ldy #>autotxt0
    bra @cont
    ;
@off:
    ldx #<offtxt0
    ldy #>offtxt0
    ;
@cont:
    jsr set_PTR_1
    lda onoff_indent_lo ; no of characters into the text to be written to
    sta ZP_PTR_4        ; target pointer
    ;
    clc
    lda #>systxt0       ; we don't check for bcc after additon
    adc onoff_indent_hi
    sta ZP_PTR_4+1      ; due to we know that systxt start adr is $X000 and added value <$100
    ldx #$04            ; copy 4 byte
    jmp syscpy_loop     ; uses rts of syscpy_loop
;===========================================
.macro act_pg2chrs
    clc
    lda current_usrtxthi
    lsr
    lsr
    eor #$04
    and #$07
    adc #$30
    sta pagenr_chrs
.endmacro
;==================
ins_pagenr2systxt:
    bbs6 post_ins, :+
    rts
    ;
   :jsr set_indent_PTR_4
    ldx #<pagetxt0  ; source
    ldy #>pagetxt0
    jsr set_PTR_1
    ;
    ldx #$05
    jsr syscpy_loop
    ;
    act_pg2chrs

    ldy #$05
    lda pagenr_chrs
    sta (ZP_PTR_4),y
    rts
;===========================================
.macro OUTDEC8
    ldx #$01
    stx lubC
    inx
    ldy #$40
@dec_loop:
    sty lubB
    lsr
@lbl2:
    rol
    bcs @lbl3
    cmp lubA,x
    bcc @lbl4
@lbl3:
    sbc lubA,x
    sec
@lbl4:
    rol lubB
    bcc @lbl2
    tay
    cpx lubC
    lda lubB
    bcc @lbl5
    beq @lbl6
    stx lubC
@lbl5:
    eor #$30            ; ascii '0'
    sta (ZP_PTR_1)      ; save each digit character
    jsr inc_PTR_1
@lbl6:
    tya
    ldy #$10
    dex
    bpl @dec_loop
.endmacro
;================
ins_secs2systxt:
    bbs5 post_ins, :+
    rts
    ;
   :ldx #<sectxt0
    ldy #>sectxt0
    jsr set_PTR_1
    ;
    ldy #$03    ; clear up buffer after last run
    lda #$20    ; spc
   :sta (ZP_PTR_1),y
    dey
    bne :-
    ;
    lda seq_steptime
    inc
    OUTDEC8
    ;
    ldx #$02
@loop:
    lda sectxt0+2   ; trd digit
    cmp #$20
    beq @shift2
    bra @write
@shift2:
    lda sectxt0+1
    sta sectxt0+2
    lda sectxt0
    sta sectxt0+1
    lda #$20
    sta sectxt0
    dex
    bne @loop
    ;
@write:
    lda secs_line_indent
    sta single_line_indent  ; set_indent_PTR_4 uses single_line_indent only
    jsr set_indent_PTR_4
    ;
    ldx #<sectxt0  ; source
    ldy #>sectxt0
    jsr set_PTR_1
    ldx #$03
    ;
    jmp syscpy_loop ; uses rts of syscpy_loop
;===========================================
ins_insert2systxt:
    bbs4 post_ins, :+
    rts
    ;
   :jsr set_indent_PTR_4
    ;
    lda ins_mode
    bne :+
    ;
    ldx #<insert0  ; source ; "overwrite"
    ldy #>insert0
    bra :++

   :ldx #<insert1  ; source ; "insert   "
    ldy #>insert1
   :jsr set_PTR_1
    ;
    ldx #$09            ; copy A byte
syscpy_loop:
    ldy #$00
   :lda (ZP_PTR_1),y    ; source
    sta (ZP_PTR_4),y    ; target
    iny
    dex
    bne :-
    rts
;==================================================================
.macro write_charset2systxt
    lda chrset_inc  ; some line down depending on mode
    sta single_line_indent  ; set_indent_PTR_4 uses single_line_indent only
    jsr set_indent_PTR_4
    ;
    lda #$20 ;'spc'     ; start value
    sta char_value
    lda #$06
    sta ZP_tmp_DATA2    ; outer loop line counter
@ascii_cpy_loop:
    jsr add_line_2_ZP4
    ldy #$00
   ;
    ldx #$10
   :lda char_value
    sta (ZP_PTR_4),y
    iny
    inc char_value
    dex
    bne :-
    ;
    dec ZP_tmp_DATA2
    bne @ascii_cpy_loop
    ;
.endmacro
;===========================================
.endif
