#include "../include/State.hpp"


// Constructors/Destructors
State::State()
	: m_quit(false)
{
	
}



// Accessors
const bool& State::getQuit() const
{
	return m_quit;
}


// Functions
void State::quitState()
{
	if(!m_quit)
	{
		m_quit = true;
	}
}

// Static Functions
Json::Value State::readJSONConfig(const char* path) // For Config Files
{

	std::ifstream file(path, std::ifstream::binary);

	Json::CharReaderBuilder builder;
	Json::Value value;
	std::string errs;
	bool state{Json::parseFromStream(builder, file, &value, &errs)};
	if ( !state ) // Error while parsing JSON file
	{
		std::cerr << errs << std::endl; 
	}
	
	file.close();

	return value;
}
