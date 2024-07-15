.ifndef DOTMENU_INC
DOTMENU_INC  = 1
;================================================
.include "menutxt.inc"
.include "dotmenukeys.inc"
;================================================
menu_mode:              .byte $00   ; 0 = off, >0 = menu no.
select_onoff:           .byte $00
;================================================================
.macro count_chars    ; used to find out the needed no. of menu doublebar chars
    ldy #$00
@count:
    lda (ZP_PTR_1),y
    beq @Ende
    iny
    cpy #$0F
    bmi @count
    dey
@Ende:
    sty ZP_tmp_DATA ; counted chars, max. $0F
.endmacro
;================================================================
setup_menu:
    sta menu_mode
    bne :+  ; exit when setup_menu = 0
    rts     ; run close_menu (no sub routine!) to exit menu_mode
    ;
   :jsr cursor_switchoff
    jsr vlineDemo_switchoff
    ;
    jsr delete_systxt
    jsr logo_switchoff
    ;
    stz char_value  ; cancel the character that took us in here
    ;                 might be otherwise processed as next F-key
    stz post_ins
    stz onoff_indent_hi
    ;
    lda menu_mode
;@m1:
    cmp #$01    ; menu 1 page 1/3
    bne @m2
    ldx #<proc_menu1keys
    ldy #>proc_menu1keys
    jsr insert_ESC
    ;
    bbs0 half_mode, :+
    ldx #<menutxt1
    ldy #>menutxt1
    jmp scopy
    ;
   :ldx #<menutxt1h
    ldy #>menutxt1h
    jmp scopy
    ;
@m2:
    cmp #$02    ; menu 2
    bne @m3
    ;
    ldx #<proc_menu2keys
    ldy #>proc_menu2keys
    jsr insert_ESC
    ;
    ldx #<menutxt2
    ldy #>menutxt2
    jmp scopy
    ;
@m3:
    cmp #$03    ; menu 3
    bne @m4
    smb4 post_ins
    ;
    ldx #<proc_menu3keys
    ldy #>proc_menu3keys
    jsr insert_ESC
    ;
    bbs0 half_mode, :+
    lda #$1E
    sta single_line_indent
    ldx #<menutxt3
    ldy #>menutxt3
    bra :++
   :lda #$5D
    sta single_line_indent
    ldx #<menutxt3h
    ldy #>menutxt3h
   :jmp scopy
    ;
@m4:
    cmp #$8A    ; menu 4   invalid  not used !!!!!!!!!!!
    bne @m6
    ;
@m6:
    cmp #$06        ; sequence menu
    bne @m7
    ;
    ldx #<proc_menu6keys
    ldy #>proc_menu6keys
    jsr insert_ESC
    lda seq_show_pgnr
    sta select_onoff
    smb7 post_ins
    ;
    lda seq_show_aniSPR_latch
    beq :+
    jsr seqanidemoSPR_switchon
    bra :++
   :jsr seqanidemoSPR_switchoff
    ;
   :bbs0 half_mode, :+
    lda #$A8
    jsr VERA_addr_to_seqanidemoYLO
    lda #$23
    sta onoff_indent_lo
    ;
    ldx #<menutxt6
    ldy #>menutxt6
    jmp scopy
    ;
   :lda #$70
    jsr VERA_addr_to_seqanidemoYLO
    lda #$9C
    sta onoff_indent_lo
    ;
    ldx #<menutxt6h
    ldy #>menutxt6h
    jmp scopy
    ;
@m7:
    cmp #$07    ; menu 7 step time
    bne @m8
    ldx #<proc_menu7keys
    ldy #>proc_menu7keys
    jsr insert_ESC
    smb6 post_ins   ; pgnr_onoff
    smb5 post_ins   ; secs_onoff
    ;
    bbs0 half_mode, :+
    ;
    lda #$1E;32
    sta single_line_indent
    lda #$34
    sta secs_line_indent
    ;
    ldx #<menutxt7
    ldy #>menutxt7
    jmp scopy
    ;
   :lda #$32;5A
    sta single_line_indent
    lda #$62
    sta secs_line_indent
    ;
    ldx #<menutxt7h
    ldy #>menutxt7h
    jmp scopy
@m8:
    cmp #$08    ; menu 8 options
    bne @m9
    ldx #<proc_menu8keys
    ldy #>proc_menu8keys
    jsr insert_ESC
    lda show_whitespc
    sta select_onoff
    smb7 post_ins
    stz onoff_indent_hi
    ;
    bbs0 half_mode, :+
    jsr setlogo
    lda #$4A
    sta onoff_indent_lo
    ldx #<menutxt8
    ldy #>menutxt8
    jmp scopy
    ;
   :lda #$B8
    sta onoff_indent_lo
    ldx #<menutxt8h
    ldy #>menutxt8h
    jmp scopy
    ;
@m9:
    cmp #$09    ; menu 9 random
    bne @mA
    ldx #<proc_menu9keys
    ldy #>proc_menu9keys
    jsr insert_ESC
    ;
    bbs0 half_mode, :+
    ldx #<menutxt9
    ldy #>menutxt9
    jmp scopy
   :ldx #<menutxt9h
    ldy #>menutxt9h
    jmp scopy
@mA:
    cmp #$0A    ; menu 10 cursor menu
    bne @mB
    lda #$11
    sta crsr_posx
    lda #$05
    sta crsr_posy
    jsr move_cursor
    jsr cursor_switchon
    ;
    ldx #<proc_menuAkeys
    ldy #>proc_menuAkeys
    jsr insert_ESC
    smb7 post_ins
    ;
    bbr1 crsrstatus, @sel_auto
    lda #01         ; "on"
    sta select_onoff
    bra :+
    ;
@sel_auto:
    lda #02         ; "auto"
    sta select_onoff
    ;
   :bbs0 half_mode, :+
    jsr set_vlineDemo_n_pos
    lda #$40
    bra :++
   :jsr set_vlineDemo_h_pos
    lda #$7C
   :sta onoff_indent_lo
    jsr vlineDemo_switchon
    ldx #<menutxtA
    ldy #>menutxtA
    jmp scopy
    ;
@mB:
    cmp #$0B    ; menu 11 color menu
    bne @mE
    ;
    ldx #<proc_menuBkeys
    ldy #>proc_menuBkeys
    jsr insert_ESC
    ;
    bbs0 half_mode, :+
    ldx #<menutxtB
    ldy #>menutxtB
    jmp scopy
   :ldx #<menutxtBh
    ldy #>menutxtBh
    jmp scopy
    ;
@mE:
    cmp #$0E    ; menu 1 page 2/3
    bne @mF
    ldx #<proc_menuEkeys
    ldy #>proc_menuEkeys
    jsr insert_ESC
    bbs0 half_mode, :+
    ldx #<menutxtE
    ldy #>menutxtE
    jmp scopy
   :ldx #<menutxtEh
    ldy #>menutxtEh
    jmp scopy
@mF:
    cmp #$0F    ; menu 1 page 3/3 ascii char.set
    beq :+
    jmp close_menu
    ;
   :ldx #<proc_menuFkeys
    ldy #>proc_menuFkeys
    jsr insert_ESC
    ;
    bbs0 half_mode, :+
    lda #$16
    bra :++
   :lda #$AC
   :sta chrset_inc
    write_charset2systxt
    ;
    ldx #<menutxtF
    ldy #>menutxtF
    ; keep on going
;=================== copys text to systxtRAM
scopy:
    jsr set_PTR_1
    lda #<systxt0   ; target
    sta ZP_PTR_4
    lda #>systxt0
    sta ZP_PTR_4+1
    ;
    count_chars
    jsr insert_menubar
    ;
@page_loop:
    ldx max_char_p_line
    jsr single_line_cpy
; ;==========
; @next_line:
; if there is a next line
    cmp #$04    ; EOF
    bne :+
    bra @post_text  ; exit on EOF
    ;
   :jsr inc_PTR_1
    lda (ZP_PTR_1)

    cmp #$04    ; EOF
    bne :+
    bra @post_text
    ;
   :jsr add_line_2_ZP4
    bra @page_loop
    ;
@post_text:     ; test for some extra features
    lda post_ins
    beq :+
    jsr ins_onofftxt2systxt
    jsr ins_pagenr2systxt
    jsr ins_secs2systxt
    jmp ins_insert2systxt   ; use rts of ins_insert...
   :rts
    ;
;==============================
close_menu: ; as part of the keyboard routine no rts !
    stz menu_mode
    ;
    jsr seqanidemoSPR_switchoff
    jsr vlineDemo_switchoff
    jsr default_keyvec
    ;
    jsr cursor_home
    jsr cursor_switchon
    jsr pageSPR_startswipeout
    jmp output
;================================================================
insert_ESC:
    stx use_key_vector
    sty use_key_vector+1
    bbs0 half_mode, :+
    ;
    lda #(<systxt0 + $B4)  ; target
    sta ZP_PTR_4
    lda #>systxt0
    sta ZP_PTR_4+1
    bra :++
    ;
   :lda #(<systxt0 + $F8)  ; target
    sta ZP_PTR_4
    lda #(>systxt0 + $02)
    sta ZP_PTR_4+1
    ;
   :ldx #<menutxtESC  ; source ; "F1 help"
    ldy #>menutxtESC
    jsr set_PTR_1
    ;
    ldx #$08    ; max char for single line copy routine
    jmp syscpy_loop
    ;
;================================================================
half_menubar:
    lda #$82
    ldx ZP_tmp_DATA2
    dex
   :sta (ZP_PTR_4),y    ; write double line
    iny
    dex
    bne :-
    rts
;
insert_menubar:
    sec
    lda max_char_p_line ; max chars per line
    sbc ZP_tmp_DATA     ; subtract chars of title
    lsr                 ; div. by 2 (space left and right)
    sta ZP_tmp_DATA2
    ;
    ldy #$00
    lda #$80
    sta (ZP_PTR_4),y    ; write left opening bracket
    iny
    ;
    jsr half_menubar
    jsr l_loop          ; write menu title text
    jsr half_menubar
    ;
    lda #$81
    sta (ZP_PTR_4),y    ; write right closing bracket
    ;
    jsr inc_PTR_1           ; inc source pointer since exit '$00' wasn't counted
    jmp add_line_2_ZP4      ; use rts of add_line...
    ;
;================================================================
.endif
