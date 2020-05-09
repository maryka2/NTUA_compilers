.PHONY: clean distclean default

CXX=c++
CXXFLAGS=-Wall -std=c++14

default: pcl

lexer.cpp: lexer.l
	flex -s -o lexer.cpp lexer.l

lexer.o: lexer.cpp lexer.hpp ast.hpp symbol.hpp

lexer: lexer.o

parser.hpp parser.cpp: parser.ypp
	bison -dv -o parser.cpp parser.ypp

parser.o: parser.cpp lexer.hpp ast.hpp symbol.hpp type.cpp

pcl: lexer.o parser.o
	$(CXX) $(CXXFLAGS) -o pcl lexer.o parser.o

clean:
	$(RM) lexer.cpp lexer parser.cpp parser.hpp parser.output *.o

distclean: clean
	$(RM) pcl
