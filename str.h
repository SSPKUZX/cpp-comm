#pragma once

#include <string>
#include <vector>

namespace str 
{
	// split sentence into words by delimeter
	std::vector<std::string> split( std::string const& sentence, char delimeter=' ');

	// replace all old_phrase in sentence into new_phrase
	void replace( std::string& sentence, std::string const& old_phrase, std::string const& new_phrase);

	// remove spaces before and after 'str'
	std::string& trim( std::string& str);
}
