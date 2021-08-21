#include "text_storage.hpp"

namespace text_processing
{

TextStorage::TextStorage(std::string&& value) : mValue(value) {}

void TextStorage::Aquire(std::string&& value)
{
	mValue = value;
}

void TextStorage::Clear()
{
	mValue.clear();
}

bool TextStorage::IsValid()
{
	return !mValue.empty();
}

TextStorage::IteratorType TextStorage::Begin() const
{
	return mValue.cbegin();
}

TextStorage::IteratorType TextStorage::End() const
{
	return mValue.cend();
}

} // namespace text_processing