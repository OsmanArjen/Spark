#include "../include/EditorState.hpp"

// Initializer Functions
void EditorState::initView()
{
	m_guiView.setSize(m_defViewSize.x, m_defViewSize.y);
	m_guiView.setCenter(m_defViewSize.x / 2.0f, m_defViewSize.y / 2.0f);

	m_mapView.setSize(m_defViewSize.x, m_defViewSize.y);
	m_mapView.setCenter((m_mapGrid.x / 2.f) * m_gridSize.x, // Set view center to center of map surface
	                    (m_mapGrid.y / 2.f) * m_gridSize.y);
}

void EditorState::initTextures()
{
	// Textures for GUI
}

void EditorState::initKeybinds()
{
	/* Initilaze Keybinds */

	Json::Value keybindcfg = State::readJSONConfig("../config/keybinds/EditorState.json");

	/* MapEditor Keybinds */

	m_keybinds[KeyBind::NEXT_LAYER] = m_stateData->suppKeys->at(keybindcfg["NEXT_LAYER"].asString());
	m_keybinds[KeyBind::PREV_LAYER] = m_stateData->suppKeys->at(keybindcfg["PREV_LAYER"].asString());
	m_keybinds[KeyBind::PREVIEW]    = m_stateData->suppKeys->at(keybindcfg["PREVIEW"].asString());

}

void EditorState::initMap()
{
	
	m_map.newLayer();
	m_map.setCurrentLayer(0);
}

void EditorState::initButtons()
{
	sp::Style idle{sf::Color(75,75,75),      sf::Color(187, 187, 189), sf::Color(40,40,40),    3.f};
	sp::Style hover{sf::Color(120,120,120),  sf::Color(187, 187, 189), sf::Color(40,40,40),    3.f};
	sp::Style active{sf::Color(140,140,140), sf::Color(187, 187, 189), sf::Color(40,40,40),    3.f};
	m_resetButton.setIdleStyle(idle);
	m_resetButton.setHoverStyle(hover);
	m_resetButton.setActiveStyle(active);
}

void EditorState::initTexts()
{
	// ZoomText
	m_zoomText.setFont(*(m_stateData->mainFont));
	m_zoomText.setCharacterSize(15);
	m_zoomText.setFillColor(sf::Color(187, 187, 189));
	m_zoomText.setStyle(sf::Text::Bold);
	m_zoomText.setPosition(25.f, 25.f);

	// TileText
	m_tileText.setFont(*(m_stateData->mainFont));
	m_tileText.setCharacterSize(15);
	m_tileText.setFillColor(sf::Color(187, 187, 189));
	m_tileText.setStyle(sf::Text::Bold);
	m_tileText.setPosition(25.f, 55.f);

}

void EditorState::initGUI()
{
	initTexts();
	initButtons();
}

// Constructors/Destructors
EditorState::EditorState(StateData* state_data, 
	const sf::Vector2f& map_grid, 
	const sf::Vector2f& grid_size, 
	const sf::Color& grid_color, 
	const sf::Color& outline_color)
	: m_stateData(state_data)
	, m_defViewSize(m_stateData->gfxSettings["width"].asFloat(),
					m_stateData->gfxSettings["height"].asFloat())
	, m_mapGrid(map_grid)
	, m_gridSize(grid_size)
	, m_zoomScale(1.0)
	, m_map(m_mapGrid, 
			m_gridSize, 
			grid_color,
			outline_color)
	, m_resetButton({25.f, 80.f}, {60.f, 30.f}, "Reset", *(m_stateData->mainFont), 12)
	, m_panningState(0), m_selectionState(0)

{
	// Initilazer functions
	initView();
	initTextures();
	initKeybinds();
	initMap();
	initGUI();

	// Initialize Variables
	m_mapSelection.setFillColor(sf::Color(0, 255, 255, 75));
	m_cursorSelection.setFillColor(sf::Color(0, 255, 255, 75));
	m_zoomFactors = std::vector<float>{
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

void EditorState::resizeEvent(sf::Event& events)
{
	if (events.type == sf::Event::Resized)
	{
		m_defViewSize.x = events.size.width;
		m_defViewSize.y = events.size.height;

		m_guiView.setSize(m_defViewSize);
		m_guiView.setCenter(m_defViewSize.x / 2.0f, m_defViewSize.y / 2.0f);
		m_mapView.setSize(m_defViewSize / m_zoomScale); // Apply the zoom level
	}
}

void EditorState::panningEvent(sf::Event& events)
{
	static sf::Vector2f startPanVect(0.0f, 0.0f);
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
	{
		if (events.type == sf::Event::MouseButtonPressed
			&&
			events.mouseButton.button == sf::Mouse::Right)
		{
			if(!m_selectionState)
			{
				startPanVect = m_stateData->window->mapPixelToCoords(sf::Vector2i(events.mouseButton.x, events.mouseButton.y));
				m_panningState = true;
			}
		}
		else if (events.type == sf::Event::MouseMoved && m_panningState)
		{
			const sf::Vector2f newMousePos = m_stateData->window->mapPixelToCoords(sf::Vector2i(events.mouseMove.x, events.mouseMove.y));
			const sf::Vector2f deltaPos = startPanVect - newMousePos;

			m_mapView.setCenter(m_mapView.getCenter() + deltaPos);
			m_stateData->window->setView(m_mapView);
			startPanVect = m_stateData->window->mapPixelToCoords(sf::Vector2i(events.mouseMove.x, events.mouseMove.y));
		}
		else if (events.type == sf::Event::MouseButtonReleased
			     &&
			     events.mouseButton.button == sf::Mouse::Right)
		{
			m_panningState = false;
		}
	}
	else
		m_panningState = false;
}

void EditorState::zoomingEvent(sf::Event& events)
{
	if (events.type == sf::Event::MouseWheelScrolled)
	{
		const sf::Vector2f oldMousePos = m_stateData->window->mapPixelToCoords(sf::Vector2i(events.mouseWheelScroll.x, events.mouseWheelScroll.y));	
		if (events.mouseWheelScroll.delta < 0 && m_zoomScale != m_zoomFactors.front()) // Zoom out
		{		

			for (std::vector<float>::reverse_iterator zfactor = m_zoomFactors.rbegin(); 
					zfactor != m_zoomFactors.rend(); ++zfactor)
			{
				if (*zfactor < m_zoomScale)
				{
					m_zoomScale = *zfactor;
					break;
				}
			} 

		}
		else if (events.mouseWheelScroll.delta > 0 && m_zoomScale != m_zoomFactors.back()) // Zoom in
		{		
			for (const float& zfactor : m_zoomFactors)
			{
				if (zfactor > m_zoomScale)
				{
					m_zoomScale = zfactor;
					break;
				}
			}
		}
		// Update our view
		m_mapView.setSize(m_defViewSize / m_zoomScale);// Reset the size and Apply the zoom level
		m_stateData->window->setView(m_mapView);

		const sf::Vector2f newMousePos = m_stateData->window->mapPixelToCoords(sf::Vector2i(events.mouseWheelScroll.x, 
																							events.mouseWheelScroll.y));
		const sf::Vector2f deltaPos = oldMousePos- newMousePos;

		m_mapView.setCenter(m_mapView.getCenter() + deltaPos);
		m_stateData->window->setView(m_mapView);
	}
}

void EditorState::selectionEvent(sf::Event& events)
{
	static bool moveState{false};
	static sf::Vector2f selectionBegin;
	static sf::Vector2f selectionEnd;

	if (events.type == sf::Event::MouseButtonPressed 
		&& 
		events.mouseButton.button == sf::Mouse::Right)
	{
		if(!m_panningState)
		{
			selectionBegin = m_stateData->window->mapPixelToCoords(sf::Vector2i(events.mouseButton.x, events.mouseButton.y));
			m_selectionState = true;
		}
	}
	else if (events.type == sf::Event::MouseMoved && m_selectionState)
	{
		moveState = true;
		selectionEnd = m_stateData->window->mapPixelToCoords(sf::Vector2i(events.mouseMove.x, events.mouseMove.y));;
		sf::Vector2f minPos  = sf::Vector2f(std::min(selectionBegin.x, selectionEnd.x), 
											std::min(selectionBegin.y, selectionEnd.y));
		sf::Vector2f maxPos  = sf::Vector2f(std::max(selectionBegin.x, selectionEnd.x), 
											std::max(selectionBegin.y, selectionEnd.y));
		m_cursorSelection.setPosition(minPos);
		m_cursorSelection.setSize(maxPos - minPos);
	}
	else if (events.type == sf::Event::MouseButtonReleased 
		     && 
		     events.mouseButton.button == sf::Mouse::Right
		     &&
		     m_selectionState)
	{
		m_selectedTiles.clear();
		if(moveState)
		{
			sf::FloatRect tmpRect{m_cursorSelection.getPosition(), m_cursorSelection.getSize()};
			for(sp::Tile& tile : m_map.getCurrentLayer()->getTileRects())
			{
				if (tile.getGlobalBounds().intersects(tmpRect))
				{
					m_selectedTiles.push_back(&tile);
					tile.setSelected(true);
				}
				else
					if (tile.getSelected())
						tile.setSelected(false);					
			}			
		}
		else
		{	
			for(sp::Tile& tile : m_map.getCurrentLayer()->getTileRects())
			{
				if (tile.getGlobalBounds().contains(selectionBegin) and !tile.getSelected())
				{
					tile.setSelected(true);
					m_selectedTiles.push_back(&tile);
					break;
				}
				else
					if (tile.getSelected())
						tile.setSelected(false);
			}		
		}


		// Reset states
		moveState = false;
		m_selectionState = false;

		// Reset selection model
		selectionBegin = sf::Vector2f(0.0f, 0.0f);
		selectionEnd   = sf::Vector2f(0.0f, 0.0f);

		// Set selection shapes
		m_cursorSelection.setSize({0.0f, 0.0f});
		m_cursorSelection.setPosition(0.0f, 0.0f);

		if(!m_selectedTiles.empty())
		{
			sp::Tile* &first = m_selectedTiles.front();
			sp::Tile* &back  = m_selectedTiles.back();
			m_mapSelection.setPosition(first->getPosition());
			m_mapSelection.setSize( (back->getPosition() - first->getPosition()) + m_gridSize);
		}
		else
		{
			m_mapSelection.setSize({0.0f, 0.0f});
			m_mapSelection.setPosition(0.0f, 0.0f);
		}

	}

}

// Handle SFML Events
void EditorState::handleEvents(sf::Event& events)
{
	// Resize Window
	resizeEvent(events);	
	// Panning The Map Surface
	panningEvent(events);
	// Zooming The Map Surface
	zoomingEvent(events);
	// Cursor Selection 
	selectionEvent(events);
}
	


// Update Functions
void EditorState::updateGUI()
{
	// Mouse positions
	sf::Vector2i mPosS = sf::Mouse::getPosition(*(m_stateData->window)); // Screen Pos
	sf::Vector2f mPosW = m_stateData->window->mapPixelToCoords(mPosS); // World Pos

	// Reset button
	m_resetButton.update(mPosS);
	if (m_resetButton.isPressed())
	{
		m_zoomScale = 1.f;
		m_mapView.setSize(m_defViewSize.x, m_defViewSize.y);
		m_mapView.setCenter((m_mapGrid.x / 2.f) * m_gridSize.x, 
		                    (m_mapGrid.y / 2.f) * m_gridSize.y);
	}

	// Tile Text

	sf::Vector2i tilePos{static_cast<int>(mPosW.x) / static_cast<int>(m_gridSize.x),
						 static_cast<int>(mPosW.y) / static_cast<int>(m_gridSize.y)};
	sf::Vector2i mPosW_i{static_cast<sf::Vector2i>(mPosW)};

	if((mPosW_i.x <= 0) && (mPosW_i.y <= 0))
		m_tileText.setString('(' + std::to_string( (tilePos.x - 1) ) + 
		                     ',' + std::to_string( (tilePos.y - 1) ) + 
		                     ')');

	else if((mPosW_i.x >= 0) && (mPosW_i.y <= 0))
		m_tileText.setString('(' + std::to_string((tilePos.x) ) + 
		                     ',' + std::to_string( (tilePos.y - 1) ) + 
		                     ')');

	else if((mPosW_i.x <= 0) && (mPosW_i.y >= 0))
		m_tileText.setString('(' + std::to_string((tilePos.x - 1) ) + 
		                     ',' + std::to_string( (tilePos.y) ) + 
		                     ')');
	else
		m_tileText.setString('(' + std::to_string( (tilePos.x) ) + 
		                     ',' + std::to_string( (tilePos.y) ) + 
		                     ')');		

	// Zoom Text
	const int precisionVal{2}; // i want 2 digits
	std::string text{std::to_string(m_zoomScale * 100)};
	m_zoomText.setString(text.substr(0, text.find(".") + precisionVal + 1) + "%"); // +1: i want to skip dot
}

void EditorState::updateInput(const float& dt)
{
	
	/* Keybinds */	
	using KeyCode = sf::Keyboard::Key;

	// Key state for layer related keybinds
	static bool layerKeyState{false};
	if (sf::Keyboard::isKeyPressed(KeyCode(m_keybinds.at(KeyBind::NEXT_LAYER))))
	{
		if (!layerKeyState)
		{
			m_map.nextLayer();
			layerKeyState = true;
		}
	}
	else if (sf::Keyboard::isKeyPressed(KeyCode(m_keybinds.at(KeyBind::PREV_LAYER))))
	{
		if (!layerKeyState)
		{
			m_map.prevLayer();
			layerKeyState = true;
		}
	}
	else
	{
		if (layerKeyState)
		{	
			layerKeyState = false;
		}
	}

	// Preview mod
	if (sf::Keyboard::isKeyPressed(KeyCode(m_keybinds.at(KeyBind::PREVIEW))))
	{
		if(!m_map.getPreview())
		{
			m_map.setPreview(true);
		}
	}
	else
	{
		if(m_map.getPreview())
		{
			m_map.setPreview(false);
		}
	}
}

void EditorState::update(const float& dt)
{
	// Update input of program
	updateInput(dt);

	// Update GUI
	updateGUI();
}

// Render Function
void EditorState::render()
{
	// Render Map
	m_map.render(m_stateData->window);
	m_stateData->window->draw(m_mapSelection);
	m_stateData->window->draw(m_cursorSelection);
	// Draw GUI's without view
	m_stateData->window->setView(m_guiView);
	// Render buttons
	m_resetButton.render(m_stateData->window);
	// Render texts
	m_stateData->window->draw(m_zoomText);
	m_stateData->window->draw(m_tileText);
	// Reset view
	m_stateData->window->setView(m_mapView);
}
