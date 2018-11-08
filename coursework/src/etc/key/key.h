/*!
 * @brief
 *
 * @addtogroup
 *
 * @date 03/11/18
 */


#ifndef PROTOCOLDEVELOPER_KEY_H
#define PROTOCOLDEVELOPER_KEY_H

#include <string>
#include "keySegment.h"

namespace etc
{
namespace key
{
class key
{
public:
    key();
    ~key() = default;
    void incrementStringObj();
    void incrementStringNorm();
    const std::string getStringObj();
    const uint8_t* getStringNorm();

private:
    keySegment _firstSeg;

    uint8_t _fullKey[17]; // 128 bit key (16 bytes, 16 chars)
};
}
}

#endif /*PROTOCOLDEVELOPER_KEY_H*/
