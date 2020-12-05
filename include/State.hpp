#ifndef STATE_HPP
#define STATE_HPP

#include "modules.hpp"


enum class KeyBind
{
	NEXT_LAYER,
	PREV_LAYER
	
}; // KeyBind

class State; // Declare State class

class StateData
{
public:
	/* Container Class for State Class*/
	
	// Default Constructor
	StateData() {}

	// Variables

	Json::Value gfxSettings;

	std::stack<State*>* states;

	sf::RenderWindow* window;

	std::map<std::string, int>* suppKeys;

};


class State
{
protected:
	// Variables 
	bool quit;
	StateData* stateData;
	std::map<KeyBind, int> keybinds;
	std::map<const char*, sf::Texture> textures;

	// Functions
	virtual void initKeybinds() = 0;
	
public:
	// Constructors/Destructors
	State(StateData* state_data);
	virtual ~State();

	// Accessors
	const  bool& getQuit() const;

	// Functions	
	void endState(); 

	// Handle SFML Events
	virtual void handleEvents(sf::Event& events) = 0;

	// Update interfaces
	virtual void updateInput(const float& dt) = 0;
	virtual void update(const float& dt) = 0;

	// Render interface
	virtual void render() = 0;

	// Static members
	static Json::Value readJSONConfig(const char* path);
	
};

#endif // STATE_HPP
