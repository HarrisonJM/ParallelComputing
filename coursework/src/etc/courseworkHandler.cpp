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

void CourseworkHandler::Start()
{
    // The key we're encrypting with
    uint8_t *definedKey = new uint8_t[AES_128_KEY_SIZE];
    definedKey[0] = '1';
    definedKey[2] = '1';
    definedKey[3] = '1';
    // The initial plaintext to encrypt
    const uint8_t* plainTextInitial = (uint8_t*) "THIS IS SOME PLAINTEXT THAT I AM ENCRYPTING";
    int encLength;
    /*int plaintextLengthinit = sizeof(plainTextInitial);*/
    int plaintextLengthinit = std::strlen((char*)plainTextInitial);

    uint8_t* iv, // initilization vector
        * plaintextFinal, // Where to store the final decrypted file
        * encryptedText; // The encrypted text

    _initThings(&iv,
                &plaintextFinal,
                &encryptedText,
                &encLength,
                &plaintextLengthinit,
                definedKey,
                plainTextInitial);

    etc::key::key key;
    omp_lock_t threadLock;
    etc::threadHandler::ThreadHandler th(threadLock);
    etc::solutionHandler::SolutionHandler sh(1,
                                             th);

    etc::solutionHandler::SolutionHandler sh2(1,
                                             th);

    etc::ssl::decipher::CipherAgent cipherAgent(0,
                                                th,
                                                sh);

    int plainTextLength = KILOBYTE16;
    cipherAgent.startCrackingSerial(encryptedText,
                                    &plaintextFinal,
                                    encLength,
                                    &plainTextLength);

    std::cout << "Final: " << plaintextFinal << std::endl;
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
void CourseworkHandler::_initThings(uint8_t** iv
                                    , uint8_t** plaintextFinal
                                    , uint8_t** encryptedText
                                    , int* encLength
                                    , int* plaintextLength
                                    , const uint8_t* key
                                    , const uint8_t* plaintext)
{
    // Setup and init iv
    *iv = new uint8_t[AES_BLOCK_SIZE];
    RAND_bytes(*iv,
               sizeof(iv));
    *plaintextFinal = new uint8_t[KILOBYTE16];
    *encryptedText = new uint8_t[KILOBYTE16];

    omp_lock_t threadLock;
    etc::threadHandler::ThreadHandler th(threadLock);

    const etc::ssl::decipher::CipherAgent ca(th);
    ca.EncipherText(key,
                    *iv,
                    plaintext,
                    encryptedText,
                    encLength,
                    plaintextLength);
}
} /* namespace etc */