
#ifndef CSVTOOLS_CSV_READER_ITERATOR_HPP
#define CSVTOOLS_CSV_READER_ITERATOR_HPP

#include <boost/filesystem/path.hpp>
#include <boost/shared_ptr.hpp>
#include <fstream>
#include <iostream>
#include <iterator>
#include <locale>
#include <sstream>
#include <stdexcept>

#include "csv_line.hpp"

namespace csvtools {

class csv_reader_iterator : public std::iterator<std::input_iterator_tag, const csv_line>
{
protected:
	const static int BUFFER_SIZE = 1024;

	typedef boost::shared_ptr<std::wistream> wistream_pt;

	csv_line current_line;
	boost::shared_ptr<std::wstringstream> field_buffer_p;
	wistream_pt input_stream_p;
	wchar_t delimiter;
	wchar_t enclosure;
	wchar_t escape;
	enum { VALID, PENULTIMATE, END } status;

	csv_reader_iterator(wistream_pt a_input_stream_p, wchar_t a_delimiter, wchar_t a_enclosure, wchar_t a_escape)
		: field_buffer_p(new std::wstringstream()),
		input_stream_p(a_input_stream_p),
		delimiter(a_delimiter),
		enclosure(a_enclosure),
		escape(a_escape),
		status(VALID)
	{
		if(input_stream_p.get() != NULL) {
			operator++();
		}
	}

	static csv_reader_iterator new_eof()
	{
		csv_reader_iterator eof_iterator(wistream_pt(), L'\0', L'\0', L'\0');
		eof_iterator.status = END;
		return eof_iterator;
	}

	void try_fill_buffer(wchar_t * buffer, int & buffer_size, int & i)
	{
		input_stream_p->read(buffer, BUFFER_SIZE);
		buffer_size = input_stream_p->gcount();
		i = 0;
	}

	void try_fill_buffer_if_empty(wchar_t * buffer, int & buffer_size, int & i)
	{
		if(i == buffer_size) {
			try_fill_buffer(buffer, buffer_size, i);
		}
	}

	void fill_buffer(wchar_t * buffer, int & buffer_size, int & i)
	{
		try_fill_buffer(buffer, buffer_size, i);
		if(0 == buffer_size) {
			throw std::logic_error("Invalid csv supplied");
		}
	}

	void fill_buffer_if_empty(wchar_t * buffer, int & buffer_size, int & i)
	{
		if(i == buffer_size) {
			fill_buffer(buffer, buffer_size, i);
		}
	}

	void add_field_from_field_buffer()
	{
		current_line.fields.push_back(field_buffer_p->str());
	}

	void read_field(wchar_t * buffer, int & buffer_size, int & i)
	{
		fill_buffer_if_empty(buffer, buffer_size, i);
		if(buffer[i] == enclosure) {
			read_quoted_field(buffer, buffer_size, i);
		} else {
			read_unquoted_field(buffer, buffer_size, i);
		}
	}

	void read_quoted_field(wchar_t * buffer, int & buffer_size, int & i)
	{
		int previous_escape_pos = -2;
		fill_buffer_if_empty(buffer, buffer_size, i);
		if(buffer[i] != enclosure) {
			throw std::logic_error("Started reading a quoted field but couln't field an enclosure character");
		}
		i += 1;
		while(true) {
			int start = i;
			while(buffer[i] != enclosure && buffer[i] != escape && i < buffer_size) {
				i += 1;
			}
			field_buffer_p->write(buffer + start, i - start);
			if(i == buffer_size) {
				fill_buffer(buffer, buffer_size, i);
				continue;
			}
			if(previous_escape_pos == i - 1) {
				field_buffer_p->write(buffer + i, 1);
				i += 1;
				continue;
			}
			if(buffer[i] == escape) {
				previous_escape_pos = i;
				i += 1;
			} else if(buffer[i] == enclosure) {
				add_field_from_field_buffer();
				i += 1;
				return;
			}
		}
	}

	void read_unquoted_field(wchar_t * buffer, int & buffer_size, int & i)
	{
		while(true) {
			int start = i;
			while(buffer[i] != delimiter && buffer[i] != L'\n' && i < buffer_size) {
				i += 1;
			}
			field_buffer_p->write(buffer + start, i - start);
			if(i < buffer_size || input_stream_p->eof()) {
				add_field_from_field_buffer();
				return;
			}
			fill_buffer(buffer, buffer_size, i);
		}
	}

	void save_buffer(wchar_t * buffer, int buffer_size, int i)
	{
		field_buffer_p->str(L"");
		field_buffer_p->write(buffer + i, buffer_size - i);
	}

	void load_saved_buffer(wchar_t * buffer, int & buffer_size)
	{
		field_buffer_p->readsome(buffer, BUFFER_SIZE);
		buffer_size = field_buffer_p->gcount();
	}

	void skip_newlines(wchar_t * buffer, int & buffer_size, int & i)
	{
		while(true) {
			try_fill_buffer_if_empty(buffer, buffer_size, i);
			if(i == buffer_size && input_stream_p->eof()) {
				return;
			}
			while(buffer[i] == L'\n' && i < buffer_size) {
				i += 1;
			}
			if(i < buffer_size) {
				return;
			}
		}
	}

public:
	csv_reader_iterator & operator++()
	{
		switch(status) {
			case PENULTIMATE:
				status = END;
				return *this;
			case END:
				throw new std::logic_error("Trying to advance invalid iterator");
			case VALID: ;
		}

		wchar_t buffer[BUFFER_SIZE];
		int buffer_size = -1;
		int i = 0;

		load_saved_buffer(buffer, buffer_size);
		current_line.fields.clear();
		skip_newlines(buffer, buffer_size, i);
		if(i == buffer_size && input_stream_p->eof()) {
			status = END;
			return *this;
		}

		// add a column of the csv line in each iteration
		while(true) {
			field_buffer_p->str(L"");
			read_field(buffer, buffer_size, i);

			if(i == buffer_size && input_stream_p->eof()) {
				break;
			}

			fill_buffer_if_empty(buffer, buffer_size, i);

			// end of csv line, were done :)
			if('\n' == buffer[i]) {
				break;
			}
			// another column, just continue the loop
			if(delimiter == buffer[i]) {
				i += 1;
				continue;
			}
			throw std::logic_error("Unexpected character encountered");
		}

		skip_newlines(buffer, buffer_size, i);
		if(input_stream_p->eof() && i == buffer_size) {
			input_stream_p = wistream_pt();
			status = PENULTIMATE;
		}
		save_buffer(buffer, buffer_size, i);

		return *this;
	}

	csv_reader_iterator operator++(int)
	{
		csv_reader_iterator tmp(*this);
		operator++();
		return tmp;
	}

	bool operator==(const csv_reader_iterator & rhs) const
	{
		return status == END && rhs.status == END;
	}

	bool operator != (const csv_reader_iterator & rhs) const
	{
		return ! operator==(rhs);
	}

	const csv_line & operator*() const
	{
		return current_line;
	}

	friend class csv_reader;
};

}

#endif

