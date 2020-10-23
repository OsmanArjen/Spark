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

void EditorState::createMapSurface(sf::IntRect& mapRect)
{
	// TODO: use sf::LineStrip instead
	this->mapShape.setSize(sf::Vector2f(mapRect.width, mapRect.height));
	this->mapShape.setPosition(mapRect.left, mapRect.top);
	this->mapShape.setFillColor(this->fillColor);
	this->mapShape.setOutlineThickness(this->defOutlineThicc);
	this->mapShape.setOutlineColor(sf::Color(250, 150, 100));	

}

// Constructors/Destructors
EditorState::EditorState(StateData* state_data, sf::IntRect map_rect, sf::Vector2i grid_size, sf::Color fill_color)
	: State(state_data), mapRect(map_rect), gridSize(grid_size), fillColor(fill_color), zoomScale(1.0f), defOutlineThicc(2.0f)

{

	// Initilazer functions
	this->initTextures();
	this->initKeybinds();

	this->createMapSurface(map_rect);

	// Initilaze Variables

}

EditorState::~EditorState()
{

}

// Handle SFML Events
void EditorState::handleEvents(sf::Event& events)
{

	/* Panning The Map Surface */

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
		this->mapShape.setOutlineThickness(this->defOutlineThicc * this->zoomScale);

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

// Render Functions
void EditorState::renderGrid(sf::RenderTarget* surface)
{
	const sf::Vector2f& mapPos = this->mapShape.getPosition();
	sf::Vector2f  coordMapPos = this->stateData->window->mapPixelToCoords(sf::Vector2i(mapPos.x, mapPos.y));

	for(float y = coordMapPos.y; y < this->mapRect.height - this->gridSize.y; y += this->gridSize.y)
	{
		std::cout << "Y: " << y << std::endl;
	}

	for(float x = coordMapPos.x; x < this->mapRect.width - this->gridSize.x; x += this->gridSize.x)
	{
		std::cout <<" X: " << x << std::endl;
	}	
}

void EditorState::render(sf::RenderTarget* surface)
{
	if(!surface)
		surface = this->stateData->window;

	surface->setView(this->view);

	this->renderGrid(surface);

	surface->draw(this->mapShape);



}
