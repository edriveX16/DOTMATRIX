.ifndef IRQ_HANDLER_INC
IRQ_HANDLER_INC = 1
;===========================================================================================
; Attention: there are some more IRQ_handler routines spread around this program
; depending on modes they may be swapped out or in.
; e.g. intro, page indicator...
;===============================================================================;
dotmatrix_irq_handler:              ; main irq handler
    lda VERA_isr
    and #VSYNC_BIT
    bne swipes ;@runsync ; non-VSYNC IRQ, no tick update
    jmp (default_irq_vector) ; continue IRQ handler
    ;
swipes:
    inc evertick
    jmp sequencer_irq_handler
    ;
cont_fromseqirq:
    lda vlineSPR_on
    beq @runsync
    jsr line_SPR_stepx
    bra @long_end_IRQ_handler
    ;
@runsync:   ; custom IRQ handling
    inc crsr_wait
    lda ticks
    cmp #$1C;#$27   ; ticks sub divider
    bne @next_tick
    ;
    stz ticks       ; reset ticks
    ;
; blink cursor
    bbr0 crsrstatus, @endtiming ; Bit0 = 1:crsr_on
    stz VERA_ctrl
    jsr VERA_addr_to_cursorZ
    lda VERA_data0  ; look whats inside z, no stride needed
    eor #$08        ; toggle cursor z bit
    sta VERA_data0  ; write it back
    ;
    bbs0 half_mode, :+
    ;
    sta VERA_data1  ; write it back
    ;
   :bbs1 crsrstatus, @endtiming  ; Bit1 = 0 crsr_auto, 1= on
    ;
    lda crsr_offtimer
    cmp #$11
    bmi @inc_offtimer
    jsr cursor_switchoff
    bra @endtiming
    ;
@long_end_IRQ_handler:
    bra @end_IRQ_handler
@inc_offtimer:
    inc crsr_offtimer
@next_tick:
    inc ticks
@endtiming:
    lda menu_mode   ; don't do cursor (moves) when in menu
    bne @chk_halfmode;@checktxtonly;@newcrsrpos_done
    bbs0 seq_statusbits, @newcrsrpos_done   ; don't do cursor (moves) when sequence runs
    ;
    lda req_crsrmove
    beq @newcrsrpos_done
    bbs0 req_crsrmove, @gor  ;right
    bbs1 req_crsrmove, @gol  ;left
    bbs2 req_crsrmove, @god  ;down
    bbs3 req_crsrmove, @gou  ;up
    ;
    bra @newcrsrpos_done
@gou:   ; go up
    lda crsr_posy
    beq @newcrsrpos_done
    dec crsr_posy
    bra @crsr_refresh
@god:   ; go down
    lda crsr_posy
    cmp max_line_no_mone
    bmi @md
    bra @newcrsrpos_done
@md:
    inc crsr_posy
    bra @crsr_refresh
@gol:   ; go left
    jsr cursor_decposx
    bra @crsr_refresh
@gor:   ; go right
    jsr cursor_incposx
    bra @crsr_refresh
@newcrsrpos_done:
    stz req_crsrmove
    bra @end_IRQ_handler
;=============
@crsr_refresh:
    stz req_crsrmove
    stz crsr_wait
    jsr move_cursor
    jsr cursor_switchon
    ;
;=============
@end_IRQ_handler:
    lda half_mode
    bne :++
    lda logo_on
    cmp X16logo_on
    beq :+
    jsr setlogo
@chk_halfmode:
   :lda half_mode
   :cmp mode_last_run
    beq @checktxtonly
    ;
    jsr set_mode
    jmp (default_irq_vector) ; continue IRQ handler
@checktxtonly:
    bbr0 textrefresh, @very_end
    jsr write_txtpage
@very_end:
    jmp (default_irq_vector) ; continue IRQ handler
    ;
;===========================================================================================
.endif
