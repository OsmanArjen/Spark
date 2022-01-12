#include "MapEditor.hpp"

// Initializer Functions
void MapEditor::initView()
{
	m_guiView.setSize(m_defViewSize.x, m_defViewSize.y);
	m_guiView.setCenter(m_defViewSize.x / 2.0f, m_defViewSize.y / 2.0f);

	m_mapView.setSize(m_defViewSize.x, m_defViewSize.y);
	m_mapView.setCenter((m_mapGrid.x * m_gridSize.x) / 2.f, // Set view center to center of map surface
	                    (m_mapGrid.y * m_gridSize.y) / 2.f);
	m_window->setView(m_mapView);
}

void MapEditor::initTextures()
{
	m_textures["set"].loadFromFile("res/Tileset.png");
}

void MapEditor::initKeybinds()
{
	/* Initilaze Keybinds */
	std::ifstream file("config/keybinds.json", std::ifstream::binary);

	Json::CharReaderBuilder builder;
	Json::Value keybinds;
	std::string errs;

	if ( !Json::parseFromStream(builder, file, &keybinds, &errs) ) // Error while parsing JSON file
	{
		std::cerr << errs << std::endl; 
	}
	
	file.close();

	m_keybinds.bind("PREVIEW", keybinds["PREVIEW"].asString());
}

void MapEditor::initFont()
{
	if (!m_font.loadFromFile("fonts/FFFFORWA.TTF"))
	{
		throw("ERROR::FONTS::>COULD NOT LOAD FONT");
	}
}

void MapEditor::initMap()
{
	
	m_map.newLayer();
	m_map.setCurrentLayer(0);
}

void MapEditor::initGUI()
{
	// ZoomText
	m_zoomText.setFont(m_font);
	m_zoomText.setCharacterSize(15);
	m_zoomText.setFillColor(sf::Color(187, 187, 189));
	m_zoomText.setStyle(sf::Text::Bold);
	m_zoomText.setPosition(25.f, 25.f);

	// TileText
	m_tileText.setFont(m_font);
	m_tileText.setCharacterSize(15);
	m_tileText.setFillColor(sf::Color(187, 187, 189));
	m_tileText.setStyle(sf::Text::Bold);
	m_tileText.setPosition(25.f, 55.f);

}



// Constructors/Destructors
MapEditor::MapEditor(sf::RenderWindow*   window,
                     const sf::Vector2f& map_grid, 
                     const sf::Vector2f& grid_size, 
                     const sf::Color&    grid_color, 
                     const sf::Color&    outline_color)
	: m_window(window)
	, m_mapGrid(map_grid)
	, m_gridSize(grid_size)
	, m_defViewSize(window->getSize())
	, m_zoomScale(1.0)
	, m_map(m_mapGrid, 
			m_gridSize, 
			grid_color,
			outline_color)
	, m_texselector(new TextureSelector(window,
	                                    grid_size,
	                                    grid_color,
	                                    outline_color))
	, m_panningState(false), m_selectionState(false)

{
	// Initilazer functions
	initView();
	initTextures();
	initKeybinds();
	initFont();
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

MapEditor::~MapEditor()
{
	delete m_texselector;
}

void MapEditor::resizeEvent(sf::Event& events)
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

void MapEditor::panningEvent(sf::Event& events)
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
				startPanVect = m_window->mapPixelToCoords(sf::Vector2i(events.mouseButton.x, events.mouseButton.y));
				m_panningState = true;
			}
		}
		else if (events.type == sf::Event::MouseMoved && m_panningState)
		{
			const sf::Vector2f newMousePos = m_window->mapPixelToCoords(sf::Vector2i(events.mouseMove.x, events.mouseMove.y));
			const sf::Vector2f deltaPos = startPanVect - newMousePos;

			m_mapView.setCenter(m_mapView.getCenter() + deltaPos);
			m_window->setView(m_mapView);
			startPanVect = m_window->mapPixelToCoords(sf::Vector2i(events.mouseMove.x, events.mouseMove.y));
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

void MapEditor::zoomingEvent(sf::Event& events)
{
	if (events.type == sf::Event::MouseWheelScrolled)
	{
		const sf::Vector2f oldMousePos{m_window->mapPixelToCoords(sf::Vector2i(events.mouseWheelScroll.x, 
		                                                                       events.mouseWheelScroll.y))};	
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
		/* Update our view */
		//-------------------
		m_mapView.setSize(m_defViewSize / m_zoomScale);// Reset the size and Apply the zoom level
		m_window->setView(m_mapView);

		const sf::Vector2f newMousePos = m_window->mapPixelToCoords(sf::Vector2i(events.mouseWheelScroll.x, 
		                                                                         events.mouseWheelScroll.y));
		const sf::Vector2f deltaPos = oldMousePos - newMousePos;

		m_mapView.setCenter(m_mapView.getCenter() + deltaPos);
		m_window->setView(m_mapView);
	}
}

void MapEditor::selectionEvent(sf::Event& events)
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
			selectionBegin = m_window->mapPixelToCoords(sf::Vector2i(events.mouseButton.x, events.mouseButton.y));
			m_selectionState = true;
		}
	}
	else if (events.type == sf::Event::MouseMoved && m_selectionState)
	{
		// Change moveState
		moveState = true;

		// Set selection size and pos
		selectionEnd = m_window->mapPixelToCoords(sf::Vector2i(events.mouseMove.x, events.mouseMove.y));;
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
			sf::FloatRect cursorSelectionRect{m_cursorSelection.getPosition(), 
			                                  m_cursorSelection.getSize()};
			for(std::vector<sp::Tile>& tilerow : m_map.getCurrentLayer()->getTileRects())
			{
				for(sp::Tile& tile : tilerow)
				{	
					if (tile.getGlobalBounds().intersects(cursorSelectionRect))
					{
						m_selectedTiles.push_back(&tile);
						tile.setSelected(true);
					}
					else
						if (tile.getSelected())
							tile.setSelected(false);					
				}
			}			
		}
		else
		{	
			for(std::vector<sp::Tile>& tilerow : m_map.getCurrentLayer()->getTileRects())
			{
				for(sp::Tile& tile : tilerow)
				{	
					if (tile.getGlobalBounds().contains(selectionBegin) and !tile.getSelected())
					{
						tile.setSelected(true);
						m_selectedTiles.push_back(&tile);
						break;
					}
					else
					{
						if (tile.getSelected())
						{
							tile.setSelected(false);
						}
					}
				}
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
			sp::Tile* &last  = m_selectedTiles.back();
			m_mapSelection.setPosition(first->getPosition());
			m_mapSelection.setSize( (last->getPosition() - first->getPosition()) + m_gridSize);
		}
		else
		{
			m_mapSelection.setSize({0.0f, 0.0f});
			m_mapSelection.setPosition(0.0f, 0.0f);
		}

	}

}

// Handle SFML Events
void MapEditor::handleEvents(sf::Event& events)
{
	// DENEME
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		m_texselector->handleEvents(events);
	}
	else
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

}
	


// Update Functions
void MapEditor::updateGUI()
{
	// Mouse position
	sf::Vector2f mPosW = m_window->mapPixelToCoords(sf::Mouse::getPosition(*(m_window)));   // World Pos

	// Tile Text

	sf::Vector2i tilePos{static_cast<int>(mPosW.x) / static_cast<int>(m_gridSize.x),
						 static_cast<int>(mPosW.y) / static_cast<int>(m_gridSize.y)};
	sf::Vector2i mPosW_i{static_cast<sf::Vector2i>(mPosW)};

	if((mPosW_i.x <= 0) && (mPosW_i.y <= 0))
		m_tileText.setString('(' + std::to_string( (tilePos.x - 1) ) + 
		                     ',' + std::to_string( (tilePos.y - 1) ) + 
		                     ')');

	else if((mPosW_i.x >= 0) && (mPosW_i.y <= 0))
		m_tileText.setString('(' + std::to_string( (tilePos.x) ) + 
		                     ',' + std::to_string( (tilePos.y - 1) ) + 
		                     ')');

	else if((mPosW_i.x <= 0) && (mPosW_i.y >= 0))
		m_tileText.setString('(' + std::to_string( (tilePos.x - 1) ) + 
		                     ',' + std::to_string( (tilePos.y) ) + 
		                     ')');
	else
		m_tileText.setString('(' + std::to_string( (tilePos.x) ) + 
		                     ',' + std::to_string( (tilePos.y) ) + 
		                     ')');		

	// Zoom Text
	const int precVal{2}; // precision value
	std::string text{std::to_string(m_zoomScale * 100)};
	m_zoomText.setString(text.substr(0, text.find(".") + precVal + 1) + "%"); // +1: i want to skip dot
}





void MapEditor::updateInput(const float& dt)
{

	// Preview mod
	if (m_keybinds.getPressed("PREVIEW"))
		m_map.setPreview(true);
	else
		m_map.setPreview(false);

}

void MapEditor::update(const float& dt)
{
	// Update the editor
	updateInput(dt);
	updateGUI();
}

// Render Function
void MapEditor::render()
{
	// DENEME
	if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		//-*-We dont need to set map view because already current view is mapView!

		// Render Map
		m_map.render(m_window);

		// Render selections
		m_window->draw(m_mapSelection);
		m_window->draw(m_cursorSelection);

		// Draw GUI without view
		m_window->setView(m_guiView);

		// Render texts
		m_window->draw(m_zoomText);
		m_window->draw(m_tileText);

		// Reset view
		m_window->setView(m_mapView);
	}
	else
	{
		// DENEME için sonra kaldırıcam
		m_window->setView(m_window->getDefaultView());
		m_texselector->render();
		m_window->setView(m_mapView);
	}
}
