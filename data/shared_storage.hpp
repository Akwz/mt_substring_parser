#pragma once

#include <memory>
#include <mutex>


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
	int64_t mDataViewLayering{0};
	/*
	Size of each fetched DataView
	*/
	int64_t mDataViewSize{256};
};

template<typename StorageType, typename ProviderType, template<typename> class ViewType>
class SharedStorage
{
public:
	SharedStorage(ProviderType& data_provider, SharedStorageOptions options) : mDataProvider(data_provider), mOptions(options) {}
	const ViewType<typename StorageType::IteratorType> TryGetNextData()
	{
		ViewType<typename StorageType::IteratorType> result;
		std::lock_guard<std::mutex> lg(mStorageMutex);
		if(TryUpdateStorageIterators())
		{
			const size_t move_distance = std::max(std::min(mStorageEnd - mCurrentPosition, mOptions.mDataViewSize), 0);
			result = ViewType<typename StorageType::IteratorType>(mCurrentPosition, mCurrentPosition + move_distance);
			MoveStorageIterators();
		}
		return result;
	}

private:
	SharedStorage() = delete;
	SharedStorage(const SharedStorage&) = delete;
	SharedStorage& operator=(const SharedStorage&) = delete;

	bool TryUpdateStorageIterators()
	{
		if(!IsStorageIteratorsValid())
		{
			TryFetchDataFromProvider();
		}
		return IsStorageIteratorsValid();
	}

	void TryFetchDataFromProvider();
	{
		mStorage.Clear();
		mStorage.Aquire(mDataProvider.Fetch());
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
		if(mCurrentPosition && mCurrentPosition != mStorageEnd)
		{
			const size_t move_distance = std::max(std::min(mStorageEnd - mCurrentPosition, mOptions.mDataViewSize - mOptions.mDataViewLayering), 0);
			mCurrentPosition += move_distance;
		}
	}

	bool IsStorageIteratorsValid()
	{
		if(!mCurrentPosition || (mCurrentPosition == mStorageEnd)) return false;
		return true;
	}

	StorageType mStorage;
	typename StorageType::IteratorType mStorageBegin;
	typename StorageType::IteratorType mStorageEnd;
	typename StorageType::IteratorType mCurrentPosition;
	std::mutex mStorageMutex;
	ProviderType mDataProvider;
	SharedStorageOptions mOptions;
};

} // data