#include "text_data_view.hpp"

namespace text_processing
{

TextDataView::TextDataView()
{}

TextDataView::TextDataView(const IteratorType& begin, const IteratorType& end)
	: mBegin(begin)
	, mEnd(end)
{}

bool TextDataView::Empty() const
{
	return mBegin == mEnd;
}

size_t TextDataView::Size() const
{
	return Empty() ? 0 : mEnd - mBegin;
}

TextDataView::IteratorType TextDataView::Begin() const
{
	return mBegin;
}

TextDataView::IteratorType TextDataView::End() const
{
	return mEnd;
}

} // namespace text_processing