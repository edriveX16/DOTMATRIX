.ifndef SEQUENCER_INC
SEQUENCER_INC  = 1
;===================================================================
; seqani_del                  = $64   ; seq. runs animation sprite ticks div.
; seq_statusbits              = $65   ; bit0=seq_run, bit1=seq_pause, bit2=seq_show_pgnr,
seq_nxttimer_div:       .byte $00   ; ticks subdiv counter
seq_show_aniSPR:        .byte $00   ; 0= off, 1= show animation sprite when sequence running
seq_show_aniSPR_latch:  .byte $00
seq_show_anidemoSPR:    .byte $00   ; 0= off, 1= show animation sprite when sequence menu
seqSPR_swiperuns:       .byte $00
seqSPR_swipedelay:      .byte $00
seqSPR_preswipedelay:   .byte $00
seq_show_pgnr:          .byte $00
previous_mode:          .byte $00,$00,$00
;===================================================================
; Einblend texte:
; page 0, page 1, page 2, page 3
; sequence start
; sequence pause
; sequence stop
;
;===================================================================
seq_aniSPR0:
.byte $00,$00,$00,$10,$57,$00,$00,$00
.byte $00,$00,$00,$10,$57,$00,$00,$00
.byte $00,$00,$00,$10,$57,$00,$00,$00
.byte $00,$00,$00,$10,$57,$00,$00,$00
.byte $00,$00,$00,$57,$10,$00,$00,$00
.byte $00,$00,$00,$57,$10,$00,$00,$00
.byte $00,$00,$00,$57,$10,$00,$00,$00
.byte $00,$00,$00,$57,$10,$00,$00,$00
;seq_aniSPR1:
.byte $00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$10,$57
.byte $00,$00,$00,$00,$00,$10,$57,$00
.byte $00,$00,$00,$57,$10,$57,$00,$00
.byte $00,$00,$57,$10,$57,$00,$00,$00
.byte $00,$57,$10,$00,$00,$00,$00,$00
.byte $57,$10,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00
;seq_aniSPR2:
.byte $00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00
.byte $57,$57,$57,$57,$10,$10,$10,$10
.byte $10,$10,$10,$10,$57,$57,$57,$57
.byte $00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00
.byte $00,$00,$00,$00,$00,$00,$00,$00
;seq_aniSPR3:
.byte $00,$57,$00,$00,$00,$00,$00,$00
.byte $00,$10,$57,$00,$00,$00,$00,$00
.byte $00,$00,$10,$57,$00,$00,$00,$00
.byte $00,$00,$00,$10,$57,$00,$00,$00
.byte $00,$00,$00,$57,$10,$00,$00,$00
.byte $00,$00,$00,$00,$57,$10,$00,$00
.byte $00,$00,$00,$00,$00,$57,$10,$00
.byte $00,$00,$00,$00,$00,$00,$57,$00
;
;============================================
VERA_addr_to_seqaniZ:
    stz VERA_ctrl
    lda #$01  ; stride =0
    sta VERA_addr_bank
    lda #$FD
    sta VERA_addr_high
    lda #$A6  ; +6 = Z
    sta VERA_addr_low
    rts
VERA_addr_to_seqanidemoZ:
    stz VERA_ctrl
    lda #$01  ; stride =0
    sta VERA_addr_bank
    lda #$FD
    sta VERA_addr_high
    lda #$AE  ; +6 = Z
    sta VERA_addr_low
    rts
    ;
VERA_addr_to_seqanidemoYLO:
    tax
    stz VERA_ctrl
    lda #$01  ; stride =0
    sta VERA_addr_bank
    lda #$FD
    sta VERA_addr_high
    lda #$AC  ; +6 = Z
    sta VERA_addr_low
    txa
    sta VERA_data0
    rts
;====================================
VERA_addr_to_seqZ:
    stz VERA_ctrl
    lda #$41  ; stride =8
    sta VERA_addr_bank
    lda #$FD
    sta VERA_addr_high
    lda #$66  ; +6 = Z
    sta VERA_addr_low
    rts
;====================================
seqaniSPR_switchon:
    jsr VERA_addr_to_seqaniZ
    lda #$0C
    sta VERA_data0
    lda #$01
    sta seq_show_aniSPR
    rts
    ;
seqaniSPR_switchoff:
    jsr VERA_addr_to_seqaniZ
    stz VERA_data0
    stz seq_show_aniSPR
    rts
seqanidemoSPR_switchon:
    jsr VERA_addr_to_seqanidemoZ
    lda #$0C
    sta VERA_data0
    lda #$01
    sta seq_show_anidemoSPR
    rts
seqanidemoSPR_switchoff:
    jsr VERA_addr_to_seqanidemoZ
    stz VERA_data0
    stz seq_show_anidemoSPR
    rts
    ;
;====================================
seqset_statusstart:
    ldx #$90
    bra seq_setstatusSPR
seqset_statuspause:
    ldx #$A0
    bra seq_setstatusSPR
seqset_statusstop:
    ldx #$B0
    bra seq_setstatusSPR
seqset_statuscont:
    ldx #$C0
seq_setstatusSPR:
    stz VERA_ctrl
    lda #$01  ; stride =0
    sta VERA_addr_bank
    lda #$FD
    sta VERA_addr_high
    lda #$68  ; 0 = lo adr
    sta VERA_addr_low
    ;
    txa
    sta VERA_data0
    rts
;====================================
seqSPR_swiperesetpos:
    stz VERA_ctrl
    lda #$11  ; stride =1
    sta VERA_addr_bank
    lda #$FD
    sta VERA_addr_high
    lda #$64  ; +4 = y pos lo
    sta VERA_addr_low
    ;
    stz VERA_data0  ; ypos lo
    stz VERA_data0  ; ypos hi
    lda #$6C  ; +4 = y pos lo
    sta VERA_addr_low
    stz VERA_data0  ; ypos lo
    stz VERA_data0  ; ypos hi
    ;
    rts
;====================================
seqSPR_switchon:
    jsr VERA_addr_to_seqZ   ; stride = 8
    lda #$0C
    sta VERA_data0
    sta VERA_data0          ; next sprite
    rts
    ;
seqSPR_switchoff:
    jsr VERA_addr_to_seqZ
    stz VERA_data0
    stz VERA_data0
    rts
    ;
;====================================
seqaniSPR_inc:
    stz VERA_ctrl
    lda #$01  ; stride =0
    sta VERA_addr_bank
    lda #$FD
    sta VERA_addr_high
    lda #$A0  ; VERA SPR addr lo
    sta VERA_addr_low
    jmp write_seqSPRdat
;     lda VERA_data0
;     cmp #$46
;     bmi :+
;     lda #$40
;     sta VERA_data0
;     rts
;    :clc
;     adc #$02
;     sta VERA_data0
;     rts
;====================================
seqanidemoSPR_inc:
    stz VERA_ctrl
    lda #$01  ; stride =0
    sta VERA_addr_bank
    lda #$FD
    sta VERA_addr_high
    lda #$A8  ; VERA SPR addr lo
    sta VERA_addr_low
write_seqSPRdat:
    lda VERA_data0
    cmp #$46
    bmi :+
    lda #$40
    sta VERA_data0
    rts
   :clc
    adc #$02
    sta VERA_data0
    rts
;====================================
seqSPR_startswipeout:
    stz seqSPR_preswipedelay
    stz seqSPR_swipedelay
    jsr seqSPR_swiperesetpos
    jsr seqSPR_switchon
    jmp seqSPR_swipeout     ; use rts of seqSPR_swipeout
    ;
seqSPR_swipeout:
    lda seqSPR_swiperuns
    beq @set_on
    ;
    stz VERA_ctrl
    lda #$11  ; stride =1
    sta VERA_addr_bank
    lda #$FD
    sta VERA_addr_high
    lda #$64  ; +4 = y pos lo
    sta VERA_addr_low
    ;
    lda VERA_data0  ; ypos lo
    sta ZP_PTR_7
    lda VERA_data0  ; ypos hi
    sta ZP_PTR_7+1
    ;
    lda ZP_PTR_7
    sec
    sbc #$01
    sta ZP_PTR_7
    bcs :+
    dec ZP_PTR_7+1
    ;
   :lda #$64  ; +4 = y pos lo
    sta VERA_addr_low
    lda ZP_PTR_7
    sta VERA_data0  ; ypos lo
    lda ZP_PTR_7+1
    sta VERA_data0  ; ypos hi
    ;
    lda #$6C  ; +4 = y pos lo
    sta VERA_addr_low
    lda ZP_PTR_7
    sta VERA_data0  ; ypos lo
    lda ZP_PTR_7+1
    sta VERA_data0  ; ypos hi
    ;
    lda ZP_PTR_7
    cmp #$F8
    bne @exit
    ;
    jsr seqSPR_swiperesetpos
    jmp seqSPR_switchoff ; use rts of switchoff...
    ;
@set_on:
    lda #01
    sta seqSPR_swiperuns
@exit:
    rts
    ;
;===================================================================
.macro create_seqaniSPR
    lda #<VRAM_seqaniSPR  ; Target
    sta ZP_PTR_2    ; Target
    lda #>VRAM_seqaniSPR ; Target
    sta ZP_PTR_2+1  ; Target
    ;
    lda #<seq_aniSPR0   ; Source
    sta ZP_PTR_6
    lda #>seq_aniSPR0   ; Source
    sta ZP_PTR_6+1
    ;
    stz VERA_ctrl       ; Target
    lda #$11  ; stride =1
    sta VERA_addr_bank
    lda ZP_PTR_2+1
    sta VERA_addr_high
    lda ZP_PTR_2
    sta VERA_addr_low
    ;
    lda #$04    ; 4 sprites
    sta ZP_tmp_DATA
@singlespr_loop:
    ldx #$40    ; 64 byte per sprite
    ldy #$00
   :lda (ZP_PTR_6),y
    sta VERA_data0
    iny
    dex
    bne :-  ; inner loop
    clc
    lda ZP_PTR_6
    adc #$40
    sta ZP_PTR_6
    bcc :+
    inc ZP_PTR_6+1
   :dec ZP_tmp_DATA
    bne @singlespr_loop
    ;
; sprite attributes
    stz VERA_ctrl
    lda #$11  ; stride =1
    sta VERA_addr_bank
    lda #$FD
    sta VERA_addr_high
    lda #$A0 ;   sprite #50
    sta VERA_addr_low
    ;
    lda #$40
    sta VERA_data0      ; VRAM addr. lo
    lda #$0A
    ora #$80            ; Bit 7 : 8bpp mode
    sta VERA_data0      ; VRAM addr. hi
    ;
    stz VERA_data0      ; xpos lo
    stz VERA_data0      ; xpos hi
    ;
    lda #$D8
    sta VERA_data0      ; ypos lo
    lda #$01
    sta VERA_data0      ; ypos hi
    ;
    stz VERA_data0      ; colis,z,flips
    stz VERA_data0      ; W=8,H=8
    ;
; extra animation sprite for the sequencer menu
; with same data
; keep on writing after previous sprite #50
; as they follow we don't need to set new VERA adr
    ;
    lda #$40
    sta VERA_data0      ; VRAM addr. lo
    lda #$0A
    ora #$80            ; Bit 7 : 8bpp mode
    sta VERA_data0      ; VRAM addr. hi
    ;
    lda #$70
    sta VERA_data0      ; xpos lo
    lda #$02
    sta VERA_data0      ; xpos hi
    ;
    lda #$6C
    sta VERA_data0      ; ypos lo
    lda #$00
    sta VERA_data0      ; ypos hi
    ;
    stz VERA_data0      ; colis,z,flips
    stz VERA_data0      ; W=8,H=8
    ;
    lda #$01            ; initialy turn on seq. animator
    sta seq_show_aniSPR
    rmb1 seq_statusbits ; initialy turn off pause
.endmacro
;===================================================================
start_seq:
    smb0 seq_statusbits
    rmb1 seq_statusbits
    jsr seqset_statusstart
    jsr cursor_switchoff
    jsr seqanidemoSPR_switchoff
    stz seq_nxttimer_div
    stz seq_tick
    lda seq_show_pgnr
    beq :+
    jsr pageSPR_startswipeout
    ;
   :lda #<proc_seqkeys
    sta use_key_vector
    lda #>proc_seqkeys
    sta use_key_vector+1
    ;
    lda seq_show_aniSPR_latch
    beq :+
    jsr seqaniSPR_switchon
   :stz char_value
    jsr seqSPR_startswipeout
    rts
    ;
stop_seq:
    rmb0 seq_statusbits
    rmb1 seq_statusbits
    jsr seqaniSPR_switchoff
    jsr seqset_statusstop
    jsr seqSPR_startswipeout
    jsr cursor_switchon
    jmp default_keyvec  ; use rts of default_keyvec
    ;
;===================================================================
proc_seqkeys:
    lda char_value
;@ESCkey:
    cmp #$1B    ; $1B=ESC key:
    bne @spckey ; @F1key
    jsr stop_seq
    jsr pageSPR_startswipeout
    bra @keyend
@spckey:
    cmp #$20    ; $1B=ESC key:
    bne @F1key
    lda seq_statusbits
    eor #$02    ; invert pause bit
    sta seq_statusbits
    bbr1 seq_statusbits, @sq_sets
    jsr seqset_statuspause
    jsr seqSPR_startswipeout
    bra @keyend
@sq_sets:
    jsr seqset_statuscont
    jsr seqSPR_startswipeout
    bra @keyend
@F1key:
    cmp #$85    ; $85=F1 key:
    bne @F5key
    jsr stop_seq
    jmp proc_defkeys
@F5key:
    cmp #$87    ; $87=F5 key:       start sequence
    bne @F6key
    jsr stop_seq
    smb1 mode_last_run          ; froces both layers to be redrawn
    bra @keyend
@F6key:
    cmp #$8B    ; $8B=F6 key:   stop seq, return to seq. menu
    bne @F7key;@keyend
    jsr stop_seq
    jmp proc_defkeys
@F7key:
    cmp #$88    ; $88=F7 key:       ; seq. timing menu
    bne @keyend
    jsr stop_seq
    jmp proc_defkeys
    ;
@keyend:
    jmp main_loop
;===================================================================
sequencer_irq_handler:
    lda seqSPR_swiperuns
    beq @endtest_swipe
    lda seqSPR_preswipedelay
    cmp #$7F
    beq @swipeseq
    inc
    sta seqSPR_preswipedelay
    bra @endtest_swipe
@swipeseq:
    lda seqSPR_swipedelay
    inc
    sta seqSPR_swipedelay
    cmp #$03
    bmi @endtest_swipe
    jsr seqSPR_swipeout
@endtest_swipe:
    lda menu_mode
    cmp #$06
    bne :++
    lda seqani_del
    cmp #$05
    bmi :+
    stz seqani_del
    jsr seqanidemoSPR_inc
   :inc seqani_del
   :bbs0 seq_statusbits,@sqpause
    bra @exit_irq
@sqpause:
    bbr1 seq_statusbits,@sq_del
    bra @exit_irq
    ;
@sq_del:
    lda seqani_del
    cmp #$05
    bmi :+
    jsr seqaniSPR_inc
    stz seqani_del
   :inc seqani_del
    ;
    lda seq_tick
    cmp #$F0    ; d240 = ca. 120 sec
    bne @next_tick ; ticks not rolling over
    stz seq_tick ; reset ticks
    ;
    inc seq_nxttimer_div
    lda seq_nxttimer_div
    cmp seq_steptime
    bmi @seq_ntimer_div
    stz seq_nxttimer_div
    ;
    jsr current_usrtxt_inc
    jsr memorize_prevmode
    jsr restore_page_settings
    jsr check_refreshmode
    lda seq_show_pgnr
    beq @exit_irq
    jsr pageSPR_startswipeout
    bra @exit_irq
    ;
@seq_ntimer_div:
    inc seq_nxttimer_div
@next_tick:
    inc seq_tick
    inc seq_tick
    ;
@exit_irq:
    jmp cont_fromseqirq
 ;===================================================================
.endif
