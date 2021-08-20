#pragma once

namespace data
{

template<typename T>
class ResultContainer
{
public:
	ResultContainer() : mValue() {}
	ResultContainer(const T& value) : mValue(value){}
	void Merge(const ResultContainer& other){}
	T Value() const {return mValue;}

private:
	T mValue;
};

template<>
void ResultContainer<int>::Merge(const ResultContainer<int>& other);

} // namespace data