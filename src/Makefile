image_inverter:main
	gcc main.o complement.o arraytoimage.o dynamic.o utils.o -o image_inverter
main:complement utils
	gcc -std=c99 -c main.c -o main.o
complement:arraytoimage dynamic
	gcc -std=c99 -c complement.c -o complement.o
arraytoimage:
	gcc -std=c99 -c arraytoimage.c -o arraytoimage.o
dynamic:
	gcc -std=c99 -c dynamic.c -o dynamic.o
utils:
	gcc -std=c99 -c utils.c -o utils.o