#include "../include/EditorState.hpp"

// Initializer Functions
void EditorState::initView()
{
	this->view.setSize(
		sf::Vector2f(
			this->stateData->gfxSettings["width"].asFloat(),
			this->stateData->gfxSettings["height"].asFloat()
			)
		);

	this->view.setCenter(
		this->stateData->gfxSettings["width"].asFloat() / 2.0f,
		this->stateData->gfxSettings["height"].asFloat() / 2.0f
		);



}

void EditorState::initTextures()
{

}

void EditorState::initKeybinds()
{
	/* Initilaze Keybinds */

	Json::Value keybindcfg = State::readJSONConfig("../config/keybinds/EditorState.json");

	/* MapEditor Keybinds */

	this->keybinds[KeyBind::NEXT_LAYER] = this->stateData->suppKeys->at(keybindcfg["NEXT_LAYER"].asString());
	this->keybinds[KeyBind::PREV_LAYER] = this->stateData->suppKeys->at(keybindcfg["PREV_LAYER"].asString());

}

void EditorState::initFonts()
{
	if (!this->font.loadFromFile("../fonts/FFFFORWA.TTF"))
	{
		throw("ERROR::MAINMENUSTATE::COULD NOT LOAD FONT");
	}
}

void EditorState::initMapOutlines()
{

	// Initialize map's outline
	this->mapOutlines[0].position = sf::Vector2f(this->mapRect.left, this->mapRect.top);
	this->mapOutlines[0].color = sf::Color::White;

	this->mapOutlines[1].position = sf::Vector2f(this->mapRect.left + this->mapRect.width, this->mapRect.top);
	this->mapOutlines[1].color = sf::Color::White;

	this->mapOutlines[2].position = sf::Vector2f(this->mapRect.left + this->mapRect.width, this->mapRect.top + this->mapRect.height);
	this->mapOutlines[2].color = sf::Color::White;

	this->mapOutlines[3].position = sf::Vector2f(this->mapRect.left, this->mapRect.top + this->mapRect.height);
	this->mapOutlines[3].color = sf::Color::White;

	this->mapOutlines[4].position = sf::Vector2f(this->mapRect.left, this->mapRect.top);
	this->mapOutlines[4].color = sf::Color::White;

}

void EditorState::initMapGridlines()
{
	// Initialize map's gridlines

	const sf::Vector2f& mapPos = this->mapOutlines[0].position;
	for(float y = mapPos.y + this->gridSize.y; y < (mapPos.y + this->mapRect.height); y += this->gridSize.y) // Ignore first and last gridlines for map's outline
	{	
		// One line has two point
		this->mapGridlines.append(sf::Vertex(sf::Vector2f(mapPos.x, y) , sf::Color::Red));
		this->mapGridlines.append(sf::Vertex(sf::Vector2f(mapPos.x + this->mapRect.width, y) , sf::Color::Red));
	}

	for(float x = mapPos.x + this->gridSize.x; x < (mapPos.x + this->mapRect.width); x += this->gridSize.x) // Ignore first and last gridlines for map's outline
	{
		// One line has two point
		this->mapGridlines.append(sf::Vertex(sf::Vector2f(x, mapPos.y) , sf::Color::Red));
		this->mapGridlines.append(sf::Vertex(sf::Vector2f(x, mapPos.y + this->mapRect.height) , sf::Color::Red));
	}	

	this->mapGridlines.setPrimitiveType(sf::Lines);
}

// Constructors/Destructors
EditorState::EditorState(StateData* state_data, sf::IntRect map_rect, sf::Vector2i grid_size)
	: State(state_data), mapRect(map_rect), gridSize(grid_size), mapOutlines(sf::LineStrip, 5), zoomScale(1.0f)

{

	// Initilazer functions
	this->initView();
	this->initTextures();
	this->initKeybinds();
	this->initMapOutlines();
	this->initMapGridlines();

}

EditorState::~EditorState()
{

}

// Handle SFML Events
void EditorState::handleEvents(sf::Event& events)
{

	/* Panning The Map Surface */
	//std::cout << this->stateData->window->mapPixelToCoords(sf::Mouse::getPosition(*this->stateData->window)).x << ", " << this->stateData->window->mapPixelToCoords(sf::Mouse::getPosition(*this->stateData->window)).y << std::endl;

	// Static Local variables
	static bool panned = false;
	static sf::Vector2f startPanVect(0.0f, 0.0f);

	if (events.type == sf::Event::MouseButtonPressed)
	{

		startPanVect = this->stateData->window->mapPixelToCoords(sf::Vector2i(events.mouseButton.x, events.mouseButton.y));
		panned = true;
	}
	else if (events.type == sf::Event::MouseMoved && panned)
	{
		const sf::Vector2f newMousePos = this->stateData->window->mapPixelToCoords(sf::Vector2i(events.mouseMove.x, events.mouseMove.y));
		const sf::Vector2f deltaPos = startPanVect - newMousePos;

		this->view.setCenter(this->view.getCenter() + deltaPos);
		this->stateData->window->setView(this->view);

		startPanVect = this->stateData->window->mapPixelToCoords(sf::Vector2i(events.mouseMove.x, events.mouseMove.y));
	}
	else if (events.type == sf::Event::MouseButtonReleased)
		panned = false;

	/* Zooming The Map Surface */

	if (events.type == sf::Event::MouseWheelScrolled)
	{	
		const sf::Vector2f oldMousePos = this->stateData->window->mapPixelToCoords(sf::Vector2i(events.mouseWheelScroll.x, events.mouseWheelScroll.y));

		
		if (events.mouseWheelScroll.delta <= -1)
		{
			this->zoomScale *= 1.1f;
			this->view.zoom(1.1f);
		}
		else if (events.mouseWheelScroll.delta >= 1)
		{
			this->zoomScale *= 0.9f;
			this->view.zoom(0.9f);
		}

		
		this->stateData->window->setView(this->view);

		const sf::Vector2f newMousePos = this->stateData->window->mapPixelToCoords(sf::Vector2i(events.mouseWheelScroll.x, events.mouseWheelScroll.y));
		const sf::Vector2f deltaPos = oldMousePos- newMousePos;

		this->view.setCenter(this->view.getCenter() + deltaPos);
		this->stateData->window->setView(this->view);
	}


	

}
	


// Update Functions

void EditorState::updateInput(const float& dt)
{
	/* Keybinds */	

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at(KeyBind::NEXT_LAYER))))
	{
		std::cout << "Pressed NEXT_LAYER\n";
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at(KeyBind::PREV_LAYER))))
	{
		std::cout << "Pressed PREV_LAYER\n";
	}



}

void EditorState::update(const float& dt)
{

	this->updateInput(dt);
}

// Render Function
void EditorState::render(sf::RenderTarget* surface)
{
	if(!surface)
		surface = this->stateData->window;

	surface->setView(this->view);

	surface->draw(this->mapGridlines);
	surface->draw(this->mapOutlines);

}
