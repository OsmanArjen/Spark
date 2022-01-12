#include "Tile.hpp"

// Our namespace
using namespace sp;

// Constructor
Tile::Tile(const sf::Vector2f& pos, 
           const sf::Vector2f& size,
           const sf::Vector2i& gridpos, 
           const bool& selected)
	: m_shape(size)
	, m_texcoords(0,0)
	, m_gridpos(gridpos)
	, m_selected(selected)
{
	m_shape.setPosition(pos);
}

// Getter methods
bool Tile::isClear()
{
	return (m_shape.getTexture() == nullptr);
}

const bool& Tile::getSelected()
{
	return m_selected;
}

const sf::Vector2f&  Tile::getPosition()
{
	return m_shape.getPosition();
}

const sf::Vector2f&  Tile::getSize()
{
	return m_shape.getSize();
}

const sf::Vector2i&  Tile::getGridPosition()
{
	return m_gridpos;
}

const sf::Texture*   Tile::getTexture()
{
	return m_shape.getTexture();
}

sf::FloatRect Tile::getGlobalBounds() 
{
	return m_shape.getGlobalBounds();
}

sf::FloatRect Tile::getLocalBounds() 
{
	return m_shape.getLocalBounds();
}


// Setter methods
void Tile::setSelected(const bool& selected)
{
	m_selected = selected;
}

void Tile::setPosition(const float& x, const float& y)
{
	m_shape.setPosition(x, y);
}

void Tile::setPosition(const sf::Vector2f& position)
{
	setPosition(position.x, position.y);
}

void Tile::setGridPosition(const int& x, const int& y)
{
	m_gridpos.x = x;
	m_gridpos.y = y;
}

void Tile::setGridPosition(const sf::Vector2i& position)
{
	setGridPosition(position.x, position.y);
}

void Tile::setSize(const sf::Vector2f& size)
{
	m_shape.setSize(size);
	m_shape.setTextureRect({m_texcoords, static_cast<sf::Vector2i>(size)});
}

void Tile::setTextureCoords(const sf::Vector2i& texcoords)
{
	m_shape.setTextureRect({texcoords, static_cast<sf::Vector2i>(m_shape.getSize())});
	m_texcoords.x = texcoords.x;
	m_texcoords.y = texcoords.y;
}

void Tile::setTexture(sf::Texture& texture, const sf::Vector2i& texcoords)
{
	m_shape.setTexture(&texture);
	setTextureCoords(texcoords);
}

void Tile::clear()
{
	m_shape.setTexture(nullptr);
	m_texcoords.x = 0;
	m_texcoords.y = 0;
}

void Tile::render(sf::RenderTarget* surface)
{
	if(m_shape.getTexture())
		surface->draw(m_shape);
}
