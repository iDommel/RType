/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** String.hpp
*/

#ifndef SPRITEHPP
#define SPRITEHPP

#include <string>

#include "Component.hpp"

namespace indie
{

    class String : public Component
    {
    public:
        /**
         * @brief Construct a new String object
         * @param str Text to display
         */
        String(const std::string &str, const std::string &fontFile = "", float fontSize = 10.0f)
            : Component(Type::TEXT), _value(str), _fontFile(fontFile), _fontSize(fontSize)
        {
            _isInitialized = true;
        }

        /// @brief Returns component text reference
        std::string &getValue() { return _value; }
        /// @brief Returns component font size reference
        float &getFontSize() { return _fontSize; }
        /// @brief Returns component font file reference
        std::string &getFontFile() { return _fontFile; }

    private:
        std::string _value;
        std::string _fontFile;
        float _fontSize;
    };

}

#endif /* !SPRITEHPP */