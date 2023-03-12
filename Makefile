run: build
	./a.out

build: main.o AST.o
	g++ -g main.o AST.o

main.o: main.cpp
	g++ -c main.cpp

AST.o: AST.h AST.cpp
	g++ -c AST.cpp
