#include "Map.hpp"

void sp::Map::initOutlines()
{
	// Initialize map's outline
	m_outlines[0].position = sf::Vector2f(0, 0);
	m_outlines[0].color = m_outlineColor;

	m_outlines[1].position = sf::Vector2f(m_size.x, 0);
	m_outlines[1].color = m_outlineColor;

	m_outlines[2].position = sf::Vector2f(m_size.x, m_size.y);
	m_outlines[2].color = m_outlineColor;

	m_outlines[3].position = sf::Vector2f(0, m_size.y);
	m_outlines[3].color = m_outlineColor;

	m_outlines[4].position = sf::Vector2f(0, 0);
	m_outlines[4].color = m_outlineColor;
}

void sp::Map::initGridlines()
{
	// Initialize map's gridlines
	for(float y = m_gridSize.y; y < m_size.y; y += m_gridSize.y) // Ignore first and last gridlines for map's outline
	{	
		// One line has two point
		m_gridlines.append(sf::Vertex(sf::Vector2f(0, y),        m_gridColor));
		m_gridlines.append(sf::Vertex(sf::Vector2f(m_size.x, y), m_gridColor));
	}

	for(float x = m_gridSize.x; x < m_size.x; x += m_gridSize.x) // Ignore first and last gridlines for map's outline
	{
		// One line has two point
		m_gridlines.append(sf::Vertex(sf::Vector2f(x, 0),        m_gridColor));
		m_gridlines.append(sf::Vertex(sf::Vector2f(x, m_size.y), m_gridColor));
	}

	m_gridlines.setPrimitiveType(sf::Lines);
}


sp::Map::Map(const sf::Vector2f& map_grid, 
             const sf::Vector2f& grid_size, 
             const sf::Color& grid_color, 
             const sf::Color& outline_color)
	: m_size(map_grid.x * grid_size.x,
			 map_grid.y * grid_size.y)
	, m_mapGrid(map_grid)
	, m_gridSize(grid_size)
	, m_gridColor(grid_color)
	, m_outlineColor(outline_color)
	, m_outlines(sf::LineStrip, 5)
	, m_currLayer(nullptr)
	, m_preview(false)
{
	initOutlines();
	initGridlines();
}

sp::Map::~Map()
{
	for(sp::MapLayer* &layerptr : m_layers)
	{
		delete layerptr;
	}
}

// Getter/Setter methods
sp::MapLayer* sp::Map::getCurrentLayer() const
{

	return m_currLayer;
}

sp::MapLayer* sp::Map::getLayer(std::size_t index) const
{

	return m_layers.at(index);
}

const bool& sp::Map::getPreview() const
{
	return m_preview;
}

void sp::Map::setPreview(const bool& preview)
{
	if( m_preview != preview )
		m_preview  = preview;
}

void sp::Map::newLayer()
{
	sp::MapLayer* tmp_backLayer = nullptr;
	if(!m_layers.empty())
		tmp_backLayer = m_layers.back();
	m_layers.push_back(new sp::MapLayer(m_mapGrid, m_gridSize, m_layers.size(), tmp_backLayer, nullptr));
	if(tmp_backLayer)
		tmp_backLayer->setSubLayer(m_layers.back());
}
// TODO: delLayer
void sp::Map::setCurrentLayer(std::size_t index)
{

	m_currLayer = m_layers.at(index);
}


void sp::Map::nextLayer()
{

	if(m_currLayer && m_currLayer->getSubLayer() != nullptr)
	{
		sp::MapLayer* baseTmp = m_currLayer->getBaseLayer();
		sp::MapLayer* subTmp  = m_currLayer->getSubLayer();

		// Swap queue indexes
		const int& queueTmp = m_currLayer->getQueueIndex();
		m_currLayer->setQueueIndex(subTmp->getQueueIndex());
		subTmp->setQueueIndex(queueTmp);

		// Swap base/sub layers
		m_currLayer->setBaseLayer(subTmp);
		m_currLayer->setSubLayer(subTmp->getSubLayer());
		subTmp->setBaseLayer(baseTmp);
		subTmp->setSubLayer(m_currLayer);

		// Apply changes by sorting layers
		sortLayersByQueue();
	}
}

void sp::Map::prevLayer()
{
	if(m_currLayer && m_currLayer->getBaseLayer() != nullptr)
	{
		sp::MapLayer* baseTmp = m_currLayer->getBaseLayer();
		sp::MapLayer* subTmp  = m_currLayer->getSubLayer();

		// Swap queue indexes
		const int& queueTmp = m_currLayer->getQueueIndex();
		m_currLayer->setQueueIndex(baseTmp->getQueueIndex());
		baseTmp->setQueueIndex(queueTmp);

		// Swap base/sub layers
		m_currLayer->setBaseLayer(baseTmp->getBaseLayer());
		m_currLayer->setSubLayer(baseTmp);
		baseTmp->setBaseLayer(m_currLayer);
		baseTmp->setSubLayer(subTmp);

		// Apply changes by sorting layers
		sortLayersByQueue();

	}
}

// Functions
void sp::Map::sortLayersByQueue()
{
	std::sort(m_layers.begin(), m_layers.end(),
		[] (sp::MapLayer* a, sp::MapLayer* b)
		{
			return (a->getQueueIndex()) < (b->getQueueIndex());
		});
}

void sp::Map::updateLayersQueue()
{
	for (std::size_t index = 0; index < m_layers.size(); ++index)
	{
		m_layers[index]->setQueueIndex(index);
	}
}

void sp::Map::render(sf::RenderTarget* surface)
{
	for(sp::MapLayer* &layer : m_layers) 
	{
		layer->render(surface);
	}
	
	if( !m_preview)
	{
		surface->draw(m_gridlines);
	}

	surface->draw(m_outlines);
}
