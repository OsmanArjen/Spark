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
	this->textures["wall_1"].loadFromFile("../assets/wall_1.png");
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

void EditorState::initMap()
{
	
	this->map.newLayer();
	this->map.setCurrentLayer(0);
	// deneme kaldır sonra
	for( sp::Tile& tile : this->map.getCurrentLayer()->getTileRects())
	{
			// DENEME SONRA KALDIR
			tile.setSelectStyle(sf::Color(0, 255, 255, 75));
			tile.setSelected(false);
	}
}

void EditorState::initTexts()
{
	this->zoomText.setFont(this->font);
	this->zoomText.setCharacterSize(15);
	this->zoomText.setFillColor(sf::Color(173,216,230));
	this->zoomText.setStyle(sf::Text::Bold);
	this->zoomText.setPosition(sf::Vector2f(25, 25));

}

// Constructors/Destructors
EditorState::EditorState(StateData* state_data, 
	const sf::FloatRect& map_rect, 
	const sf::Vector2f& grid_size, 
	const sf::Color& grid_color, 
	const sf::Color& outline_color)
	: State(state_data)
	, mapRect(map_rect)
	, gridSize(grid_size)
	, zoomScale(1.0)
	, map(this->mapRect, 
		  this->gridSize, 
		  grid_color, outline_color)

{
	// Initilazer functions
	this->initView();
	this->initTextures();
	this->initKeybinds();
	this->initFonts();
	this->initMap();
	this->initTexts();

	// Initialize Variables
	this->cursorSelection.setFillColor(sf::Color(0, 255, 255, 75));
	this->zoomFactors = std::vector<float>{
		0.0625,
		0.125,
		0.25,
		0.33,
		0.5,
		0.75,
		1.0,
		1.5,
		2.0,
		3.0,
		4.0,
		5.5,
		8.0,
		11.0,
		16.0,
		23.0,
		32.0,
		45.0,
		64.0,
		90.0,
		128.0,
	};
}

EditorState::~EditorState()
{

}

void EditorState::panningEvent(sf::Event& events)
{
	static bool panned = false;
	static sf::Vector2f startPanVect(0.0f, 0.0f);
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
	{
		if (events.type == sf::Event::MouseButtonPressed && events.mouseButton.button == sf::Mouse::Right)
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
		else if (events.type == sf::Event::MouseButtonReleased && events.mouseButton.button == sf::Mouse::Right)
			panned = false;
	}
}

void EditorState::zoomingEvent(sf::Event& events)
{
	if (events.type == sf::Event::MouseWheelScrolled)
	{	
		const sf::Vector2f oldMousePos = this->stateData->window->mapPixelToCoords(sf::Vector2i(events.mouseWheelScroll.x, events.mouseWheelScroll.y));	
		if (events.mouseWheelScroll.delta <= -1) // Zoom out
		{
			for (std::vector<float>::reverse_iterator zfactor = this->zoomFactors.rbegin(); 
					zfactor != this->zoomFactors.rend(); ++zfactor)
			{
				if (*zfactor < this->zoomScale)
				{
					this->zoomScale = *zfactor;
					break;
				}
			} 

		}
		else if (events.mouseWheelScroll.delta >= 1) // Zoom in
		{		
			for (const float& zfactor : this->zoomFactors)
			{
				if (zfactor > this->zoomScale)
				{
					this->zoomScale = zfactor;
					break;
				}
			}
		}
		// Update our view
		this->view.setSize(this->stateData->window->getDefaultView().getSize()); // Reset the size
		this->view.setSize(this->view.getSize() / this->zoomScale);// Apply the zoom level
		this->stateData->window->setView(this->view);

		const sf::Vector2f newMousePos = this->stateData->window->mapPixelToCoords(sf::Vector2i(events.mouseWheelScroll.x, 
																								events.mouseWheelScroll.y));
		const sf::Vector2f deltaPos = oldMousePos- newMousePos;

		this->view.setCenter(this->view.getCenter() + deltaPos);
		this->stateData->window->setView(this->view);
	}
}

void EditorState::selectionEvent(sf::Event& events)
{
	static bool selectionState{false};
	static bool moveState{false};
	static sf::Vector2f selectionBegin;
	static sf::Vector2f selectionEnd;
	if (events.type == sf::Event::MouseButtonPressed && events.mouseButton.button == sf::Mouse::Right)
	{
		selectionBegin = this->stateData->window->mapPixelToCoords(sf::Vector2i(events.mouseButton.x, events.mouseButton.y));
		selectionState = true;
	}
	else if (events.type == sf::Event::MouseMoved && selectionState)
	{
		moveState = true;
		selectionEnd = this->stateData->window->mapPixelToCoords(sf::Vector2i(events.mouseMove.x, events.mouseMove.y));;
		sf::Vector2f minPos  = sf::Vector2f(std::min(selectionBegin.x, selectionEnd.x), 
											std::min(selectionBegin.y, selectionEnd.y));
		sf::Vector2f maxPos  = sf::Vector2f(std::max(selectionBegin.x, selectionEnd.x), 
											std::max(selectionBegin.y, selectionEnd.y));
		this->cursorSelection.setPosition(minPos);
		this->cursorSelection.setSize(maxPos - minPos);
	}
	else if (events.type == sf::Event::MouseButtonReleased && events.mouseButton.button == sf::Mouse::Right)
	{
		// TODO: make panned, moved, selectionState... member variable and link them (mesela panned varsa selection olamaz bu şekilde)
		this->selectedTiles.clear();
		if(moveState)
		{
			sf::FloatRect tmpRect{this->cursorSelection.getPosition(), this->cursorSelection.getSize()};
			for( sp::Tile& tile : this->map.getCurrentLayer()->getTileRects())
			{
				if (tile.getGlobalBounds().intersects(tmpRect))
				{
					tile.setSelected(true);// bir vektöre pushla çünkü seçili
					this->selectedTiles.push_back(&tile);
				}
				else
				{
					if(tile.getSelected())
						tile.setSelected(false);
				}
			}			
		}
		else
		{	
			for( sp::Tile& tile : this->map.getCurrentLayer()->getTileRects())
			{
				if (tile.getGlobalBounds().contains(selectionBegin) && !tile.getSelected())
				{
					tile.setSelected(true); 
					this->selectedTiles.push_back(&tile);
				}
				else
					if(tile.getSelected())
						tile.setSelected(false);
			}		
		}

		for(sp::Tile* &tile : this->selectedTiles)
		{
			tile->setTexture(this->textures["wall_1"]);
		}

		moveState      = false;
		selectionState = false;
		selectionBegin = sf::Vector2f(0.0f, 0.0f);
		selectionEnd   = sf::Vector2f(0.0f, 0.0f);
		this->cursorSelection.setSize(sf::Vector2f(0.0f, 0.0f));
		this->cursorSelection.setPosition(0.0f, 0.0f);
	}

}

// Handle SFML Events
void EditorState::handleEvents(sf::Event& events)
{
	// Panning The Map Surface
	this->panningEvent(events);
	// Zooming The Map Surface
	this->zoomingEvent(events);
	// Cursor Selection to current layer 
	this->selectionEvent(events);// (NOT: şimdi deneme olarak normal map üzerinde yapıcam)
}
	


// Update Functions
void EditorState::updateInput(const float& dt)
{
	
	/* Keybinds */	

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at(KeyBind::NEXT_LAYER))))
		this->map.nextLayer();
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at(KeyBind::PREV_LAYER))))
		this->map.prevLayer();
}

void EditorState::update(const float& dt)
{
	// Update input of program
	this->updateInput(dt);
	// Update Map
	this->map.update();
	// Update GUI
	this->zoomText.setString(std::to_string(this->zoomScale   * 100 ) + "%");// TEST: remove later
}

// Render Function
void EditorState::render()
{
	// Render Map
	this->map.render(*(this->stateData->window));
	this->stateData->window->draw(this->cursorSelection);
	// Draw GUI's without view
	this->stateData->window->setView(this->stateData->window->getDefaultView());
	this->stateData->window->draw(this->zoomText);
	this->stateData->window->setView(this->view);
}
