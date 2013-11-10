
#ifndef CSVTOOLS_CSV_READER_HPP
#define CSVTOOLS_CSV_READER_HPP

#include <boost/filesystem/path.hpp>
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
	boost::filesystem::path path;
	wchar_t delimiter;
	wchar_t enclosure;
	wchar_t escape;
	std::locale locale;

public:
	csv_reader(boost::filesystem::path a_path, wchar_t a_delimiter = L',', wchar_t a_enclosure = L'"', wchar_t a_escape = L'\\', const std::locale & a_locale = std::locale(""))
		: path(a_path),
		delimiter(a_delimiter),
		enclosure(a_enclosure),
		escape(a_escape),
		locale(a_locale)
	{
	}

	iterator begin()
	{
		return iterator::new_from_path(path, delimiter, enclosure, escape, locale);
	}

	const_iterator begin() const
	{
		return iterator::new_from_path(path, delimiter, enclosure, escape, locale);
	}

	iterator end()
	{
		return iterator::new_eof();
	}

	const_iterator end() const
	{
		return iterator::new_eof();
	}

	virtual ~csv_reader()
	{
	}
};

}

#endif

