#include "../include/SparkApp.hpp"

// Initializer Functions
void SparkApp::initStyles()
{
	m_styleConfig = State::readJSONConfig("../config/styles.json");
}

void SparkApp::initFonts()
{
	if (!m_mainFont.loadFromFile("../fonts/FFFFORWA.TTF"))
	{
		throw("ERROR::FONTS::>COULD NOT LOAD FONT");
	}
}

void SparkApp::initIcon()
{
   m_icon.loadFromFile("../assets/icon.png");
}

void SparkApp::initWindow()
{

	m_gfxConfig = State::readJSONConfig("../config/graphics.json");

	
	m_gfxContext.antialiasingLevel = m_gfxConfig["antialiasing_level"].asInt();

	if (m_gfxConfig["fullscreen"].asBool())
		m_window = new sf::RenderWindow(
			sf::VideoMode(m_gfxConfig["width"].asInt(),m_gfxConfig["height"].asInt()), 
			m_gfxConfig["title"].asString(),
			sf::Style::Fullscreen,
			m_gfxContext);
			
	else
		m_window = new sf::RenderWindow(
			sf::VideoMode(m_gfxConfig["width"].asInt(),m_gfxConfig["height"].asInt()), 
			m_gfxConfig["title"].asString(),
			sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close,
			m_gfxContext);

	m_window->setFramerateLimit(m_gfxConfig["framerate_limit"].asInt());
	m_window->setVerticalSyncEnabled(m_gfxConfig["vertical_sync"].asBool());
	m_window->setIcon(m_icon.getSize().x, m_icon.getSize().y, m_icon.getPixelsPtr());
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
			m_suppKeys[keyname] = keyvalue;
		}
	}

	ifs.close();


}

void SparkApp::initStateData()
{
	m_stateData.window      = m_window;
	m_stateData.gfxSettings = m_gfxConfig;
	m_stateData.mainFont    = &m_mainFont;
	m_stateData.suppKeys    = &m_suppKeys;
	m_stateData.states      = &m_states;
}

void SparkApp::initStates()
{
	// TODO: get color styles from config files
	m_states.push(new EditorState(&m_stateData, {20, 20}, {32, 32}, {4,217,255}, sf::Color::White));
}




// Constructors/Destructors
SparkApp::SparkApp()
{
	// Initilazer functions
	initFonts();
	initStyles();
	initIcon();
	initWindow();
	initKeys();
	initStateData();
	initStates();
}

SparkApp::~SparkApp()
{

	delete m_window;
	while(!m_states.empty())
	{
		delete m_states.top();
		m_states.pop();
	}
}

// Main loop functions
void SparkApp::updateDeltaTime()
{
	m_deltatime = m_clock.restart().asSeconds();
}

void SparkApp::updateSFMLEvents()
{
	while (m_window->pollEvent(m_sfEvents))
	{
		if (m_sfEvents.type == sf::Event::Closed)
			m_window->close();

		// Handle Events in States
		if (!m_states.empty())
		{

			m_states.top()->handleEvents(m_sfEvents);
		}
	}
}

void SparkApp::update()
{
	updateSFMLEvents();

	if (!m_states.empty())
	{
		if (m_window->hasFocus())
		{
			m_states.top()->update(m_deltatime);	
			if(m_states.top()->getQuit())
			{
				delete m_states.top();
				m_states.pop();
			}
		}
	}
	else
	{
		// End the Application
		m_window->close();
	}
}

void SparkApp::render()
{
	m_window->clear(sf::Color(51,51,51));

	// Render Current State...
	if(!m_states.empty())
	{
		m_states.top()->render();
	}

	m_window->display();
}

void SparkApp::run()
{
	while(m_window->isOpen())
	{
		updateDeltaTime();
		update();
		render();
	}
}

