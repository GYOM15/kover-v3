.PHONY: build clean
# Récupérer les flags pour Cairo à partir de pkg-config
CAIRO_CFLAGS = $(shell pkg-config --cflags cairo)
CAIRO_LIBS = $(shell pkg-config --libs cairo)

CFLAGS = -Wall -Wextra $(CAIRO_CFLAGS) -D_USE_MATH_DEFINES -DM_PI=3.14159265358979323846
LDFLAGS = $(CAIRO_LIBS)
exec = kover
modules = scene validation antenna construction drawing
modules_c_files = $(patsubst %,%.c,$(modules))
modules_h_files = $(patsubst %,%.h,$(modules))
modules_o_files = $(patsubst %,%.o,$(modules))

build: $(exec)

$(exec): $(exec).o $(modules_o_files)
	gcc $^ -o $@ $(LDFLAGS)

$(exec).o: $(exec).c
	gcc $(CFLAGS) -c $<

$(modules_o_files): %.o: %.c %.h
	gcc $(CFLAGS) -c $<

clean:
	rm -f $(modules_o_files)
	rm -f $(exec)