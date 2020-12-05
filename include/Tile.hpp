#ifndef TILE_HPP
#define TILE_HPP
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

namespace sp
{
class Tile 
{
private:
	sf::RectangleShape shape;
	sf::RectangleShape selectShape;
	bool selected;

public:
	// Constructors
	Tile(const sf::Vector2f& pos, const sf::Vector2f& size);
	Tile(const sf::Vector2f& pos, 
		 const sf::Vector2f& size,
		 const bool& p_selected,
		 const sf::Color& selectColor, 
		 const sf::Color& selectOutline = sf::Color::Transparent,
		 const float& outlineThickness  = 0.0f);
	
	// Getter methods
	const bool& getSelected();
	const sf::Vector2f&  getPosition();
	const sf::Vector2f&  getSize();
	const sf::Texture*   getTexture();
	sf::FloatRect getGlobalBounds();
	// Setter methods
	void setSelected(const bool& p_selected);
	void setPosition(const float& x, const float& y);
	void setPosition(const sf::Vector2f& position);
	void setSize(const sf::Vector2f& size);
	void setTexture(sf::Texture& texture);
	void setSelectStyle(const sf::Color& fillcolor, 
						const sf::Color& outline = sf::Color::Transparent,
						const float& outlineThickness = 0.0f);
	void clear();
	// Render
	void render(sf::RenderTarget& surface);
};
} // sp

#endif // TILE_HPP
