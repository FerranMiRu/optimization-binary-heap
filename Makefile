all: ordered_linked_list.exe BH1.exe BH2.exe

ordered_linked_list.exe: src/ordered_linked_list.c create_executables_dir
	gcc -Wall $< -o executables/$@ -lm

BH1.exe: src/BH1.c src/binary_heap.c src/queue.c src/binary_heap.h src/queue.h src/utils.h create_executables_dir
	gcc -Wall src/BH1.c src/binary_heap.c src/queue.c -o executables/$@ -lm

BH2.exe: src/BH2.c src/binary_heap.c src/queue.c src/binary_heap.h src/queue.h src/utils.h create_executables_dir
	gcc -Wall src/BH2.c src/binary_heap.c src/queue.c -o executables/$@ -lm

create_executables_dir:
	mkdir -p executables

clean:
	rm -rf executables/
	rm -rf *.dSYM

.PHONY: all clean create_executables_dir
