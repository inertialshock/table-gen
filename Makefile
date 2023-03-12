run: build
	./table-gen

build: main.o AST.o
	g++ -g main.o AST.o -o table-gen

main.o: main.cpp
	g++ -c main.cpp

AST.o: AST.h AST.cpp
	g++ -c AST.cpp
