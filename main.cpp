#include <iostream>

#include "text_processor.hpp"


int main(int argc, char** argv)
{
	const std::string fname("../tmp_file");
	text_processing::TextProcessor processor{fname, "sa?"};
	auto result = processor.Process();

	std::cout << "Done: " << result.mLayers[0].size() << std::endl;
	return 0;
}