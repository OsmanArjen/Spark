#include "../include/SparkGUI.hpp"
#include <iostream>
// Button widget

void sp::Button::updateText()
{
	// Adding local bounds position because local bounds have offset
	// The local bounds aren't set to position 0,0 as with a rectangle or sprite
    m_text.setOrigin(m_text.getGlobalBounds().width  / 2.f + m_text.getLocalBounds().left,
    				 m_text.getGlobalBounds().height / 2.f + m_text.getLocalBounds().top);
    m_text.setPosition(m_shape.getPosition() + (m_shape.getSize() / 2.f));	
}

// Constructor/Destructor
sp::Button::Button(const sf::Vector2f& pos, const sf::Vector2f& size,
				   const std::string& text, sf::Font& font,
				   unsigned int charSize,
				   const sp::Style& idle_style,
				   const sp::Style& hover_style,
				   const sp::Style& active_style)
	: m_text(text, font, charSize)
	, m_charSize(charSize)
	, m_Bstate(ButtonState::IDLE)


{
	// Initialize styles
	setIdleStyle(idle_style);
	setHoverStyle(hover_style);
	setActiveStyle(active_style);

	// Initialize shape and text
	m_shape.setPosition(pos);
	m_shape.setSize(size);
	m_shape.setFillColor(m_idleStyle.backgroundColor);
	m_shape.setOutlineThickness(m_idleStyle.outlineThickness);
	m_shape.setOutlineColor(m_idleStyle.outlineColor);
	m_text.setFillColor(m_idleStyle.foregroundColor);			
	updateText();						
}

sp::Button::~Button()
{
	
}

// Accessors
bool sp::Button::isPressed() const
{
	return (m_Bstate == ButtonState::ACTIVE);
}

const sf::Vector2f& sp::Button::getPosition() const
{
	return m_shape.getPosition();
}

const sf::Vector2f& sp::Button::getSize() const
{
	return m_shape.getSize();
}

const std::string sp::Button::getText() const
{
	return m_text.getString();
}

const sf::Font* sp::Button::getFont() const
{
	return m_text.getFont();
}

unsigned int sp::Button::getCharSize() const
{
	return m_text.getCharacterSize();
}

const sp::Style& sp::Button::getIdleStyle() const
{
	return m_idleStyle;
}

const sp::Style& sp::Button::getHoverStyle() const
{
	return m_hoverStyle;
}

const sp::Style& sp::Button::getActiveStyle() const
{
	return m_activeStyle;
}

// Modifiers
void sp::Button::setPosition(const sf::Vector2f& pos)
{
	m_shape.setPosition(pos);
	updateText();
}

void sp::Button::setSize(const sf::Vector2f& size)
{
	m_shape.setSize(size);
	updateText();
}

void sp::Button::setText(const std::string& text)
{
	m_text.setString(text);
	updateText();
}

void sp::Button::setFont(const sf::Font& font)
{
	m_text.setFont(font);
	updateText();
}

void sp::Button::setCharSize(unsigned int charSize)
{
	m_text.setCharacterSize(charSize);
	updateText();
}

void sp::Button::setIdleStyle(const sp::Style& style)
{
	m_idleStyle = style;
}

void sp::Button::setHoverStyle(const sp::Style& style)
{
	m_hoverStyle = style;
}

void sp::Button::setActiveStyle(const sp::Style& style)
{
	m_activeStyle = style;
}

void sp::Button::setIdleColorStyle(const sf::Color& background, const sf::Color& foreground, const sf::Color& outline)
{
	m_idleStyle.backgroundColor = background;
	m_idleStyle.foregroundColor = foreground;
	m_idleStyle.outlineColor    = outline;
}

void sp::Button::setHoverColorStyle(const sf::Color& background, const sf::Color& foreground, const sf::Color& outline)
{
	m_hoverStyle.backgroundColor = background;
	m_hoverStyle.foregroundColor = foreground;
	m_hoverStyle.outlineColor    = outline;
}

void sp::Button::setActiveColorStyle(const sf::Color& background, const sf::Color& foreground, const sf::Color& outline)
{
	m_activeStyle.backgroundColor = background;
	m_activeStyle.foregroundColor = foreground;
	m_activeStyle.outlineColor    = outline;
}

void sp::Button::setOutlineThickness(const float& idle, const float& hover, const float& active)
{
	m_idleStyle.outlineThickness   = idle;
	m_hoverStyle.outlineThickness  = hover;
	m_activeStyle.outlineThickness = active;
}

// Functions
void sp::Button::update(const sf::Vector2i& mousePosWindow)
{
	// IDLE
	m_Bstate = ButtonState::IDLE;

	if(m_shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow)))
	{
		// HOVER
		m_Bstate = ButtonState::HOVER;

		if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			// ACTIVE
			m_Bstate = ButtonState::ACTIVE;
		}
	}

	switch(m_Bstate)
	{
	case ButtonState::IDLE:
		// Set button to IDLE
		m_shape.setFillColor(m_idleStyle.backgroundColor);
		m_text.setFillColor(m_idleStyle.foregroundColor);
		m_shape.setOutlineColor(m_idleStyle.outlineColor);
		m_shape.setOutlineThickness(m_idleStyle.outlineThickness);
		break;
	case ButtonState::HOVER:
		// Set button to HOVER
		m_shape.setFillColor(m_hoverStyle.backgroundColor);
		m_text.setFillColor(m_hoverStyle.foregroundColor);
		m_shape.setOutlineColor(m_hoverStyle.outlineColor);
		m_shape.setOutlineThickness(m_hoverStyle.outlineThickness);
		break;
	case ButtonState::ACTIVE:
		// Set button to ACTIVE
		m_shape.setFillColor(m_activeStyle.backgroundColor);
		m_text.setFillColor(m_activeStyle.foregroundColor);
		m_shape.setOutlineColor(m_activeStyle.outlineColor);
		m_shape.setOutlineThickness(m_activeStyle.outlineThickness);
		break;
	}

}

void sp::Button::render(sf::RenderTarget* surface)
{
	surface->draw(m_shape);
	surface->draw(m_text);
}
