CPPFLAGS = -mcpu=68000 -g -O3 -fno-exceptions -nostartfiles -std=c++11
CPPFLAGS_DADDY = -D__DADDY__=1
CPPFLAGS_EASY = -D=__EASY__

all: game_daddy.elf game_easy.S68
clean:
	rm -f game_daddy.elf game_easy.elf game_easy.S68

game_easy.S68: game_easy.elf
	m68k-elf-objcopy -O srec $< $@

game_easy.elf: game/game.cpp game/script.lds game/easy/screen.cpp
	m68k-elf-g++ game/game.cpp game/easy/screen.cpp -o $@ -T game/script.lds $(CPPFLAGS) $(CPPFLAGS_EASY)

game_daddy.elf: game/game.cpp game/script.lds game/daddy/screen.cpp game/daddy/ipc.cpp
	m68k-elf-g++ game/game.cpp game/daddy/screen.cpp game/daddy/ipc.cpp -o $@ -T game/script.lds $(CPPFLAGS) $(CPPFLAGS_DADDY)
