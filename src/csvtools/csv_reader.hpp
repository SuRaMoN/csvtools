
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
    csv_line header;
    wchar_t delimiter;
    wchar_t enclosure;

    inline void read_header()
    {
        iterator it(input_stream_p, delimiter, enclosure);
        header = *it;
    }

public:
    csv_reader(wistream_pt a_input_stream_p, wchar_t a_delimiter = L',', wchar_t a_enclosure = L'"', bool a_has_header = true)
        : input_stream_p(a_input_stream_p),
        delimiter(a_delimiter),
        enclosure(a_enclosure)
    {
        if (a_has_header) {
            read_header();
        }
    }

    csv_reader(boost::filesystem::path a_path, wchar_t a_delimiter = L',', wchar_t a_enclosure = L'"', bool a_has_header = true, const std::locale & a_locale = std::locale(""))
        : input_stream_p(new std::wifstream(a_path.c_str())),
        delimiter(a_delimiter),
        enclosure(a_enclosure)
    {
        input_stream_p->imbue(a_locale);
        if (a_has_header) {
            read_header();
        }
    }

    inline static csv_reader new_without_header(boost::filesystem::path a_path, wchar_t a_delimiter = L',', wchar_t a_enclosure = L'"')
    {
        return csv_reader(a_path, a_delimiter, a_enclosure, false);
    }

    inline static csv_reader new_from_utf8_file(boost::filesystem::path a_path, wchar_t a_delimiter = L',', wchar_t a_enclosure = L'"', bool a_has_header = true)
    {
        if(! boost::filesystem::exists(a_path)) {
            throw std::ios_base::failure("Trying to read non existing file");
        }
        return csv_reader(a_path, a_delimiter, a_enclosure, a_has_header, boost::locale::generator().generate("en_US.UTF-8"));
    }

    inline static csv_reader new_from_utf8_file_without_header(boost::filesystem::path a_path, wchar_t a_delimiter = L',', wchar_t a_enclosure = L'"')
    {
        return new_from_utf8_file(a_path, a_delimiter, a_enclosure, false);
    }
    
    inline static csv_reader new_from_string(const std::wstring & a_string, wchar_t a_delimiter = L',', wchar_t a_enclosure = L'"', bool a_has_header = true)
    {
        return csv_reader(boost::shared_ptr<std::wistream>(new std::wstringstream(a_string)), a_delimiter, a_enclosure, a_has_header);
    }

    inline static csv_reader new_from_string_without_header(const std::wstring & a_string, wchar_t a_delimiter = L',', wchar_t a_enclosure = L'"')
    {
        return csv_reader::new_from_string(a_string, a_delimiter, a_enclosure, false);
    }

    inline csv_line get_header()
    {
        return header;
    }

    inline iterator begin()
    {
        iterator it(input_stream_p, delimiter, enclosure);
        return it;
    }

    inline const_iterator begin() const
    {
        iterator it(input_stream_p, delimiter, enclosure);
        return it;
    }

    inline iterator end()
    {
        return iterator::new_eof();
    }

    inline const_iterator end() const
    {
        return iterator::new_eof();
    }

    inline void reset()
    {
        input_stream_p->clear();
        input_stream_p->seekg(0);
    }

    inline virtual ~csv_reader()
    {
    }
};

}

#endif

