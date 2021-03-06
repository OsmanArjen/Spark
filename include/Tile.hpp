#ifndef TILE_HPP
#define TILE_HPP
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

namespace sp
{
class Tile 
{
private:
	sf::RectangleShape m_shape;
	sf::Vector2i m_texcoords;
	bool m_selected;

public:
	// Constructors
	Tile(const sf::Vector2f& pos, 
         const sf::Vector2f& size, 
         const bool& selected=false);

	// Getter methods
	bool isClear();
	const bool& getSelected();
	const sf::Vector2f&  getPosition();
	const sf::Vector2f&  getSize();
	const sf::Texture*   getTexture();
	sf::FloatRect getGlobalBounds();
	sf::FloatRect getLocalBounds();
	
	// Setter methods
	void setSelected(const bool& selected);
	void setPosition(const float& x, const float& y);
	void setPosition(const sf::Vector2f& position);
	void setSize(const sf::Vector2f& size);
	void setTextureCoords(const sf::Vector2i& texcoords);
	void setTexture(sf::Texture& texture, const sf::Vector2i& texcoords={0,0});
	void clear();
	// Render
	void render(sf::RenderTarget* surface);
};
} // sp

#endif // TILE_HPP
