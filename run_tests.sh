#!/bin/sh

cmake -Htests/csvtoolstests -Bbuild && make -C build && make -C build test

