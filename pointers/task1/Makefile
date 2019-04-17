CXX_FLAGS=-Werror -Wall
all: main
./bin/main.o: ./src/main.cpp
	g++ $(CXX_FLAGS) -c ./src/main.cpp -o ./bin/main.o
main: ./bin/main.o
	g++ $(CXX_FLAGS) ./bin/main.o -o ./bin/main
clean:
	rm -f ./bin/main ./bin/*.o
.PHONY: clean all