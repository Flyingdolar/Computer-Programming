CC = gcc
INCLUDE = -lcurl -lxml2
MATH = -lm

.PHONY: clean
all:
	if [ -f fin01.c ]; then ${CC} fin01.c -o fin01 ${MATH}; fi
	if [ -f fin02.c ]; then ${CC} fin02.c -o fin02 ${MATH}; fi
	if [ -f fin03.c ]; then ${CC} fin03.c -o fin03 ${MATH}; fi

clean:
	if [ -d out ]; then rm -rf out; fi
	if [ -f fin01 ]; then rm fin01; fi
	if [ -f fin02 ]; then rm fin02; fi
	if [ -f fin03 ]; then rm fin03; fi
