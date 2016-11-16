
.PHONY: tests clean
SHELL=bash

tests:
	mkdir -p build
	if [[ ! -f build/Makefile ]]; then cmake -H. -Bbuild; fi
	$(MAKE) -C build csvtoolstests
	cd tests/csvtoolstests && ../../build/csvtoolstests --log_level=error

clean:
	rm -Rf build

