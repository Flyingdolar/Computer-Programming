CC = gcc
INCLUDE = -lcurl -lxml2
MATH = -lm

.PHONY: clean
all:
	if [ -f hw0401.c ]; then ${CC} hw0401.c -o hw0401 ${MATH}; fi
	if [ -f hw0402.c ]; then ${CC} hw0402.c -o hw0402 ${MATH}; fi
	if [ -f hw0403.c ]; then ${CC} hw0403.c -o hw0403 ${MATH}; fi
	if [ -f hw0404.c ]; then ${CC} hw0404.c -o hw0404 ${MATH}; fi
	if [ -f hw0405.c ]; then ${CC} hw0405.c cJSON.c -o hw0405 ${MATH} ${INCLUDE}; fi

clean:
	if [ -d out ]; then rm -rf out; fi
	if [ -f hw0401 ]; then rm hw0401; fi
	if [ -f hw0402 ]; then rm hw0402; fi
	if [ -f hw0403 ]; then rm hw0403; fi
	if [ -f hw0404 ]; then rm hw0404; fi
	if [ -f hw0405 ]; then rm hw0405; fi
