all:

	flex -i cassandra.l
	gcc -c lex.yy.c
	gcc -c cassandra.c
	gcc -o zzc cassandra.o lex.yy.o -lfl
	./zzc <Syntaxes
