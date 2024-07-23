.ifndef CREATE_20X10MODE_TILESETS_INC
CREATE_20X10MODE_TILESETS_INC = 1
.include "dotcolors.inc"
.include "presets.inc"
;===========================================================================
; functions to create tiles for both layers in 'normal' mode 20x10
;===========================================================================
lite_col:               .byte $05   ; refers to a color value
litebg_col:             .byte $00
frame_col:              .byte $00
framebg_col:            .byte $51
corner_col:             .byte $51
;
cheat_litecol:          .byte $E0   ; refers to a tile data offset in VRAM
cheat_litebgcol:        .byte $10
cheat_colframe:         .byte $18
cheat_stylframe:        .byte $00
;====================================
sub_pattern1:
    lda framebg_col
    sta VERA_data0;
    sta VERA_data0
    ;
    lda corner_col
    sta VERA_data0
    lda frame_col
    sta VERA_data0
    sta VERA_data0
    lda corner_col
    sta VERA_data0
    rts
;
WRITE_pattern1_line:
    jsr sub_pattern1
    jsr sub_pattern1
    ;
    lda framebg_col
    sta VERA_data0
    sta VERA_data0
    ;
    lda corner_col
    sta VERA_data0
    lda frame_col
    sta VERA_data0
    rts
;==================
sub_pattern2:
    lda framebg_col
    sta VERA_data0
    sta VERA_data0
    ;
    lda frame_col
    sta VERA_data0
    stz VERA_data0  ; stz !
    stz VERA_data0  ; stz !
    sta VERA_data0
    rts
;
WRITE_pattern2_line:
    jsr sub_pattern2
    jsr sub_pattern2
    ;
    lda framebg_col
    sta VERA_data0
    sta VERA_data0
    ;
    lda frame_col
    sta VERA_data0
    stz VERA_data0
    rts
;====================================================== Create two types of 16x16 frame tiles
create_fg_tiles:
    lda framebg_col
    jsr WRITE_64_byte
    ;
    ldx #$04
   :jsr WRITE_pattern1_line
    jsr WRITE_pattern2_line
    jsr WRITE_pattern2_line
    jsr WRITE_pattern1_line
    ;
    lda framebg_col
    jsr WRITE_16_byte
    jsr WRITE_16_byte
    dex
    bne :-
    jsr WRITE_pattern1_line
    jsr WRITE_pattern2_line
    jsr WRITE_pattern2_line
    jmp WRITE_pattern1_line ; use rts of WRITE_pattern1_line
    ;
;=========================== create copys in multiple colors using corner_col
create_4frametypes:         ; resulting in different dot 'styles'
    sta framebg_col     ; standard "round"
    sta corner_col
    stz frame_col
    jsr create_fg_tiles
    ;
    stz corner_col      ; quadradtic
    jsr create_fg_tiles
    ;
    lda framebg_col     ; small
    sta corner_col
    sta frame_col
    jsr create_fg_tiles
    ;
    stz corner_col      ; "star"
    jmp create_fg_tiles
    ;
;====================================================== Create eight types of 8x8 lite tiles
create_bg_tiles:
    sta litebg_col
;========================================                      tileno.
;
;======================================== blank tile            #00
    lda litebg_col
    jsr WRITE_64_byte
;========================================= full_tile            #01
    lda lite_col
    jsr WRITE_64_byte
;================================ left col side_tile            #02
    ldx #$08
   :lda lite_col
    jsr WRITE_4_byte
    lda litebg_col
    jsr WRITE_4_byte
    dex
    bne :-
;====================================== top row_tile            #03
    lda lite_col
    jsr WRITE_16_byte
    jsr WRITE_16_byte
    lda litebg_col
    jsr WRITE_16_byte
    jsr WRITE_16_byte
;============================== top left single tile            #04
    ldx #$04
   :lda lite_col
    jsr WRITE_4_byte
    lda litebg_col
    jsr WRITE_4_byte
    dex
    bne :-
    lda litebg_col
    jsr WRITE_16_byte
    jsr WRITE_16_byte
;================== top left to bottom right diagonal tile     #05
    ldx #$04
   :lda lite_col
    jsr WRITE_4_byte
    lda litebg_col
    jsr WRITE_4_byte
    dex
    bne :-
    ldx #$04
   :lda litebg_col
    jsr WRITE_4_byte
    lda lite_col
    jsr WRITE_4_byte
    dex
    bne :-
;=========== top left to bottom left to bottom right L- tile   #06
    lda lite_col
    ldx #$04
   :lda lite_col
    jsr WRITE_4_byte
    lda litebg_col
    jsr WRITE_4_byte
    dex
    bne :-
    lda lite_col
    jsr WRITE_16_byte
    jsr WRITE_16_byte
;=============== checkered tile - sofar this tile is NOT USED  #07
; but is does well as padding to 8 tiles for keeping code easy aligned to 1 byte
; when or-ing in 'cheat_values'
    ldx #$20
   :lda lite_col
    sta VERA_data0
    lda litebg_col
    sta VERA_data0
    dex
    bne :-
    rts
;===============
create_4bgmatch:    ; series of 4 predefined bg color
    sta lite_col    ; used to toggle bg color
    lda #$00    ;bg
    jsr create_bg_tiles ; 0-7
    lda #$01    ; white bg
    jsr create_bg_tiles ; 0-7
    lda #$82    ; dark green bg
    jsr create_bg_tiles ; 0-7
    lda #$36    ; dark red bg
    jmp create_bg_tiles ; 0-7
    ;
;============================================ create all tiles
.macro create_tiles
; fg tiles
    stz VERA_ctrl
    lda #$10   ; stride = 1 ; $06000
    sta VERA_addr_bank
    lda #$80 ;(>VRAM_frametiles)    ; $08000
    sta VERA_addr_high
    stz VERA_addr_low
    ;
    lda #$51                ; dark brown
    jsr create_4frametypes
    lda #$10                ; "almost black"
    jsr create_4frametypes
    lda #$01                ; white
    jsr create_4frametypes
    lda #$06                ; blue
    jsr create_4frametypes
    lda #$36                ; dark red
    jsr create_4frametypes
    lda #$82                ; dark green
    jsr create_4frametypes
    lda #$D8                ; purple
    jsr create_4frametypes
    lda #$6C                ; "LCD green"
    jsr create_4frametypes
    ;
; bg tiles
    stz VERA_ctrl
    lda #$10   ; stride = 1 ;$0C000
    sta VERA_addr_bank
    lda #$C0    ;$0C000
    ;
    sta VERA_addr_high
    stz VERA_addr_low
    ;
    lda #$10        ; almost black
    jsr create_4bgmatch ; 0-3
    lda #$05        ; green
    jsr create_4bgmatch ; 4-7
    lda #$3B        ; red
    jsr create_4bgmatch ; 8-11
    lda #$57        ; amber
    jsr create_4bgmatch ; 12-15
    lda #$01        ; white
    jsr create_4bgmatch ; 16-19
    lda #$A4        ; bright blue
    jsr create_4bgmatch ; 20-23
    ;
    ; 24-27 "manual"==================
    lda #$FF        ; magenta
    sta lite_col
    lda #$00        ; bg
    jsr create_bg_tiles
    lda #$01        ; white bg
    jsr create_bg_tiles
    lda #$82        ; dark green bg
    jsr create_bg_tiles
    lda #$BC        ; dark blue bg
    jsr create_bg_tiles
    ;
    ; 28-32 "manual" ====================
    lda #$06        ; blue
    sta lite_col
    lda #$00        ; bg
    jsr create_bg_tiles
    lda #$01        ; white bg
    jsr create_bg_tiles
    lda #$01        ; white
    sta lite_col
    lda #$06        ; blue bg
    jsr create_bg_tiles
    lda #$75
    sta lite_col    ; LCD on gray-green (dark)
    lda #$6B        ; bg: LCD off green
    jsr create_bg_tiles
    ;
.endmacro
;==============================================
.endif
