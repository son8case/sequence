.PHONY: usage cmake ninja ctest clean allin build tests

usage:
	cat Makefile.usage.txt

cmake:
	cd build && cmake -GNinja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .. && cd ..

ninja:
	cd build && ninja && cd ..

ctest:
	ctest --test-dir build --output-on-failure

clean:
	rm -rf build/ && git restore build/.gitignore

allin:
	make clean && make cmake && make ninja && make ctest

build:
	make cmake && make ninja

tests:
	make ninja && make ctest
