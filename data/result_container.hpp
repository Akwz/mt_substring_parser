#pragma once

namespace data
{

template<typename T>
class ResultContainer
{
public:
	ResultContainer() : mValue() {}
	ResultContainer(const T& value) : mValue(value){}
	void Merge(const ResultContainer& other){ mValue += other.Value();}
	T Value() const {return mValue;}

private:
	T mValue;
};

} // namespace data