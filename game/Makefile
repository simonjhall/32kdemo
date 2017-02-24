CPPFLAGS = -mcpu=68000 -g -O3 -fno-exceptions -nostartfiles -std=c++11 -fno-asynchronous-unwind-tables -fno-rtti
CPPFLAGS_DADDY = -D__DADDY__=1
CPPFLAGS_EASY = -D=__EASY__

HEADERS = game/small_font_0.h game/small_font.h game/screen.h game/common.h game/common.h game/MiniGame.h game/PacmanGame.cpp game/ScreenDemo.h game/Test3D.h game/Cube.h game/debugger/debugger.h

SOURCES = game/game.cpp game/text.cpp game/MiniGame.cpp game/PacmanGame.cpp game/ScreenDemo.cpp game/screen_shared.cpp game/Test3D.cpp game/Cube.cpp game/debugger/debugger.cpp game/debugger/vector.S
SOURCES_EASY = game/easy/screen.cpp
SOURCES_DADDY = game/daddy/screen.cpp game/daddy/ipc.cpp

all: game_daddy.elf game_daddy.bin game_easy.S68 game_easy.L68 game_daddy.bin
clean:
	rm -f game_daddy.elf game_easy.elf game_easy.S68 game_easy.L68 game_daddy.bin

game_easy.S68: game_easy.elf
	m68k-elf-objcopy -O srec $< $@

game_easy.L68: game_easy.elf
	m68k-elf-nm game_easy.elf | grep 'T _start' > game_easy.L68
	m68k-elf-objdump.exe -S game_easy.elf | sed 's/://g' | sed 's/   8/0008/g' >> game_easy.L68

game_easy.elf: $(HEADERS) $(SOURCES) $(SOURCES_EASY) game/script.lds
	m68k-elf-g++ $(SOURCES) $(SOURCES_EASY) -o $@ -T game/script.lds $(CPPFLAGS) $(CPPFLAGS_EASY)

game_daddy.elf: $(HEADERS) $(SOURCES) $(SOURCES_DADDY) game/script.lds
	m68k-elf-g++ $(SOURCES) $(SOURCES_DADDY) -o $@ -T game/script.lds $(CPPFLAGS) $(CPPFLAGS_DADDY)

game_daddy.bin: game_daddy.elf
	m68k-elf-objcopy.exe -S -O binary game_daddy.elf game_daddy.bin
