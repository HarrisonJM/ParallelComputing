/*!
 * @brief Fetches all the key segments and forms a single key
 *
 * @date 03/11/18
 */

#include "key.h"

namespace etc::key
{

/*!
 * @brief Constructor
 */
key::key()
    : _firstSeg(0)
{
}
/*!
 * @brief Increments the current string
 */
void key::incrementString()
{
    _firstSeg.incrementC();
}
/*!
 * @brief Gets the entire key from all stored segments
 * @return A std::string containing the key
 */
const std::string key::getString()
{
    std::string keyStr = "";

    for (keySegment *ks = &_firstSeg;
         ks!=nullptr;
         ks = ks->getNextSegment())
    {
        keyStr += ks->getCharacter();
    }

    return keyStr;
}
} /* NAMESPACE etc::key */