all:
	g++ code-generator.cpp code-generator.h main.cpp node.cpp node.h nonterminal.cpp nonterminal.h parser.cpp parser.h scanner.cpp scanner.h scope.cpp scope.h token.cpp token.h util.cpp util.h -o compfs -std=c++11
clean:
	rm compfs *.asm
