#include <tuple>

#include "text_processor.hpp"


namespace text_processing
{

TextProcessor::TextProcessor(const std::string& file_path, const std::string& mask)
	: mDataStorage{TextProvider{file_path, TextProviderOptions{}}, data::SharedStorageOptions{}}
	, mActivityManager{activity::ActivityManager::Instance()}
	, mMask(mask)
{}

ParsingResult TextProcessor::Process()
{
	const size_t activity_task_count = mActivityManager->SuitableTaskCount();
	activity::Activity<ParsingResult> text_parsing_activity;
	const auto parsing_unit = [&storage = mDataStorage, &mask = mMask]() -> ParsingResult
	{
		ParsingResult result;
		auto next_data = storage.TryGetNextData();
		size_t current_order_id = std::get<1>(next_data);
		TextDataView text_to_process(std::get<0>(next_data));
		while(!text_to_process.Empty())
		{
			if(result.mLayers.find(current_order_id) == result.mLayers.end())
			{
				result.mLayers[current_order_id] = std::vector<Layer>();
			}
			TextParser parser(text_to_process, mask);
			parser.Parse(result.mLayers[current_order_id]);

			next_data = storage.TryGetNextData();
			text_to_process = std::get<0>(next_data);
			current_order_id = std::get<1>(next_data);
		}
		return result;
	};

	for(size_t i = 0; i < activity_task_count; ++i)
	{
		text_parsing_activity.tasks.push_back(parsing_unit);
	}
	mActivityManager->Process(text_parsing_activity);
	return text_parsing_activity.result;
}



} // namespace text_processing