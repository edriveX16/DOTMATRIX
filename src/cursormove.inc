.ifndef CURSORMOVE_INC
CURSORMOVE_INC  = 1
;==========================================================================================
; some functions for moving the cursor
; switching the cursor on / off
; further function applie to the "erase line" aka vline
;==========================================================================================
; joystick_state:      .byte $00
; joystick_latch:      .byte $CF
; line_SPR_xposlo:        .byte $00
; line_SPR_xposhi:        .byte $00
; zp_line_SPR_xposlo  = $42
; zp_line_SPR_xposhi  = $43
; digit_inc_cnt       = $53
; dot_inc_cnt         = $54
; del_rowptr          = $57
; old_oct             = $58
;
crspr_posx_lo:       .byte $00,$20,$40,$60,$80,$A0,$C0,$E0           ; digit 0-7 hi=0, 8-15 hi=1, 16-19 hi =2
crspr_posy_lo:       .byte $28,$58,$88,$B8,$E8,$18,$48,$78,$A8,$D8   ; digit >= 5 -> hi=1
;
hcrspr_posy_lo:      .byte $14,$2C,$44,$5C,$74,$8C,$A4,$BC,$D4,$EC,  $04,$1C,$34,$4C,$64,$7C,$94,$AC,$C4,$DC ; scheint fuer nicht insert okay zu sein
;                                                                    ; digit 0-A hi=0, B-19 hi=1
;
; neue overwrite y_Werte:
ov_crspr_posy_lo:    .byte $22,$52,$82,$B2,$E2,$12,$42,$72,$A2,$D2   ; digit >= 5 -> hi=1
h_ov_crspr_posy_lo:  .byte $0E,$26,$3E,$56,$6E,$86,$9E,$B6,$CE,$E6,  $FE,$16,$2E,$46,$5E,$76,$8E,$A6,$BE,$D6
;                                                                    ; digit 0-B hi=0, C-19 hi=1
;====================================
cursor_switchon:
   jsr move_cursor      ; adjust position
   jsr VERA_addr_to_cursorZ
   lda ins_mode         ; ggf. v flip
   eor #$01
   asl
   ora #$08             ; z on
   sta VERA_data0
   ;
   bbs0 half_mode, :+
   sta VERA_data1
   ;
  :smb0  crsrstatus  ; set bit 0 = crsr_on  /on
   stz crsr_offtimer    ; restart cursor blanking timer
   rts
   ;
cursor_switchoff:
   jsr VERA_addr_to_cursorZ
   stz VERA_data0
   stz VERA_data1
   rmb0  crsrstatus  ; reset bit 0 = crsr_on /off
   rts
   ;
;====================================
cursor_incposx:
   lda crsr_posx
   cmp max_char_p_line_mone
   bmi :++
   lda crsr_posy
   cmp max_line_no_mone
   bmi :+
   rts
   ;
  :inc crsr_posy
   stz crsr_posx
   rts
   ;
  :inc crsr_posx
   rts
   ;
;================
cursor_decposx:
   lda crsr_posx
   beq :+
   dec crsr_posx
   rts
   ;
  :lda crsr_posy
   beq :+
   dec crsr_posy
   lda max_char_p_line_mone
   sta crsr_posx
  :rts
   ;
;================
cursor_home:
   stz crsr_posx
   stz crsr_posy
   jsr move_cursor
   stz crsr_offtimer
   rts
;==============================================================
move_cursor:
   stz VERA_ctrl
   lda #$11    ; sprite #60
   sta VERA_addr_bank
   lda #$FD
   sta VERA_addr_high
   lda #$F2    ; incl. positon offset
   sta VERA_addr_low
   ;
   bbs0 half_mode, @move_hcursor
   ; 2nd cursor sprite #61 only in normal mode 20x10
   lda #$01
   sta VERA_ctrl
   lda #$11    ; sprite #61
   sta VERA_addr_bank
   lda #$FD
   sta VERA_addr_high
   lda #$FA    ; incl. positon offset
   sta VERA_addr_low


   ; calc cusror-sprite screen coords
   lda #<crspr_posx_lo  ; start of look up array
   sta ZP_PTR_2
   lda #>crspr_posx_lo
   sta ZP_PTR_2+1
   ;
   ldx #$00
   lda crsr_posx
   cmp #$08
   bmi :+      ; smaller than 8 -> hi=0
   inx
   sec
   sbc #$08
   cmp #$08
   bmi :+   ; larger than 8 but smaller than 16 -> hi =1
   inx      ; larger than 16 (up to 32-1) -> hi =2
   sec
   sbc #$08
   ;
  :tay         ; smaller than 8
   lda (ZP_PTR_2),y
   sta VERA_data0
   sta VERA_data1
   txa
   sta VERA_data0 ; x hi pos
   sta VERA_data1
    ;
;@calc_ypos:
   lda ins_mode
   bne :+

   lda #<ov_crspr_posy_lo  ; start of look up array
   sta ZP_PTR_2
   lda #>ov_crspr_posy_lo
   sta ZP_PTR_2+1

   bra :++

  :lda #<crspr_posy_lo  ; start of look up array
   sta ZP_PTR_2
   lda #>crspr_posy_lo
   sta ZP_PTR_2+1
   ;
  :ldx #$00
   lda crsr_posy
   cmp #$05
   bmi :+
   inx
   ;
  :tay
   lda (ZP_PTR_2),y
   sta VERA_data0
   inc
   inc
   sta VERA_data1
   txa
   sta VERA_data0 ; y hi pos
   sta VERA_data1
   rts
   ;
;=========
@move_hcursor:
; calc cusror-sprite screen coords
   ldx #$00
   lda crsr_posx
   cmp #$10
   bmi :+   ;+  ; smaller than 16  -> hi 0
; calc cusror-sprite screen coords
   inx
   sec
   sbc #$10
   cmp #$10
   bmi :+   ; smaller than 32 -> hi =1
   ;
   inx      ; larger than 32 -> hi =2
   sec
   sbc #$10
   ;
  :asl      ; smaller than 32 -> hi =1
   asl
   asl
   asl
   sta VERA_data0 ;xpos lo
   txa
   sta VERA_data0 ;xpos hi
   ;
;@calc_hypos:
   ldx #$00
   lda crsr_posy
   tay
   lda ins_mode
   beq :+
   ;lda crsr_posy
   tya
   cmp #$0A
   ;cmp #$0B
   bmi :+      ; smaller than 10  -> hi 0
   inx
   bra :++
   ;
  :tya
   cmp #$0B
   bmi :+      ; smaller than 10  -> hi 0
   inx
  :lda ins_mode
   bne :+
   ;
   lda #<h_ov_crspr_posy_lo
   sta ZP_PTR_2
   lda #>h_ov_crspr_posy_lo
   sta ZP_PTR_2+1
   bra :++
   ;
  :lda #<hcrspr_posy_lo  ; start of look up array
   sta ZP_PTR_2
   lda #>hcrspr_posy_lo
   sta ZP_PTR_2+1
   ;
  :lda (ZP_PTR_2),y
   sta VERA_data0 ;ypos lo
   txa
   sta VERA_data0
   rts
   ;
;===================================================
vline_off:
   ldx #$00
   stz vlineSPR_on
   bra vline_writeZ
vline_on:
   lda #$01
   sta vlineSPR_on
   ldx #$08
;sprites_on:
vline_writeZ:
   stz VERA_ctrl
   lda #$41   ;stride = 8
   sta VERA_addr_bank
   lda #$FC
   sta VERA_addr_high
   lda #$16
   sta VERA_addr_low
   ;
   txa
   ldx #$08
  :sta VERA_data0  ; sprite Z
   dex
   bne :-
   rts
   ;
;===================================================
reset_vline:
   stz zp_line_SPR_xposlo
   stz zp_line_SPR_xposhi
   lda #$05    ; prepare for next run
   sta dot_inc_cnt
   stz digit_inc_cnt
   jmp set_vlineSPRxpos ; use rts of set_vlineSPRxpos
   ;
;===================================================
line_SPR_stepx:   ; moves that vertical line one step right at clr screen
; part1: inc xpos
   lda digit_inc_cnt
   cmp #$13
   bmi :+
   lda dot_inc_cnt
   beq @endstep
  :clc
   lda zp_line_SPR_xposlo
   adc #$06
   sta zp_line_SPR_xposlo
   bcc :+
   inc zp_line_SPR_xposhi
  :dec dot_inc_cnt
   bne @endstep
   ;
   lda digit_inc_cnt
   cmp #$13
   beq @endstep
   ;
   lda #$05
   sta dot_inc_cnt
   clc
   lda zp_line_SPR_xposlo
   adc #$02
   sta zp_line_SPR_xposlo
   bcc :+
   inc zp_line_SPR_xposhi
  :inc digit_inc_cnt
@endstep:
   lda zp_line_SPR_xposhi
   cmp #$02
   bmi set_vlineSPRxpos
   lda zp_line_SPR_xposlo
   cmp #$7A       ; $027A = approx. 640 pix
   bmi set_vlineSPRxpos
   stz vlineSPR_on
   jsr vline_off
   jsr reset_vline
   stz del_rowptr
   stz old_oct
   jsr delete_txtRAM
   jsr cursor_switchon
;=======
; part2: distribute new xpos to sprites 2-10
set_vlineSPRxpos:
   stz VERA_ctrl
   lda #$11   ;stride = 1
   sta VERA_addr_bank
   lda #$FC
   sta VERA_addr_high
   lda #$12  ; $10 + $02
   sta VERA_addr_low
   ;
   ldx #$08
; position
  :lda zp_line_SPR_xposlo
   sta VERA_data0  ; xpos lo
   lda zp_line_SPR_xposhi
   sta VERA_data0  ; xpos hi
   ;
   clc
   lda VERA_addr_low
   adc #$06
   sta VERA_addr_low
   ;
   dex
   bne :-
;=======
; part3: when vline cursor proceeds >8 pix then delete vertical bg row in map0
   bbr0 half_mode, :+
   stz cheat_col  ; cheat_col only in normal mode, must be 00 in half_mode
  :lda zp_line_SPR_xposlo
   lsr
   lsr
   lsr
   bit old_oct
   sta old_oct
   bne @del_row   ;:+
   rts
   ;
@del_row:
   stz VERA_ctrl
   lda #$90  ; stride = 256
   sta VERA_addr_bank
   stz VERA_addr_high
   lda del_rowptr
   asl
   sta VERA_addr_low
   ;
   lda cheat_col
   ldx #$00
  :sta VERA_data0 ; Tile index
   inx
   cpx #$3C       ; $3C = d60
   bmi :-
   ;
   inc del_rowptr
   rts
   ;
;===================================================
; ask_cursorkeys:
;   ; rts
; ;   jsr JOYSTICK_SCAN ; only when IRQ completely replaced
; ;   lda #$01 ; select joystick #1
; ;   jsr JOYSTICK_GET
; ;   cpy #$00  ; y = 0 when joystick present
; ;   beq @check_buttons
;
; ;====================================================
;    lda #$00       ; select joystick #0 = keyboard joystick
;    jsr JOYSTICK_GET
;    ;
;    and #$0F
;    eor #$0F
;    sta zpreq_crsmove
;    ;
;    rts
;===================================================
.endif
