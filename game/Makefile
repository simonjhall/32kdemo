CPPFLAGS = -mcpu=68000 -g -Os -fno-exceptions -nostartfiles -std=c++11
CPPFLAGS_DADDY = -D__DADDY__=1
CPPFLAGS_EASY = -D=__EASY__

all: game_daddy.elf game_daddy.bin game_easy.S68 game_easy.L68 game_daddy.bin
clean:
	rm -f game_daddy.elf game_easy.elf game_easy.S68 game_easy.L68 game_daddy.bin

game_easy.S68: game_easy.elf
	m68k-elf-objcopy -O srec $< $@

game_easy.L68: game_easy.elf
	m68k-elf-nm game_easy.elf | grep 'T _start' > game_easy.L68
	m68k-elf-objdump.exe -S game_easy.elf | sed 's/://g' | sed 's/   /000/g' >> game_easy.L68

game_easy.elf: game/game.cpp game/script.lds game/easy/screen.cpp game/small_font_0.h game/small_font.h
	m68k-elf-g++ game/game.cpp game/easy/screen.cpp -o $@ -T game/script.lds $(CPPFLAGS) $(CPPFLAGS_EASY)

game_daddy.elf: game/game.cpp game/script.lds game/daddy/screen.cpp game/daddy/ipc.cpp game/small_font_0.h game/small_font.h
	m68k-elf-g++ game/game.cpp game/daddy/screen.cpp game/daddy/ipc.cpp -o $@ -T game/script.lds $(CPPFLAGS) $(CPPFLAGS_DADDY)

game_daddy.bin: game_daddy.elf
	m68k-elf-objcopy.exe -S -O binary game_daddy.elf game_daddy.bin
