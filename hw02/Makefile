all: hw0201.c hw0202.c hw0203.c matrix.c
	gcc hw0201.c matrix.c -o hw0201 -lm
	gcc hw0202.c -o hw0202
	gcc hw0203.c -o hw0203
	gcc hw0204.c -o hw0204
	# gcc hw0205.c

debug: hw0204.c
	gcc hw0204.c -g -o test

clean: hw0201 hw0202 hw0203
	rm hw0201
	rm hw0202
	rm hw0203
	rm hw0204
	# rm hw0205
