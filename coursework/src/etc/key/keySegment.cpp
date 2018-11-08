/*!
 * @brief
 *
 * @addtogroup
 *
 * @date 04/11/18
 */



#include "keySegment.h"

namespace etc::key
{
/*!
 * @brief Constructor
 * @param segNum The Segment ID
 */
keySegment::keySegment(int segNum)
    : _segNum(segNum)
      , _c(33)
      , _nextSegment(nullptr)
{
}
keySegment::~keySegment()
{
    delete _nextSegment;
}
/*!
 * @brief increments the stored character, handles lapses, checks if the next
 * segment needs incrementing
 */
void keySegment::incrementC()
{
    _c++;

    if (_c > 126)
    {
        _c = 33;

        if (_nextSegment==nullptr)
            _nextSegment = new keySegment(_segNum + 1);
        else
            _nextSegment->incrementC();
    }
}
/*!
 * @brief returns the stored character
 * @return The character stored
 */
char keySegment::getCharacter()
{
    return _c;
}
/*!
 * @brief Allows access to the next segment
 * @return A pointer to the next key segment
 */
keySegment *keySegment::getNextSegment()
{
    return _nextSegment;
}
} /* NAMEPSACE etc::key */