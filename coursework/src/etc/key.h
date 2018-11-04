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
    void incrementString();
    const std::string getString();

private:
    keySegment _firstSeg;
};
}
}

#endif /*PROTOCOLDEVELOPER_KEY_H*/
