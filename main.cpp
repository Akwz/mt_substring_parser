#include <iostream>

#include "text_processor.hpp"


int main(int argc, char** argv)
{
	const std::string fname("test_data/test_file");
	text_processing::TextProcessor processor{fname, "?r?"};
	auto result = processor.Process();

	// for(const auto& entry : result.mLayers)
	// {
	// 	for(const auto& layer : entry.second)
	// 	{
	// 		// std::cout << layer.appearences.size() << std::endl;
	// 		for(const auto& app : layer.appearences)
	// 		{
	// 			std::cout << app.word << std::endl;
	// 		}
	// 	}
	// }

	std::cout << "Done: " << result.mLayers.size() << std::endl;
	return 0;
}