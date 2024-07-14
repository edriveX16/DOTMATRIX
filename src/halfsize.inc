.ifndef HALFSIZE_INC
HALFSIZE_INC  = 1
;=============================================================================
; no need for (faster) zp-usage here since the tile creation is only run once at start.
pb_ul:      .byte $00   ; pattern def byte upper left
pb_ur:      .byte $00   ; pattern def byte upper right
pb_ll:      .byte $00   ; pattern def byte lower left
pb_lr:      .byte $00   ; pattern def byte lower right
;
;=================================================
; sets all pages to half mode
set_allpages2half_mode:
    lda #$07
    sta ZP_tmp_DATA
    ;
    lda actual_usrtxtlo;
    sta ZP_PTR_5
    lda actual_usrtxthi
    sta ZP_PTR_5+1
    lda #$01
    tax
    jsr set_pagesettings_ptr
    ldy #$01            ; byte0=seq_steptime, byte1= mode
    ;
    txa
    sta (ZP_PTR_5),y    ; mode
    ;
@stah_loop:
    clc
    lda ZP_PTR_5+1
    adc #$04
    sta ZP_PTR_5+1
    txa
    sta (ZP_PTR_5),y    ; mode
    ;
    dec ZP_tmp_DATA
    bne @stah_loop
   ;
    rts
;==========================================
set_PTR_2:
    stx ZP_PTR_2
    sty ZP_PTR_2+1
    rts
;==========================================
WRITE_0011:
    lda litebg_col
    sta VERA_data0
    sta VERA_data0
    lda lite_col
    sta VERA_data0
    sta VERA_data0
    rts
    ;
WRITE_1100:
    lda lite_col
    sta VERA_data0
    sta VERA_data0
    lda litebg_col
    sta VERA_data0
    sta VERA_data0
    rts
    ;
;========================
setup_testbytes:
    sta ZP_tmp_DATA2
    and #$C0
    sta zp_flips
    lda ZP_tmp_DATA2
    and #$07
    rts
;========================
upper4:
    jsr setup_testbytes
    bne @tileup1
;@tileup0:
    lda litebg_col
    jmp WRITE_4_byte
    ;
@tileup1:
    cmp #$01
    bne @tileup2
    lda lite_col
    jmp WRITE_4_byte
    ;
@tileup2:
    cmp #$02
    bne @tileup3
    ;
    bbs6 zp_flips, :+
    jmp WRITE_1100
   :jmp WRITE_0011
    ;
@tileup3:   ; tile 3 top row tile
    cmp #$03
    bne @tileup4
    ;
    bbs7 zp_flips, :+
    lda lite_col
    jmp WRITE_4_byte
   :lda litebg_col
    jmp WRITE_4_byte
    ;
@tileup4:           ;top left single tile            #04
    cmp #$04
    bne @tileup5
    bbs7 zp_flips, :++  ; bit7 = vertical flip
    bbs6 zp_flips, :+   ; bit6 = horizontal flip
    ;
    jmp WRITE_1100
   :jmp WRITE_0011
    ;
   :lda litebg_col
    jmp WRITE_4_byte
    ;
@tileup5:       ;top left to bottom right diagonal tile     #05
    cmp #$05
    bne @tileup6
    ;
    bbs7 zp_flips, :++
    bbs6 zp_flips, :+
    ;
    jmp WRITE_1100
   :jmp WRITE_0011
    ;
   :bbs6 zp_flips, :+

    jmp WRITE_0011
   :jmp WRITE_1100
    ;
@tileup6:       ; L-type
    cmp #$06
    bne alternate
    ;
    bbs7 zp_flips, :++
    bbs6 zp_flips, :+
    jmp WRITE_1100
   :jmp WRITE_0011
    ;
   :lda lite_col
    jmp WRITE_4_byte
    ;
;==========
alternate: ; no tile nr. match found, do empty tile as excuse
    lda litebg_col
    jmp WRITE_4_byte
    ;
;========================================================================
;nur untere hälfte vom 4er tile, also für 2 weitere pix nach unten,
lower4:
    jsr setup_testbytes
    bne @tiledwn1
    ;
;@tiledwn0:
    lda litebg_col
    jmp WRITE_4_byte   ; emtpy tile allways background col
    ;
@tiledwn1:
    cmp #$01
    bne @tiledwn2
    lda lite_col
    jmp WRITE_4_byte
    ;
@tiledwn2:
    cmp #$02
    bne @tiledwn3
    ;
    bbs6 zp_flips, :+
    jmp WRITE_1100
   :jmp WRITE_0011
    ;
@tiledwn3:
    cmp #$03
    bne @tiledwn4
    ;
    bbs7 zp_flips, :+
    lda litebg_col
    jmp WRITE_4_byte
    ;
   :lda lite_col
    jmp WRITE_4_byte
    ;
@tiledwn4:  ; tile 4: top left single tile (allways bg when no v-flip)
    cmp #$04
    bne @tiledwn5
    ;
    bbs7 zp_flips, :+
    lda litebg_col
    jmp WRITE_4_byte
    ;
   :bbs6 zp_flips, :+
    jmp WRITE_1100
   :jmp WRITE_0011
    ;
@tiledwn5:  ; tile 5 top left to bottom right diagonal tile
    cmp #$05
    bne @tiledwn6
    ;
    bbs7 zp_flips, :++
    bbs6 zp_flips, :+
    jmp WRITE_0011
   :jmp WRITE_1100
    ;
   :bbs6 zp_flips, :+
    jmp WRITE_1100
   :jmp WRITE_0011
    ;
@tiledwn6: ; tile 6 L-shape-type tile (allways colored when no v-flip)
    cmp #$06
    bne alternate
    ;
    bbs7 zp_flips, :+
    lda lite_col
    jmp WRITE_4_byte
    ;
   :bbs6 zp_flips, :+
    jmp WRITE_1100
   :jmp WRITE_0011
    ;
;=====================================================================
.macro create_pattern_tile
    lda pb_ul
    jsr upper4
    lda pb_ur
    jsr upper4
    lda pb_ul
    jsr upper4
    lda pb_ur
    jsr upper4
    ;
    lda pb_ul
    jsr lower4
    lda pb_ur
    jsr lower4
    lda pb_ul
    jsr lower4
    lda pb_ur
    jsr lower4
; worte 4 pix lines
    lda pb_ll
    jsr upper4
    lda pb_lr
    jsr upper4
    lda pb_ll
    jsr upper4
    lda pb_lr
    jsr upper4
    ;
    lda pb_ll
    jsr lower4
    lda pb_lr
    jsr lower4
    lda pb_ll
    jsr lower4
    lda pb_lr
    jsr lower4
; all 8 pix lines written
.endmacro
;=================================================================
get_patternbytes:
    lda (ZP_PTR_2),y    ; read from pattern look up table
    sta pb_ul           ; pattern def byte upper left
    iny
    lda (ZP_PTR_2),y
    sta pb_ur
    iny
    lda (ZP_PTR_2),y
    sta pb_ll
    iny
    lda (ZP_PTR_2),y
    sta pb_lr           ; pattern def byte lower right
    rts
    ;
;=================================================================
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
;================================= Create two types of 8x8 frame tiles
create_hfg_tiles:
    jsr WRITE_16_byte   ; first 2 empty 8 byte lines for 1st tile
    ;
    ldx #$04
   :jsr WRITE_hframe_tile_line ; 2 times 8 byte pattern lines + 8 byte : 2 loops for each tile
    jsr WRITE_hframe_tile_line
    jsr WRITE_8_byte
    dex
    bne :-
    ;
    jsr WRITE_hframe_tile_line ; last 2 8 byte pattern lines for 2nd tile
    jsr WRITE_hframe_tile_line
    rts
    ;
;=================================================================
create_all_patterns_loop:
; x must be preloaded with max patternno. of pattern table
; A must be preloaded with color value
; ZP_PTR_2 must be preloaded with pointer to pattern inside pat. table
   :ldy #$00
    jsr get_patternbytes
    create_pattern_tile
    clc
    lda ZP_PTR_2
    adc #$04
    sta ZP_PTR_2
    bcc :+
    inc ZP_PTR_2+1
   :dex
    bne :--
    rts
    ;
;============
.macro create_halftiles
;fg 8er tiles:
    stz VERA_ctrl
    lda #$11   ; stride = 1 ; $16000
    sta VERA_addr_bank
    lda #$60 ;(>VRAM_halfframes)
    sta VERA_addr_high
    stz VERA_addr_low
    ;
    lda #$51                ; dark brown
    jsr create_hfg_tiles
    lda #$10                ; "almost black"
    jsr create_hfg_tiles
    lda #$01                ; white
    jsr create_hfg_tiles
    ;lda #$06                ; blue
    lda #$C2                ; blue
    jsr create_hfg_tiles
    lda #$1F                ; light grey
    jsr create_hfg_tiles
    lda #$1E                ; light grey
    jsr create_hfg_tiles
    lda #$07                ; purple
    jsr create_hfg_tiles
    lda #$6C                ; "LCD green"
    jsr create_hfg_tiles
    ;
;===========================================
;bg die eigentlichen pattern tiles:
    stz VERA_ctrl
    lda #$11   ; stride = 1 ; $17000
    sta VERA_addr_bank
    lda #$70 ;(>VRAM_halftiles)
    sta VERA_addr_high
    stz VERA_addr_low
    ;
;===================  tileset color 0
    lda #$3B    ; rot
    sta lite_col
    stz litebg_col
    ;
; temppointer auf Muster
    ldx #<Muster
    ldy #>Muster
    jsr set_PTR_2
    ;
    ldx #$1A    ; 26 no of Muster patters
    jsr create_all_patterns_loop
    ;
; temppointer auf MStr
    ldx #<MStR
    ldy #>MStR
    jsr set_PTR_2
    ;
    ldx #$35    ; no of MStR patterns
    jsr create_all_patterns_loop
    ;
;===================  tileset color 1
    stz VERA_ctrl
    lda #$11   ; stride = 1 ; $19000
    sta VERA_addr_bank
    lda #$90 ;(>VRAM_halftiles)
    sta VERA_addr_high
    stz VERA_addr_low
    ;
    lda #$05    ; green
    sta lite_col
    ;
; temppointer auf Muster
    ldx #<Muster
    ldy #>Muster
    jsr set_PTR_2
    ;
    ldx #$1A    ; 26
    jsr create_all_patterns_loop
    ;
; temppointer auf MStR
    ldx #<MStR
    ldy #>MStR
    jsr set_PTR_2
    ;
    ldx #$35
    jsr create_all_patterns_loop
    ;
;===================  tileset color 2
    stz VERA_ctrl
    lda #$11   ; stride = 1 ; $1B000
    sta VERA_addr_bank
    lda #$B0 ;(>VRAM_halftiles)
    sta VERA_addr_high
    stz VERA_addr_low
    ;
    lda #$01    ; wht
    sta lite_col
    ;
; temppointer to Muster
    ldx #<Muster
    ldy #>Muster
    jsr set_PTR_2
    ;
    ldx #$1A    ; 26
    jsr create_all_patterns_loop
    ;
; temppointer to MStR
    ldx #<MStR
    ldy #>MStR
    jsr set_PTR_2
    ;
    ldx #$35
    jsr create_all_patterns_loop
    ;
;===================  tileset color 3
    stz VERA_ctrl
    lda #$11   ; stride = 1 ; $1D000
    sta VERA_addr_bank
    lda #$D0 ;(>VRAM_halftiles)
    sta VERA_addr_high
    stz VERA_addr_low
    ;
    lda #$10    ; almost black
    sta lite_col
    lda #$1E    ; light grey
    sta litebg_col
    ;
; temppointer to Muster
    ldx #<Muster
    ldy #>Muster
    jsr set_PTR_2
    ;
    ldx #$1A    ; 26
    jsr create_all_patterns_loop
    ;
; temppointer to MStR
    ldx #<MStR
    ldy #>MStR
    jsr set_PTR_2
    ;
    ldx #$35
    jsr create_all_patterns_loop
.endmacro
;=======================================================
.endif
