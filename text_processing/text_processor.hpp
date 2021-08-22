#pragma once

#include <memory>
#include <string>

#include "shared_storage.hpp"
#include "text_storage.hpp"
#include "text_provider.hpp"
#include "text_data_view.hpp"
#include "activity_manager.hpp"
#include "text_parser.hpp"


namespace text_processing
{

class TextProcessor
{
public:
	explicit TextProcessor(const std::string& file_path, const std::string& mask);
	ParsingResult Process();

private:
	ParsingResult ProcessFrame(const TextDataView& frame);

	data::SharedStorage<TextStorage, TextProvider, std::string>  mDataStorage;
	std::shared_ptr<activity::ActivityManager> mActivityManager;
	std::string mMask;
};

} // namespace text_processing