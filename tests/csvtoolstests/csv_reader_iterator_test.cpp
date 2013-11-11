
#include "csv_reader_iterator_test_deps.hpp"
#include <boost/test/unit_test.hpp>
#include "csvtools/csv_reader.hpp"

using namespace boost::lambda;
using namespace boost::range;
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

BOOST_AUTO_TEST_CASE(test_no_final_newline_for_unquoted_field)
{
	csv_reader reader = csv_reader::new_from_string(L"abc,def\nhij,klm");
	BOOST_CHECK_MESSAGE(count_if(reader, constant(true)) == 2, "input should contain 2 lines");
}

BOOST_AUTO_TEST_CASE(test_no_final_newline_for_quoted_field)
{
	csv_reader reader = csv_reader::new_from_string(L"abc,def\nhij,\"klm\"");
	BOOST_CHECK_MESSAGE(count_if(reader, constant(true)) == 2, "input should contain 2 lines");
}

BOOST_AUTO_TEST_CASE(test_intermediate_newlines_for_unquoted_field)
{
	csv_reader reader = csv_reader::new_from_string(L"abc,def\n\nhij,klm");
	BOOST_CHECK_MESSAGE(count_if(reader, constant(true)) == 2, "input should contain 2 lines");
}

BOOST_AUTO_TEST_CASE(test_intermediate_newlines_for_quoted_field)
{
	csv_reader reader = csv_reader::new_from_string(L"abc,\"def\"\n\n\"hij\",klm");
	BOOST_CHECK_MESSAGE(count_if(reader, constant(true)) == 2, "input should contain 2 lines");
}

BOOST_AUTO_TEST_CASE(test_intial_newlines_for_unquoted_field)
{
	csv_reader reader = csv_reader::new_from_string(L"\n\nabc,def\nhij,klm");
	BOOST_CHECK_MESSAGE(count_if(reader, constant(true)) == 2, "input should contain 2 lines");
}

BOOST_AUTO_TEST_CASE(test_intial_newlines_for_quoted_field)
{
	csv_reader reader = csv_reader::new_from_string(L"\n\n\"abc\",def\nhij,klm");
	BOOST_CHECK_MESSAGE(count_if(reader, constant(true)) == 2, "input should contain 2 lines");
}

BOOST_AUTO_TEST_CASE(test_end_newlines_for_unquoted_field)
{
	csv_reader reader = csv_reader::new_from_string(L"abc,def\nhij,klm\n\n");
	BOOST_CHECK_MESSAGE(count_if(reader, constant(true)) == 2, "input should contain 2 lines");
}

BOOST_AUTO_TEST_CASE(test_end_newlines_for_quoted_field)
{
	csv_reader reader = csv_reader::new_from_string(L"abc,def\nhij,\"klm\"\n\n");
	BOOST_CHECK_MESSAGE(count_if(reader, constant(true)) == 2, "input should contain 2 lines");
}

BOOST_AUTO_TEST_CASE(test_empty_string)
{
	csv_reader reader = csv_reader::new_from_string(L"");
	BOOST_CHECK_MESSAGE(count_if(reader, constant(true)) == 0, "input should contain 2 lines");
}

BOOST_AUTO_TEST_CASE(test_only_newlines_empty_string)
{
	csv_reader reader = csv_reader::new_from_string(L"");
	BOOST_CHECK_MESSAGE(count_if(reader, constant(true)) == 0, "input should contain 2 lines");
}

BOOST_AUTO_TEST_SUITE_END()

