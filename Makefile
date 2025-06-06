CC = gcc
CCFLAGS = -Wall -iquote src/include

build : main.o vector.o
	mkdir -p build
	$(CC) $(CCFLAGS) out/main/main.o out/main/vector.o -o build/main.exe

main.o : src/main/main.c src/include/vector.h
	mkdir -p out/main
	$(CC) $(CCFLAGS) -c src/main/main.c -o out/main/main.o

vector.o : src/main/vector.c src/include/vector.h
	mkdir -p out/main
	$(CC) $(CCFLAGS) -c src/main/vector.c -o out/main/vector.o 

test : build_test run_test

build_test : test_main.o test_vector.o vector.o
	mkdir -p build
	$(CC) $(CCFLAGS) out/test/main.o out/main/vector.o -o build/test_main.exe -lcunit

run_test : build_test
	valgrind -s --leak-check=full ./build/test_main.exe

test_main.o : src/test/main.c test_vector.o
	mkdir -p out/test
	$(CC) $(CCFLAGS) -c src/test/main.c -o out/test/main.o

test_vector.o : src/test/test_vector.c
	mkdir -p out/test
	$(CC) $(CCFLAGS) -c src/test/test_vector.c -o out/test/test_vector.o

clean :
	rm -rf out build
