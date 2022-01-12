#ifndef KEYBIND_HANDLER_HPP
#define KEYBIND_HANDLER_HPP
#include <map>
#include <vector>
#include <utility>
#include <SFML/Window.hpp>
namespace sp
{
class KeybindHandler
{
private:
	// Static members
	static std::map<std::string,       sf::Keyboard::Key>  m_keyStringMap;
	static std::map<sf::Keyboard::Key, std::string>        m_keyCodeMap;

	// Keybind map
	std::map<std::string, sf::Keyboard::Key> m_keybinds;

public:
	void bind(const std::string& bind, 
	          const std::string& key);
	bool getPressed(const std::string& bind) const; 

	// Static functions 
	static const sf::Keyboard::Key& getKeyCode(const std::string& key) const;
	static const std::string&       getStringKey(const sf::Keyboard::Key& key) const;


};
} // sp
#endif // KEYBIND_HANDLER_HPP
