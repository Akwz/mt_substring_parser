#include <vector>

#include "text_provider.hpp"


namespace text_processing
{

TextProvider::TextProvider(const std::string& file_path, const TextProviderOptions& options)
	: mSourceFilePath(file_path)
	, mOptions(options)
{}

TextProvider::TextProvider(TextProvider&& provider)
	: mSourceFilePath(std::move(provider.mSourceFilePath))
	, mOptions(std::move(provider.mOptions))
	, mIStream(std::move(provider.mIStream))
{}

TextProvider::~TextProvider()
{
	if(mIStream.is_open()) mIStream.close();
}

std::string TextProvider::Fetch()
{
	std::string result;
	if(!mIStream.is_open()) mIStream.open(mSourceFilePath, std::ios::binary);
	if(mIStream.is_open() && mIStream.good())
	{
		std::vector<char> buffer;
		buffer.reserve(mOptions.mFetchDataMaxSize);
		mIStream.read(buffer.data(), mOptions.mFetchDataMaxSize);
		if(mIStream.gcount() > 0)
		{
			size_t read_count = std::min(mIStream.gcount(), mOptions.mFetchDataMaxSize);
			result = std::string(buffer.begin(), buffer.begin() + read_count);
		}
	}
	return result;
}

} // namespace text_processing