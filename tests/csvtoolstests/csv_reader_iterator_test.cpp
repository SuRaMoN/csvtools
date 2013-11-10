
#include "stdafx.h"
#include <boost/test/unit_test.hpp>
#include "csvtools/csv_reader.hpp"

using namespace csvtools;
using namespace std;

BOOST_AUTO_TEST_SUITE(csv_reader_iterator_test)

BOOST_AUTO_TEST_CASE(test_simple_read_without_quoted_columns)
{
	csv_reader reader = csv_reader("testdata/simple.csv");
	csv_reader::iterator i = reader.begin();
	csv_reader::iterator end = reader.end();

	BOOST_CHECK(i != end);

	BOOST_CHECK((*i).get(0) == L"aze");
	BOOST_CHECK((*i).get(1) == L"rty");

	BOOST_CHECK(i != end);

	++i;

	BOOST_CHECK((*i).get(0) == L"uio");
	BOOST_CHECK((*i).get(1) == L"pqs");

	BOOST_CHECK(i != end);

	++i;

	BOOST_CHECK(i == end);
}

BOOST_AUTO_TEST_CASE(test_simple_read_with_quoted_columns)
{
	csv_reader reader = csv_reader("testdata/simple_with_quoted_fields.csv");
	csv_reader::iterator i = reader.begin();
	csv_reader::iterator end = reader.end();

	BOOST_CHECK(i != end);

	BOOST_CHECK((*i).get(0) == L"aze");
	BOOST_CHECK((*i).get(1) == L"rty");

	BOOST_CHECK(i != end);

	++i;

	BOOST_CHECK((*i).get(0) == L"uio");
	BOOST_CHECK((*i).get(1) == L"p,q\"s\\");

	BOOST_CHECK(i != end);

	++i;

	BOOST_CHECK(i == end);
}

BOOST_AUTO_TEST_SUITE_END()

