.ifndef DOTSYSDECLARATIONS_INC
DOTSYSDECLARATIONS_INC  = 1
;=============================================================================
;
;==========================     stuff usually found in x16.inc     ===========
;
;=============================================================================
; RAM Interrupt Vectors
IRQVec              = $0314
; VERA
VERA_addr_low       = $9F20
VERA_addr_high      = $9F21
VERA_addr_bank      = $9F22
VERA_data0          = $9F23
VERA_data1          = $9F24 ; DATA1 port is used in the routine creating those page indicators
VERA_ctrl           = $9F25
VERA_ien            = $9F26
VERA_isr            = $9F27
VSYNC_BIT           = $01
VERA_dc_video       = $9F29 ; DC = Display Composer
VERA_dc_hscale      = $9F2A
VERA_dc_vscale      = $9F2B
;VERA_dc_border      = $9F2C
VERA_L0_config      = $9F2D
VERA_L0_mapbase     = $9F2E
VERA_L0_tilebase    = $9F2F ; constant in normal mode, gets changed in half mode
VERA_L0_hscroll_l   = $9F30
VERA_L0_hscroll_h   = $9F31
VERA_L0_vscroll_l   = $9F32
VERA_L0_vscroll_h   = $9F33
VERA_L1_config      = $9F34
VERA_L1_mapbase     = $9F35
VERA_L1_tilebase    = $9F36
VERA_L1_hscroll_l   = $9F37
VERA_L1_hscroll_h   = $9F38
VERA_L1_vscroll_l   = $9F39
VERA_L1_vscroll_h   = $9F3A
VRAM_sprattr       = $1FC00 ; 17 Bit adr.
; Kernal
REBOOT_X16          = $FEC9
MEMORY_FILL         = $FEE4
MEMORY_COPY         = $FEE7
clock_set_date_time = $FF4D
clock_get_date_time = $FF50
JOYSTICK_SCAN       = $FF53
JOYSTICK_GET        = $FF56
GETIN               = $FFE4
; OPEN                = $FFC0
; CLOSE               = $FFC3
; READST              = $FFB7
SETLFS              = $FFBA
SETNAM              = $FFBD
LOAD                = $FFD5
;SAVE                = $FFD8
BSAVE               = $FEBA
; CHKOUT              = $FFC9
; CLRCHN              = $FFCC
; PETSCII
;CHAR_Q              = $51
CHAR_ISOMODE         = $0F
; CHAR_PETSCIIMODE
CHROUT              = $FFD2
;
default_irq_vector: .addr 0 ; gets swapped out depending on mode
use_key_vector:     .addr 0 ; gets swapped out depending on mode
;
; kernal zp space -> include "dot_zp.inc"
;
;=========================================  end x16.inc  =====================
;
;=============================================================================
;                                       DOTMATRIX individual
;
; VRAM 17Bit Addresses
; maps must align to 512B address
VRAM_layer0_map     = $00000
VRAM_layer1_map     = $04000 ;
; Tiles or Bitmaps must align to 2KB address
VRAM_frametiles     = $08000 ; 8 tilesets with 2 (16x16) tiles = 16 tiles @256B = 4096B ($1000)
VRAM_backtiles      = $0C000 ; 8 tilesets with 8 (8x8) tiles = 64 tiles @64B = 512B
VRAM_vlineSPR       = $10000 ; 8 (8x64) sprites (reserve up to 16)
VRAM_charsprites    = $11000 ; 8 (64x8) sprites
VRAM_logo           = $12000 ; 20 tiles, mutiple sizes
VRAM_cursor         = $13000 ; 16 (32x8) tiles
VRAM_hcursor        = $14000 ; 16 (16x8) tiles
VRAM_seqaniSPR      = $14800 ; 4 (8x8) sprites
VRAM_seqinfoSPR     = $15000 ;
VRAM_halfframes     = $16000 ; 8 tilesets with 2 (8x8) tiles =
;  --unused-- ->    = $16800 ; 2KB free                            ;
VRAM_halftiles      = $17000 ; 4 tilesets with 96+ x special (8x8) tiles =
;VRAM_syschars      = $1F000 ; (VERA build in )
;
; usr text RAM:
usrtxt0             = $5000 ; RAM for user txt storage,
usrtxt1             = $5400 ; needs d800 byte = $320 byte for each page
usrtxt2             = $5800 ; + additional 16 byte for settings sorage per page
usrtxt3             = $5C00 ;
usrtxt4             = $6000
usrtxt5             = $6400
usrtxt6             = $6800
usrtxt7             = $6C00
systxt0             = $7000 ; RAM for temporary sys txt storage, needs d800 byte = $320 byte
; Remark:
; mode storage:      txtRAM + $320
; npreset storage:   txtRAM + $321
; hpreset storage:   txtRAM + $328
; ...
; ...
; xxxxxx storage:    txtRAM + $32F
;
;===========================
; ; Zero-Page userspace
; ; temporary pointers
; -> include "dot_zp.inc"
;===========================
no_intro:               .byte $00
crsr_posx:              .byte $00 ; in digits
crsr_posy:              .byte $00
vlineSPR_on:            .byte $00
service_mode:           .byte $00
X16logo_on:             .byte $00
;
max_line_no:            .byte $0A   ;$0A normal mode, half mode= $14
max_char_p_line:        .byte $14   ;$14 normal mode, half mode= $28
max_line_no_mone:       .byte $09   ;$09 normal mode, half mode= $13
max_char_p_line_mone:   .byte $13   ;$13 normal mode, half mode= $27
;
joy_wait_time:          .byte $07
;padding:           .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
;
; REM: include files may include further include files !!!
.include "dot_zp.inc"
.include "vectors.inc"
.include "random.inc"
.include "textutils3.inc"
.include "grafics.inc"
.include "create_crsrsprites.inc"
.include "charcodes.inc"
.include "pattern.inc"
.include "decoder.inc"
.include "dcvideo.inc"
.include "dotintro.inc"
.include "postinsert.inc"
.include "dotmenu.inc"
.include "halfsize.inc"
.include "info_sprite_tools.inc"
.include "create_info_sprites.inc"
.include "cursormove.inc"
.include "keyboard.inc"
.include "sequencer.inc"
;.include "dcvideo.inc"
.include "irq_handler.inc"
.include "dotinit.inc"
;
filename: .byte "@:dottext.txt" ; '@:' = overwrite
end_filename:
;======================================================
.endif
