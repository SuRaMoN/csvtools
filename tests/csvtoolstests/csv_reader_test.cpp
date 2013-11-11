
#include "csv_reader_test_deps.hpp"
#include <boost/test/unit_test.hpp>
#include "csvtools/csv_reader.hpp"

using namespace boost;
using namespace boost::filesystem;
using namespace boost::lambda;
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
				BOOST_CHECK(line[0] == L"uio");
				BOOST_CHECK(line[1] == L"pqs");
				break;

			default:
				BOOST_ERROR("simple.csv only contains 2 rows, but currently at third");
		}
		i += 1;
	}
	BOOST_CHECK_MESSAGE(i == 2, "simple.csv only containes 2 rows");
}

BOOST_AUTO_TEST_CASE(test_new_utf8_file)
{
	csv_reader reader = csv_reader::new_from_utf8_file(path("testdata/utf-8.csv"));
	csv_reader_iterator i = reader.begin();
	csv_line line = *i;

	BOOST_CHECK_MESSAGE(line[0].length() == 7, "First column on the first row is 7 uft-8 characters long");
	BOOST_CHECK_MESSAGE(line[1].length() == 8, "Second column on the first row is 8 uft-8 characters long");

	++i;
	line = *i;

	BOOST_CHECK_MESSAGE(line[0].length() == 4, "First column on the second row is 4 uft-8 characters long");
	BOOST_CHECK_MESSAGE(line[1].length() == 4, "Second column on the second row is 4 uft-8 characters long");

	++i;
	BOOST_CHECK(i == reader.end());
}

BOOST_AUTO_TEST_CASE(test_new_from_string_stream)
{
	csv_reader reader = csv_reader::new_from_string(L"abc,def\nhij,klm\n");
	BOOST_CHECK_MESSAGE(count_if(reader, constant(true)) == 2, "input should contain 2 lines");
}

BOOST_AUTO_TEST_SUITE_END()

