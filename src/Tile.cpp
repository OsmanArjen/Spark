#include "../include/Tile.hpp"

// Constructor
sp::Tile::Tile(const sf::Vector2f& pos, const sf::Vector2f& size)
	: shape(size)
	, selectShape(size)
	, selected(false)
{
	this->shape.setPosition(pos);
	this->selectShape.setPosition(pos);
	this->clear();

}

sp::Tile::Tile(const sf::Vector2f& pos, 
		 const sf::Vector2f& size,
		 const bool& p_selected,
		 const sf::Color& selectColor, 
		 const sf::Color& selectOutline,
		 const float& outlineThickness)
	: shape(size)
	, selectShape(size)
	, selected(p_selected)

{
	this->shape.setPosition(pos);
	this->selectShape.setPosition(pos);
	this->clear();
	this->setSelectStyle(selectColor, selectOutline, outlineThickness);
}

// Getter methods
const bool& sp::Tile::getSelected()
{
	return this->selected;
}
const sf::Vector2f&  sp::Tile::getPosition()
{
	return this->shape.getPosition();
}

const sf::Vector2f&  sp::Tile::getSize()
{
	return this->shape.getSize();
}

const sf::Texture*   sp::Tile::getTexture()
{
	return this->shape.getTexture();
}

sf::FloatRect sp::Tile::getGlobalBounds() 
{
	return this->shape.getGlobalBounds();
}

// Setter methods
void sp::Tile::setSelected(const bool& p_selected)
{
	this->selected = p_selected;
}

void sp::Tile::setPosition(const float& x, const float& y)
{
	this->shape.setPosition(x, y);
	this->selectShape.setPosition(x, y);
}

void sp::Tile::setPosition(const sf::Vector2f& position)
{
	this->setPosition(position.x, position.y);
}

void sp::Tile::setSize(const sf::Vector2f& size)
{
	this->shape.setSize(size);
	this->selectShape.setSize(size);
}

void sp::Tile::setTexture(sf::Texture& texture)
{
	this->shape.setTexture(&texture);
}

void sp::Tile::clear()
{
	this->shape.setTexture(nullptr);
}

void sp::Tile::setSelectStyle(const sf::Color& fillcolor, 
							  const sf::Color& outline,
							  const float& outlineThickness)
{
	this->selectShape.setFillColor(fillcolor);
	this->selectShape.setOutlineColor(outline);
	this->selectShape.setOutlineThickness(outlineThickness);
}

void sp::Tile::render(sf::RenderTarget& surface)
{
	if(this->shape.getTexture())
		surface.draw(this->shape);
	if(this->selected)
		surface.draw(this->selectShape);
}
