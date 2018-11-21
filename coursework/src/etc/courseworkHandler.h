/*!
 * @brief
 *
 * @addtogroup
 *
 * @date 07/11/18
 */

#ifndef PROTOCOLDEVELOPER_COURSEWORKHANDLER_H
#define PROTOCOLDEVELOPER_COURSEWORKHANDLER_H

#include <cstdint>

#define AES_128_KEY_SIZE 128
#define AES_BLOCK_SIZE 16

#ifndef KILOBYTE16
#define KILOBYTE16 16384
#endif

namespace etc
{

class CourseworkHandler
{
public:
    CourseworkHandler();
    ~CourseworkHandler() = default;
    void StartSerial();
private:
    void _initThings(uint8_t* iv
                     , uint8_t** plaintextFinal
                     , uint8_t** encryptedText
                     , int* encLength
                     , int* plaintextLength
                     , const uint8_t* key
                     , uint8_t* plaintext);

    uint8_t* _plainTextInitial;
    uint8_t* _iv;
    uint8_t* _definedKey;
};
}

#endif /*PROTOCOLDEVELOPER_COURSEWORKHANDLER_H*/
