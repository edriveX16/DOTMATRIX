.ifndef PATTERN_INC
PATTERN_INC  = 1
; pattern to tile lookup table
;
; Pattern encoding: Muster = two 8s tiles only half character, left half
; lower 6 bit: tileno.
; bit7: v-flip
; bit6: h-flip
;===========================
; adpot when pattern table 'Muster' is growing:
MStR_tile_offset    =    $1A
;===========================
Muster:
;ooo ;Muster 0
;ooo
.byte $00, $00, $00, $00
; upper row
; $00, $00
; lower row
; $00, $00
;
;o** ;Muster 1              =*oo   ,if flipped horizontal
;*oo                        =o**
.byte $00, $83, $01, $00
;
;*** ;Muster 2              =***    , if flipped
;*oo                        =o**
.byte $01, $83, $01, $00
;
;*oo ;Muster 3              =o**
;*oo                        =o**
.byte $01, $00, $01, $00
;
;o** ;Muster 4
;oo*
.byte $00, $01, $00, $42
;
;*** ;Muster 5
;ooo
.byte $01, $83, $00, $00
;
;ooo ;Muster 6
;oo*
.byte $00, $00, $00, $42
;
;*o* ;Muster 7
;o*o
.byte $01, $42, $00, $02
;
;*oo ;Muster 8
;o*o
.byte $01, $00, $00, $02
;
;*** ;Muster 9
;oo*
.byte $01, $01, $00, $42
;
;oo* ;Muster 10
;oo*
.byte $00, $42, $00, $42
;
;o*o ;Muster 11
;oo*
.byte $00, $02, $00, $42
;
;o** ;Muster 12
;o*o
.byte $00, $01, $00, $02
;
;o*o ;Muster 13
;o*o
.byte $00, $02, $00, $02
;
;ooo ;Muster 14
;*o*
.byte $00, $00, $01, $42
;
;oo* ;Muster 15
;oo*
.byte $00, $42, $00, $42
;
;*oo ;Muster 16
;**o
.byte $01, $00, $01, $02
;
;oo* ;Muster 17             = M6 if v-flip  ========== possibly double
;ooo
.byte $00, $42, $00, $00
;
;*o* ;Muster 18
;o**
.byte $01, $42, $00, $01
;
;**o ;Muster 19
;**o
.byte $01, $02, $01, $02
;
;o*o ;Muster 20
;***
.byte $00, $02, $01, $01
;
;*oo ;Muster 21             =o**    , if flipped horizontal
;ooo                        =ooo
.byte $01, $00, $00, $00
;
; since lower case
;**o ;Muster 22 (ex 26)
;*o*
.byte $01, $02, $01, $42
;
;o*o ;Muster 23
;ooo
.byte $00, $02, $00, $00
;
;*o* ;Muster 24
;*oo
.byte $01, $42, $01, $00
;
;*o* ;Muster 25
;*o*
.byte $01, $42, $01, $42
;
; adopt MStR_tile_offset when adding new pattern
;==================================================================================
MStR:
;ooo  ; MStR 0
;ooo
;ooo
.byte $00, $00, $00, $00
;
;*oo    ; MStR 1            =o**, if flipped horizontal
;***                        =***
;*oo                        =o**
.byte $01, $83, $01, $03
;
;*oo    ; MStR 2            =o**  ,  if flipped horizontal
;*oo                        =o**
;*oo                        =o**
.byte $01, $00, $01, $00
;
;*oo    ; MStR 3
;*o*
;*oo
.byte $01, $C4, $01, $44
;
;oo*    ; MStR 4
;oo*
;oo*
.byte $00, $42, $00, $42
;
;*o*    ; MStR 5
;**o
;*o*
.byte $01, $45, $01, $05
;
;*o*    ; MStR 6
;*oo
;*oo
.byte $01, $44, $01, $00
;
;**o    ; MStR 7
;*o*
;*oo
.byte $01, $05, $01, $44
;
;*oo    ; MStR 8        =o** , if flipped horizontal
;o**                    =*oo
;ooo                    =ooo
.byte $03, $83, $00, $03
;
;o*o    ; MStR 9
;oo*
;o*o
.byte $00, $05, $00, $85
;
;*oo    ; MStR 10
;o*o
;oo*
.byte $03, $84, $00, $05
;
;ooo    ; MStR 11
;oo*
;o*o
.byte $00, $C4, $00, $85
;
;o*o    ; MStR 12
;ooo
;ooo
.byte $00, $04, $00, $00
;
;*oo    ; MStR 13
;ooo
;oo*
.byte $03, $00, $00, $C4
;
;ooo    ; MStR 14       =ooo , if flipped horizontal
;o**                    =*oo
;ooo                    =ooo
.byte $00, $83, $00, $03
;
;ooo    ; MStR 15
;oo*
;oo*
.byte $00, $C4, $00, $42
;
;*oo    ; MStR 16       =o** , if flipped horizontal
;o**                    =*oo
;*oo                    =o**
.byte $03, $83, $83, $03
;
;ooo    ; MStR 17
;***
;ooo
.byte $83, $83, $03, $03
;
;***    ; MStR 18
;ooo
;***
.byte $03, $03, $83, $83
;
;ooo    ; MStR 19 "/"
;oo*
;o*o
.byte $00, $C4, $00, $45
;
;ooo    ; MStR 20 "G"       =ooo , if flipped horizontal
;***                        =***
;*oo                        =0**
.byte $83, $83, $01, $03
;
;oo*    ; MStR 21  =$15
;ooo
;oo*
.byte $00, $44, $00, $C4
;
;*oo    ; MStR 22 =$16  =o** ,if flipped horizontal
;*oo                    =o**
;***                    =***
.byte $01, $00, $01, $83
;
;
;o*o    ; MStR 23
;oo*
;oo*
.byte $00, $05, $00, $42
;
;oo*    ; MStR 24
;***
;oo*
.byte $83, $46, $03, $C6
;
;oo*    ; MStR 25 =$19
;o*o
;oo*
.byte $00, $45, $00, $05
;
;ooo    ; MStR 26
;ooo
;oo*
.byte $00, $00, $00, $C4
;
;o*o    ; MStR 27
;***
;o*o
.byte $83, $06, $03, $86
;
;o**    ; MStR 28
;*o*
;oo*
.byte $83, $C6, $03, $42
;
;ooo    ; MStR 29
;o**
;*o*
.byte $00, $83, $83, $C6
;
;*oo    ; MStR 30
;*o*
;*o*
.byte $01, $C4, $01, $42
;
;***    ; MStR 31
;o*o
;***
.byte $03, $86, $83, $06
;
;*o*    ; MStR 32
;o**
;oo*
.byte $03, $46, $00, $C6
;
;*o*    ; MStR 33 = $21
;o*o
;*o*
.byte $03, $45, $83, $05
;
;o*o    ; MStR 34 = $22
;o*o
;o*o
.byte $00, $02, $00, $02
;
;ooo    ; MStR 35 = $23     =ooo   , if flipped horizontal
;ooo                        =ooo
;o**                        =*oo
.byte $00, $00, $00, $83
;
;o*o    ; MStR 36
;*oo
;***
.byte $83, $04, $01, $83
;
;o*o    ; MStR 37
;o*o
;***
.byte $00, $02, $83, $06
;
;oo*    ; MStR 38
;ooo
;*o*
.byte $00, $44, $83, $C4
;
;o**    ; MStR 39   =*oo , if flipped horizontal
;ooo                =ooo
;o**                =*oo
.byte $00, $03, $00, $83
;
; since lower case
;o**    ; MStR 40   =*oo ,  if flipped horizontal
;*oo                =o**
;***                =***
.byte $83, $03, $01, $83
;
;*o*    ; MStR 41
;**o
;*oo
.byte $01, $45, $01, $04
;
;o**    ; MStR 42 = $2A     =*oo   , if flipped horizontal
;*oo                        =o**
;*oo                        =o**
.byte $83, $03, $01, $00
;
;ooo    ; MStR 43
;oo*
;ooo
.byte $00, $C4, $00, $44
;
;***    ; MStR 44   =*** , if flipped horizontal
;*oo                =o**
;***                =***
.byte $01, $03, $01, $83
;
;o**    ; MStR 45
;oo*
;oo*
.byte $00, $C6, $00, $42
;
;o*o    ; MStR 46
;*o*
;*o*
.byte $83, $05, $01, $42
;
;o**    ; MStR 47 =$2F
;o*o
;o*o
.byte $00, $86, $00, $02
;
;**o    ; MStR 48 =$30
;*o*
;*o*
.byte $01, $05, $01, $42
;
;*o*    ; MStR 49 =$31 !!
;**o
;***
.byte $01, $85, $01, $06
;
;***    ; MStR 50 =$32
;ooo
;oo*
.byte $03, $03, $00, $C4
;
;***    ; MStR 51 =$33
;o*o
;oo*
.byte $03, $86, $00, $05
;o*o    ; MStR 52 =$34
;*oo
;o*o
.byte $83, $04, $03, $84
;
.endif
