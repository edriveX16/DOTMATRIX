.ifndef PRESETS_INC
PRESETS_INC = 1
;============================================
.macro make_samesteptime
    clc
    lda #>usrtxt0;
    adc #$03
    sta ZP_PTR_2+1
    ;
    lda #$20
    sta ZP_PTR_2
    ;
    ldx #$08
@ms_loop:
    ;
    lda seq_steptime
    sta (ZP_PTR_2)
    ;
    clc
    lda ZP_PTR_2+1
    adc #$04
    sta ZP_PTR_2+1
    ;
    dex
    bne @ms_loop
    ;
.endmacro
;==========================================================================================================
get_presetcol:  ; only for normal mode 20X10
    lda #<preset_colschemes     ; look up table
    sta ZP_PTR_2
    lda #>preset_colschemes
    sta ZP_PTR_2+1
    ;
    ldy npreset         ; preset number for 20x10 mode
    ;
    lda (ZP_PTR_2),y
    sta cheat_colframe
    iny
    ;
    lda (ZP_PTR_2),y
    sta cheat_stylframe
    ora cheat_colframe
    sta cheat_frame
    iny
    ;
    lda (ZP_PTR_2),y
    sta cheat_litecol
    iny
    ;
    clc
    lda (ZP_PTR_2),y
    sta cheat_litebgcol
    adc cheat_litecol
    sta cheat_col
    iny
    ;
    jsr VERA_addr_to_both_cursorSPR
    lda (ZP_PTR_2),y
    sta VERA_data0
    sta VERA_data1
    iny
    ;
    lda (ZP_PTR_2),y
    sta cheat_vlinecol
    ;
    ; don't overwrite logo on
    ;
    rts
;=====================
get_h_presetcols:   ; only for half mode 40X20
    lda #<hpreset_colschemes
    sta ZP_PTR_2
    lda #>hpreset_colschemes
    sta ZP_PTR_2+1
    ;
    ldy hpreset         ; preset number for 40x20 mode
    ;
    lda (ZP_PTR_2),y
    sta halfc_frame
    iny
    ;
    lda (ZP_PTR_2),y    ; $00
    iny
    ;
    lda (ZP_PTR_2),y
    sta VERA_L0_tilebase    ; ! tilebase in half_mode !
    iny
    ;
    lda (ZP_PTR_2),y    ; $00 ommit anyway, cheat_col should be $00 in half_mode
    iny
    ;
    jsr VERA_addr_to_both_cursorSPR
    lda (ZP_PTR_2),y
    sta VERA_data0
    iny
    ;
    lda (ZP_PTR_2),y
    sta cheat_vlinecol
    ;
    rts
;=========================================
; Remember the page settings so that you can compare them with the new settings when changing pages.
; If the mode does not change and the background color remains the same,
; layer1 does not need to be redrawn when the page is changed.
; Which reduces flickering (and is faster).
memorize_prevmode:
    lda half_mode
    sta previous_mode
    lda cheat_frame
    sta previous_mode+1
    lda halfc_frame
    sta previous_mode+2
    rts
;==================
check_refreshmode:
    lda half_mode
    cmp previous_mode
    bne @ref_all
    ;
    bbs0 half_mode, :+
    lda cheat_frame
    cmp previous_mode+1
    bne @ref_all
    bra @ref_txt
   :lda halfc_frame
    cmp previous_mode+2
    bne @ref_all
    ;
@ref_txt:
    smb0 textrefresh    ; only redraw text pattern tiles in layer 0
    rts
@ref_all:
    smb1 mode_last_run ; forces both layers to be redrawn
    rts
;=========================================
; store page settings behind text RAM area
save_page_settings:
    jsr set_pagesettings_ptr
    ;
    ldy #$00
; Byte 0: step time
    lda seq_steptime
    sta (ZP_PTR_5),y
    iny
; Byte 1: mode
; Byte 2: npreset

; Byte 3-8: cheat_colframe, cheat_stylframe, cheat_litecol, cheat_litebgcol, cursor # (lower part of Vaddr), vline SPR# (lower part of Vaddr)
; Byte 9: X16logo_on
; half
; Byte 10: hpreset
; Byte 11- 17: half_fg index offset, spare $00!, hlite_col L0 tilebase, spare $00!, cursor SPR# (lower part of Vaddr), vline SPR# (lower part of Vaddr), spare $00
    bbs0 half_mode, @save_half
    ;
; values for normal mode 20x10
    lda #$00
    sta (ZP_PTR_5),y    ; mode
    iny
    ;
    lda npreset
    sta (ZP_PTR_5),y
    iny
    ;
    ;
    lda cheat_colframe
    sta (ZP_PTR_5),y
    iny
    ;
    lda cheat_stylframe
    sta (ZP_PTR_5),y
    iny
    ;
    lda cheat_litecol
    sta (ZP_PTR_5),y
    iny
    ;
    lda cheat_litebgcol
    sta (ZP_PTR_5),y
    iny
    ;
    jsr VERA_addr_to_both_cursorSPR
    lda VERA_data0
    sta (ZP_PTR_5),y
    iny
    ;
    lda cheat_vlinecol
    sta (ZP_PTR_5),y
    iny
    ;
    lda X16logo_on
    sta (ZP_PTR_5),y
    rts
;========= save values in half mode 40x20
@save_half:
    lda #$01
    sta (ZP_PTR_5),y    ;mode
    ;
    ldy #$0A
    ;
    lda hpreset
    sta (ZP_PTR_5),y
    iny
    ;
    lda halfc_frame
    sta (ZP_PTR_5),y
    iny
    ;
    iny     ; spare $00!
    ;
    lda VERA_L0_tilebase    ; ! tilebase in 40x20 mode !
    sta (ZP_PTR_5),y
    iny
    ;
    iny     ; spare $00!
    ;
    jsr VERA_addr_to_both_cursorSPR
    lda VERA_data0
    sta (ZP_PTR_5),y
    iny
    ;
    lda cheat_vlinecol
    sta (ZP_PTR_5),y
    ; ; spare $00!
    rts
    ;
;======================
restore_page_settings:
    jsr set_pagesettings_ptr
    ldy #$00
    ;
    lda (ZP_PTR_5),y
    sta seq_steptime
    iny
    ;
    lda (ZP_PTR_5),y    ; mode
    bne @load_half
;normal
    stz half_mode
    iny     ; from mode
    ;
    lda (ZP_PTR_5),y
    sta npreset
    iny
    ;
    lda (ZP_PTR_5),y
    sta cheat_colframe
    iny
    ;
    lda (ZP_PTR_5),y
    sta cheat_stylframe
    iny
    ;
    ora cheat_colframe
    sta cheat_frame
    ;
    lda (ZP_PTR_5),y
    sta cheat_litecol
    iny
    ;
    lda (ZP_PTR_5),y
    sta cheat_litebgcol
    iny
    ;
    clc
    adc cheat_litecol
    sta cheat_col
    ;
    jsr VERA_addr_to_both_cursorSPR
    lda (ZP_PTR_5),y
    sta VERA_data0
    sta VERA_data1
    iny
    ;
    lda (ZP_PTR_5),y
    sta cheat_vlinecol
    ;
    iny
    ;
    lda (ZP_PTR_5),y
    sta X16logo_on
    ;
    rts
;===========================
@load_half:
    smb0 half_mode
    ;
    ldy #$0A
    ;
    lda (ZP_PTR_5),y
    sta hpreset
    iny
    ;
    lda (ZP_PTR_5),y
    sta halfc_frame
    iny
    ;
    iny ; spare
    ;
    lda (ZP_PTR_5),y
    sta VERA_L0_tilebase    ; ! tilebase in 40x20 mode !
    iny
    ;
    iny ; spare
    ;
    jsr VERA_addr_to_both_cursorSPR
    lda (ZP_PTR_5),y
    sta VERA_data0
    iny
    ;
    lda (ZP_PTR_5),y
    sta cheat_vlinecol
    ;
    rts
    ;
;==========================================================================================================
.endif
