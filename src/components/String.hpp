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

namespace ecs
{

    class String : public Component
    {
    public:
        /**
         * @brief Construct a new String object
         * @param str Text to display
         */
        String(const std::string &str, const std::string &fontFile = "", float fontSize = 10.0f, const std::string path = "")
            : Component(Type::TEXT), _value(str), _fontFile(fontFile), _fontSize(fontSize), _path(path)
        {
            _isInitialized = true;
        }

        /// @brief Returns component text reference
        std::string &getValue() { return _value; }
        /// @brief Returns component font size reference
        float &getFontSize() { return _fontSize; }
        /// @brief Returns component font file reference
        std::string &getFontFile() { return _fontFile; }
        /// @brief Returns component path reference
        std::string &getPath() { return _path; }

    private:
        std::string _value;
        std::string _fontFile;
        float _fontSize;
        std::string _path;
    };

}

#endif /* !SPRITEHPP */