.ifndef DECODER_INC
DECODER_INC  = 1
;
char_pattern_set:       .byte $00,$00,$00,$00,$00,$00
;MStR_tile_offset    =    $1A -> moved to "pattern.inc"
show_whitespc:          .byte $00
;
;==========================
set_VERA_addr_2_char:
    stz VERA_ctrl
    lda #$10 ; stride = 1
    sta VERA_addr_bank
    lda char_VRAM_adr_hi
    sta VERA_addr_high
    lda char_VRAM_adr_lo
    sta VERA_addr_low
    rts
    ;
;=========================================================================================
make_upperlinesubset:    ; preset x before entering: $00 for left half, $02 for right half
    ldy #$00
    bbr6 zp_flips, @tv_flip ; test vertical flip
    bbr7 zp_flips, @h_flip  ; flip horizontal
    ;
;@vh_flip:  ; flip vertical & horizontal
    ldy #$03
    lda (ZP_PTR_8),y        ;+3 ZP_PTR_8 =  pattern pointer within array
    eor #$C0
    sta char_pat_linesubset,x
    dey
    inx
    lda (ZP_PTR_8),y        ;+2
    eor #$C0
    sta char_pat_linesubset,x
    rts
    ;
@tv_flip:
    bbr7 zp_flips, @nix_flip
;@v_flip:   ; flip vertical
    ldy #$02
    lda (ZP_PTR_8),y        ;+2
    eor #$80
    sta char_pat_linesubset,x
    iny
    inx
    lda (ZP_PTR_8),y        ;+3
    eor #$80
    sta char_pat_linesubset,x
    rts
    ;
@h_flip:    ; flip horizontal
    iny
    lda (ZP_PTR_8),y        ;+1
    eor #$40
    sta char_pat_linesubset,x
    dey
    inx
    lda (ZP_PTR_8),y        ; +-0
    eor #$40
    sta char_pat_linesubset,x
    rts
@nix_flip:  ; don't flip
    lda (ZP_PTR_8),y        ;+-0
    sta char_pat_linesubset,x
    iny
    inx
    lda (ZP_PTR_8),y        ;+1
    sta char_pat_linesubset,x
    rts
    ;
;=========================================================================================
make_lowerlinesubset:    ; preset x before entering: $00 for left half, $02 for right half
    ldy #$02
    bbr6 zp_flips, @tv_flip; bit #$40   ; test vertical flip
    bbr7 zp_flips, @h_flip ; bit #$80   ; flip horizontal
;@vh_flip:
    ldy #$01
    lda (ZP_PTR_8),y        ;1
    eor #$C0
    sta char_pat_linesubset,x
    dey
    inx
    lda (ZP_PTR_8),y        ;0
    eor #$C0
    sta char_pat_linesubset,x
    rts
    ;
@tv_flip:
    bbr7 zp_flips, @nix_flip ; bit #$80
;@v_flip:   ; flip vertical
    ldy #$00
    lda (ZP_PTR_8),y        ;0
    eor #$80
    sta char_pat_linesubset,x
    iny
    inx
    lda (ZP_PTR_8),y        ;1
    eor #$80
    sta char_pat_linesubset,x
    rts
@h_flip:
    iny
    lda (ZP_PTR_8),y        ;3
    eor #$40
    sta char_pat_linesubset,x
    dey
    inx
    lda (ZP_PTR_8),y        ;2
    eor #$40
    sta char_pat_linesubset,x
    rts
@nix_flip:
    lda (ZP_PTR_8),y        ;2
    sta char_pat_linesubset,x
    iny
    inx
    lda (ZP_PTR_8),y        ;3
    sta char_pat_linesubset,x
    rts
    ;
;=========================================================================================
create_temp_patternpointer:
; calculates offset within pattern table "Muster" or "MStR"
; saves also flip bits and offset-pointer
    lda char_pattern_set,y
    tax
    and #$C0
    sta zp_flips ; save flips
    txa
    and #$3F    ; mask out flip part and take 4 times, since every pattern is 4 byte
    asl
    asl
    sta pattern_offset_ptr
    ;
    lda ZP_PTR_9+1
    sta ZP_PTR_8+1
    ;
    lda ZP_PTR_9
    ;           ; add offset-pointer to start-pointer
    clc         ; 8 to 16 Bit-Addition
    adc pattern_offset_ptr
    sta ZP_PTR_8
    bcc :+
    inc ZP_PTR_8+1  ; ZP_PTR_8 now pointer to pattern incl. offset
   :rts
    ;
.macro make_upperlinesub
    jsr create_temp_patternpointer
    ldx #$00
    jsr make_upperlinesubset
; right half
    ldy zp_index
    iny
    jsr create_temp_patternpointer
    ldx #$02
    jsr make_upperlinesubset
.endmacro
;
.macro make_lowerlinesub
; left half
    ldy zp_index
    jsr create_temp_patternpointer
    ldx #$00
    jsr make_lowerlinesubset
; right half
    ldy zp_index
    iny
    jsr create_temp_patternpointer
    ldx #$02
    jsr make_lowerlinesubset
.endmacro
;=====================================
make_bothlinesubs:
    sty zp_index
    ;
    make_upperlinesub
    jsr set_VERA_addr_2_char
    ;
    ldx #$00
   :lda char_pat_linesubset,x
    and #$3F
    ora cheat_col
    sta VERA_data0 ; Tile index
    lda char_pat_linesubset,x
    and #$C0
    lsr
    lsr
    lsr
    lsr
    sta VERA_data0 ; color + flip modes
    inx
    cpx #$04
    bne :-
    inc char_VRAM_adr_hi
    make_lowerlinesub
    jsr set_VERA_addr_2_char
    ;
    ldx #$00
   :lda char_pat_linesubset,x
    and #$3F
    ora cheat_col
    sta VERA_data0 ; Tile index
    lda char_pat_linesubset,x
    and #$C0
    lsr
    lsr
    lsr
    lsr
    sta VERA_data0 ; color + flip modes
    inx
    cpx #$04
    bne :-
    rts
;=====================================
make_halflinesub:
    jsr set_VERA_addr_2_char
    ;
    lda char_pattern_set,y
    tax
    and #$3F ; mask out flip part
    clc
    adc ZP_tmp_DATA
    sta VERA_data0 ; Tile index
    ;
    txa
    and #$C0
    lsr
    lsr
    lsr
    lsr
    sta VERA_data0 ; color + flip modes
    ;
    iny
    ;
    lda char_pattern_set,y
    tax
    and #$3F ; mask out flip part
    clc
    adc ZP_tmp_DATA
    sta VERA_data0 ; Tile index
    ;
    txa
    and #$C0
    lsr
    lsr
    lsr
    lsr
    sta VERA_data0 ; color + flip modes
    ;
    inc char_VRAM_adr_hi
    rts
;
.macro read_char_pat_set
    clc
    lda pattern_offset_ptr
    adc pattern_offset_ptr
    adc pattern_offset_ptr
    asl             ; multiply by 6
    sta pattern_offset_ptr
    clc             ; 8 to 16 Bit-Addition
    lda ZP_PTR_8
    adc pattern_offset_ptr
    sta ZP_PTR_8
    bcc :+
    inc ZP_PTR_8+1
   :ldy #$00
   :lda (ZP_PTR_8),y
    sta char_pattern_set,y
    iny
    cpy #$06
    bne :-
.endmacro
;============================
Vwrite_blank:   ; blanks only @ character position
    bbs0 half_mode, @hm_blank
    ;
    jsr set_VERA_addr_2_char
    lda cheat_col
    jsr WRITE_8alternate_byte
    ;
    ldx #05
   :inc char_VRAM_adr_hi
    jsr set_VERA_addr_2_char
    lda cheat_col
    jsr WRITE_8alternate_byte
    dex
    bne :-
    ;
    rts
;=================
; half_mode blank:
@hm_blank:
    jsr set_VERA_addr_2_char
    jsr WRITE_4z_byte
    ;
    inc char_VRAM_adr_hi
    jsr set_VERA_addr_2_char
    jsr WRITE_4z_byte
    ;
    inc char_VRAM_adr_hi
    jsr set_VERA_addr_2_char
    jsr WRITE_4z_byte
    rts
    ;
;================================================================
write_singlechar:
; char_VRAM_adr_hi & char_VRAM_adr_lo must be set before entering
; 1st part: check if char value is valid
    lda char_value
    bne @chkspc
    lda show_whitespc
    bne @chklowspecial
@wb:
    jmp Vwrite_blank    ; use rts of Vwrite_blank
    ;
@chkspc:
    cmp #$20 ; 'spc'
    bne @chkrange
    jmp Vwrite_blank    ; take a shortcut for space: instead decoding draw directly empty tiles
    ;
@chkrange:
    rol
    bcs @chkspecial     ; highest hi bit set ->  >=d128 , >=$7F
    ;
    lda char_value
    and #$7F
    sta char_value
    cmp #$20            ; lower than  <d32 , <$20
    bmi @chklowspecial
    ;
    sec
    sbc #$60
    bpl @range_60
    beq @range_60
;@chkrange_40
    lda char_value
    sec
    sbc #$40
    bpl @range_40
    beq @range_40
;@chkrange_20
    lda char_value
    sec
    sbc #$20
@chkrange_20:
    sta pattern_offset_ptr  ; char_value - $20
    lda #<char_pattern_code
    sta ZP_PTR_8
    lda #>char_pattern_code
    sta ZP_PTR_8+1
    bra @read_chars
@range_40:
    sta pattern_offset_ptr  ; char_value - $40
    lda #<characters
    sta ZP_PTR_8
    lda #>characters
    sta ZP_PTR_8+1
    bra @read_chars
@range_60:
    sta pattern_offset_ptr ; char_value - $60
    lda #<lower_case
    sta ZP_PTR_8
    lda #>lower_case
    sta ZP_PTR_8+1
    bra @read_chars
    ;
@chklowspecial:
    lda show_whitespc
    beq @wb
    lda char_value
    beq :+
    lda #$05   ; big dot
    bra @outsp
   :lda #$04   ; small dot
    bra @outsp
@chkspecial:            ; >=128
    lda menu_mode       ; extra for menu bar chars
    bne @sp
    lda char_value
    and #$7F
    cmp #$08
    bmi @sp
    lda show_whitespc
    beq @wb
    lda #$05   ; big dot
    bra @outsp
    ;
@sp:lda char_value
    and #$07
@outsp:
    sta pattern_offset_ptr
    lda #<special_chars
    sta ZP_PTR_8
    lda #>special_chars
    sta ZP_PTR_8+1
@read_chars:
    read_char_pat_set
    ;
    bbs0 half_mode, halfsubs
;======================================
normsubs:   ; when in 20x10 mode
; 2nd part: decode & draw the character
    lda #<Muster
    sta ZP_PTR_9
    lda #>Muster
    sta ZP_PTR_9+1
    ldy #$00
    jsr make_bothlinesubs
; upper third done
    inc char_VRAM_adr_hi
    lda #<MStR
    sta ZP_PTR_9
    lda #>MStR
    sta ZP_PTR_9+1
    ldy #$02
    jsr make_bothlinesubs
; mid done
    inc char_VRAM_adr_hi
    lda #<Muster
    sta ZP_PTR_9
    lda #>Muster
    sta ZP_PTR_9+1
    ldy #$04
    jsr make_bothlinesubs
; lower third done
    rts
    ;
;@halfsubs:  ; when in half mode 40x20
halfsubs:
    stz ZP_tmp_DATA
    ldy #$00
    jsr make_halflinesub
    ;
    lda #MStR_tile_offset
    sta ZP_tmp_DATA
    ldy #$02
    jsr make_halflinesub
    ;
    stz ZP_tmp_DATA
    ldy #$04
    jsr make_halflinesub
    rts
.endif
