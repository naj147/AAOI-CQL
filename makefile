all:

	flex -i cassandra.l
	gcc -c lex.yy.c
	gcc -c cassandra.c
	gcc -c error.c
	gcc -o zzc cassandra.o lex.yy.o error.o -lfl -ljansson
	./zzc<decl
