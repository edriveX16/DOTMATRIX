.ifndef DOT_ZP_INC
DOT_ZP_INC = 1
;===================================================
;=========================
; Kernal Registers
r0					= $02
r0L					= r0
r0H					= r0+1
r1					= $04
r1L					= r1
r1H					= r1+1
r2					= $06
r2L					= r2
r2H					= r2+1
r3					= $08
r3L					= r3
r3H					= r3+1
;=========================
; Zero-Page userspace
; temporary pointers
ZP_PTR_1            = $22   ; dotintro, dotmenu
ZP_PTR_2            = $24   ; cusor moves, get color presets, get patterns
ZP_PTR_3            = $26   ; main output pointer !! ,  creat hcursor, cerate vlineSPR,
ZP_PTR_4            = $28   ; fill sys-&txtRAM, dotmenu
ZP_PTR_5            = $30   ; dotintro, keyboard bei mode wechsel bzw. restore/save settings
ZP_PTR_6            = $32   ; dotintro, char 2 RAM, make same!!!
ZP_PTR_7            = $34   ; swipes excl.
ZP_PTR_8            = $36   ; decoder excl.
ZP_PTR_9            = $38   ; decoder excl.
; temp data
ZP_tmp_DATA         = $3A   ; set_logoSPR
ZP_tmp_DATA2        = $3B   ; set_logoSPR
ZP_tmp_DATA3        = $3C
ZP_tmp_DATA4        = $3D
;
char_value          = $3E
Z_no                = $3F ; temp. index ->include file 'textutils'
zp_index            = $40   ; ->include file 'decoder'
Z_index             = $41 ; ->include file 'textutils'
zp_line_SPR_xposlo  = $42 ; ->include file 'cursormove'
zp_line_SPR_xposhi  = $43 ; ->include file 'cursormove'
ticks               = $44
crsr_offtimer       = $45
crsr_wait            = $46
;count_256          = $47 ; not used
Z_pointer_lo        = $48
Z_pointer_hi        = $49
Z_pointer_inc_lo    = $4A   ;$08 normal mode, half mode= $04
Z_pointer_inc_hi    = $4B   ;$06 normal mode, half mode= $03
current_usrtxtlo     = $4C
current_usrtxthi     = $4D
char_VRAM_adr_lo    = $4E
char_VRAM_adr_hi    = $4F
;
half_mode           = $50  ; 0 = off (=20*10 chars), 1 = half mode (=40*20 chars)
;zp_crsrstatus       = $51   ; Bit0 = on, 1= auto, (0= auto, 1= allways on),
crsrstatus       = $51   ; Bit0 = on, 1= auto, (0= auto, 1= allways on),
row_fill_cnt        = $52   ;.byte $20 CVIDEO_INC
digit_inc_cnt       = $53 ; ->include file 'cursormove'
dot_inc_cnt         = $54 ; ->include file 'cursormove'
cheat_col           = $55
cheat_frame         = $56 ;
cheat_useframe      = $57 ;
old_oct             = $58 ; ->include file 'cursormove'
pageSPR_swiperuns   = $59 ; ->include file 'charrom'
req_crsrmove         = $5A ; bit0 right, bit1 left, bit2 down, bit3 up
zp_flips            = $5B ; ->include file 'decoder', 'halfsize'
textrefresh         = $5C
mode_last_run       = $5D
;
post_ins            = $5E ; ; ->include file 'postinsert'
pattern_offset_ptr  = $5F ; ->include file 'decoder'
;count_256          = $60 ; not used
dc_mode             = $60 ;
;count_256          = $61 ; not used

;
del_rowptr          = $62 ; ->include file 'cursormove'
halfc_frame         = $63 ;
seqani_del          = $64   ; seq. runs animation sprite ticks div.
seq_statusbits      = $65   ; bit0=seq_run, bit1=seq_pause, bit2=seq_show_pgnr,
seq_steptime        = $66
evertick            = $67
seq_tick            = $68
rnd_seed            = $69 ; ->include file 'dotintro'
rnd2_seed           = $6A
rnd3_seed           = $6B
char_pat_linesubset  = $6C  ; occupies 4 byte!
;       uses up to     $6F
;===================================================
.endif
