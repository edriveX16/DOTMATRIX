 .ifndef KEYBOARD_INC
KEYBOARD_INC  = 1
;================================================================
tab_inc: .byte $04
;================================================================
restore_close:  ; restores settings, selects mode and closes possibly open menu
    jsr memorize_prevmode
    jsr restore_page_settings
    jsr check_refreshmode
    jmp close_menu
    ;
;===============================================================
; main default keyboard procedure
proc_defkeys:
    lda char_value
    cmp #$13    ; $13= home key
    bne @crsr_rikey
    jsr cursor_home
    bra @m_l
    ;
@crsr_rikey:
    cmp #$1D    ; $1D= cursor right key:
    bne @crsr_lekey
    smb0 zpreq_crsmove
    bra @m_l
@crsr_lekey:    ; $9D= cursor left key:
    cmp #$9D
    bne @crsr_dnkey
    smb1 zpreq_crsmove
    bra @m_l
@crsr_dnkey:    ; $11= cursor down key:
    cmp #$11
    bne @crsr_upkey
    smb2 zpreq_crsmove
    bra @m_l
@crsr_upkey:
    cmp #$91    ; $91= cursor up key:
    bne @ins_key    ;@returnkey
    smb3 zpreq_crsmove
    bra @m_l
@ins_key:
    cmp #$94    ; $94= insert key:
    bne @returnkey
    lda ins_mode
    eor #$01
    sta ins_mode
    jsr cursor_switchon
    bra @m_l
    ;
@returnkey:
    cmp #$0D
    bne @pg_dwnkey
    lda crsr_posy
    inc
    cmp max_line_no     ; ($09 when normal mode)
    bmi :+
    bra @m_l
    ;
   :inc crsr_posy
    stz crsr_posx
    jsr move_cursor
    stz crsr_offtimer
@m_l:
    jmp main_loop
    ;
@pg_dwnkey:
    cmp #$02
    bne @pg_upkey
    jsr save_page_settings
    jsr actual_usrtxt_inc
    jsr pageSPR_startswipeout
    jmp restore_close
    ;
@pg_upkey:
    cmp #$82
    bne @tabkey
    jsr save_page_settings
    jsr actual_usrtxt_dec
    jsr pageSPR_startswipeout
    jmp restore_close
    ;
@tabkey:
    cmp #$09
    bne @delkey
    lda #$04
    sta tab_inc
    ;
   :lda tab_inc
    cmp crsr_posx
    bmi @tab_s      ; reduce spaghettiness?
    beq @tab_s
    bra @tab_end    ; or add some parmesan?
    ;
@tab_s:
    clc
    adc #$04
    sta tab_inc
    cmp max_char_p_line_mone
    bmi :-
    ;
    lda max_char_p_line_mone
    cmp tab_inc
    bmi :+
    lda tab_inc
    bra @tab_end
   :lda max_char_p_line_mone
@tab_end:
    sta crsr_posx
    jsr move_cursor
    stz crsr_offtimer
    bra @m_l
@delkey:
    cmp #$14    ; $14=DEL/BCKSPC key:
    bne @fwd_delkey
    lda crsr_posx
    beq :+
    dec crsr_posx
    bra :++
   :lda crsr_posy
    beq :+
    dec crsr_posy
    lda max_char_p_line_mone
    sta crsr_posx
   :jsr move_cursor
    stz crsr_offtimer
    lda #$01
    sta ovr_ride_insmode
@spc:
    lda #$20     ; 'spc'
    sta char_value
    jsr char_2_txtRAM
    lda ins_mode
    beq :+
    jsr del_shift_txtRAM
   :jmp output
    ;
@fwd_delkey:
    cmp #$19    ; $19=FWD DEL key:
    bne @endkey
    lda ins_mode
    beq @spc      ; bra to space obove
    jsr del_shift_txtRAM
    jmp output
@endkey:
    cmp #$04    ; $04=end key:
    bne @F1key
    lda max_char_p_line_mone
    sta crsr_posx
    jsr move_cursor
    stz crsr_offtimer
    jmp main_loop
@F1key:
    cmp #$85    ; $85=F1 key:
    bne @F2key
    lda #$01
    jsr setup_menu
    jmp output
@F2key:
    cmp #$89    ; $89=F2 key: save text file
    bne @F3key
    lda #$02
    jsr setup_menu
    bra @out
@F3key:
    cmp #$86    ; $86=F3 key: delete text buffer
    bne @F4key
    jsr pageSPR_startswipeout
    lda #$03
    jsr setup_menu
    bra @out
@F4key:
    cmp #$8A    ; $8A=F4 key: toggles mode
    bne @F5key
    jsr save_page_settings  ; speichert UND versorgt ZP_PTR_5 mit actual_usrtxt pointer
    ldy #$01
    bbs0 half_mode, :+
    lda #$01
    sta (ZP_PTR_5),y
    jsr pageSPR_startswipeout
    jmp restore_close
    ;
   :lda #$00
    sta (ZP_PTR_5),y
    jsr pageSPR_startswipeout
    jmp restore_close
@F5key:
    cmp #$87    ; $87=F5 key:       start sequence
    bne @F6key;@keyend
    stz menu_mode
    jsr start_seq
    bra @out
@F6key:
    cmp #$8B    ; $8B=F6 key: sequence menu
    bne @F7key
    lda #$06
    jsr setup_menu
    bra @out
@F7key:
    cmp #$88    ; $88=F7 key:       step time
    bne @F8key
    lda #$07
    jsr setup_menu
    bra @out
@F8key:
    cmp #$8C    ; $8C=F8 key:
    bne @F9key
    lda #$08        ; options menu
    jsr setup_menu
    bra @out
@F9key:
    cmp #$10    ; $10=F9 key: random menu
    bne @F10key
    lda #$09
    jsr setup_menu
    bra @out
@F10key:
    cmp #$15    ; $15=F10 key: toggle cursor auto mode
    bne @F11key
    lda #$0A ; cursor menu
    jsr setup_menu
    bra @out
@F11key:
    cmp #$16    ; $16=F11 key: color menu
    bne @text
    lda #$0B ; color menu
    jsr setup_menu
    jsr pageSPR_startswipeout
@out:
    jmp output
;================
@text:
    jmp enter_char
;================================================================
.endif
