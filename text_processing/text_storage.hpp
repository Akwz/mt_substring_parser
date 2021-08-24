#pragma once

#include <string>

namespace text_processing
{

class TextStorage
{
public:
	using IteratorType = std::string::const_iterator;
	TextStorage() = default;
	TextStorage(std::string&& value);
	void Append(std::string&& value);
	void Clear();
	/*
	Erase substring [from, to)
	*/
	void Erase(const IteratorType& from, const IteratorType& to);
	bool IsValid() const;
	IteratorType Begin() const;
	IteratorType End() const;
	size_t Size() const;
private:
	std::string mValue;
};

} // namespace text_processing