#pragma once

#include <string>

namespace text_processing
{

class TextDataView
{
	using IteratorType = std::string::const_iterator;
public:
	TextDataView();
	explicit TextDataView(const IteratorType& begin, const IteratorType& end);
	bool Empty() const;
	size_t Size() const;
	IteratorType Begin() const;
	IteratorType End() const;
private:
	IteratorType mBegin;
	IteratorType mEnd;
};

} // namespace text_processing