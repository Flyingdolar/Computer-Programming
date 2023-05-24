.PHONY: clean debug

all:
	if [ -f hw0401.c ]; then gcc hw0401.c -o hw0401 -lm; fi
	if [ -f hw0402.c ]; then gcc hw0402.c zip.c -o hw0402 -lm; fi
	if [ -f hw0403.c ]; then gcc hw0403.c bmp.c -o hw0403 -lm; fi
	# if [ -f hw0404.c ]; then gcc hw0404.c -o hw0404 -lm; fi
	if [ -f hw0405.c ]; then gcc hw0405.c midi.c -o hw0405 -lm; fi

debug:
	if [ ! -d out ]; then mkdir out; fi
	if [ -f hw0401.c ]; then gcc hw0401.c -D DEBUG -Wall -Wextra -g -o out/hw0401; fi
	if [ -f hw0402.c ]; then gcc hw0402.c zip.c -D DEBUG -Wall -Wextra -g -o out/hw0402; fi
	if [ -f hw0403.c ]; then gcc hw0403.c bmp.c -D DEBUG -Wall -Wextra -g -o out/hw0403; fi
	if [ -f hw0404.c ]; then gcc hw0404.c -D DEBUG -Wall -Wextra -g -o out/hw0404; fi
	if [ -f hw0405.c ]; then gcc hw0405.c midi.c -D DEBUG -Wall -Wextra -g -o out/hw0405; fi

clean:
	if [ -d out ]; then rm -rf out; fi
	if [ -f hw0401 ]; then rm hw0401; fi
	if [ -f hw0402 ]; then rm hw0402; fi
	if [ -f hw0403 ]; then rm hw0403; fi
	if [ -f hw0404 ]; then rm hw0404; fi
	if [ -f hw0405 ]; then rm hw0405; fi
