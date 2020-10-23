#include "../include/State.hpp"


// Constructors/Destructors
State::State(StateData* state_data)
{
	this->stateData = state_data;
	this->quit = false;
}

State::~State()
{
	
}

// Accessors
const bool& State::getQuit() const
{
	return this->quit;
}


// Functions
void State::endState()
{
	this->quit = true;
}

// Static Functions
Json::Value State::readJSONConfig(const char* path) // For Config Files
{
	std::ifstream file(path);

	Json::Reader reader;
	Json::Value value;
		
	reader.parse(file, value);

	file.close();

	return value;
}
