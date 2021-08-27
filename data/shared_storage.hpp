#pragma once

#include <memory>
#include <mutex>
#include <tuple>


namespace data
{

struct SharedStorageOptions
{
	/*
	Layering of each fetched DataView:
	Storage: [0 1 2 3 4 5 6 7 8 9]
	First fetched data view: [0 1 2 3]
	Next fetched data view with layering = 3: [1 2 3 4]
	Next fetched data with same layering: [2 3 4 5]
	*/
	size_t mDataViewLayering{0};
	/*
	Size of each fetched DataView
	*/
	size_t mDataViewSize{256};
};

template<typename StorageType, typename ProviderType, typename ViewType>
class SharedStorage
{
public:
	SharedStorage(ProviderType&& data_provider, const SharedStorageOptions& options) : mDataProvider{std::move(data_provider)}, mOptions{options}, mExtractedDataCounter{0} {}
	const std::tuple<ViewType, size_t> TryGetNextData()
	{
		ViewType result;
		std::lock_guard<std::mutex> lg(mStorageMutex);
		size_t order_id{0};
		if(TryUpdateStorageIterators())
		{
			const size_t move_distance = std::max<size_t>(std::min<size_t>(mStorageEnd - mCurrentPosition, mOptions.mDataViewSize), 0);
			result = ViewType(mCurrentPosition, mCurrentPosition + move_distance);
			order_id = ExtractedDataOrderId();
			MoveStorageIterators();
		}
		return std::make_tuple<ViewType, size_t>(std::move(result), std::move(order_id));
	}

private:
	SharedStorage() = delete;
	SharedStorage(const SharedStorage&) = delete;
	SharedStorage& operator=(const SharedStorage&) = delete;

	size_t ExtractedDataOrderId() const
	{
		return mExtractedDataCounter;
	}

	bool TryUpdateStorageIterators()
	{
		if(!IsStorageIteratorsValid())
		{
			TryFetchDataFromProvider();
		}
		return IsStorageIteratorsValid();
	}

	void TryFetchDataFromProvider()
	{
		if(mStorage.IsValid()) mStorage.Erase(mStorage.Begin(), mStorage.End() - mOptions.mDataViewLayering);
		auto before_fetching = mStorage.Size();
		mStorage.Append(mDataProvider.Fetch());
		if(mStorage.Size() == before_fetching)
		{
			mStorage.Clear();
		}
		ResetStorageIterators();
	}

	void ResetStorageIterators()
	{
		if(mStorage.IsValid())
		{
			mStorageBegin = mCurrentPosition = mStorage.Begin();
			mStorageEnd = mStorage.End();
		}
	}

	void MoveStorageIterators()
	{
		const size_t move_distance = std::max<size_t>(std::min<size_t>(mStorageEnd - mCurrentPosition, mOptions.mDataViewSize - mOptions.mDataViewLayering), 0);
		mCurrentPosition += move_distance;
		++mExtractedDataCounter;
	}

	bool IsStorageIteratorsValid()
	{
		return mCurrentPosition != mStorageEnd;
	}

	StorageType mStorage;
	typename StorageType::IteratorType mStorageBegin;
	typename StorageType::IteratorType mStorageEnd;
	typename StorageType::IteratorType mCurrentPosition;
	std::mutex mStorageMutex;
	ProviderType mDataProvider;
	SharedStorageOptions mOptions;
	size_t mExtractedDataCounter;
};

} // data