.ifndef DOTINTRO_INC
DOTINTRO_INC = 1
.include "logo.inc"
;===================================================;
;   intro has it's own main loop + irg routine      ;
;   called as sub routine                           ;
;===================================================;
itxt0:  ; "commander X16"
.byte $63,$6F,$6D,$6D,$61, $6E,$64,$65,$72,$20
.byte $58,$31,$36,$20,$00
itxt1:  ; "DOT MATRIX"
.byte $44,$4F,$54,$20,$4D, $41,$54,$52,$49,$58
.byte $00,$00,$00,$00,$00
itxt2:  ; scrolltext...
.byte $20,$2B,$2B,$2B,$20, $46,$34,$3A,$20,$65
.byte $6E,$74,$65,$72,$20, $34,$30,$2A,$32,$30
.byte $20,$6D,$6F,$64,$65, $20,$2B,$2B,$2B,$20
.byte $46,$31,$3A,$20,$6D, $65,$6E,$75,$20,$2B
.byte $20,$65,$6E,$74,$65, $72,$20,$32,$30,$2A
.byte $31,$30,$20,$6D,$6F, $64,$65,$20,$2B,$2B
.byte $2B,$20,$46,$33,$3A, $20,$72,$65,$73,$74
.byte $61,$72,$74,$20,$69, $6E,$74,$72,$6F,$20
.byte $2B,$2B,$2B,$20,$61, $6C,$6C,$20,$6F,$74
.byte $68,$65,$72,$20,$6B, $65,$79,$73,$3A,$20
.byte $65,$6E,$74,$65,$72, $20,$32,$30,$2A,$31
.byte $30,$20,$6D,$6F,$64, $65,$20,$2B,$2B,$2B
.byte $20,$20,$00,$00,$00, $00,$00,$00,$00,$00
; 13 zeilen @ 10 chars = d130;
;
; ZP_tmp_DATA3 bit7:  1, when upper animation "DOT MATRIX" has completed
; ZP_tmp_DATA3 bit6:  1, when lower animation "commander X16" has completed
ticks_div:          .byte $00
scroll_strt_delay:  .byte $00
logo_strt_delay:    .byte $00
;========================
make_intro_txt:
    ldx #$28
    ldy #$00
    ;
   :jsr get_rand
    lsr
    sta (ZP_PTR_3),y
    lda #$00
    sta (ZP_PTR_2),y ; zero rnd robb cnt
    iny
    jsr get_rand
    lsr
    sta (ZP_PTR_3),y
    lda #$00
    sta (ZP_PTR_2),y ; zero rnd robb cnt
    iny
    dex
    dex
    bne :-
    rts
    ;
;========================
.macro copy_i2txt2systxt ; hi + $02
    lda #<systxt0           ; used for scroll line animation
    clc
    adc max_char_p_line     ; text starts not @ begin of systext0 so
    ;                       ; the scroll line routine needs to push in text first.
    sta ZP_PTR_1            ; target pointer
    clc
    lda #>systxt0
    adc #$02
    sta ZP_PTR_1+1          ; systext uses the area beyond systext+$0200
    ;
    ldx #$80                ; copy 128 byte
    ldy #$00
   :lda itxt2,y
    sta (ZP_PTR_1),y    ; target
    iny
    dex
    bne :-
    lda #<systxt0
    sta ZP_PTR_1
.endmacro
;========================
.macro pop_ilayer1  ; similar like 'populate_layer1' used for only partial tile fill
                    ; other than done with setup routine
; layer 1 map uses 16x16pix @8bpp tiles, map size is 64x32 tiles in normal mode
    stz VERA_ctrl
    lda #$10        ; stride = 1
    sta VERA_addr_bank
    lda #$41
    sta VERA_addr_high
    lda #$80    ; d128
    sta VERA_addr_low
    lda #$0A    ; (amber) square on black
    sta cheat_useframe
    first_row
    second_row
    third_row
.endmacro
;========================
.macro single_ichar
    lda Z_pointer_hi
    sta char_VRAM_adr_hi
    lda Z_pointer_lo
    sta char_VRAM_adr_lo
    ;
    lda (ZP_PTR_3),y
    sta char_value
    jsr write_singlechar
.endmacro
;===========================================================
; ZP_PTR_3 actual systext written
; ZP_PTR_1 compare text
; ZP_PTR_2 round robin counters
;
; systxt0 wird mit in den ersten 16 byte  mit zufalss zahlen belegt, dient als ausgabe für animation
; systxt0 + $0100 = round robin zähler
; systxt0 + $0200 ff = scrolltext
;===========================================================
start_intro:
    ;
    lda #$1E    ;rot auf sw
    sta npreset  ; attention: preset counts in steps of $06 only
    jsr get_presetcol
    jsr set_normal_mode
    ;
restart_intro:
    stz scroll_strt_delay
    stz logo_strt_delay
    stz ZP_tmp_DATA3    ; bit 7 = upper animation done, bit 6 = lower animation done
    jsr logo_switchoff

    jsr cursor_switchoff
    jsr delete_systxt
    jsr write_txtpage
    copy_i2txt2systxt
    pop_ilayer1
    lda #$60
    sta cheat_col
    ;
    lda #<systxt0
    sta ZP_PTR_3    ; text to use
    sta ZP_PTR_2
    lda #>systxt0
    sta ZP_PTR_3+1
    clc
    adc #$01
    sta ZP_PTR_2+1  ; rnd robb cnt $100 offset in systext
    ;
    jsr make_intro_txt
    ;
    sei
    lda #<intro_irq_handler
    sta IRQVec
    lda #>intro_irq_handler
    sta IRQVec+1
    lda #VSYNC_BIT ; make VERA only generate VSYNC IRQs
    sta VERA_ien
    cli
    ;
;=========================================================================
; "start_intro" is called as sub routine, so exit intro_mainloop with rts!
@intro_mainloop:    ; does keyboard
    wai
    ;
    jsr GETIN
    beq @intro_mainloop
;@F1key:
    cmp #$85    ; $85=F1 key:
    bne :+      ; @F3key
    lda #$01
    sta menu_mode
    rts
;@F3key:
   :cmp #$86    ; $86=F3 key: restart intro
    bne :+      ; @F4 key
    sei
    jmp restart_intro
;@F4key:
   :cmp #$8A    ; $8A=F4 key: enter 40*20 mode
    bne @anykey
    jsr set_allpages2half_mode
@anykey:
    jsr logo_switchoff
    rts
;=========================================================================
; irq routines:
;============================================================
inc_cmp_txt:
    lda (ZP_PTR_2),y    ; rnd robb cnt
    cmp #$01
    bne :+
    lda (ZP_PTR_3),y    ; char value
    cmp (ZP_PTR_5),y
    bne :+
    rts
    ;
   :lda (ZP_PTR_3),y    ; char value
    cmp #$7F
    bne :+
    lda #$20
    sta (ZP_PTR_3),y
    lda (ZP_PTR_2),y    ; rnd robb cnt
    inc
    sta (ZP_PTR_2),y
    rts
    ;
   :lda (ZP_PTR_3),y    ; inc char value
    inc
    sta (ZP_PTR_3),y
    rts
    ;
;============================================================
animate:
    lda Z_index
    sta Z_pointer_lo    ; +$08 per digit offset
    ldy ZP_tmp_DATA
    jsr inc_cmp_txt
    single_ichar
    inc ZP_tmp_DATA
    clc
    lda Z_index
    adc #$08
    sta Z_index
    dec ZP_tmp_DATA2
    bne animate
    ;
    rts
;============================================================
check_ani_done:
    ldx ZP_tmp_DATA2
    dec ZP_tmp_DATA2
    ;
    ldy #$00
   :lda (ZP_PTR_3),y    ; char value
    cmp (ZP_PTR_5),y
    bne :++
    cpy ZP_tmp_DATA2
    bne :+
    lda #$01
    rts
   :iny
    dex
    bne :--
   :lda #$00
    rts
    ;
;============================================================
.macro do_scrollline
    lda cheat_col
    sta ZP_tmp_DATA2
    ;
    lda #$36            ; $36 = lowest line
    sta Z_pointer_hi    ; hi+$06 = line offset
    ;
    lda #$00            ; lo+$08 = digit offset
    sta Z_pointer_lo    ; lo+$08 = digit offset
    lda #$A0
    sta cheat_col
    lda ZP_PTR_1
    sta ZP_PTR_3    ; text to use
    lda ZP_PTR_1+1
    sta ZP_PTR_3+1
    jsr write_txtline
    ;
    inc ZP_PTR_1
    lda ZP_PTR_1
    cmp #$9A ; $8C gapless
    bne :+
    lda #<systxt0
    sta ZP_PTR_1
   :lda #<systxt0
    sta ZP_PTR_3    ; text to use
    lda #>systxt0
    sta ZP_PTR_3+1
    lda ZP_tmp_DATA2
    sta cheat_col
    ;
.endmacro
;============================================================
intro_irq_handler:
    lda VERA_isr
    and #VSYNC_BIT
    bne @cont_intro
    jmp (default_irq_vector)    ; continue IRQ handler in case no VSYNC IRQ
    ;
@cont_intro:
    inc evertick
    lda ticks_div
    cmp #$04
    bne @cont_la
    stz ticks_div
    lda scroll_strt_delay
    cmp #$14
    bmi @cnt_la
    do_scrollline
    bra @cont_la
@cnt_la:
    inc scroll_strt_delay
    ;
@cont_la:
    bbr6 ZP_tmp_DATA3, @cont_ticks  ; check for ldone

    lda logo_on
    bne @cont_ticks
    ;
    inc logo_strt_delay
    lda logo_strt_delay
    cmp #$3F
    bmi @cont_ticks
    jsr logo_switchon
    ;
@cont_ticks:
    lda ticks
    cmp #$03        ; anispeed
    bne @new_tick   ; ticks not elapsed
    stz ticks       ; reset ticks
    inc ticks_div
    ;
    bbs7 ZP_tmp_DATA3, @lower_animation ; upper animation done, go to next
    ;
;@upper animation
    lda #$06
    sta Z_pointer_hi            ; hi+$06 = line offset
    lda #$20                    ; lo+$08 = digit offset
    sta Z_index
    stz ZP_tmp_DATA
    lda #<itxt1
    sta ZP_PTR_5
    lda #>itxt1
    sta ZP_PTR_5+1
    lda #$0A
    sta ZP_tmp_DATA2    ; loop cnt
    jsr animate
    ;
;check for done
    lda #$0A
    sta ZP_tmp_DATA2    ; loop cnt
    jsr check_ani_done
    bne :+
    jmp (default_irq_vector) ; continue IRQ handler
   :smb7 ZP_tmp_DATA3   ; udone
    jsr make_intro_txt
    lda #$40
    sta cheat_col
    bra @cont_def_irq   ; continue IRQ handler
;============
@new_tick:
    inc ticks
    bra @cont_def_irq   ; continue IRQ handler
;============
@lower_animation:
    bbs6 ZP_tmp_DATA3, @cont_def_irq; :+   ; ldone
    ;
    lda #$2A
    sta Z_pointer_hi            ; hi+$06 = line offset
    lda #$18
    sta Z_index
    stz ZP_tmp_DATA     ; y-reg ersatz
    lda #<itxt0
    sta ZP_PTR_5
    lda #>itxt0
    sta ZP_PTR_5+1
    lda #$0D
    sta ZP_tmp_DATA2    ; loop cnt
    jsr animate
    ;
;check for done
    lda #$0D
    sta ZP_tmp_DATA2    ; loop cnt
    jsr check_ani_done
    bne :+
@cont_def_irq:
    jmp (default_irq_vector) ; continue IRQ handler
   :smb6 ZP_tmp_DATA3   ; ldone
    bra @cont_def_irq   ; continue IRQ handler
;===================================
.endif
