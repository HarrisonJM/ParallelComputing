/*!
 * @brief Runs the coursework
 *
 * @date 07/11/18
 */

#include <cstdint>
#include <openssl/rand.h>
#include <iostream>
#include <cstring>

#include "key/key.h"
#include "ssl/cipherAgent.h"
#include "courseworkHandler.h"

namespace etc
{

CourseworkHandler::CourseworkHandler()
    : _plainTextInitial((uint8_t*) "1234567890123456123456789012345612345678901234561234567890123456")
      , _iv(nullptr)
{
    // Setup and init _iv
    _iv = new uint8_t[AES_BLOCK_SIZE];
//    RAND_bytes(*_iv,
//               sizeof(_iv));

    for (int i = 0; i <= AES_BLOCK_SIZE; ++i)
    {
        (_iv)[i] = 0x01;
    }


    // The key we're encrypting with
    _definedKey = new uint8_t[AES_128_KEY_SIZE/8];
    for (int i = 0; i <= AES_128_KEY_SIZE/8; ++i)
    {
        _definedKey[i] = 0x00;
    }

    _definedKey[0] = 0x01;
    _definedKey[1] = 0x02;
    _definedKey[2] = 0x03;
}

void CourseworkHandler::StartSerial()
{
    // The thread lock we're using
    omp_lock_t threadLock;
    int encLength;
    /*int plaintextLengthinit = sizeof(_plainTextInitial);*/
    auto plaintextLengthinit = (int) std::strlen((char*) _plainTextInitial);
    // The encrypted text
    uint8_t* encryptedText;
    // Where to store the final decrypted file
    uint8_t* plaintextFinal;
    _initThings(_iv,
                &plaintextFinal,
                &encryptedText,
                &encLength,
                &plaintextLengthinit,
                _definedKey,
                _plainTextInitial);

    etc::key::key key;

//    std::cout << "IV:" << _iv << std::endl;
//    std::cout << "PT:" << _plainTextInitial << std::endl;
//    std::cout << "ET:" << encryptedText << std::endl;


    int plaintextLengthSerial;
    etc::ssl::decipher::CipherAgent::DecipherText(_definedKey,
                                                  _iv,
                                                  &plaintextFinal,
                                                  &encryptedText,
                                                  &plaintextLengthSerial,
                                                  &encLength);

    std::cout << "Final: " << plaintextFinal << std::endl;

    etc::threadHandler::ThreadHandler th(threadLock);
    etc::solutionHandler::SolutionHandler sh(1,
                                             th);
    etc::solutionHandler::SolutionHandler sh2(1,
                                              th);
}
/*!
 * @brief initialises storage and encrypts plaintext
 * @param iv Init Vector
 * @param plaintextFinal Where the final plaintext is sotred
 * @param encryptedText Where the encrytped text will be stored
 * @param encLength The length of the encrypted file
 * @param plaintextLength The length of the plaintext
 * @param key The key
 * @param plaintext The plaintext
 */
void CourseworkHandler::_initThings(uint8_t* iv
                                    , uint8_t** plaintextFinal
                                    , uint8_t** encryptedText
                                    , int* encLength
                                    , int* plaintextLength
                                    , const uint8_t* key
                                    , uint8_t* plaintext)
{

    *plaintextFinal = new uint8_t[KILOBYTE16];
    *encryptedText = new uint8_t[KILOBYTE16];

    etc::ssl::decipher::CipherAgent::EncipherText(key,
                                                  iv,
                                                  &plaintext,
                                                  encryptedText,
                                                  encLength,
                                                  plaintextLength);
}
} /* namespace etc */