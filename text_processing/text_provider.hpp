#pragma once

#include <fstream>

namespace text_processing
{

struct TextProviderOptions
{
	/*
	Option tells how much symbols will fetch on Fetch() call
	*/
	int64_t mFetchDataMaxSize{512};
};

class TextProvider
{
public:
	TextProvider(const std::string& file_path, const TextProviderOptions& options);
	TextProvider(TextProvider&& provider);
	~TextProvider();

	std::string Fetch();

private:
	const std::string mSourceFilePath;
	std::ifstream mIStream;
	const TextProviderOptions mOptions;
};

} // namespace text_processing