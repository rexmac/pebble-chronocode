# Makefile for multi-language Pebble WordSquare
#
# To build for a specific langauge:
#
#   $ make lang=xx_XX
#
# where xx_XX is a language identifier in POSIX locale format,
# e.g. en_US, de_DE, etc.
#
.PHONY: all build clean install

lang ?= "en_US"

all: build

build: clean
	@echo "Building WordSquare watchface for language: $(lang)"
	@cp -r ./src ./pebble-src
	@sed -i s/@LANG@/$(lang)/g ./pebble-src/wordsquare.c
	@pebble build

clean:
	@rm -rf ./pebble-src

install:
	@pebble install --logs --phone $(phone)

