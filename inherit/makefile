
all: *.o print

print:
	make -f make2file

main: $(wildcard *.o)
	gcc -o $@ $^


%.o: %.c
	gcc -c $^


.PHONY:clean

clean:
	rm main *.o
