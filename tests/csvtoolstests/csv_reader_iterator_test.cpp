
#include "csv_reader_iterator_test_deps.hpp"
#include <boost/test/unit_test.hpp>
#include "csvtools/csv_reader.hpp"

using namespace boost::lambda;
using namespace boost::range;
using namespace boost;
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
	BOOST_CHECK_MESSAGE(count_if(reader, constant(true)) == 0, "input should not contain any csv lines");
}

BOOST_AUTO_TEST_CASE(test_only_newlines_empty_string)
{
	csv_reader reader = csv_reader::new_from_string(L"\n");
	BOOST_CHECK_MESSAGE(count_if(reader, constant(true)) == 0, "input should not contain any csv lines");
}

BOOST_AUTO_TEST_CASE(test_reset_in_memeory_csv)
{
	csv_reader reader = csv_reader::new_from_string(L"abc,def\nhij,klm");
	BOOST_CHECK(count_if(reader, constant(true)) == 2);
	reader.reset();
	BOOST_CHECK(count_if(reader, constant(true)) == 2);
}

BOOST_AUTO_TEST_CASE(test_reset_from_file)
{
	csv_reader reader = csv_reader::new_from_utf8_file("testdata/simple.csv");
	BOOST_CHECK(count_if(reader, constant(true)) == 2);
	reader.reset();
	BOOST_CHECK(count_if(reader, constant(true)) == 2);
}

BOOST_AUTO_TEST_CASE(test_reading_large_input_simple)
{
	csv_reader reader = csv_reader::new_from_utf8_file("testdata/large_input_simple.csv");

	vector<csv_line> lines;
	boost::copy(reader, std::back_inserter(lines));

	BOOST_CHECK(lines.size() == 7);

	BOOST_CHECK(lines[0][0].length() == 354);
	BOOST_CHECK(lines[1][0].length() == 360);
	BOOST_CHECK(lines[2][0].length() == 240);
	BOOST_CHECK(lines[3][0].length() == 10);
	BOOST_CHECK(lines[4][0].length() == 10);
	BOOST_CHECK(lines[5][0].length() == 960);
	BOOST_CHECK(lines[6][0].length() == 8);
}

BOOST_AUTO_TEST_CASE(test_reading_large_input_advanced)
{
	csv_reader reader = csv_reader::new_from_utf8_file("testdata/large_input_advanced.csv");
	// TODO: find out why this wont work on msvc
	//BOOST_CHECK(count_if(reader, bind(&csv_line::size, _1) == constant(2)) == 209);
	//reader.reset();
	BOOST_CHECK(count_if(reader, constant(true)) == 209);
}

BOOST_AUTO_TEST_SUITE_END()

