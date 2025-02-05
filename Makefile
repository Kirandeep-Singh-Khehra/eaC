CC = gcc
HEADERS = $(wildcard *.h)
EXAMPLES_DIRS = $(patsubst %.h, examples/%, $(HEADERS))
INSTALL_DIR = /usr/local/include

.PHONY: all build install uninstall clean examples

all: build install examples

build:
	@echo "Nothing to be built"

install:
	cp $(HEADERS) $(INSTALL_DIR)
	@echo "All headers installed"

uninstall:
	for header in $(HEADERS); do \
		rm -f $(INSTALL_DIR)/$$header; \
	done
	@echo "All headers removed"

examples:
	@for dir in $(EXAMPLES_DIRS); do \
		$(MAKE) -C $$dir build; \
	done

	@echo "All examples built"

clean:
	@for dir in $(EXAMPLES_DIRS); do \
      $(MAKE) -C $$dir clean; \
	done

