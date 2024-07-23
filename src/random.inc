.ifndef RANDOM_INC
RANDOM_INC = 1
;==================================================================
oldpreset:      .byte $00
mem_pg_hi:    .byte $00
ftype:          .byte $00
charonly:       .byte $00
patonly:        .byte $00
nocx16:         .byte $00
; some magic numbers for random initialization
magic_nr0:      .byte $00,$2d   ;$002d
magic_nr1:      .byte $08,$FB   ;$08fb
magic_nr2:      .byte $20,$8F   ;$208f
magic_nr3:      .byte $41,$E1   ;$41e1
magic_nr4:      .byte $99,$47   ;$9947
magic_nr5:      .byte $DE,$A5   ;$dea5
magic_nr6:      .byte $DC,$91   ;$dc91
magic_nr7:      .byte $FC,$0F   ;$fc0f
magic_hi:       .byte $00
magic_lo:       .byte $00
;================================
; random patterns are generated from a set of 4 characters
; there are 8 char. sets for pattern gerneration to choose from
; diagonal
pattern0: .byte $2F,$87,$2F,$87 ;/,\,$5F_,\
pattern1: .byte $20,$60,$20,$87 ;spc,\ "short backspace", spc,\
pattern2: .byte $3C,$3E,$2F,$87 ;<,>,/,\
pattern3: .byte $20,$85,$6F,$87 ;spc, . "big dot at bottom",o,\
; straight
pattern4: .byte $5F,$20,$83,$82    ; _ under score, spc, box, dash on top and bottom
pattern5: .byte $20,$2D,$83,$82    ; spc, - , box, dash on top and bottom
pattern6: .byte $7C,$2D,$82,$86    ;| vertical divider,- middle dash, dash on top and bottom,| pipe symbol
pattern7: .byte $7C,$20,$83,$2D    ;| vertical divider,spc,box, -
;================================
; The program uses both a simple 8-bit and a 16-bit pseudo-random generator
; set initial seed for 8-Bit random generator below
init_rnd:
    jsr clock_get_date_time
    lda r2H     ; secs
    asl
    asl
    sta rnd_seed
    lda r3L     ; jiffeis
    asl
    ora rnd_seed
    sta rnd_seed
    rts
    ;
;========================
; small pseudo random generator
get_rand:
    lda rnd_seed
    beq :+
    asl
    beq :++
    bcc :++
   :eor #$1D
   :sta rnd_seed
    rts
;==================================
; set initial seed for 16-Bit random generator below
init16bitrandom:
    lda #<magic_nr0
    sta ZP_PTR_2
    lda #>magic_nr0
    sta ZP_PTR_2+1
    ;
    lda evertick
    and #$07
    asl
    tay
    lda (ZP_PTR_2),y
    sta magic_hi
    iny
    lda (ZP_PTR_2),y
    sta magic_lo
    rts
;
get16bitrnd:
    lda rnd2_seed
    beq @lowZero     ; $0000 and $8000 are special values to test for
    asl rnd2_seed   ; Do a normal shift
    lda rnd3_seed
    rol
    bcc @noEor
@doEor:  ; high byte is in .A
    eor magic_hi    ;#>magic
    sta rnd3_seed
    lda rnd2_seed
    eor magic_lo    ;#<magic
    sta rnd2_seed
    rts
    ;
@lowZero:
    lda rnd3_seed
    beq @doEor   ; High byte is also zero, so apply the EOR
                ; For speed, you could store 'magic' into 'rnd2_seed' directly
                ; instead of running the EORs
    ; wasn't zero, check for $8000
    asl
    beq @noEor ; if $00 is left after the shift, then it was $80
    bcs @doEor ; else, do the EOR based on the carry bit as usual
@noEor:
    sta rnd3_seed
    rts
;==================================
ins_cx16_atrand_pos:
    ldx #<intro_txt0    ; declared and defined in 'dotintro.inc'
    ldy #>intro_txt0    ; "commander X16"
    jsr set_PTR_1
    ;
    lda ZP_PTR_6+1
    sta ZP_PTR_4+1
    bbr3 rnd3_seed, @irdx_end
    ;
    lda rnd3_seed
    and #$3F
    clc
    adc rnd2_seed
    sta ZP_PTR_4
    bcc @test_dir
    inc ZP_PTR_4+1
    ;
@test_dir:
    ldy #$00
    ldx #$0D
    ;
    lda rnd2_seed
    cmp #$3F
    bmi @diagonal
    cmp #$7F
    bmi @horizontal
;@vertikal:
   :lda (ZP_PTR_1),y
    sta (ZP_PTR_4)
    iny
    jsr add_line_2_ZP4
    dex
    bne :-
    rts
@horizontal:
   :lda (ZP_PTR_1),y
    sta (ZP_PTR_4),y
    iny
    dex
    bne :-
    rts
@diagonal:
   :lda (ZP_PTR_1),y
    sta (ZP_PTR_4),y
    iny
    jsr add_line_2_ZP4
    dex
    bne :-
@irdx_end:
    rts
;===========================================
random_txt:
    stz ZP_PTR_5
    stz ZP_PTR_6
    lda current_usrtxthi
    sta ZP_PTR_5+1
    sta ZP_PTR_6+1
    clc
    adc #$03
    sta ZP_tmp_DATA
    ;
    jsr get16bitrnd
    lda rnd2_seed
    sta ZP_tmp_DATA3
    stz nocx16
next_rnd:
    jsr get16bitrnd
    bbs4 ZP_tmp_DATA3, @allchars
    bbs3 ZP_tmp_DATA3, @allchars
    lda rnd2_seed
    and #$0F
    cmp #$0A
    bmi @unterneun
    bbs4 rnd2_seed, @space
    cmp #$0C
    beq @pnauf
    cmp #$0E
    beq @equal
    ora #$20
    bra @outnox
@pnauf:
    lda #$5E
    bra @outnox
@equal:
    lda #$3D
    bra @outnox
@unterneun:
    bbs5 rnd2_seed, @space
    ora #$30
    bra @outnox
@space:
    lda #$20
@outnox:
    sta nocx16
    bra @schreiben
@allchars:
    stz nocx16
    and #$7F
    cmp #$20
    bmi :+
    bra @schreiben
   :lda #$20
@schreiben:
    jsr sto_inc_nload_PTR5
    cmp #$1F
    beq @testhi
    bra next_rnd
@testhi:
    lda ZP_PTR_5+1
    cmp ZP_tmp_DATA
    beq @fertig
    bra next_rnd
@fertig:
    lda nocx16
    bne :+
    jsr ins_cx16_atrand_pos
   :rts
;===========================================
rndpattern:
    stz ZP_PTR_5
    lda current_usrtxthi
    sta ZP_PTR_5+1
    clc
    adc #$03
    sta ZP_tmp_DATA
    ;
sel_pattype:
    jsr get16bitrnd
    bbs6 rnd3_seed, @straight
    bbs5 rnd2_seed, @dia2
    bbs4 rnd2_seed, @dia3
    ldx #<pattern0
    ldy #>pattern0
    bra @endselect
@dia3:
    ldx #<pattern1
    ldy #>pattern1
    bra @endselect
@dia2:
    bbs4 rnd2_seed, @dia4
    ldx #<pattern2
    ldy #>pattern2
    bra @endselect
@dia4:
    ldx #<pattern3
    ldy #>pattern3
    bra @endselect
@straight:
    bbs5 rnd2_seed, @strai2
    bbs4 rnd2_seed, @strai3
    ldx #<pattern4
    ldy #>pattern4
    bra @endselect
@strai3:
    ldx #<pattern5
    ldy #>pattern5
    bra @endselect
@strai2:
    bbs4 rnd2_seed, @strai4
    ldx #<pattern6
    ldy #>pattern6
    bra @endselect
@strai4:
    ldx #<pattern7
    ldy #>pattern7
@endselect:
    jsr set_PTR_1
;===============================
next_rndp:
    jsr get16bitrnd
    lda rnd3_seed
    bbs7 rnd3_seed, @upper
    ldy #$00
    bra @cmp
@upper:
    and #$7F
    ldy #$02
@cmp:
    cmp #$3F
    bmi :+
    bra @schreiben
   :iny
@schreiben:
    lda (ZP_PTR_1),y
    jsr sto_inc_nload_PTR5
    cmp #$1F
    beq @testhi
    bra next_rndp
@testhi:
    lda ZP_PTR_5+1
    cmp ZP_tmp_DATA
    beq @fertig
    bra next_rndp
@fertig:
    rts
;===========================================
all_pg_random_something:
    stz patonly
    stz charonly
    stz ftype
    stz ZP_PTR_6
    lda #>usrtxt0;
    sta ZP_PTR_6+1
    jsr get16bitrnd
    lda rnd2_seed
    sta ZP_tmp_DATA3    ; preload DATA3 with random function
    ;
    lda #$08            ; 8 pages to randomize
    sta ZP_tmp_DATA2
@apr_loop:
    stz ZP_PTR_5
    lda ZP_PTR_6+1
    sta ZP_PTR_5+1
    clc
    adc #$03
    sta ZP_tmp_DATA
    ;
    lda ftype
    bne @rndtxt
    jsr sel_pattype     ; proceeds to next_rndp
    bra @rndinc
@rndtxt:
    jsr next_rnd
@rndinc:
    jsr get16bitrnd
    lda rnd2_seed
    sta ZP_tmp_DATA3
    clc
    lda ZP_PTR_6+1
    adc #$04
    sta ZP_PTR_6+1
    ;
    lda charonly
    bne @charonly
    lda patonly
    bne @patonly
    ;
    lda rnd2_seed
    and #$01
    sta ftype
    bra @exit
@patonly:
    stz ftype
    bra @exit
@charonly:
    lda #$01
    sta ftype
@exit:
    stz nocx16
    dec ZP_tmp_DATA2
    bne @apr_loop
    rts
;===========================================
random_preset:
    bbs0 half_mode, :+
    lda npreset
    bra :++
   :lda hpreset
   :sta oldpreset
    ;
    stz ZP_tmp_DATA2
    jsr get16bitrnd
    lda rnd2_seed
    and #$0E
    beq @pout
    lsr
    tay
@add_loop:
    clc
    lda ZP_tmp_DATA2
    adc #$06
    sta ZP_tmp_DATA2
    dey
    bne @add_loop
    ;
@pout:
    lda ZP_tmp_DATA2
    cmp oldpreset
    beq @doppelt
    sta oldpreset
    ;
    bbs4 rnd2_seed, @doppelt
    bbs0 half_mode, @hpout
    bra @npout
;
@doppelt:
    sta oldpreset
    bbs0 half_mode, :+
    smb0 half_mode
@hpout:
    lda ZP_tmp_DATA2
    sta hpreset
    jsr get_h_presetcols
    rts
    ;
   :stz half_mode
@npout:
    lda ZP_tmp_DATA2
    sta npreset
;@pexit:
    jsr get_presetcol
    rts
;============================================
random_allpreset:
    lda current_usrtxthi
    sta mem_pg_hi
    lda #>usrtxt0;
    sta current_usrtxthi
    stz current_usrtxtlo
    ;
    ldx #$08
    ;
   :jsr random_preset
    jsr save_page_settings
    jsr current_usrtxt_inc
    dex
    bne :-
    stz current_usrtxtlo
    lda mem_pg_hi
    sta current_usrtxthi
    rts
;==========================
.macro random_steptime_logo
    clc
    lda #>usrtxt0;
    adc #$03
    sta ZP_PTR_2+1
    ;
    lda #$20
    sta ZP_PTR_2
    ;
    ldx #$08
@pg_loop:
    jsr get16bitrnd
    and #$0F
    sta (ZP_PTR_2) ; steptime
    ;
    ldy #$01        ; test for half mode
    lda (ZP_PTR_2),y
    bne @nxt_pg         ; no normal mode
    bbr7 rnd2_seed, @no_rlo
    bbr6 rnd2_seed, @no_rlo
    lda #$01
    bra @dec_cnt
@no_rlo:
    lda #$00
@dec_cnt:
    ldy #$09
    sta (ZP_PTR_2),y
@nxt_pg:
    clc
    lda ZP_PTR_2+1
    adc #$04
    sta ZP_PTR_2+1
    dex
    bne @pg_loop
.endmacro
;==================================================================
.endif
