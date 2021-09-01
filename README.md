# About:
Multithread text parser for substing searching from text by given mask
Input:
1 - path to file to parse
2 - searching string mask(symbol '?' means "every symbol" except new line symbol)

Output:
<line_number> <character_number> <found_word>

# Intro:

Init repo state:
git submodule init
git submodule update


Build targets:
cd <repo_path>
cmake .
make


Run tests:
make parser_test
./parser_test


Run application:
make substring_parser
./substring_parser <path_to_text_file> <mask>


Example run:
./substring_parser test_data/light_test_file ag?

Example output:
1 0 aga
1 7 agg
3 0 agg
6 1 age
11 16 aga


# Dependencies:
google test framework