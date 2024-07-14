.ifndef TEXTUTILS3_INC
TEXTUTILS3_INC  = 1
;==================================
ins_mode:           .byte $00
ovr_ride_insmode:   .byte $00
;==================================
delete_systxt:
    lda #<systxt0
    sta r0L
    lda #>systxt0
    sta r0H
    ;
    lda #$20    ; $20
    sta r1L
    lda #$03    ; $03
    sta r1H     ; $0320 = d800, in case only the text region should be deleted
    ;           ;               would leave garbage in between usrtxt0-7 RAM's
    bra del_tRAM
    ;
delete_txtRAM:
    lda actual_usrtxtlo
    sta r0L
    lda actual_usrtxthi
    sta r0H
    ;
    lda #$20    ; $20
    sta r1L
    lda #$03    ; $03
    sta r1H     ; $0320 = d800, in case only the text region should be deleted
    ;           ;               would leave garbage in between usrtxt0-7 RAM's
del_tRAM:
    lda #$20    ; 'spc'
    jmp MEMORY_FILL ;use rts of MEMORY_FILL
   ; rts
;==================================
.macro blockdel_txtRAM
    lda #<usrtxt0
    sta ZP_PTR_4
    sta r0L
    lda #>usrtxt0
    sta ZP_PTR_4+1
    sta r0H
    ;
    lda #$00    ; $20
    sta r1L
    lda #$04    ; $03
    sta r1H     ; $0320 = d800, in case only the text region should be deleted
    ;           ;               would leave garbage in between usrtxt0-7 RAM areas
    lda #$08
    sta ZP_tmp_DATA
    ;
   :jsr del_tRAM
    jsr add_4_PTR_4_hi
    sta r0H
    lda ZP_PTR_4
    sta r0L
    ;
    dec ZP_tmp_DATA
    bne :-
.endmacro
;======================
delete_allusrtxt:
    lda #<usrtxt0
    sta ZP_PTR_4
    sta r0L
    lda #>usrtxt0
    sta ZP_PTR_4+1
    sta r0H
    ;
    lda #$20    ; $20
    sta r1L
    lda #$03    ; $03
    sta r1H     ; $0320 = d800, in case only the text region should be deleted
    ;           ;               would leave garbage in between usrtxt0-7 RAM areas
    lda #$08
    sta ZP_tmp_DATA
    ;
   :jsr del_tRAM
    jsr add_4_PTR_4_hi
    sta r0H
    lda ZP_PTR_4
    sta r0L
    ;
    dec ZP_tmp_DATA
    bne :-
    rts
;==================================================================
.macro insert_page_nums ; writes "page#" at beginning of each text page
    lda #<usrtxt0       ; target
    sta ZP_PTR_4
    lda #>usrtxt0
    sta ZP_PTR_4+1
    ;
    lda #$08
    sta ZP_tmp_DATA
    lda #$30
    sta ZP_tmp_DATA2
@ini_loop:
    ldx #<pagetxt0       ; source
    ldy #>pagetxt0
    jsr set_PTR_1
    ldx #$05
    jsr syscpy_loop     ; from PTR1->PTR4
    ;
    ldy #$05
    lda ZP_tmp_DATA2
    sta (ZP_PTR_4),y
    inc ZP_tmp_DATA2
    jsr add_4_PTR_4_hi
    dec ZP_tmp_DATA
    bne @ini_loop
.endmacro
;===========================================
i_onepagesettings:
    smb0 half_mode
    stz hpreset
    jsr get_h_presetcols
    jsr save_page_settings
    ;
    stz half_mode
    stz npreset
    jsr get_presetcol
    jmp save_page_settings  ; use rts of save_page...
;========================
init_pagesettings:
    lda #<usrtxt0
    sta actual_usrtxtlo
    lda #>usrtxt0
    sta actual_usrtxthi
    ;
    jsr i_onepagesettings
    ;
    ldx #$07
   :jsr actual_usrtxt_inc
    jsr i_onepagesettings
    ;
    dex
    bne :-
    ;
    ; exit with usrtxt0 being the actual one
    lda #<usrtxt0
    sta actual_usrtxtlo
    lda #>usrtxt0
    sta actual_usrtxthi
    ;
    rts
;===========================================
del_shift_txtRAM:  ; shifts text 2 left and adds $20 'spc' at last position
    lda actual_usrtxtlo
    sta ZP_PTR_4            ; REM: moved from ZP_PTR_5 to ZP_PTR_4
    lda actual_usrtxthi
    sta ZP_PTR_4+1
    ;
    ldx crsr_posy
    beq @chkxpos
   :jsr add_line_2_ZP4  ; multiplication loop
    dex
    bne :-
@chkxpos:
    ldy crsr_posx
    cpy max_char_p_line_mone
    bmi @do_shift
@spc:
    lda #$20
    sta (ZP_PTR_4),y ; writes 'spc' character into text RAM
    rts
@do_shift:
; need loop counter first
    sec
    lda max_char_p_line_mone
    sbc crsr_posx
    tax
   :iny
    lda (ZP_PTR_4),y
    dey
    sta (ZP_PTR_4),y
    iny
    cpx #$01
    beq @spc
    dex
    bne :-
    rts
;===========================================
shift_right_txtRAM:  ; shifts text 2 right
    ldy crsr_posx
    cpy max_char_p_line_mone
    bmi @do_shift
    rts
@do_shift:
    ldx max_char_p_line_mone
   :txa
    tay
    dey
    lda (ZP_PTR_4),y
    iny
    sta (ZP_PTR_4),y
    ;
    cpx #$01
    beq :+
    dex
    cpx crsr_posx
    bne :-
    ;
   :rts
;===========================================
; char_2_sysRAM:    ; writes character into systextRAM instead of usrtext
;     lda #<systxt0 ; preserve this outcommented for future user inputs within menus
;     sta ZP_PTR_4
;     lda #>systxt0
;     sta ZP_PTR_4+1
;     bra char_2_tRAM
char_2_txtRAM:  ; writes character into actual usertextRAM
    lda actual_usrtxtlo
    sta ZP_PTR_4        ; ZP_PTR_4 can't be used otherwise insert won't work
    lda actual_usrtxthi
    sta ZP_PTR_4+1
;char_2_tRAM:
    ldx #$01    ; preload error code in case we exit
    lda char_value
    cmp #$20 ; 'spc' 1st possible character
    bmi @exit
    ;
    lda #$7F ; '<-' last possible character
    cmp char_value
    bmi @exit
    ;
    ldx crsr_posy
    beq @chkxpos
   :jsr add_line_2_ZP4  ; multiplication loop
    dex
    bne :-
@chkxpos:
    lda ins_mode
    beq :+
    lda ovr_ride_insmode
    bne :+
    jsr shift_right_txtRAM
   :stz ovr_ride_insmode
    ldy crsr_posx
    lda char_value
    sta (ZP_PTR_4),y ; writes single character into user text
    ldx #$00
@exit:
    txa         ; return 1 when nothing was written, 0 if done
    rts
    ;
;===========================================
set_filedevice:
    lda #01   ; Logical Number = 1
    ldx #08   ; Device = "SD card" (emulation host FS)
    ldy #02   ; Secondary Address = 2, no header, target adr. as in .X,.Y when loading
    jsr SETLFS
    lda #(end_filename-filename) ; filename length
    ldx #<filename
    ldy #>filename
    jmp SETNAM  ; use rts of SETNAM
    ;rts
    ;
;=====================
load_file:
    jsr set_filedevice
    ;
    lda #00   ; load target CPU RAM
    ldx actual_usrtxtlo
    ldy actual_usrtxthi
    jsr LOAD ; -> c= error, a= error code
    bcc :+
    jmp save_file
   :rts
    ;
save_file:
    jsr set_filedevice
    ;
    clc
    lda actual_usrtxtlo
    sta ZP_PTR_4
    adc #$20
    tax
    ; Achtung kein bcc abgefragt! geht hier weil bekannt, dass Adresse auf 00en endet.
    lda actual_usrtxthi
    sta ZP_PTR_4+1
    adc #$03        ; $0320 = d800 = 40*20
    tay
    lda #ZP_PTR_4
    jmp BSAVE   ; use rts of BSAVE
    ;
    ;rts
;==========================================
copypage:
    lda actual_usrtxtlo
    sta r0L
    lda actual_usrtxthi
    sta r0H
    jsr actual_usrtxt_inc
    lda actual_usrtxtlo
    sta r1L
    lda actual_usrtxthi
    sta r1H
    ;
    lda #$20    ; $20
    sta r2L
    lda #$03    ; $03
    sta r2H     ; $0320 = d800, in case only the text region should be deleted
    ;           ;               would leave garbage in between usrtxt0-7 RAM's
    jmp MEMORY_COPY ; use rts of MEMORY_COPY
    ;rts
;================================================================
; copys one line of text
; exits on $00 or $04 (EOF), increments source pointer PTR1
single_line_cpy:    ; needs to be preloaded with max. char# in x
    tax             ; copies from ZP_PTR_1 source
    ldy #$00        ; to ZP_PTR_4
l_loop:             ; may be used to cpy line with undefined lenght (see menu bar)
    lda (ZP_PTR_1)
    beq :+              ; $00 = end of line
    cmp #$04            ; EOF
    bne :++
   :rts                 ; exit on $00 or EOF
    ;
   :cmp #$0F            ; Function   26.02.2024 : wird noch nicht genutzt, Exit on special function
    beq :+
    sta (ZP_PTR_4),y
   :jsr inc_PTR_1
    iny
    dex
    bne l_loop
    lda (ZP_PTR_1)
    rts
;================================================================
write_txtline:  ; part of output to screen
    stz Z_index
   :lda Z_pointer_hi
    sta char_VRAM_adr_hi
    lda Z_pointer_lo
    sta char_VRAM_adr_lo
    ;
    ldy Z_index
    lda (ZP_PTR_3),y
    sta char_value
    ;
    jsr write_singlechar
    ;
    clc
    lda Z_pointer_lo
    adc Z_pointer_inc_lo
    sta Z_pointer_lo
    bcc :+
    inc Z_pointer_hi
    ;
   :inc Z_index
    lda Z_index
    cmp max_char_p_line
    bmi :--
    ;
    rts
;==========================================
write_txtpage:
    lda menu_mode       ; checkout possible menu mode
    bne @sys_txt        ; to select text pointer
    ;
    lda actual_usrtxtlo ; usr text pointer changes by page
    sta ZP_PTR_3
    lda actual_usrtxthi
    sta ZP_PTR_3+1
    bra @write_txt
@sys_txt:
    lda #<systxt0       ; sys text gets set up depending on e.g. active menu
    sta ZP_PTR_3        ; ZP_PTR_3 is used as source text pointer further on
    lda #>systxt0
    sta ZP_PTR_3+1
    ;
@write_txt:
    lda max_line_no             ; d10 when in 20*10 mode, d20 when in 40*20 mode
    sta Z_no                    ; line counter
    stz Z_pointer_hi            ; hi+$06 = line offset
    stz Z_pointer_lo            ; lo+$08 = digit offset
    ;
@page_loop:
    jsr write_txtline           ; writes one line of text, from given source pointer ZP_PTR_3
    ;                           ; and set line, digit offset(<- should be $0), all depends on mode
    stz Z_pointer_lo
    clc
    lda Z_pointer_hi
    adc Z_pointer_inc_hi        ; inc differs on 20*10 mode or 40*20 mode
    sta Z_pointer_hi
    clc
    lda ZP_PTR_3
    adc max_char_p_line         ; d20 when in 20*10 mode, d40 when in 40*20 mode
    sta ZP_PTR_3
    bcc :+
    inc ZP_PTR_3+1
   :dec Z_no
    bne @page_loop
    ;
    stz textrefresh
    ;
    rts
;==========================================
.endif
