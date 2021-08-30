#include <cassert>
#include <algorithm>

#include "single_prefix_mask.hpp"


namespace text_processing
{

constexpr char REGEX_SYMBOL{'?'};
constexpr char FORBIDDEN_LIST[]{'\n'};
constexpr size_t MAX_MASK_SIZE{100};

SinglePrefixMask::SinglePrefixMask(const std::string& source)
{
	assert(!source.empty() && source.size() <= MAX_MASK_SIZE);
	mStorage.mNodes.reserve(source.size() + 1);
	mStorage.mNodes.emplace_back(PrefixList::Node());
	for(size_t i = 1; i <= source.size(); ++i)
	{
		assert(!std::any_of(std::begin(FORBIDDEN_LIST), std::end(FORBIDDEN_LIST), [symbol = source[i - 1]](char forbidden){return forbidden == symbol;}));
		auto* parent = &(mStorage.mNodes.back());
		mStorage.mNodes.emplace_back(
			PrefixList::Node{
				source[i - 1],
				parent,
				BuildSuffixLink(parent, source[i - 1]),
				nullptr
			}
		);
		parent->child = &(mStorage.mNodes.back());
	}
}

const SinglePrefixMask::PrefixList::Node* SinglePrefixMask::BuildSuffixLink(const PrefixList::Node* parent, char symbol)
{
	const PrefixList::Node* link{parent->suffix_link};
	while(link)
	{
		if(link->child && CompareSymbols(symbol, link->child->value))
		{
			return link->child;
		}

		if(link->suffix_link) link = link->suffix_link;
		else return link;
	}
	return parent;
}

bool SinglePrefixMask::CompareSymbols(char symbol, char mask_symbol)
{
	return (symbol == mask_symbol || mask_symbol == REGEX_SYMBOL);
}

MaskView::MaskView(SinglePrefixMask const * const mask)
	: mMask(mask)
{
	assert(Size() > 0);
	Reset();
}

bool MaskView::Compare(char symbol)
{
	if(!IsReachedEnd())
	{
		mPosition = mPosition->child;
		if(SinglePrefixMask::CompareSymbols(symbol, mPosition->value))
		{
			return true;
		}
		else
		{
			ResetToSuffixValue(symbol);
		}
	}
	return false;
}

bool MaskView::IsReachedEnd() const
{
	return !mPosition->child;
}

void MaskView::Reset()
{
	mPosition = &mMask->mStorage.mNodes.front();
}

void MaskView::ResetToSuffixValue(char symbol)
{
	assert(mPosition);
	const SinglePrefixMask::PrefixList::Node* link = mPosition->parent ? mPosition->parent->suffix_link : nullptr;

	while(true)
	{
		if(link)
		{
			if(SinglePrefixMask::CompareSymbols(link->child->value, symbol))
			{
				mPosition = link->child;
				return;
			}
			else
			{
				link = link->suffix_link;
			}
		}
		else
		{
			Reset();
			return;
		}
	}
}

} // namespace text_processing