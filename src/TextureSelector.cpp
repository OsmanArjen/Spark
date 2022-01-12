/*
TODO: WE NEED A RENDERTEXURE FOR LAYER BETWEEN WINDOW AND TEXTURE SELECTOR
*/

#include "TextureSelector.hpp"

void TextureSelector::initView()
{
	m_mapView.setSize(m_defViewSize.x, m_defViewSize.y);
	m_mapView.setCenter((m_mapGrid.x * m_gridSize.x) / 2.f, // Set view center to center of map surface
	                    (m_mapGrid.y * m_gridSize.y) / 2.f);
	m_renderLayer.setView(m_mapView);
}

void TextureSelector::initTexMap()
{
	m_texmap.newLayer();
	m_texmap.setCurrentLayer(0);
}

void TextureSelector::initRenderTexture()
{
	sf::Vector2u size{m_window->getSize()};
	m_renderLayer.create(size.x, size.y); // Just for now, i need to use window (Width x Height)
}

// Constructor/Destructor
TextureSelector::TextureSelector(sf::RenderWindow*   window, 
	                             const sf::Color&    grid_color, 
	                             const sf::Color&    outline_color)
	: m_window(window)
	, m_mapGrid({1, 1}) // Default is 1x1 grid because no tileset file processed 
	, m_gridSize(grid_size)
	, m_defViewSize(window->getSize())
	, m_zoomScale(1.0f)
	, m_texmap(m_mapGrid, 
	           m_gridSize,
	           grid_color,
	           outline_color)
	, m_panningState(false), m_selectionState(false) 
{
	// Initialize TextureSelector
	initRenderTexture();
	initTexMap();
	initView();

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

TextureSelector::~TextureSelector()
{

}

// Handle SFML Events
//-*-void resizeEvent: im not gonna add this just for now because im planning to implement imgui
void TextureSelector::panningEvent(sf::Event& events)
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
				startPanVect = m_renderLayer.mapPixelToCoords(sf::Vector2i(events.mouseButton.x, events.mouseButton.y));
				m_panningState = true;
			}
		}
		else if (events.type == sf::Event::MouseMoved && m_panningState)
		{
			const sf::Vector2f newMousePos = m_renderLayer.mapPixelToCoords(sf::Vector2i(events.mouseMove.x, events.mouseMove.y));
			const sf::Vector2f deltaPos = startPanVect - newMousePos;

			m_mapView.setCenter(m_mapView.getCenter() + deltaPos);
			m_renderLayer.setView(m_mapView);
			startPanVect = m_renderLayer.mapPixelToCoords(sf::Vector2i(events.mouseMove.x, events.mouseMove.y));
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

void TextureSelector::zoomingEvent(sf::Event& events)
{
	if (events.type == sf::Event::MouseWheelScrolled)
	{
		const sf::Vector2f oldMousePos{m_renderLayer.mapPixelToCoords(sf::Vector2i(events.mouseWheelScroll.x, 
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
		m_renderLayer.setView(m_mapView);

		const sf::Vector2f newMousePos = m_renderLayer.mapPixelToCoords(sf::Vector2i(events.mouseWheelScroll.x, 
		                                                                             events.mouseWheelScroll.y));
		const sf::Vector2f deltaPos = oldMousePos- newMousePos;

		m_mapView.setCenter(m_mapView.getCenter() + deltaPos);
		m_renderLayer.setView(m_mapView);
	}
}

void TextureSelector::selectionEvent(sf::Event& events)
{
	static bool moveState{false};
	static sf::Vector2f selectionBegin;
	static sf::Vector2f selectionEnd;

	if (events.type == sf::Event::MouseButtonPressed 
		&& 
		events.mouseButton.button == sf::Mouse::Left)
	{
		if(!m_panningState)
		{
			selectionBegin = m_renderLayer.mapPixelToCoords(sf::Vector2i(events.mouseButton.x, events.mouseButton.y));
			m_selectionState = true;
		}
	}
	else if (events.type == sf::Event::MouseMoved && m_selectionState)
	{
		// Change moveState
		moveState = true;

		// Set selection size and pos
		selectionEnd = m_renderLayer.mapPixelToCoords(sf::Vector2i(events.mouseMove.x, events.mouseMove.y));;
		sf::Vector2f minPos  = sf::Vector2f(std::min(selectionBegin.x, selectionEnd.x), 
											std::min(selectionBegin.y, selectionEnd.y));
		sf::Vector2f maxPos  = sf::Vector2f(std::max(selectionBegin.x, selectionEnd.x), 
											std::max(selectionBegin.y, selectionEnd.y));
		m_cursorSelection.setPosition(minPos);
		m_cursorSelection.setSize(maxPos - minPos);
	}
	else if (events.type == sf::Event::MouseButtonReleased 
		     && 
		     events.mouseButton.button == sf::Mouse::Left
		     &&
		     m_selectionState)
	{
		// Clear all selected tiles for new tiles
		m_selectedTexTiles.clear();

		// Check move state for single selection or multiple selection
		if(moveState)
		{
			sf::FloatRect cursorSelectionRect{m_cursorSelection.getPosition(), 
			                                  m_cursorSelection.getSize()};
			for(std::vector<sp::Tile>& tilerow : m_texmap.getCurrentLayer()->getTileRects())
			{
				for(sp::Tile& tile : tilerow)
				{	
					if (tile.getGlobalBounds().intersects(cursorSelectionRect))
					{
						m_selectedTexTiles.push_back(&tile);
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
			for(std::vector<sp::Tile>& tilerow : m_texmap.getCurrentLayer()->getTileRects())
			{
				for(sp::Tile& tile : tilerow)
				{	
					if (tile.getGlobalBounds().contains(selectionBegin) and !tile.getSelected())
					{
						tile.setSelected(true);
						m_selectedTexTiles.push_back(&tile);
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

		if(!m_selectedTexTiles.empty())
		{
			sp::Tile* &first = m_selectedTexTiles.front();
			sp::Tile* &last  = m_selectedTexTiles.back();
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

void TextureSelector::handleEvents(sf::Event& events)
{
	// Panning The Map Surface
	panningEvent(events);
	// Zooming The Map Surface
	zoomingEvent(events);
	// Cursor Selection 
	selectionEvent(events);
}

// Update
void TextureSelector::update(const float& dt)
{
	// Update GUI, etc...
	
}

// Render
void TextureSelector::render()
{
	// Clear render layer
	m_renderLayer.clear();

	// Render map
	m_texmap.render(&m_renderLayer);

	// Render selections
	m_renderLayer.draw(m_mapSelection);
	m_renderLayer.draw(m_cursorSelection);

	// Finish drawing into render layer
	m_renderLayer.display();
	//----------------------------
	// Draw render layer to screen
	sf::Sprite renderSprite(m_renderLayer.getTexture());
	m_window->draw(renderSprite);
}
