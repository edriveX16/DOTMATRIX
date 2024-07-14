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
;==================================
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
actual_usrtxt_inc:
    lda actual_usrtxthi
    cmp #>usrtxt7
    bmi @tinc
    lda #>usrtxt0
    sta actual_usrtxthi
    rts
@tinc:
    clc
    lda actual_usrtxthi
    adc #$04
    sta actual_usrtxthi
    rts
;=======================
actual_usrtxt_dec:
    lda actual_usrtxthi
    cmp #>usrtxt0
    beq @tdec
    lda actual_usrtxthi
    sec
    sbc #$04
    sta actual_usrtxthi
    rts
@tdec:
    lda #>usrtxt7
    sta actual_usrtxthi
    rts
;================================================
.endif
