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
    void StartSerial2();
    void StartOpenMP();
private:

    uint8_t* _plaintextInitial;
    int _plaintextInitialLength;
    uint8_t* _encryptedText;
    int _encLength;
    uint8_t* _iv;
    uint8_t _definedKey[17];

};
}

#endif /*PROTOCOLDEVELOPER_COURSEWORKHANDLER_H*/
