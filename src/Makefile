NAME = DOTMAT
ASSEMBLER6502 = cl65
ASFLAGS = -t cx16 -l $(NAME).list

PROG = $(NAME).PRG
LIST = $(NAME).list
MAIN = dotmatrix10.asm
SOURCES = $(MAIN) \
	dot_zp.inc \
	dotsysdeclarations.inc \
	vectors.inc \
	vera_helpers.inc \
	random.inc \
	textutils3.inc \
	dotcolors.inc \
	presets.inc \
	create_20x10mode_tilesets.inc \
	create_crsrsprites.inc \
	charcodes.inc \
	pattern.inc \
	decoder.inc \
	logo.inc \
	dcvideo.inc \
	dotintro.inc \
	menutxt.inc \
	dotmenukeys.inc \
	postinsert.inc \
	dotmenu.inc \
	create_40x20mode_tilesets.inc \
	info_sprite_tools.inc \
	create_info_sprites.inc \
	cursormove.inc \
	keyboard.inc \
	sequencer.inc \
	irq_handler.inc \
	dotcopyright.inc \
	dotinit.inc \

all: $(PROG)

# resources: $(RESOURCES)
$(PROG): $(SOURCES)
	$(ASSEMBLER6502) $(ASFLAGS) -o $(PROG) $(MAIN)

run: all #resources
	x16emu -prg $(PROG) -run
#	x16emu -keymap de -rockwell -prg $(PROG) -run
#	x16emu -keymap de -prg $(PROG) -run
#	x16emu -debug -keymap de -prg $(PROG) -run
#	x16emu -scale 2 -prg $(PROG) -run

clean:
	rm -f $(PROG) $(LIST)
	rm -f DOTTEXT.TXT
