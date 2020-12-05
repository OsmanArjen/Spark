// FIX: layer sisteminde düzenlemeler veya hatalar olacak onları düzelt!
#include "../include/MapLayer.hpp"

// Initializer Function for tileRects
void sp::MapLayer::initLayerTileRects()
{
	sf::Vector2f mapPos{this->mapRect.left, this->mapRect.top};
	for (float y = mapPos.y; y < (mapPos.y + this->mapRect.height); y += this->gridSize.y)
	{
		for (float x = mapPos.x; x < (mapPos.x + this->mapRect.width); x += this->gridSize.x)
		{
			this->tileRects.push_back(sp::Tile({x, y}, this->gridSize));
		}
	}
}

// Constructor
sp::MapLayer::MapLayer(const sf::FloatRect& map_rect, 
					   const sf::Vector2f& grid_size,
					   int queue_indx,
					   MapLayer* base_layer,
					   MapLayer* sub_layer)
	: mapRect(map_rect)
	, gridSize(grid_size)
	, visible(true)
	, baseLayer(base_layer)
	, queueIndex(queue_indx)
	, subLayer(sub_layer)
{
	this->initLayerTileRects();
}

// Functions
const int& sp::MapLayer::getQueueIndex() const
{
	return this->queueIndex;
}

const bool& sp::MapLayer::getVisible() const
{
	return this->visible;
}

std::vector<sp::Tile>& sp::MapLayer::getTileRects()
{
	return this->tileRects;
}

sp::MapLayer* sp::MapLayer::getBaseLayer()
{
	return this->baseLayer;
}

sp::MapLayer* sp::MapLayer::getSubLayer()
{
	return this->subLayer;
}

void sp::MapLayer::setQueueIndex( int index)
{
	this->queueIndex = index;
}

void sp::MapLayer::setVisible(bool visible)
{
	this->visible = visible;
}

void sp::MapLayer::setTileRects(const std::vector<sp::Tile>& tile_rects)
{
	this->tileRects = tile_rects;
}

void sp::MapLayer::setBaseLayer(sp::MapLayer* base_layer)
{
	this->baseLayer = base_layer;
}

void sp::MapLayer::setSubLayer(sp::MapLayer* sub_layer)
{
	this->subLayer = sub_layer;

}

void sp::MapLayer::update(float mousePos)
{
	if(this->visible)
	{

	}
}

void sp::MapLayer::render(sf::RenderTarget& surface)
{
	if(this->visible)
	{	
		for(sp::Tile& tile : this->tileRects)
		{
			tile.render(surface);
		}
	}
}
