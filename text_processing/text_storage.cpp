#include "text_storage.hpp"

namespace text_processing
{

TextStorage::TextStorage(std::string&& value) : mValue(value) {}

void TextStorage::Append(std::string&& value)
{
	mValue.append(value);
}

void TextStorage::Clear()
{
	mValue.clear();
}

size_t TextStorage::Size() const
{
	return mValue.size();
}

bool TextStorage::IsValid() const
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

void TextStorage::Erase(const IteratorType& from, const IteratorType& to)
{
	bool from_valid = from >= Begin() && from < End();
	bool to_valid = to > Begin() && to <= End();
	if(from_valid && to_valid)
	{
		mValue = std::string(Begin(), from) + std::string(to, End());
	}
}

} // namespace text_processing