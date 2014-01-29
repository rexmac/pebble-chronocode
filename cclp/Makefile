CC=gcc
CFLAGS=-Wall --std=c99 -I.
IDIR=include
ODIR=src/obj
SDIR=src

_OBJ = cclp.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c
	mkdir -p $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

cclp: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o cclp
