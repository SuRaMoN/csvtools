#!/bin/sh

set -e

cmake -Htests/csvtoolstests -Bbuild
make -C build
cd tests/csvtoolstests/
../../build/csvtoolstests --log_level=all

