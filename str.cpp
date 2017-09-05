#include "str_util.h"

#include <sstream>
#include <algorithm>
#include <functional>

namespace str
{
	std::vector<std::string> split( std::string const& sentence, char delimeter)
	{
		using namespace std;

		stringstream ss(sentence);
		vector<string>	words;
		string	word;
		while( getline( ss, word, delimeter ) )
		{
			words.push_back(word);	
		}

		return words;
	}

	void replace( std::string& sentence, std::string const& old_phrase, std::string const& new_phrase)
	{
		size_t pos = sentence.find( old_phrase, 0);
		while( pos != std::string::npos )
		{
			sentence.replace( pos++, old_phrase.size(), new_phrase );
			pos = sentence.find( old_phrase, pos );
		}
	}

	static inline std::string& ltrim( std::string& str)
	{
		str.erase(str.begin(), std::find_if(str.begin(), str.end(),
					std::not1(std::ptr_fun<int, int>(std::isspace) ) ) );
		return str;
	}

	static inline std::string& rtrim(std::string& str) {
		str.erase(std::find_if(str.rbegin(), str.rend(),
					std::not1(std::ptr_fun<int, int>(std::isspace) ) ).base(), str.end());
		return str;
	}

	std::string& trim( std::string& str) {
		return ltrim(rtrim(str));
	}

}
