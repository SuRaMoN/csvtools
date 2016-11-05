# Csvtools
[![Build Status](https://travis-ci.org/SuRaMoN/csvtools.png?branch=master)](https://travis-ci.org/SuRaMoN/csvtools)

Basic CSV reader for c++. Compiles on linux and w$ndows. It is a header
only implementation that uses wchar by default. The boost libraries are
the only dependecies.

# How to compile?
It's header only, so it can't compile.
If you want to run the tests, just run ./run_tests.sh it will build and run.
If you want to compile them on w$indows, call cmake and it will create a sln file for you.

# Example usage
```
BOOST_FOREACH(const csv_line & line, csv_reader::new_from_utf8_file(path("file.csv"))) {
    line[0]; // will return first field
}
```

See the test directory for more examples
