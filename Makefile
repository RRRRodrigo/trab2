all:
	gcc -omain main.c imageprocessing.c -I./ -lfreeimage
	gcc -omultiprocess main_multiprocesso.c imageprocessing.c -g -I./ -lfreeimage
	gcc -omultithread main_multithread.c imageprocessing.c -I./ -lfreeimage -lpthread


