sync: oSync
	./oSync ${DIR1} ${DIR2}
oSync: sync.o
	gcc sync.o -o oSync
sync.o: sync.c
	gcc -c sync.c
clean:
	rm -rf *o oSync
