run: build
	./table-gen

build: main.o AST.o
	g++ main.o AST.o -o table-gen -std=c++11

main.o: main.cpp
	g++ -c main.cpp -std=c++11

AST.o: AST.h AST.cpp
	g++ -c AST.cpp -std=c++11
