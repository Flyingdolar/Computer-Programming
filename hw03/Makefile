.PHONY: clean debug

all:
	if [ -f hw0301.c ]; then gcc hw0301.c -o hw0301 -lm; fi
	if [ -f hw0302.c ]; then gcc hw0302.c zip.c -o hw0302 -lm; fi
	if [ -f hw0303.c ]; then gcc hw0303.c bmp.c -o hw0303 -lm; fi
	if [ -f hw0304.c ]; then gcc hw0304.c -o hw0304 -lm; fi
	if [ -f hw0305.c ]; then gcc hw0305.c -o hw0305 -lm; fi

debug:
	if [ ! -d out ]; then mkdir out; fi
	if [ -f hw0301.c ]; then gcc hw0301.c -D DEBUG -Wall -Wextra -g -o out/hw0301; fi
	if [ -f hw0302.c ]; then gcc hw0302.c zip.c -D DEBUG -Wall -Wextra -g -o out/hw0302; fi
	if [ -f hw0303.c ]; then gcc hw0303.c bmp.c -D DEBUG -Wall -Wextra -g -o out/hw0303; fi
	if [ -f hw0304.c ]; then gcc hw0304.c -D DEBUG -Wall -Wextra -g -o out/hw0304; fi
	if [ -f hw0305.c ]; then gcc hw0305.c -D DEBUG -Wall -Wextra -g -o out/hw0305; fi

clean:
	if [ -d out ]; then rm -rf out; fi
	if [ -f hw0301 ]; then rm hw0301; fi
	if [ -f hw0302 ]; then rm hw0302; fi
	if [ -f hw0303 ]; then rm hw0303; fi
	if [ -f hw0304 ]; then rm hw0304; fi
	if [ -f hw0305 ]; then rm hw0305; fi
