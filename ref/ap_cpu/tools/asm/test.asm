;------------------------------------------
; | 0000 - 00ff |    ROM                 |
; | 0100 - 01ff |    RAM                 |
; | 0200        | SPI Control            |
; | 0201        | SPI TX Data            |
; | 0202        | SPI RX Data            |
;------------------------------------------

	move	r0, 0x12345678
	move	r1, 0x11111111

init:
	inc		r0
	dec		r1
	jmp		init
