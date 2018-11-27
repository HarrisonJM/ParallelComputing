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
    : _fullKey()
      , _keyLength(0)
{
    //! @todo foreach
    for (int i = 0; i < 17; ++i)
    {
        _fullKey[i] = 0x00;
    }
}
/*!
 * @brief increments an array and not a list of objects
 */
void key::incrementStringNorm()
{
    int counter = 0;

    while (counter < 17)
    {
        uint8_t* stringRef = _fullKey + counter;

        // If the current indexed part of the key is less than 255
        if (*(stringRef) + 1 < 256)
        {
            // increment this segment by 1
            *(_fullKey + counter) += 1;
            // Exit loop
            counter = 17;
            _keyLength++;
        }

        if (*(stringRef) + 1 == 256)
        {
            *(_fullKey + counter) = 0;
        }

        ++counter;
    }
}
/*!
 * @brief Puts the currently stored string into the supplied key
 * @param keyGet
 * @return
 */
void key::getStringNorm(uint8_t** keyGet)
{
    memcpy(*keyGet,
           _fullKey,
           16);
}
} /* NAMESPACE etc::key */