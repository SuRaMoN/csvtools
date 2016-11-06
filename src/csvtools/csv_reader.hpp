
#ifndef CSVTOOLS_CSV_READER_HPP
#define CSVTOOLS_CSV_READER_HPP

#include <boost/filesystem.hpp>
#include <boost/locale.hpp>
#include <ios>
#include <iterator>
#include <locale>

#include "csv_reader_iterator.hpp"

namespace csvtools {

class csv_reader
{
public:
	typedef csv_reader_iterator iterator;
	typedef csv_reader_iterator const_iterator;

protected:
	typedef boost::shared_ptr<std::wistream> wistream_pt;

	wistream_pt input_stream_p;
	wchar_t delimiter;
	wchar_t enclosure;

public:
	csv_reader(wistream_pt a_input_stream_p, wchar_t a_delimiter = L',', wchar_t a_enclosure = L'"')
		: input_stream_p(a_input_stream_p),
		delimiter(a_delimiter),
		enclosure(a_enclosure)
	{
	}

	csv_reader(boost::filesystem::path a_path, wchar_t a_delimiter = L',', wchar_t a_enclosure = L'"', const std::locale & a_locale = std::locale(""))
		: input_stream_p(new std::wifstream(a_path.c_str())),
		delimiter(a_delimiter),
		enclosure(a_enclosure)
	{
		input_stream_p->imbue(a_locale);
	}

	static csv_reader new_from_utf8_file(boost::filesystem::path a_path, wchar_t a_delimiter = L',', wchar_t a_enclosure = L'"')
	{
		if(! boost::filesystem::exists(a_path)) {
			throw std::ios_base::failure("Trying to read non existing file");
		}
		return csv_reader(a_path, a_delimiter, a_enclosure, boost::locale::generator().generate("en_US.UTF-8"));
	}

	static csv_reader new_from_string(const std::wstring & a_string, wchar_t a_delimiter = L',', wchar_t a_enclosure = L'"')
	{
		return csv_reader(boost::shared_ptr<std::wistream>(new std::wstringstream(a_string)), a_delimiter, a_enclosure);
	}

	iterator begin()
	{
		return iterator(input_stream_p, delimiter, enclosure);
	}

	const_iterator begin() const
	{
		return iterator(input_stream_p, delimiter, enclosure);
	}

	iterator end()
	{
		return iterator::new_eof();
	}

	const_iterator end() const
	{
		return iterator::new_eof();
	}

	void reset()
	{
		input_stream_p->clear();
		input_stream_p->seekg(0);
	}

	virtual ~csv_reader()
	{
	}
};

}

#endif

