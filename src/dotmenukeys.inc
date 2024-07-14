.ifndef DOTMENUKEYS_INC
DOTMENUKEYS_INC  = 1
;================================================================
; each menu has it's own keyborad routine
;================================================================
proc_menu1keys:                     ; help menu
    lda char_value
    cmp #$1B    ; $1B=ESC key:      close menu
    bne @F1key
    jmp close_menu
@F1key:
    cmp #$85    ; $85=F1 key:       menu 2/2
    bne @F2key
;@next_menu_pg:
    lda #$0E
    jsr setup_menu
    jmp output
@F2key:
    cmp #$89    ; $89=F2 key:       file menu
    bne @F3key
    bra @defkey
@F3key:
    cmp #$86    ; $86=F3 key:       edit menu
    bne @F4key
    bra @defkey
@F4key:
    cmp #$8A    ; $8A=F4 key:       sets and toggles preset
    bne @F5key
    bra @defkey
@F5key:
    cmp #$87    ; $87=F5 key:       start sequencer
    bne @F6key
    stz menu_mode
    bra @defkey
@F6key:
    cmp #$8B    ; $8B=F6 key:       sequencer menu
    bne @F7key
    bra @defkey
@F7key:
    cmp #$88    ; $88=F7 key:       sequence step time
    bne @keyend
@defkey:
    jmp proc_defkeys
@keyend:
    jmp main_loop
;================================================================
proc_menuEkeys:                     ; help menu
    lda char_value
    cmp #$1B    ; $1B=ESC key:      close menu
    bne @pg_dwnkey
    jmp close_menu
@pg_dwnkey:
    cmp #$02
    bne @pg_upkey
    bra @defkey                ; page down
@pg_upkey:
    cmp #$82
    bne @F1key
    bra @defkey                ; page up
@F1key:
    cmp #$85    ; $85=F1 key:       ascii menu
    bne @F8key
@next_menu_pg:
    lda #$0F
    jsr setup_menu
    jmp output
@F8key:
    cmp #$8C    ; $8C=F8 key:       options menu
    bne @F9key
    bra @defkey
@F9key:
    cmp #$10    ; $10=F9 key:        random menu
    bne @F10key
    bra @defkey
@F10key:
    cmp #$15    ; $15=F10 key:      cursor menu
    bne @F11key ;@keyend
    bra @defkey
@F11key:
    cmp #$16    ; $16=F11 key:      color menu
    bne @keyend
@defkey:
    jmp proc_defkeys
@keyend:
    jmp main_loop
;=============================
proc_menu2keys:                     ; file menu
    lda char_value
    cmp #$1B    ; $1B=ESC key:      close menu
    bne @Qkey
    jmp close_menu
@Qkey:
    cmp #$51    ; $51=Q key:        reset
    bne @lqkey
    jmp reset
@lqkey:
    cmp #$71    ; $71=q key:
    bne @F1key
    jmp reset
@F1key:
    cmp #$85    ; $85=F1 key:       help menu
    bne @F2key
    jmp proc_defkeys
@F2key:
    cmp #$89    ; $89=F2 key:       save
    bne @F3key
    jsr save_file
    jmp close_menu
@F3key:
    cmp #$86    ; $86=F3 key:       load
    bne @keyend
    jsr load_file
    jmp close_menu
@keyend:
    jmp main_loop
;=============================
proc_menu3keys:                     ; edit menu
    lda char_value
    cmp #$1B    ; $1B=ESC key:      close menu
    bne @ins_key;@F1key
    jmp close_menu
@ins_key:
    cmp #$94    ; $94= insert key:
    bne @F1key  ;@returnkey
    lda ins_mode
    eor #$01
    sta ins_mode
    jsr ins_insert2systxt
    jmp output
@F1key:
    cmp #$85    ; $85=F1 key:       help menu
    bne @F3key
    jmp proc_defkeys
@F3key:
    cmp #$86    ; $86=F3 key:       delete page
    bne @F4key
    bra @del_out
@F4key:
    cmp #$8A    ; $8A=F4 key:       delete text buffer all pages
    bne @F5key
    jsr delete_allusrtxt
@del_out:
    jsr cursor_home
    jsr vline_on
    jmp close_menu
@F5key:
    cmp #$87    ; $87=F5 key:       copy text to next page
    bne @keyend
    jsr memorize_prevmode
    jsr copypage
    jsr restore_page_settings
    jsr set_vlinecol
    jsr check_refreshmode
    jmp close_menu
@keyend:
    jmp main_loop
;=============================  6
proc_menu6keys:                     ; sequencer menu
    lda char_value
    cmp #$1B    ; $1B=ESC key:      close menu
    bne @F1key
    jmp close_menu
@F1key:
    cmp #$85    ; $85=F1 key:       F1 menu
    bne @F3key
    jsr seqanidemoSPR_switchoff
    jmp proc_defkeys
@F3key:
    cmp #$86    ; $86=F3 key:       show page numbers at sequence
    bne @F4key
    lda seq_show_pgnr
    eor #$01
    sta seq_show_pgnr
    sta select_onoff
    jsr ins_onofftxt2systxt
    jmp output
@F4key:
    cmp #$8A    ; $8A=F4 key:       show seq. anim sprite
    bne @F5key
    lda seq_show_aniSPR_latch
    beq :+
    stz seq_show_aniSPR_latch
    stz seq_show_anidemoSPR
    jsr seqanidemoSPR_switchoff
    jmp main_loop
   :lda #$01
    sta seq_show_aniSPR_latch
    sta seq_show_anidemoSPR
    jsr seqanidemoSPR_switchon
    bra @keyend
@F5key:
    cmp #$87    ; $87=F5 key:       start sequence
    bne @F7key
    stz menu_mode
    jsr start_seq
    jmp output
@F7key:
    cmp #$88    ; $88=F7 key:       ; seq. timing menu
    bne @keyend
    jsr seqanidemoSPR_switchoff
    lda #$07
    jsr setup_menu
    jmp output
@keyend:
    jmp main_loop
;=============================      7
proc_menu7keys:                     ; seq. timing menu
    lda char_value
    cmp #$1B        ; $1B=ESC key:
    bne @crsr_dnkey
    jmp close_menu
    ;
@crsr_dnkey:    ; $11=left cursor key:
    cmp #$11
    bne @crsr_upkey
    lda #$00
    cmp seq_steptime
    bmi :+
    bra @renew_secondstxt
    ;
   :dec seq_steptime
    bra @renew_secondstxt
    ;
@crsr_upkey:
    cmp #$91    ; $91=left cursor key:
    bne @pg_dwnkey
    lda seq_steptime
    cmp #$77
    bmi :+
    bra @renew_secondstxt
    ;
   :inc seq_steptime
@renew_secondstxt:
    jsr ins_secs2systxt
    stz char_value
    jmp output
    ;
@pg_dwnkey:                     ; next page
    cmp #$02
    bne @pg_upkey
    ;
    jsr save_page_settings
    jsr memorize_prevmode
    jsr actual_usrtxt_inc
    bra @proc_dwnkey
    ;
@pg_upkey:                      ; previous page
    cmp #$82
    bne @F1key
    ;
    jsr save_page_settings
    jsr memorize_prevmode
    jsr actual_usrtxt_dec
@proc_dwnkey:
    jsr restore_page_settings
    jsr check_refreshmode
    jsr get_mode_values
    ;
    lda #$07 ; seq timing menu
    jsr setup_menu
    jsr pageSPR_startswipeout
    jmp output
    ;
@F1key:
    cmp #$85    ; $85=F1 key:
    bne @F5key
    jmp proc_defkeys
@F5key:
    cmp #$87    ; $87=F5 key:       start sequence
    bne @F6key
    stz menu_mode
    jsr start_seq
    jmp output
@F6key:
    cmp #$8B    ; $8B=F6 key:       sequencer menu
    bne @F10key
    jmp proc_defkeys
@F10key:
    cmp #$15    ; $15=F10 key:  make all pages make_same
    bne @keyend
    make_samesteptime
    jmp output
@keyend:
    jmp main_loop
;=============================      8
proc_menu8keys:                     ; options menu
    lda char_value
    cmp #$1B    ; $1B=ESC key:
    bne @F1key
    jmp close_menu
@F1key:
    cmp #$85    ; $85=F1 key:
    bne @F2key
    jmp proc_defkeys
@F2key:
    cmp #$89    ; $89=F2 key:       insert page nr.
    bne @F5key
    insert_page_nums
    jmp close_menu
@F5key:
    cmp #$87    ; $87=F5            show non printables
    bne @F6key
    lda show_whitespc
    eor #$01
    sta show_whitespc
    sta select_onoff
    jsr ins_onofftxt2systxt
    jmp output
@F6key:
    cmp #$8B    ; $8B=F6 key:       clock
    bne @F8key

    bra @keyend
;     jsr insert_clock
;     jmp output
    ;
@F8key:
    cmp #$8C    ; $8C=F8 key:       toggle logo
    bne @FXkey
    lda half_mode
    bne @FXkey
    lda X16logo_on
    eor #$01
    sta X16logo_on
    jsr setlogo
    jsr save_page_settings
    jmp close_menu
    ;
;===service mode===
; following keys only available in service mode
@FXkey:
    lda service_mode
    beq @keyend
    lda char_value
    cmp #$8B    ; $8B=F6 key:       restart without intro
    bne @F9key
    lda #$01
    sta no_intro
    stz service_mode
    jmp restart
@F9key:
    cmp #$10    ; $10=F9 key:       toggle layer 1
    bne @keyend ;@F9key
    lda dc_mode
    eor #$20
    sta dc_mode
    sta VERA_dc_video
    jmp close_menu
@keyend:
    jmp main_loop
;=============================      9
proc_menu9keys:                     ; random menu
    lda char_value
    cmp #$1B    ; $1B=ESC key:
    bne @F1key
    bra @clsmnu
@F1key:
    cmp #$85    ; $85=F1 key:       menu 1/2
    bne @F5key
    jmp proc_defkeys
@F5key:
    cmp #$87    ; $87=F5            random text single page
    bne @F6key
    lda #$01
    sta charonly
    stz patonly
    lda #$01
    sta ftype
    jsr random_txt
    bra @clsmnu
@F6key:
    cmp #$8B    ; $8B=F6 key:       random text all pages
    bne @F7key
    lda #$01
    sta charonly
    stz patonly
    lda #$01
    sta ftype
    jsr all_pg_random_something
    bra @clsmnu
@F7key:
    cmp #$88    ; $88=F7 key:       pattern single page
    bne @F8key
    stz ftype
    jsr rndpattern
    bra @clsmnu
@F8key:
    cmp #$8C    ; $8C=F8 key:       pattern all pages
    bne @F9key
    lda #$01
    sta patonly
    stz charonly
    stz ftype
    jsr all_pg_random_something
    bra @clsmnu
@F9key:
    cmp #$10    ; $10=F9 key:       random preset single page
    bne @F10key
    jsr memorize_prevmode
    jsr random_preset
    jsr set_vlinecol
    jsr save_page_settings
    jsr check_refreshmode
    bra @clsmnu
@F10key:
    cmp #$15    ; $15=F10 key:      random preset all pages
    bne @F11key
    jsr memorize_prevmode
    jsr random_allpreset
    jsr restore_page_settings
    jsr set_vlinecol
    jsr check_refreshmode
@clsmnu:
    jmp close_menu
@F11key:
    cmp #$16    ; $16=F11 key:      randomize everything
    bne @keyend
    jsr memorize_prevmode
    stz patonly
    stz charonly
    stz ftype
    jsr all_pg_random_something
    jsr random_allpreset
    jsr restore_page_settings
    jsr set_vlinecol
    random_steptime_logo
    stz menu_mode
    jsr start_seq
    ;
    smb1 mode_last_run          ; froces both layers to be redrawn
    jmp output
@keyend:
    jmp main_loop
;=============================
proc_menuBkeys:                 ; color menu
    lda char_value
    cmp #$1B    ; $1B=ESC key:  ; close menu
    bne @pg_dwnkey
    jmp close_menu
@pg_dwnkey:                     ; next page
    cmp #$02
    bne @pg_upkey
    jsr save_page_settings
    jsr memorize_prevmode
    jsr actual_usrtxt_inc
    bra @proc_dwnkey
@pg_upkey:                      ; previous page
    cmp #$82
    bne @F1key
    jsr save_page_settings
    jsr memorize_prevmode
    jsr actual_usrtxt_dec
@proc_dwnkey:
    jsr restore_page_settings
    jsr check_refreshmode
    jsr get_mode_values
    lda #$0B ; color menu
    jsr setup_menu
    ;
    jsr pageSPR_startswipeout
    jmp output
@F1key:
    cmp #$85    ; $85=F1 key:   F1 help menu
    bne @F4key
    jmp proc_defkeys
@F4key:
    cmp #$8A    ; $8A=F4 key:   next preset
    bne @F5key
    jmp next_preset
@F5key:
    cmp #$87    ; $87=F5 key:   toggle dot color when on
    bne @F6key
    jmp next_litecol
@F6key:
    cmp #$8B    ; $8B=F6 key:   toggle dot color when off - only in normal mode
    bne @F7key
    lda half_mode
    bne @keyend;:+
    jmp next_litebgcol
@F7key:
    cmp #$88    ; $88=F7 key:   toggle background color
    bne @F8key
    jmp next_col_frame
@F8key:
    cmp #$8C    ; $8C=F8 key:   toggle dot style (corner color) - only in normal mode
    bne @F10key
    lda half_mode
    bne @keyend
    jmp next_styl_frame
@F10key:
    cmp #$15    ; $15=F10 key:  make all pages make_same
    bne @keyend
    make_samecolors
    jmp close_menu
    ;
@keyend:
    jmp main_loop
;================================================================
proc_menuAkeys: ; cursor menu
    lda char_value
    cmp #$1B    ; $1B=ESC key:  close menu
    ;bne @F1key
    bne @ins_key;@F1key
    jmp close_menu
    ;
@ins_key:
    cmp #$94    ; $94= insert key:
    bne @F1key  ;@returnkey
    lda ins_mode
    eor #$01
    sta ins_mode
    jsr move_cursor
    jsr cursor_switchon
;    jsr ins_insert2systxt
    ;jmp output
    bra @out

@F1key:
    cmp #$85    ; $85=F1 key:   F1 help menu
    bne @F8key
    jsr cursor_home
    jmp proc_defkeys
@F8key:
    cmp #$8C    ; $8C=F8 key:   toggle cursor auto mode
    bne @F9key
    ;
    bbs1 zp_crsrstatus, @F8out
    smb1 zp_crsrstatus ; Bit1 = crsr_auto
    lda #$01    ;1 = "on"
    sta select_onoff
    jsr cursor_switchon
    jsr ins_onofftxt2systxt
    bra @out
@F8out:
    rmb1 zp_crsrstatus ; Bit1 = crsr_auto
    lda #$02    ;2 = "auto"
    sta select_onoff
    jsr ins_onofftxt2systxt
    bra @out
@F9key:
    cmp #$10    ; $10=F9 key:   toggle cursor color
    bne @F10key
    stz crsr_offtimer
    jsr toggle_crsr_col
    jsr cursor_switchon
    bra @out
@F10key:
    cmp #$15    ; $15=F10 key:  toggle vline color
    bne @otherkey
    jsr next_vlinecol
    jsr cursor_switchon
    bra @out
@otherkey:
    cmp #$00
    beq @keyend
    jsr cursor_switchon
@out:
    jmp output
    ;
@keyend:
    jmp main_loop
;=============================
proc_menuFkeys:                 ; ascii menu
    lda char_value
    cmp #$1B    ; $1B=ESC key:  close menu
    bne @F1key
    jmp close_menu
@F1key:
    cmp #$85    ; $85=F1 key:   close menu
    bne @keyend
    jmp close_menu
@keyend:
    jmp main_loop
;====================
.endif
