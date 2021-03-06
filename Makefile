SOURCES=src/game.c src/globals.c src/physics.c src/entity.c src/la.c src/util.c src/player.c src/box.c
LINUX_SOURCES=${SOURCES} src/sdl_backend.c
WASM_SOURCES=${SOURCES} 

FLAGS=-Wall -Wextra -Wswitch-enum 
LINUX_FLAGS=${FLAGS} `pkg-config sdl2 --libs --cflags`
WASM_FLAGS=${FLAGS} -fno-builtin --target=wasm32-unknown-unknown --no-standard-libraries -Wl,--no-entry -Wl,--allow-undefined -Wl,--export-all -DWASM

linux: build/game

build/game: ${LINUX_SOURCES}
	[ -d "build" ] || mkdir build
	cc ${LINUX_SOURCES} ${LINUX_FLAGS} -o build/game

wasm: web/game.wasm

web/game.wasm: ${WASM_SOURCES}
	clang ${WASM_FLAGS} ${WASM_SOURCES}
	mv a.out web/game.wasm

