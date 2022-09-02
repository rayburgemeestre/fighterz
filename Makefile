SHELL:=/bin/bash

build_:
	mkdir -p build
	CMAKE_EXE_LINKER_FLAGS=-fuse-ld=gold CXX=$$(which c++) cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -GNinja -B build
	cmake --build build
	strip --strip-debug build/server
	strip --strip-debug build/client
	strip --strip-debug build/mapmaker

debug:
	make clean
	mkdir -p build
	pushd build && ASAN_SYMBOLIZER_PATH=/usr/lib/llvm-12/bin/llvm-symbolizer ASAN_OPTIONS=symbolize=1 CXX=$$(which c++) cmake -DSANITIZER=1 -DDEBUG=on ..
	pushd build && ASAN_SYMBOLIZER_PATH=/usr/lib/llvm-12/bin/llvm-symbolizer ASAN_OPTIONS=symbolize=1 make VERBOSE=1 -j $$(nproc)
	ln -fs $$PWD/build/compile_commands.json $$PWD/compile_commands.json

format:
	cmake --build build --target clangformat

clean:
	rm -rf build
