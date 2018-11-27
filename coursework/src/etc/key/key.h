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
#include <queue>

namespace etc
{
namespace key
{
class key
{
public:
    key();
    ~key() = default;
    void incrementStringNorm();
    void getStringNorm(uint8_t** keyGet);

private:
    uint8_t _fullKey[17]; // 128 bit key (16 bytes, 16 chars)
    int _keyLength;
};
}
}

#endif /*PROTOCOLDEVELOPER_KEY_H*/
