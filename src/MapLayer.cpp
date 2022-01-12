// FIX: layer sisteminde düzenlemeler veya hatalar olacak onları düzelt!
#include "MapLayer.hpp"

// Initializer Function for tileRects
void sp::MapLayer::initLayerTileRects()
{
	for (int y = 0; y < m_mapGrid.y; y++)
	{
		m_tileRects.push_back(std::vector<Tile>());
		for (int x = 0; x < m_mapGrid.x; x++)
		{
			std::vector<Tile>& TileRow{m_tileRects.back()};
			TileRow.push_back({ {x * m_gridSize.x, y * m_gridSize.y}, m_gridSize, {x,y} });
		}
	}
}

// Constructor
sp::MapLayer::MapLayer(const sf::Vector2f& map_grid, 
					   const sf::Vector2f& grid_size,
					   int queue_indx,
					   MapLayer* base_layer,
					   MapLayer* sub_layer)
	: m_mapGrid(map_grid)
	, m_gridSize(grid_size)
	, m_visible(true)
	, m_baseLayer(base_layer)
	, m_queueIndex(queue_indx)
	, m_subLayer(sub_layer)
{
	initLayerTileRects();
}

// Functions
const int& sp::MapLayer::getQueueIndex() const
{
	return m_queueIndex;
}

const bool& sp::MapLayer::getVisible() const
{
	return m_visible;
}

sp::MapLayer::TileRects_t& sp::MapLayer::getTileRects()
{
	return m_tileRects;
}

sp::MapLayer* sp::MapLayer::getBaseLayer()
{
	return m_baseLayer;
}

sp::MapLayer* sp::MapLayer::getSubLayer()
{
	return m_subLayer;
}

void sp::MapLayer::setQueueIndex( int index)
{
	m_queueIndex = index;
}

void sp::MapLayer::setVisible(bool visible)
{
	m_visible = visible;
}

void sp::MapLayer::setBaseLayer(sp::MapLayer* base_layer)
{
	m_baseLayer = base_layer;
}

void sp::MapLayer::setSubLayer(sp::MapLayer* sub_layer)
{
	m_subLayer = sub_layer;

}

void sp::MapLayer::render(sf::RenderTarget* surface)
{
	if(m_visible)
	{	
		for(std::vector<Tile>& tilerow : m_tileRects)
		{	
			for(Tile& tile : tilerow)
			{
				tile.render(surface);
			}
		}
	}
}
