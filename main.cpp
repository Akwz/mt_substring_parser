#include <iostream>
#include <cassert>

#include "text_processor.hpp"
#include "text_provider.hpp"


int main(int argc, char** argv)
{
	if(argc == 3)
	{
		const std::string fname(*(argv + 1));
		const std::string mask(*(argv + 2));
		text_processing::TextProcessor<text_processing::TextProvider> processor{
			text_processing::TextProvider{
				fname
			},
			mask
		};
		auto result = processor.Process();
		std::cout << result.GetValue();
	}
	else
	{
		std::cout <<
		"Incorrect input params:\n" \
		"1 - parsing target path\n" \
		"2 - mask\n";
	}

	return 0;
}