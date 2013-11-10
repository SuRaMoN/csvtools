
#include "stdafx.h"
#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>
#include <iostream>

#include "csvtools/csv_reader.hpp"

using namespace boost::filesystem;
using namespace csvtools;
using namespace std;

#define foreach BOOST_FOREACH

BOOST_AUTO_TEST_SUITE(csv_reader_test)

BOOST_AUTO_TEST_CASE(test_foreach_loop)
{
	int i = 0;
	foreach(const csv_line & line, csv_reader(path("testdata/simple.csv"))) {
		switch(i) {
			case 0:
				BOOST_CHECK(line.get(0) == L"aze");
				BOOST_CHECK(line.get(1) == L"rty");
				break;

			case 1:
				BOOST_CHECK(line.get(0) == L"uio");
				BOOST_CHECK(line.get(1) == L"pqs");
				break;

			default:
				BOOST_ERROR("simple.csv only contains 2 rows, but currently at third");
		}
		i += 1;
	}
	BOOST_CHECK(i == 2);
}

BOOST_AUTO_TEST_SUITE_END()
