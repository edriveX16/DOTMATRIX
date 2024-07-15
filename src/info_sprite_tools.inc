.ifndef INFO_SPRITE_TOOLS_INC
INFO_SPRITE_TOOLS_INC = 1
;
;============================================
pageSPR_mode:           .byte   $00
pageSPR_swipedelay:     .byte   $00
pageSPR_preswipedelay:  .byte   $00
;============================================
VERA_addr_to_pageZ:
   stz VERA_ctrl
   lda #$01  ; stride =0
   sta VERA_addr_bank
   lda #$FD
   sta VERA_addr_high
   lda #$16  ; +6 = Z
   sta VERA_addr_low
   rts
;====================================
set_VERA_pageadr:
    stz VERA_ctrl
    lda #$01  ; stride =0
    sta VERA_addr_bank
    lda #$FD
    sta VERA_addr_high
    lda #$10  ; +0 = (lo adr >>5)
    sta VERA_addr_low
    ;
    lda current_usrtxthi
    and #$1C
    eor #$10
    asl
    asl
    ora #$80
    sta VERA_data0
    rts
;====================================
pageSPR_switchon:
    stz pageSPR_swiperuns
    jsr pageSPR_swiperespos
    jsr VERA_addr_to_pageZ
    lda #$0C
    sta VERA_data0
    lda #$01
    sta pageSPR_mode
    jsr set_page_IRQVec
    rts
    ;
pageSPR_switchoff:
   jsr VERA_addr_to_pageZ
   stz VERA_data0
   stz pageSPR_mode
   jsr set_std_IRQVec
   stz pageSPR_swiperuns
   rts
   ;
;====================================
set_page_IRQVec:
    sei
    lda #<page_irq_handler
    sta IRQVec
    lda #>page_irq_handler
    sta IRQVec+1
    lda #VSYNC_BIT ; make VERA only generate VSYNC IRQs
    sta VERA_ien
    cli
    rts
;====================================
pageSPR_swiperespos:
    stz VERA_ctrl
    lda #$11  ; stride =1
    sta VERA_addr_bank
    lda #$FD
    sta VERA_addr_high
    lda #$14  ; +4 = y pos lo
    sta VERA_addr_low
    ;
    stz VERA_data0  ; ypos lo
    stz VERA_data0  ; ypos hi
    ;
    rts
;====================================
pageSPR_startswipeout:
    jsr set_VERA_pageadr
    stz pageSPR_preswipedelay
    stz pageSPR_swipedelay
    jsr pageSPR_swiperespos
    jsr pageSPR_switchon
    jsr pageSPR_swipeout
    rts
    ;
pageSPR_swipeout:
    lda pageSPR_swiperuns
    beq @set_on
    ;
    stz VERA_ctrl
    lda #$11  ; stride =1
    sta VERA_addr_bank
    lda #$FD
    sta VERA_addr_high
    lda #$14  ; +4 = y pos lo
    sta VERA_addr_low
    ;
    lda VERA_data0  ; ypos lo
    sta ZP_PTR_7
    lda VERA_data0  ; ypos hi
    sta ZP_PTR_7+1
    lda ZP_PTR_7
    sec
    sbc #$01
    sta ZP_PTR_7
    bcs :+
    dec ZP_PTR_7+1
    ;
   :lda #$14  ; +4 = y pos lo
    sta VERA_addr_low
    ;
    lda ZP_PTR_7
    sta VERA_data0  ; ypos lo
    lda ZP_PTR_7+1
    sta VERA_data0  ; ypos hi
    ;
    lda ZP_PTR_7
    cmp #$F8
    bne :+
    jsr pageSPR_swiperespos
    jmp pageSPR_switchoff
   :rts
@set_on:
    lda #01
    sta pageSPR_swiperuns
    rts
    ;
;============================================
page_irq_handler:
    lda VERA_isr
    and #VSYNC_BIT
    bne @test_pageswipe  ; non-VSYNC IRQ, no tick update
    jmp (default_irq_vector) ; continue IRQ handler
    ;
@test_pageswipe:
    lda pageSPR_swiperuns
    beq @endtest_swipe
    lda pageSPR_preswipedelay
    cmp #$7F
    beq @swipepage
    inc
    sta pageSPR_preswipedelay
    bra @endtest_swipe
@swipepage:
    lda pageSPR_swipedelay
    inc
    sta pageSPR_swipedelay
    cmp #$03
    bmi @endtest_swipe
    jsr pageSPR_swipeout
@endtest_swipe:
    jmp swipes ; found in  main default IRQ routine
;===================================================================

.endif

