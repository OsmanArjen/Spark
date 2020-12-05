#ifndef SPARK_GUI_HPP
#define SPARK_GUI_HPP

// SFML CORE
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace sp
{

// Style struct
struct Style
{
	sf::Color backgroundColor;
	sf::Color foregroundColor;
	sf::Color outlineColor;
	float outlineThickness;
};

// Enum class for buttons current state
enum class ButtonState {IDLE, HOVER, ACTIVE};

class Button
{
private:
	// Private Variables
	sf::RectangleShape shape;
	sf::Text text;
	ButtonState Bstate;

	// Styles
	sp::Style idleStyle;
	sp::Style hoverStyle;
	sp::Style activeStyle;
public:
	// Constructor/Destructor
	Button(const sf::Vector2f& pos, const sf::Vector2f& size,
		   const std::string& text, sf::Font& font,
		   unsigned int charSize,
		   const sp::Style& idle_style,
		   const sp::Style& hover_style,
		   const sp::Style& active_style);
	~Button();

	// Accessors
	bool isPressed() const;
	const sf::Vector2f& getPosition() const;
	const sf::Vector2f& getSize() const;
	const std::string getText() const;
	const sf::Font*	getFont() const;
	unsigned int getCharSize() const;
	const sp::Style& getIdleStyle() const;
	const sp::Style& getHoverStyle() const;
	const sp::Style& getActiveStyle() const;
	

	// Modifiers
	void setPosition(const sf::Vector2f& pos);
	void setSize(const sf::Vector2f& size);
	void setText(const std::string& text);
	void setFont(const sf::Font& font);
	void setCharSize(unsigned int charSize = 30);
	void setIdleStyle(const sp::Style& style);
	void setHoverStyle(const sp::Style& style);
	void setActiveStyle(const sp::Style& style);
	void setIdleColorStyle(const sf::Color& background, const sf::Color& foreground, const sf::Color& outline);
	void setHoverColorStyle(const sf::Color& background, const sf::Color& foreground, const sf::Color& outline);
	void setActiveColorStyle(const sf::Color& background, const sf::Color& foreground, const sf::Color& outline);
	void setOutlineThickness(const float& idle, const float& hover, const float& active);

	// Functions
	void update(const sf::Vector2i& mousePosWindow);
	void render(sf::RenderTarget& surface);
};	



} // sp

#endif // SPARK_GUI_HPP
