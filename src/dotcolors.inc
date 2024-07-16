.ifndef DOTCOLORS_INC
DOTCOLORS_INC  = 1
;==========================================================================================================
preset:         .byte $00
npreset:        .byte $00
hpreset:        .byte $00
cheat_vlinecol: .byte $00
;==========================================================================================================
; standard default presets:
preset_colschemes:
; numbers represent offset address to background tileset data and cursor sprite
; cheat_colframe, cheat_stylframe, cheat_litecol, cheat_litebgcol, cursor # (lower part of Vaddr), vline SPR# (lower part of Vaddr)
.byte $18, $00, $E0, $10, $88, $10  ; #$00 white on blue
.byte $08, $00, $80, $00, $80, $00  ; #$06 white on black
.byte $10, $00, $00, $08, $A8, $50  ; #$0C "almost black" on white
.byte $38, $02, $E0, $18, $90, $20  ; #$12 LCD
;
.byte $08, $02, $60, $00, $80, $00  ; #$18 amber square on black
.byte $08, $00, $40, $00, $A0, $40  ; #$1E red on black
.byte $30, $06, $C0, $18, $80, $30  ; #$24 small magenta blue on purple
.byte $00, $00, $20, $00, $80, $00  ; #$2A green on dark brown
;
hpreset_colschemes:
; numbers represent offset to next tileset for bg tiles used as tileindex
; tilebase L0 for litecol   used as offset in tilebase
; cursor # (lower part of Vaddr)
; half_fg index offset, spare $00!, hlite_col L0 tilebase, spare $00!, cursor SPR# (lower part of Vaddr), vline SPR# (lower part of Vaddr)
.byte $02, $00, $B8, $00, $00, $00   ; red on blk, blue crsr
.byte $02, $00, $C8, $00, $08, $20   ; grn on blk, red crsr
.byte $02, $00, $D8, $00, $38, $70   ; wht on blk, bright prpl cursr
.byte $08, $00, $E8, $00, $08, $50   ; grey on light LCD grey, grey crsr
;
.byte $0A, $00, $E8, $00, $00, $00   ; grey on light grey, blue crsr
.byte $0C, $00, $E8, $00, $1C, $70   ; grey on light yellow, turqouise crsr
.byte $0E, $00, $E8, $00, $08, $20   ; grey on LCD green, red crsr
.byte $00, $00, $B8, $00, $00, $00   ; red on dark brown, blue crsr
;
;==========================================================================================================
crsr_colschemes:
; upper half color, lower half color, ...repeats for each cursor sprite
; only upper color used (seen part) - yet (additional color may be used in future when vertically flipped)
; lower part of cursor VERAaddr
        ;$80            ;$88        ;$90        ;$98        ;$A0        ;$A8        ;$B0        ;$B8
; numbers represent color index in X16 color palette
        ; pastel blue   ; ocker     ; red       ; white     ;frog green ; gray      ; kakhi     ; turqousie
.byte   $0E,$34,        $50,$10,    $34,$10,    $01,$10,    $73,$10,    $17,$10,    $49,$10,    $AB,$10
        ;$C0            ;$C8        ;$D0        ;$D8         ;$E0       ;$E8        ;$F0        ;$F8
        ; blk/invisible ; pink      ; dark pink ; sandgray  ; darkmint  ; brght blu ;brght purpl; hmm
.byte   $00,$10,        $26,$10,    $2D,$10,    $40,$10,    $7F,$10,    $BA,$10,    $E3,$10,    $AB,$10
;
; vline_colors (lower part of Vaddr):
; $00 light blue, $10 okker, $20 red, $30 wht, $40 green, $50 blk, $60 yellowish, $70 turqouise

; .byte   $0E,            $50,        $34,        $01,        $73,        $17,        $49,        $AB
; .byte   $00,            $26,        $2D,        $40,        $7F,        $BA,        $E3,        $AB
;==========================================================================================================
colframe_inc:       .byte $08 ; $02 in halfmode
maxcolframe_cnt:    .byte $40 ; $20 in halfmode
;==========================================================================================================
.macro make_samecolors
    clc
    lda #>usrtxt0;
    adc #$03
    sta ZP_PTR_6+1
    ;
    lda #$20
    sta ZP_PTR_6
    ;
    jsr set_pagesettings_ptr    ; provides ZP_PTR_5 with actual page
    ;
    lda #$08
    sta ZP_tmp_DATA
@ms_loop:
    ldx #$10    ; > eigentlich egal, zwischenraum ist gross..
    ldy #$01    ; byte0 = step time, we don't want to make this same, so start @ byte1
   :lda (ZP_PTR_5),y
    sta (ZP_PTR_6),y
    iny
    dex
    bne :-
    ;
    clc
    lda ZP_PTR_6+1
    adc #$04
    sta ZP_PTR_6+1
    ;
    dec ZP_tmp_DATA
    bne @ms_loop
    ;
.endmacro
;======================
next_col_frame:
    bbs0 half_mode, :+
    ;
    clc
    lda cheat_colframe
    adc #$08            ; colframe_inc
    sta cheat_colframe
    cmp #$40            ; maxcolframe_cnt
    bmi redraw_frame
    stz cheat_colframe
    bra redraw_frame
   :clc
    lda halfc_frame
    adc #$02            ; adc colframe_inc
    sta halfc_frame
    cmp #$10            ; maxcolframe_cnt
    bmi redraw_frame
    stz halfc_frame
    bra redraw_frame
;======================
next_styl_frame:
    bbr0 half_mode, :+
    stz cheat_stylframe
    bra next_col_frame
   :clc
    lda cheat_stylframe
    adc #$02
    sta cheat_stylframe
    cmp #$08
    bmi redraw_frame
    stz cheat_stylframe
redraw_frame:
    lda cheat_colframe
    ora cheat_stylframe
    sta cheat_frame
    jsr save_page_settings
    smb1 mode_last_run  ;     lda #$02
    jmp main_loop
    ;
;======================
next_preset:
    jsr memorize_prevmode
    bbs0 half_mode, @np_half_mode
    ;
    lda npreset
    cmp #$2A
    bmi :+
    stz npreset
    bra @np_out
   :clc
    adc #06
    sta npreset
@np_out:
    sei
    jsr get_presetcol
    jsr set_vlinecol
    cli
    bra color_out
@np_half_mode:
    ;
    lda hpreset
    cmp #$2A
    bmi :+
    stz hpreset
    bra @hp_out
   :clc
    adc #06
    sta hpreset
@hp_out:
    sei
    jsr get_h_presetcols
    jsr set_vlinecol
    cli
    jsr save_page_settings
    jsr check_refreshmode
    jmp output
    ;
;======================
next_litebgcol:
    jsr memorize_prevmode
    bbs0 half_mode, next_halflitebg; :+
    clc
    lda cheat_litebgcol
    adc #$08
    sta cheat_litebgcol
    cmp #$20
    bmi color_out
    stz cheat_litebgcol
    bra color_out
    ;
;======================
next_litecol:
    jsr memorize_prevmode
    bbs0 half_mode, :+
    clc
    lda cheat_litecol
    adc #$20
    sta cheat_litecol ; may overflow ;-)
;=========
color_out:
    clc
    lda cheat_litebgcol
    adc cheat_litecol
    sta cheat_col
    ;
    jsr save_page_settings
    jsr check_refreshmode
    jmp output
;========= half mode
next_halflitebg:
   :lda VERA_L0_tilebase
    cmp #$E8
    bmi :+
    lda #$B8
    sta VERA_L0_tilebase
    ;
    jsr check_refreshmode
    jmp output
    ;
   :clc
    adc #$10
    sta VERA_L0_tilebase
    jsr save_page_settings
    smb1 mode_last_run  ;     lda #$02
    jmp output
    ;
;======================
toggle_crsr_col:
    bbs0 half_mode, toggle_hcrsr
    jsr VERA_addr_to_both_cursorSPR
    ;
    clc
    lda VERA_data0
    adc #$08                ; normal mode
    cmp #$F8
    bmi :+
    lda #$80
   :sta VERA_data0  ; set sprite attr
    sta VERA_data1  ; set sprite attr
    jmp save_page_settings  ; use rts of save_page_settings
;======================
toggle_hcrsr:

    jsr VERA_addr_to_cursorSPR

    clc
    lda VERA_data0
    adc #$04                ; half mode
    cmp #$38
    bmi :+
    lda #$00
   :sta VERA_data0  ; set sprite attr
    jmp save_page_settings  ; use rts of save_page_settings
;======================
next_vlinecol:
    lda cheat_vlinecol
    cmp #$60
    bmi :+
    stz cheat_vlinecol
    jmp set_vlinecol
   :clc
    lda cheat_vlinecol
    adc #$10
    sta cheat_vlinecol
    jsr save_page_settings
    ;
set_vlinecol:
    jsr VERA_addr_to_vlineSPR
    ldx #$08
   :lda cheat_vlinecol
    sta VERA_data0  ; low address 8 sprite attr used: #2-#9
    dex
    bne :-
    jsr VERA_addr_to_vlineDEMOSPR
    lda cheat_vlinecol
    sta VERA_data0
    rts
    ;
;======================
.endif
