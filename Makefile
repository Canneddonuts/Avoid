clang :
	clang src/Main.c -std=c99 -Wall -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o Avoid

tcc :
	tcc src/Main.c -std=c99 -Wall -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o Avoid

gcc :
	gcc src/Main.c -std=c99 -Wall -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o Avoid
