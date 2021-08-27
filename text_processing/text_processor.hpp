#pragma once

#include <memory>
#include <string>
#include <tuple>

#include "shared_storage.hpp"
#include "text_storage.hpp"
#include "text_provider.hpp"
#include "text_data_view.hpp"
#include "activity_manager.hpp"
#include "text_parser.hpp"


namespace text_processing
{

template<typename ProviderType>
class TextProcessor
{
public:
	explicit TextProcessor(ProviderType&& data_provider, const std::string& mask)
		: mDataStorage{std::move(data_provider), data::SharedStorageOptions{mask.size(), 256}}
		, mActivityManager{activity::ActivityManager::Instance()}
		, mMask(mask)
	{}

	ParsingResult Process()
	{
		const size_t activity_task_count = mActivityManager->SuitableTaskCount();
		activity::Activity<ParsingResult> text_parsing_activity;
		text_parsing_activity.result.SetLayering(mMask.size());
		const auto parsing_unit = [&storage = mDataStorage, &mask = mMask]() -> ParsingResult
		{
			ParsingResult result;
			result.SetLayering(mask.size());
			auto next_data = storage.TryGetNextData();
			size_t current_order_id = std::get<1>(next_data);
			TextDataView text_to_process(std::get<0>(next_data).begin(), std::get<0>(next_data).end());
			while(!text_to_process.Empty())
			{

				TextParser parser(text_to_process, mask);
				parser.Parse(result.mResultSequence[current_order_id]);
				next_data = storage.TryGetNextData();
				text_to_process = TextDataView{std::get<0>(next_data).begin(), std::get<0>(next_data).end()};
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

private:
	ParsingResult ProcessFrame(const TextDataView& frame);

	data::SharedStorage<TextStorage, ProviderType, std::string>  mDataStorage;
	std::shared_ptr<activity::ActivityManager> mActivityManager;
	std::string mMask;
};

} // namespace text_processing