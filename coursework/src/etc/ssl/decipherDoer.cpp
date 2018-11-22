/*!
 * @brief
 *
 * @addtogroup
 *
 * @date 21/11/18
 */

#include <cstring>
#include <iostream>
#include <functional>
#include "decipherDoer.h"

namespace etc::ssl
{

decipherDoer::decipherDoer(int agentID
                           , uint8_t* iv
                           , uint8_t* encText
                           , int encLength
                           , uint8_t* plainText
                           , int plainTextLength
                           , etc::threadHandler::ThreadHandler &th
                           , std::function<uint8_t()>* solutionGetter)
    : _agentID(agentID)
      , _iv(new uint8_t[AES_BLOCK_SIZE])
      , _encryptedText(new uint8_t[encLength])
      , _encryptedTextLength(encLength)
      , _plainTextcmp(new uint8_t[16384])
      , _th(th)
      , _solutions(solutionGetter)
{
    // copy IV to agent
    for (int i = 0; i < AES_BLOCK_SIZE; ++i)
    {
        _iv[i] = iv[i];
    }

    // copy encText to agent
    for (int i = 0; i < encLength; ++i)
    {
        _encryptedText[i] = encText[i];
    }

    // copy encText to agent
    for (int i = 0; i < plainTextLength; ++i)
    {
        _plainTextcmp[i] = plainText[i];
    }
}
/*!
 * @brief performs the deciphering
 */
void decipherDoer::startDecrypting()
{
    auto plaintextFinal = new uint8_t[16384];
    int success = 0;
    do
    {
//        omp_set_lock(_lock);
        // If done, solution was found
        if (_th.getDone())
        {
//            omp_unset_lock(_lock);
            return;
        }
//        omp_unset_lock(_lock);

//        auto plaintextLengthSerial = 0;
//        success = etc::ssl::decipher::CipherDoer::DecipherText(_solutions->front(),
//                                                               _iv,
//                                                               &plaintextFinal,
//                                                               &_encryptedText,
//                                                               &plaintextLengthSerial,
//                                                               &_encryptedTextLength);

//        std::cout << "testing key: " << std::hex << _solutions->front() << std::endl;
        if (success)
        {
            if (std::strcmp((char*) plaintextFinal,
                            (char*) _plainTextcmp) != 0)
            {
                break;
            }
        }

//        _solutions->pop();
    } while (!success);

    std::cout << "Result: " << plaintextFinal << std::endl;
//    std::cout << "Key: " << (_solutions->front()) << std::endl;

    // We have been successful, alert the other threads
//    omp_set_lock(_lock);
    _th.setDone(true);
//    omp_unset_lock(_lock);
}
} /* CHANGEME */