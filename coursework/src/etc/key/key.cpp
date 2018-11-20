/*!
 * @brief Fetches all the key segments and forms a single key
 *
 * @date 03/11/18
 */

#include <cstring>
#include "key.h"

namespace etc::key
{

/*!
 * @brief Constructor
 */
key::key()
    : _firstSeg(0)
    , _fullKey("")
    , _keyLength(0)
{
}
/*!
 * @brief Increments the current string
 */
void key::incrementStringObj()
{
    _firstSeg.incrementC();
}
/*!
 * @brief increments an array and not a list of objects
 */
void key::incrementStringNorm()
{
    int counter = 0;

    while(counter < 17)
    {
        uint8_t* stringRef = _fullKey+counter;

        // If the current indexed part of the key is less than 255
        if( *(stringRef)+1 < 255 )
        {
            // increment this segment by 1
            *(_fullKey+counter) += 1;
            // Exit loop
            counter = 17;
            _keyLength++;
        }

        if( *(stringRef)+1 == 255 )
        {
            *(_fullKey+counter) = 0;
        }

        ++counter;
    }
}
/*!
 * @brief Gets the entire key from all stored segments
 * @return A std::string containing the key
 */
const std::string key::getStringObj()
{
    std::string keyStr;

    for (keySegment *ks = &_firstSeg;
         ks!=nullptr;
         ks = ks->getNextSegment())
    {
        keyStr += ks->getCharacter();
    }

    return keyStr;
}
const uint8_t *key::getStringNorm()
{
    auto* newKey = new uint8_t[16];
    memcpy(newKey, _fullKey, 16);

    return newKey;
}
} /* NAMESPACE etc::key */