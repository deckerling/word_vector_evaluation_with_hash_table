CFLAGS := -g -Wall
BUILDDIR := build
SRCS := $(wildcard src/*.cc)
HDR := $(wildcard src/*.h)

all: builddir wvewht

builddir:
	mkdir -p $(BUILDDIR)
wvewht: $(OBJS)
	g++ $(SRCS) $(HDR) -o $(BUILDDIR)/wvewht $(CFLAGS)

clean:
	rm -rf wvewht build
