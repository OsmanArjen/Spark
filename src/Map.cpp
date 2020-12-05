#include "../include/Map.hpp"

void Map::initOutlines()
{
	// Initialize map's outline
	this->outlines[0].position = sf::Vector2f(this->rect.left, this->rect.top);
	this->outlines[0].color = this->outlineColor;

	this->outlines[1].position = sf::Vector2f(this->rect.left + this->rect.width, this->rect.top);
	this->outlines[1].color = this->outlineColor;

	this->outlines[2].position = sf::Vector2f(this->rect.left + this->rect.width, this->rect.top + this->rect.height);
	this->outlines[2].color = this->outlineColor;

	this->outlines[3].position = sf::Vector2f(this->rect.left, this->rect.top + this->rect.height);
	this->outlines[3].color = this->outlineColor;

	this->outlines[4].position = sf::Vector2f(this->rect.left, this->rect.top);
	this->outlines[4].color = this->outlineColor;
}

void Map::initGridlines()
{
	// Initialize map's gridlines
	const sf::Vector2f& mapPos = this->outlines[0].position;
	for(float y = mapPos.y + this->gridSize.y; y < (mapPos.y + this->rect.height); y += this->gridSize.y) // Ignore first and last gridlines for map's outline
	{	
		// One line has two point
		this->gridlines.append(sf::Vertex(sf::Vector2f(mapPos.x, y), this->gridColor));
		this->gridlines.append(sf::Vertex(sf::Vector2f(mapPos.x + this->rect.width, y), this->gridColor));
	}

	for(float x = mapPos.x + this->gridSize.x; x < (mapPos.x + this->rect.width); x += this->gridSize.x) // Ignore first and last gridlines for map's outline
	{
		// One line has two point
		this->gridlines.append(sf::Vertex(sf::Vector2f(x, mapPos.y), this->gridColor));
		this->gridlines.append(sf::Vertex(sf::Vector2f(x, mapPos.y + this->rect.height), this->gridColor));
	}	
	this->gridlines.setPrimitiveType(sf::Lines);
}



Map::Map(const sf::FloatRect& map_rect, 
		const sf::Vector2f& grid_size, 
		const sf::Color& grid_color, 
		const sf::Color& outline_color)
	: rect(map_rect)
	, gridSize(grid_size)
	, gridColor(grid_color)
	, outlineColor(outline_color)
	, currLayer(nullptr)
	, outlines(sf::LineStrip, 5)
{
	this->initOutlines();
	this->initGridlines();
}

Map::~Map()
{
	for(sp::MapLayer* &layerptr : this->layers)
	{
		delete layerptr;
	}
}

// Getter/Setter methods
sp::MapLayer* Map::getCurrentLayer()
{

	return this->currLayer;
}

sp::MapLayer* Map::getLayer(std::size_t index)
{

	return this->layers.at(index);
}

void Map::newLayer()
{
	sp::MapLayer* tmp_backLayer = nullptr;
	if(!this->layers.empty())
		tmp_backLayer = this->layers.back();
	this->layers.push_back(new sp::MapLayer(this->rect, this->gridSize, this->layers.size(), tmp_backLayer, nullptr));
	if(tmp_backLayer)
		tmp_backLayer->setSubLayer(this->layers.back());
}
// TODO: delLayer
void Map::setCurrentLayer(std::size_t index)
{

	this->currLayer = this->layers.at(index);
}


void Map::nextLayer()
{

	if(this->currLayer && this->currLayer->getSubLayer() != nullptr)
	{
		sp::MapLayer* baseTmp = this->currLayer->getBaseLayer();
		sp::MapLayer* subTmp  = this->currLayer->getSubLayer();

		// Swap queue indexes
		const int& queueTmp = this->currLayer->getQueueIndex();
		this->currLayer->setQueueIndex(subTmp->getQueueIndex());
		subTmp->setQueueIndex(queueTmp);

		// Swap base/sub layers
		this->currLayer->setBaseLayer(subTmp);
		this->currLayer->setSubLayer(subTmp->getSubLayer());
		subTmp->setBaseLayer(baseTmp);
		subTmp->setSubLayer(this->currLayer);

		// Apply changes by sorting layers
		this->sortLayersByQueue();
	}
}

void Map::prevLayer()
{
	if(this->currLayer && this->currLayer->getBaseLayer() != nullptr)
	{
		sp::MapLayer* baseTmp = this->currLayer->getBaseLayer();
		sp::MapLayer* subTmp  = this->currLayer->getSubLayer();

		// Swap queue indexes
		const int& queueTmp = this->currLayer->getQueueIndex();
		this->currLayer->setQueueIndex(baseTmp->getQueueIndex());
		baseTmp->setQueueIndex(queueTmp);

		// Swap base/sub layers
		this->currLayer->setBaseLayer(baseTmp->getBaseLayer());
		this->currLayer->setSubLayer(baseTmp);
		baseTmp->setBaseLayer(this->currLayer);
		baseTmp->setSubLayer(subTmp);

		// Apply changes by sorting layers
		this->sortLayersByQueue();

	}
}

// Functions
void Map::sortLayersByQueue()
{
	std::sort(this->layers.begin(), this->layers.end(),
		[] (sp::MapLayer* a, sp::MapLayer* b)
		{
			return (a->getQueueIndex()) < (b->getQueueIndex());
		});
}

void Map::updateLayersQueue()
{
	for (std::size_t index = 0; index < this->layers.size(); ++index)
	{
		this->layers[index]->setQueueIndex(index);
	}
}

void Map::update()
{
	if(this->currLayer)	
		this->currLayer->update(0.0f);// @param: mousePos
	
}

void Map::render(sf::RenderTarget& surface)
{
	for(sp::MapLayer* &layer : this->layers) 
	{
		layer->render(surface);
	}
	surface.draw(this->gridlines);
	surface.draw(this->outlines);
}
