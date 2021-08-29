#include <string>
#include <vector>


namespace text_processing
{

class MaskView;

class SinglePrefixMask
{
public:
	struct PrefixList
	{
		struct Node
		{
			size_t index{0};
			char value;
			const Node* parent{nullptr};
			const Node* suffix_link{nullptr};
			const Node* child{nullptr};
		};
		std::vector<Node> mNodes;
	};

	SinglePrefixMask(const std::string& mask_value);
	SinglePrefixMask(const SinglePrefixMask&) = delete;
	SinglePrefixMask& operator=(const SinglePrefixMask&) = delete;
	SinglePrefixMask(SinglePrefixMask&&) = default;

private:
	friend class MaskView;

	static bool CompareSymbols(char symbol, char mask_sybmol);
	static const PrefixList::Node* BuildSuffixLink(PrefixList::Node* parent, char symbo);
	
	PrefixList mStorage;
};

class MaskView
{
public:
	MaskView(const SinglePrefixMask& mask);
	MaskView(const MaskView&) = default;

	size_t Size() const {return std::max<size_t>(mMask.mStorage.mNodes.size() - 1, 0);}
	bool Compare(char symbol);
	bool IsReachedEnd() const;
private:
	void Reset();
	void ResetToSuffixValue();

	const SinglePrefixMask& mMask;
	const SinglePrefixMask::PrefixList::Node* mPosition;
};

} // namespace text_processing