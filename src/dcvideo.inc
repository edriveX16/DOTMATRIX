.ifndef DCVIDEO_INC
DCVIDEO_INC  = 1
;======================================================
; functions to switch between both modes 20x10 <-> 40x20
; In case of a mode change both layers have to be
; redrawn. Some mandatory values have to be adpoted.
; During mode change (layer repaint) we disable video output.
; Otherwise flickering may occur.
;======================================================
;row_fill_cnt:           .byte $20
;==========================================
; "normal mode" refers to 20*10 character mode
; "half mode"   refers to 40*20 character mode
;==========================================
; set mandatory mode depeinding values:
.macro set_normalmode_values        ; values for 20*10 char mode
    lda #$08
    sta colframe_inc
    lda #$40
    sta maxcolframe_cnt
    lda #$0A
    sta max_line_no
    dec
    sta max_line_no_mone
    ;
    lda #$14
    sta max_char_p_line
    dec
    sta max_char_p_line_mone
    ;
    lda #$08
    sta Z_pointer_inc_lo
    lda #$06
    sta Z_pointer_inc_hi
    lda #$20
    sta row_fill_cnt
    lda #$07
    sta crsr_wait_time
.endmacro
;==========================================
.macro set_halfmode_values          ; values for 40*20 char mode
    lda #$02
    sta colframe_inc
    lda #$11
    sta maxcolframe_cnt
    lda #$14
    sta max_line_no
    dec
    sta max_line_no_mone
    ;
    lda #$28
    sta max_char_p_line
    dec
    sta max_char_p_line_mone
    ;
    lda #$04
    sta Z_pointer_inc_lo
    lda #$03
    sta Z_pointer_inc_hi
    lda #$40
    sta row_fill_cnt
    lda #$04
    sta crsr_wait_time
    ;
    stz X16logo_on      ; no logo availiable in 40*20 mode
    jsr logo_switchoff
    ;
.endmacro
;==========================================
get_mode_values:
    bbr0 half_mode, :+
    set_halfmode_values
    rts
   :set_normalmode_values
    rts
;==========================================
;====================================================== populate layer 0 map
populate_layer0:        ; layer 0 map uses 8x8pix @8bpp tiles, map size is 128x64 tiles
    stz VERA_ctrl
    lda #$10
    sta VERA_addr_bank
    stz VERA_addr_high
    stz VERA_addr_low
;====================== start with all being tiletype #0: (applies to both modes)
    ldy #$80
   :ldx #$40
   :stz VERA_data0
    stz VERA_data0
    dex
    bne :-
    dey
    bne :--
    rts
;====================================================== populate layer 1 map
.macro first_row
    ldx row_fill_cnt
@row1:
    lda cheat_useframe
    sta VERA_data0
    stz VERA_data0  ; no flip
    sta VERA_data0
    lda #$04        ; h-flip
    sta VERA_data0
    dex
    bne @row1
.endmacro
;==================
.macro second_row
    ldx row_fill_cnt
@row2:
    lda #$01        ; mid left tile
    ora cheat_useframe
    sta VERA_data0
    stz VERA_data0  ; no flip
    sta VERA_data0
    lda #$04        ; h-flip
    sta VERA_data0
    dex
    bne @row2
.endmacro
;==================
.macro third_row
    ldx row_fill_cnt
@row3:
    lda cheat_useframe
    sta VERA_data0
    lda #$08
    sta VERA_data0  ; V - flip, no color Offset
    lda cheat_useframe
    sta VERA_data0
    lda #$0C        ; v+h-flip
    sta VERA_data0  ; V+H flip, no color Offset
    dex
    bne @row3
.endmacro
;==================
populate_layer1:
; layer 1 map uses 16x16pix @8bpp tiles, map size is 64x32 tiles in normal mode
    stz VERA_ctrl
    lda #$10
    sta VERA_addr_bank
    lda #$40
    sta VERA_addr_high
    stz VERA_addr_low
    ;
    lda half_mode
    bne @pop_half
    ;
    lda cheat_frame
    sta cheat_useframe
    bra @threelines
    ;
@pop_half:
    lda halfc_frame
    sta cheat_useframe
    ;
@threelines:
    ldy max_line_no
   :first_row
    second_row
    third_row
    ;
    dey
    bne :-
    rts
;=================================================  set video and layer modes
set_mode:
    stz VERA_dc_video   ; disable display during changing layer modes and redrawing everything
    bbs0 half_mode, set_half_mode
;=================================================
set_normal_mode:    ; also used by intro
    jsr get_mode_values
    ;
    stz VERA_ctrl   ; set sprite attr to cursor sprite #1
    lda #$11        ; stride = 1
    sta VERA_addr_bank
    lda #$FD
    sta VERA_addr_high
    lda #$F0
    sta VERA_addr_low
    ;
    lda #$01
    sta VERA_ctrl   ; set sprite attr to cursor sprite #1
    lda #$11        ; stride = 1
    sta VERA_addr_bank
    lda #$FD
    sta VERA_addr_high
    lda #$F8
    sta VERA_addr_low
    ;
    ;
    ; shouldn't 0 be written here?
    ; because of the stride we get one byte further
    ; but the first address byte of the sprite is read like this
    ; only read? Dto. half_mode see below.
    lda VERA_data0      ; lda #<(VRAM_cursor >> 5)
    lda VERA_data1
    ;
    ;
    lda #>(VRAM_cursor >> 5)
    ora #$80        ; Bit 7 : 8bpp mode
    sta VERA_data0
    sta VERA_data1
; position
    stz VERA_data0  ; xpos lo
    stz VERA_data1

    lda VERA_data0  ; xpos hi
    lda VERA_data1  ; xpos hi
    ;
    lda crspr_posy_lo   ; initial normal cursor screen y pos
    sta VERA_data0  ; ypos lo
    inc
    inc
    sta VERA_data1  ; ypos lo
    ;
    stz VERA_data0  ; ypos hi
    stz VERA_data1  ; ypos hi
    ;
    lda crsrstatus ; Bit0 = crsr_on
    and #$FE
    asl
    asl
    asl
    sta ZP_tmp_DATA
    lda ins_mode         ; ggf. v flip
    eor #$01
    asl
    ora ZP_tmp_DATA             ; z on
    sta VERA_data0  ; z-depth
    sta VERA_data1  ; z-depth
    ;
    lda #$20        ; sprite32W,8H=$20, palette offset 0
    sta VERA_data0
    sta VERA_data1
    ;
    bra set_video
;=======================================================
set_half_mode:
    jsr logo_switchoff
    ;
    jsr get_mode_values
    ; set sprite attr to cursor sprite #1
    stz VERA_ctrl
    lda #$11        ;stride = 1
    sta VERA_addr_bank
    lda #$FD
    sta VERA_addr_high
    lda #$F0
    sta VERA_addr_low
    ;
    lda VERA_data0  ; lda #<(VRAM_hcursor >> 5), lower half of cursor sprite will be written by page restore routine
    lda #>(VRAM_hcursor >> 5)   ; cursor sprite data adr. same for both normal and half mode
    ora #$80        ; Bit 7 : 8bpp mode
    sta VERA_data0
;position:
    stz VERA_data0  ; xpos lo
    lda VERA_data0  ; xpos hi
    ;
    lda hcrspr_posy_lo  ; initial half cursor screen y pos
    sta VERA_data0  ; ypos lo
    stz VERA_data0  ; ypos hi
    lda crsrstatus ; Bit0 = crsr_on
    and #$FE
    asl
    asl
    asl
    sta ZP_tmp_DATA
    lda ins_mode         ; ggf. v flip
    eor #$01
    asl
    ora ZP_tmp_DATA             ; z on
    sta VERA_data0  ; z-depth
    lda #$10        ; sprite16W,8H=$10, palette offset 0
    sta VERA_data0
    ;
    jsr VERA_addr_to_cursorZ
    stz VERA_data1
    ; keep on going
;=================================================  set video and layer modes
set_video:
    stz VERA_dc_video ; disable display
    lda #128   ; 640x480
    sta VERA_dc_hscale
    sta VERA_dc_vscale
    ;
; Layer 0 in all modes
    lda #$63 ; 128x64 tiles 8x8pix @8bpp
    sta VERA_L0_config
    stz VERA_L0_mapbase
    stz VERA_L0_hscroll_l
    stz VERA_L0_hscroll_h
    stz VERA_L0_vscroll_l
    stz VERA_L0_vscroll_h
    ;
; Layer 1 in all modes
    lda #(VRAM_layer1_map >> 9)
    sta VERA_L1_mapbase
    stz VERA_L1_hscroll_l
    stz VERA_L1_hscroll_h
    stz VERA_L1_vscroll_l
    stz VERA_L1_vscroll_h
    ;
    stz crsr_posx
    stz crsr_posy
    jsr move_cursor ; (half_mode bit already valid and sprite base address in attrb. set)
    jsr set_vlinecol
    ;
    bbs0 half_mode, @half_map
; Layer 0 in normal mode
    lda #((((VRAM_backtiles >> 11) & $3F) << 2) | $00)  ; 8x8 tiles
    sta VERA_L0_tilebase
; Layer 1 in normal mode
    lda #$13 ; 64x32 tiles 16x16pix @8bpp
    sta VERA_L1_config
    lda #((((VRAM_frametiles >> 11) & $3F) << 2) | $03)  ; 16x16 tiles
    sta VERA_L1_tilebase
    ;
    bra @enable_video
    ;
@half_map:
; L0 in half mode
    ; tilebase adr. written by set_half_mode
    ;
; L1 in half mode
    lda #$63 ; 128x64 tiles 8x8pix @8bpp
    sta VERA_L1_config
    lda #(((VRAM_halfframes >> 11) & $3F) << 2)         ; 8x8 tiles
    sta VERA_L1_tilebase
    ;
@enable_video:  ; all modes
    jsr write_txtpage       ; writes Layer 0
    jsr populate_layer1     ; mandatory
    ;
    lda dc_mode
    sta VERA_dc_video
    lda half_mode
    sta mode_last_run
    ;
    rts
;======================
.endif
