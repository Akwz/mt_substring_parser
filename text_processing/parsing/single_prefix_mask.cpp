#include <cassert>

#include "single_prefix_mask.hpp"


namespace text_processing
{

constexpr char REGEX_SYMBOL{'\n'};

SinglePrefixMask::SinglePrefixMask(const std::string& source)
{
	assert(!source.empty());
	mStorage.mNodes.emplace_back(PrefixList::Node());
	for(size_t i = 1; i <= source.size(); ++i)
	{
		auto& parent = mStorage.mNodes.back();
		mStorage.mNodes.emplace_back(
			PrefixList::Node{
				i,
				source[i-1],
				&parent,
				BuildSuffixLink(&parent, source[i - 1]),
				nullptr
			}
		);
		parent.child = &mStorage.mNodes.back();
	}
}

const SinglePrefixMask::PrefixList::Node* SinglePrefixMask::BuildSuffixLink(PrefixList::Node* parent, char symbol)
{
	const PrefixList::Node* link{parent->suffix_link};
	while(link)
	{
		if(link->child && CompareSymbols(link->child->value, symbol))
		{
			return link->child;
		}
		link = link->suffix_link;
	}
	return nullptr;
}

bool SinglePrefixMask::CompareSymbols(char symbol, char mask_symbol)
{
	return (symbol == mask_symbol || symbol == REGEX_SYMBOL);
}

MaskView::MaskView(const SinglePrefixMask& mask)
	: mMask(mask)
{
	assert(Size() > 0);
	Reset();
}

bool MaskView::Compare(char symbol)
{
	if(!IsReachedEnd() && SinglePrefixMask::CompareSymbols(mPosition->value, symbol))
	{
		mPosition = mPosition->child;
		return true;
	}
	else
	{
		ResetToSuffixValue();
	}
	return false;
}

bool MaskView::IsReachedEnd() const
{
	return !mPosition->child;
}

void MaskView::Reset()
{
	mPosition = &mMask.mStorage.mNodes[1];
}

void MaskView::ResetToSuffixValue()
{
	assert(mPosition);
	const auto* link = mPosition->parent ? mPosition->parent->suffix_link : nullptr;
	if(link) mPosition = link->child->child;
	else 	 Reset();
}

} // namespace text_processing