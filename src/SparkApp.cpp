#include "../include/SparkApp.hpp"

// Initializer Functions
void SparkApp::initWindow()
{

	this->gfxConfig = State::readJSONConfig("../config/graphics.json");

	
	this->gfxContext.antialiasingLevel = this->gfxConfig["antialiasing_level"].asInt();

	if (this->gfxConfig["fullscreen"].asBool())
		this->window = new sf::RenderWindow(
			sf::VideoMode(this->gfxConfig["width"].asInt(),this->gfxConfig["height"].asInt()), 
			this->gfxConfig["title"].asString(),
			sf::Style::Fullscreen,
			this->gfxContext);
			
	else
		this->window = new sf::RenderWindow(
			sf::VideoMode(this->gfxConfig["width"].asInt(),this->gfxConfig["height"].asInt()), 
			this->gfxConfig["title"].asString(),
			sf::Style::Titlebar | sf::Style::Close,
			this->gfxContext);

	this->window->setFramerateLimit(this->gfxConfig["framerate_limit"].asInt());
	this->window->setVerticalSyncEnabled(this->gfxConfig["vertical_sync"].asBool());
}

void SparkApp::initKeys()
{
	std::ifstream ifs("../config/suppKeys.ini");

	if (ifs.is_open())
	{
		std::string keyname = "";
		int keyvalue = 0;
		
		while (ifs >> keyname >> keyvalue)
		{
			this->suppKeys[keyname] = keyvalue;
		}
	}

	ifs.close();


}

void SparkApp::initStateData()
{
	this->stateData.window      = this->window;
	this->stateData.gfxSettings = this->gfxConfig;
	this->stateData.suppKeys    = &this->suppKeys;
	this->stateData.states      = &this->states;
}

void SparkApp::initStates()
{

	this->states.push(new EditorState(&this->stateData, {50, 50, 500, 500}, {25, 25}));
}




// Constructors/Destructors
SparkApp::SparkApp()
{
	
	this->initKeys();
	this->initWindow();
	this->initStateData();
	this->initStates();
}

SparkApp::~SparkApp()
{

	delete this->window;

	while(!this->states.empty())
	{
		delete this->states.top();
		this->states.pop();
	}
}

// Main loop functions
void SparkApp::updateDeltaTime()
{

	this->dt = this->clock.restart().asSeconds();
}

void SparkApp::updateSFMLEvents()
{
	while (this->window->pollEvent(this->sfEvents))
	{
		if (this->sfEvents.type == sf::Event::Closed)
			this->window->close();


		// Handle Events in States
		if (!this->states.empty())
		{

			this->states.top()->handleEvents(this->sfEvents);
		}
	}
}

void SparkApp::update()
{

	this->updateSFMLEvents();

	if (!this->states.empty())
	{
		if (this->window->hasFocus())
		{
			this->states.top()->update(this->dt);
			
			if(this->states.top()->getQuit())
			{

				delete this->states.top();
				this->states.pop();
			}
		}

	}
	else
	{
		// End the Application


		this->window->close();
	}
}

void SparkApp::render()
{
	this->window->clear();

	// Render Current State...
	if(!this->states.empty())
	{
		this->states.top()->render();
	}

	this->window->display();
}

void SparkApp::run()
{

	while(this->window->isOpen())
	{
		this->updateDeltaTime();
		this->update();
		this->render();
	}
}

