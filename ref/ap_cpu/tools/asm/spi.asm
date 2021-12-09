;------------------------------------------
; | 0000 - 0fff |    ROM                 |
; | 1000 - 1fff |    RAM                 |
; | 2000        | SPI Control            |
; | 2001        | SPI TX Data            |
; | 2002        | SPI RX Data            |
; | 3000 - ffff |    SRAM                |
;------------------------------------------

#define ILI9341_NOP        0x00
#define ILI9341_SWRESET    0x01
#define ILI9341_RDDID      0x04
#define ILI9341_RDDST      0x09
#define ILI9341_SLPIN      0x10
#define ILI9341_SLPOUT     0x11
#define ILI9341_PTLON      0x12
#define ILI9341_NORON      0x13
#define ILI9341_RDMODE     0x0A
#define ILI9341_RDMADCTL   0x0B
#define ILI9341_RDPIXFMT   0x0C
#define ILI9341_RDIMGFMT   0x0A
#define ILI9341_RDSELFDIAG 0x0F
#define ILI9341_INVOFF     0x20
#define ILI9341_INVON      0x21
#define ILI9341_GAMMASET   0x26
#define ILI9341_DISPOFF    0x28
#define ILI9341_DISPON     0x29
#define ILI9341_CASET      0x2A
#define ILI9341_PASET      0x2B
#define ILI9341_RAMWR      0x2C
#define ILI9341_RAMRD      0x2E
#define ILI9341_PTLAR      0x30
#define ILI9341_MADCTL     0x36
#define ILI9341_PIXFMT     0x3A
#define ILI9341_FRMCTR1    0xB1
#define ILI9341_FRMCTR2    0xB2
#define ILI9341_FRMCTR3    0xB3
#define ILI9341_INVCTR     0xB4
#define ILI9341_DFUNCTR    0xB6
#define ILI9341_PWCTR1     0xC0
#define ILI9341_PWCTR2     0xC1
#define ILI9341_PWCTR3     0xC2
#define ILI9341_PWCTR4     0xC3
#define ILI9341_PWCTR5     0xC4
#define ILI9341_VMCTR1     0xC5
#define ILI9341_VMCTR2     0xC7
#define ILI9341_RDID1      0xDA
#define ILI9341_RDID2      0xDB
#define ILI9341_RDID3      0xDC
#define ILI9341_RDID4      0xDD
#define ILI9341_GMCTRP1    0xE0
#define ILI9341_GMCTRN1    0xE1
#define ILI9341_PWCTR6     0xFC

#define MADCTL_MY          0x80
#define MADCTL_MX          0x40
#define MADCTL_MV          0x20
#define MADCTL_ML          0x10
#define MADCTL_RGB         0x00
#define MADCTL_BGR         0x08
#define MADCTL_MH          0x04

; MADCTL_MV | MADCTL_MX | MADCTL_BGR
#define TMP_MADCTL	0x68

init:
	move	r14, 0x1010		; set stack pointer

	call	delay_long
	call	delay_long

init_tft:
	call	spi_select

	call	delay

	move	r0, 0xef
	call	spi_sendCommand
	move	r0, 0x03
	call	spi_sendData
	move	r0, 0x80
	call	spi_sendData
	move	r0, 0x02
	call	spi_sendData

	call	delay

	move	r0, 0xcf
	call	spi_sendCommand
	move	r0, 0x00
	call	spi_sendData
	move	r0, 0xc1
	call	spi_sendData
	move	r0, 0x30
	call	spi_sendData

	call	delay

	move	r0, 0xed
	call	spi_sendCommand
	move	r0, 0x64
	call	spi_sendData
	move	r0, 0x03
	call	spi_sendData
	move	r0, 0x12
	call	spi_sendData
	move	r0, 0x81
	call	spi_sendData

	call	delay

	move	r0, 0xe8
	call	spi_sendCommand
	move	r0, 0x85
	call	spi_sendData
	move	r0, 0x00
	call	spi_sendData
	move	r0, 0x78
	call	spi_sendData

	call	delay

	move	r0, 0xcb
	call	spi_sendCommand
	move	r0, 0x39
	call	spi_sendData
	move	r0, 0x2c
	call	spi_sendData
	move	r0, 0x00
	call	spi_sendData
	move	r0, 0x34
	call	spi_sendData
	move	r0, 0x02
	call	spi_sendData

	call	delay

	move	r0, 0xf7
	call	spi_sendCommand
	move	r0, 0x20
	call	spi_sendData

	call	delay

	move	r0, 0xea
	call	spi_sendCommand
	move	r0, 0x00
	call	spi_sendData
	move	r0, 0x00
	call	spi_sendData

	call	delay

	move	r0, ILI9341_PWCTR1
	call	spi_sendCommand
	move	r0, 0x23
	call	spi_sendData

	call	delay

	move	r0, ILI9341_PWCTR2
	call	spi_sendCommand
	move	r0, 0x10
	call	spi_sendData

	call	delay

	move	r0, ILI9341_VMCTR1
	call	spi_sendCommand
	move	r0, 0x3e
	call	spi_sendData
	move	r0, 0x28
	call	spi_sendData

	call	delay

	move	r0, ILI9341_VMCTR2
	call	spi_sendCommand
	move	r0, 0x86
	call	spi_sendData

	call	delay

	move	r0, ILI9341_MADCTL
	call	spi_sendCommand
	move	r0, TMP_MADCTL ; MADCTL_MV | MADCTL_MX | MADCTL_BGR
	call	spi_sendData

	call	delay

	move	r0, ILI9341_PIXFMT
	call	spi_sendCommand
	move	r0, 0x55
	call	spi_sendData

	call	delay

	move	r0, ILI9341_FRMCTR1
	call	spi_sendCommand
	move	r0, 0x00
	call	spi_sendData
	move	r0, 0x18
	call	spi_sendData

	call	delay

	move	r0, ILI9341_DFUNCTR
	call	spi_sendCommand
	move	r0, 0x08
	call	spi_sendData
	move	r0, 0x82
	call	spi_sendData
	move	r0, 0x27
	call	spi_sendData

	call	delay

	move	r0, 0xf2
	call	spi_sendCommand
	move	r0, 0x00
	call	spi_sendData

	call	delay

	move	r0, ILI9341_GAMMASET
	call	spi_sendCommand
	move	r0, 0x01
	call	spi_sendData

	call	delay

	move	r0, ILI9341_GMCTRP1
	call	spi_sendCommand
	move	r0, 0x0f
	call	spi_sendData
	move	r0, 0x31
	call	spi_sendData
	move	r0, 0x2b
	call	spi_sendData
	move	r0, 0x0c
	call	spi_sendData
	move	r0, 0x0e
	call	spi_sendData
	move	r0, 0x08
	call	spi_sendData
	move	r0, 0x4e
	call	spi_sendData
	move	r0, 0xf1
	call	spi_sendData
	move	r0, 0x37
	call	spi_sendData
	move	r0, 0x07
	call	spi_sendData
	move	r0, 0x10
	call	spi_sendData
	move	r0, 0x03
	call	spi_sendData
	move	r0, 0x0e
	call	spi_sendData
	move	r0, 0x09
	call	spi_sendData
	move	r0, 0x00
	call	spi_sendData

	call	delay

	move	r0, ILI9341_GMCTRN1
	call	spi_sendCommand
	move	r0, 0x00
	call	spi_sendData
	move	r0, 0x0e
	call	spi_sendData
	move	r0, 0x14
	call	spi_sendData
	move	r0, 0x03
	call	spi_sendData
	move	r0, 0x11
	call	spi_sendData
	move	r0, 0x07
	call	spi_sendData
	move	r0, 0x31
	call	spi_sendData
	move	r0, 0xc1
	call	spi_sendData
	move	r0, 0x48
	call	spi_sendData
	move	r0, 0x08
	call	spi_sendData
	move	r0, 0x0f
	call	spi_sendData
	move	r0, 0x0c
	call	spi_sendData
	move	r0, 0x31
	call	spi_sendData
	move	r0, 0x36
	call	spi_sendData
	move	r0, 0x0f
	call	spi_sendData

	call	delay

	move	r0, ILI9341_SLPOUT
	call	spi_sendCommand

	call	delay

	move	r0, ILI9341_DISPON
	call	spi_sendCommand

	call	delay_long

	; Finished intialize screen,
	; start contineously updating screen
	; with framebuffer content.

	move	r1, 0x0000
main:
	; Set address window.
	move	r0, ILI9341_CASET
	call	spi_sendCommand
	move	r0, 0
	call	spi_sendData
	move	r0, 0
	call	spi_sendData
	move	r0, 0x01
	call	spi_sendData
	move	r0, 0x3f
	call	spi_sendData

	call	delay

	move	r0, ILI9341_PASET
	call	spi_sendCommand
	move	r0, 0
	call	spi_sendData
	move	r0, 0
	call	spi_sendData
	move	r0, 0x00
	call	spi_sendData
	move	r0, 0xef
	call	spi_sendData

	call	delay

	move	r0, ILI9341_RAMWR
	call	spi_sendCommand



	; Initialize SRAM
	move	r4, 0x3000
_init_sram:
	store	r4, r4
	inc 	r4
	cmpeq	r4, 0xffff
	jmpz    _init_sram



	; Copy SRAM to screen
	move	r4, 0x3000

	move	r3, 240
fill_y:
	move	r2, 320
fill_x:

	load	r5, r4

	move	r0, r5
	call	spi_sendData

	move	r0, r5
	shr 	r0, 8
	call	spi_sendData

	inc 	r4
	cmpeq	r4, 0xffff
	jmpz    _not_end
	move 	r4, 0x3000
_not_end:

	dec		r2
	move	r0, r2
	jmpnz   fill_x
	dec		r3
	move	r0, r3
	jmpnz	fill_y

	inc 	r1
	jmp		main


;------------------------------------------
; ~150 ms
;------------------------------------------
delay_long:
	push	r0
	move 	r0, 0x0a
_loop_lng:
	call	delay
	dec		r0
	jmpnz	_loop_lng	
	pop		r0
	ret


;------------------------------------------
; ~15 ms
;------------------------------------------
delay:
	push	r0
	move 	r0, 0x2000
_loop_shrt:
	dec		r0
	jmpnz	_loop_shrt	
	pop		r0
	ret


;------------------------------------------
;------------------------------------------
spi_select:
	load	r0, 0x2000
	or		r0, 0b00000100
	store	0x2000, r0
	ret


;------------------------------------------
;------------------------------------------
spi_unselect:
	load	r0, 0x2000
	and		r0, ~0b00000100
	store	0x2000, r0
	ret


;------------------------------------------
; R0 = Command to send
;------------------------------------------
spi_sendCommand:
	store	0x2001, r0  ; Load data into SPI TX register.

	load	r0, 0x2000
	or		r0, 0b00000001
	and		r0, 0b00000101
	store	0x2000, r0 	; Execute

_spi_sendCommand_wui:
	load	r0, 0x2000
	and		r0, 0b00000001
	jmpnz	_spi_sendCommand_wui

	ret


;------------------------------------------
; R0 = Data to send
;------------------------------------------
spi_sendData:
	store	0x2001, r0  ; Load data into SPI TX register.

	load	r0, 0x2000
	or		r0, 0b00000011
	store	0x2000, r0 	; Data|Execute

_spi_sendData_wui:
	load	r0, 0x2000
	and		r0, 0b00000001
	jmpnz	_spi_sendData_wui

	ret
