output: demo.o inputbuf.o lexer.o compiler.o
	g++ demo.o inputbuf.o lexer.o compiler.o

demo.o: demo.cc
	g++ -c demo.cc

inputbuf.o: inputbuf.cc inputbuf.h
	g++ -c inputbuf.cc

lexer.o: lexer.cc lexer.h
	g++ -c lexer.cc

compiler.o: compiler.cc compiler.h
	g++ -c compiler.cc

clean:
	rm *.o
	rm a.out