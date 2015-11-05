# extra flags pour le link
LDFLAGS = -lm -fopenmp

# Flags pour la compilation
CFLAGS = -O3 -fopenmp

# Flags pour la compilation
CXXFLAGS = -O3 -fopenmp

CC = gcc
CXX = g++

## liste des exécutables à créer
EXEC = display_omp scalar_product fft

.PHONY: clean, mrproper

all : $(EXEC)

display_omp: display_omp.o
	$(CC) $(LDFLAGS) -o $@ $+

scalar_product: scalar_product.o
	$(CC) $(LDFLAGS) -o $@ $+

fft: fft.o
	$(CXX) $(LDFLAGS) -o $@ $+

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

%.o: %.c %.h
	$(XX) $(CXXFLAGS) -c $<

clean:
	rm -f *.o

mrproper: clean
	rm $(EXEC)
