
#ifndef CSVTOOLS_CSV_LINE_HPP
#define CSVTOOLS_CSV_LINE_HPP

#include <vector>
#include <string>

namespace csvtools {

class csv_line
{
protected:
	std::vector<std::wstring> fields;

	csv_line()
	{
	}

public:
	const std::wstring & get(int i) const
	{
		return fields[i];
	}

	virtual ~csv_line()
	{
	}

	friend class csv_reader_iterator;
};

}
 
#endif

