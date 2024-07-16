.ifndef VECTORS_INC
VECTORS_INC = 1
;================================================
set_std_IRQVec:                     ; set standard IRQ handler
    sei
    lda #<dotmatrix_irq_handler
    sta IRQVec
    lda #>dotmatrix_irq_handler
    sta IRQVec+1
    lda #VSYNC_BIT ; make VERA only generate VSYNC IRQs
    sta VERA_ien
    cli
    rts
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
default_keyvec:
    lda #<proc_defkeys
    sta use_key_vector
    lda #>proc_defkeys
    sta use_key_vector+1
    rts
;================================================
set_PTR_1:
    stx ZP_PTR_1
    sty ZP_PTR_1+1
    rts
;========================
inc_PTR_1:
    clc                 ; inc source pointer since exit '$00' wasn't counted
    lda ZP_PTR_1
    adc #$01
    sta ZP_PTR_1
    bcc :+
    inc ZP_PTR_1+1
   :rts
;========================
set_PTR_2:
    stx ZP_PTR_2
    sty ZP_PTR_2+1
    rts
;==================================
add_line_2_ZP4:
    clc
    lda ZP_PTR_4
    adc max_char_p_line
    sta ZP_PTR_4
    bcc :+
    inc ZP_PTR_4+1
   :rts
;=============
add_4_PTR_4_hi:
    clc
    lda ZP_PTR_4+1
    adc #$04
    sta ZP_PTR_4+1
    rts
;========================
set_indent_PTR_4:
    lda #<systxt0   ; target
    sta ZP_PTR_4
    lda #>systxt0
    sta ZP_PTR_4+1
    clc
    lda ZP_PTR_4
    adc single_line_indent
    sta ZP_PTR_4
    bcc :+
    inc ZP_PTR_4+1
   :rts
;==================================
sto_inc_nload_PTR5:
    sta (ZP_PTR_5)
    clc
    lda ZP_PTR_5
    adc #$01
    sta ZP_PTR_5
    bcc :+
    inc ZP_PTR_5+1
   :lda ZP_PTR_5
    rts
;================================
current_usrtxt_inc:     ; move current_usrtxt ptr to next page
    lda current_usrtxthi
    cmp #>usrtxt7       ; check for round robin
    bmi @tinc
    lda #>usrtxt0
    sta current_usrtxthi
    rts
@tinc:
    clc
    lda current_usrtxthi
    adc #$04
    sta current_usrtxthi
    rts
;=======================
current_usrtxt_dec:     ; move current_usrtxt ptr to previous page
    lda current_usrtxthi
    cmp #>usrtxt0       ; check for round robin
    beq @tdec
    lda current_usrtxthi
    sec
    sbc #$04
    sta current_usrtxthi
    rts
@tdec:
    lda #>usrtxt7
    sta current_usrtxthi
    rts
;==================================
set_pagesettings_ptr:   ; page settings storage starts at end of each text page RAM
    lda #$20
    sta ZP_PTR_5
    clc
    lda current_usrtxthi
    adc #$03
    sta ZP_PTR_5+1
    rts
;================================================
.endif
