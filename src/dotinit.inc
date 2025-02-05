.ifndef DOTINIT_INC
DOTINIT_INC = 1
;===================================================
.macro initdotmatrix
;=================================================================================
; service mode for testing porposes
; enables 2 additional functions when in the options (F8) menu:
; F6 -> restarts the program, but without the intro (works only once)
; F9 -> toggles layer 1  on <-> off,
;       reveals the compressed character generation by some tile patterns when off
; The extra service functions will not be mentioned in the menu
;
; To eneble service mode enter the following commands before running the program
; POKE 48,68
; POKE 49,77
; POKE 50,83
;
    stz service_mode
    lda $30     ; d48
    cmp #$44    ; d68 'D' = Dot
    bne @no_service
    lda $31     ; d49
    cmp #$4D    ; d77 'M' = Matrix
    bne @no_service
    lda $32     ; d50
    cmp #$53    ; d83 'S' = Service
    bne @no_service
    lda #$01
    sta service_mode
@no_service:
; uncoment to activate service mode by default:
;    lda #$01
;    sta service_mode
;=================================================================================
    cld ; ommit dec mode
    ;
    stz char_value
    stz no_intro
    stz half_mode
    stz mode_last_run
    stz textrefresh
    stz logo_on
    stz X16logo_on
    stz digit_inc_cnt
    stz del_rowptr
    stz old_oct
    lda #$05
    sta dot_inc_cnt
    lda #$1D
    lda $08
    sta Z_pointer_inc_lo
    lda $06
    sta Z_pointer_inc_hi
    stz Z_no
    lda $20
    sta row_fill_cnt
    lda #($31|$40)          ; | $40=SPRITE_EN
    sta dc_mode
    ;
    ldx #<sectxt0
    ldy #>sectxt0
    jsr set_PTR_1
    lda #$09
    sta seq_steptime
    stz seq_show_aniSPR_latch
    jsr seqaniSPR_switchoff
    ;
    lda #CHAR_ISOMODE
    jsr CHROUT  ; force ISO mode since PETSCII isn't supported (yet)
    ;
    lda IRQVec       ; save default IRQ vector
    sta default_irq_vector
    lda IRQVec+1
    sta default_irq_vector+1
    ;
;     lda #$3A
;     sta r0L
;     lda #$03
;     sta r0H
;     stz r1L
;     stz r1H
;     stz r2L
;     stz r2H
;
;     jsr clock_set_date_time
    jsr init_rnd
    ;
restart:
    stz crsrstatus
    stz req_crsrmove
    stz menu_mode
    ;
    jsr default_keyvec  ; set default keyboard routine
    ;
    lda crsr_wait_time
    sta crsr_wait
    stz char_VRAM_adr_hi
    stz char_VRAM_adr_lo
    ;
    create_logoSPR      ; creates sprite set used to show X16 logo
    set_logoSPR         ; sets sprite attributes of X16 logo sprites
    create_tiles        ; creates tile sets for 20x10 mode
    create_cursor       ; creates cursor sprites for 20x10 mode
    create_hcursor      ; creates cursor sprites for 40x20 mode
    create_verlineSPR   ; creates erase line sprites, mode independent (so far)
    create_halftiles    ; creates tile sets for 40x20 mode
    create_infoSPR      ; creates indicator sprites
    create_seqaniSPR    ; creates sprite set for seq. animation
    ;
    blockdel_txtRAM     ; deletes all RAM used to store text (and also the gaps between pages)
    jsr init_pagesettings       ; have the page settings something valid
    jsr logo_switchoff          ; initially turn off
    ;
    lda no_intro                ; check if to do intro
    bne intro_fin
    jsr start_intro             ; <-- intro animation routine
intro_fin:
    jsr init16bitrandom         ; since intro features also evertick
    jsr restore_page_settings   ; after intro might messed up values
    jsr load_file               ; try to load or create "DOTTEXT.TXT"
;    stz logo_on
;    stz X16logo_on
    ;
    smb1 mode_last_run          ; forces both layers to be redrawn
    jsr set_std_IRQVec          ; sets and activates irq handler
    ;
    jsr cursor_home
    jsr cursor_switchon
    ;
    lda menu_mode               ; might be set on intro exit
    beq :+
    jsr setup_menu              ; if so, execute menu directly
    bra main_loop
   :jsr pageSPR_startswipeout
    ;                           ; (continues to main_loop)
.endmacro
;===================================================
.endif
