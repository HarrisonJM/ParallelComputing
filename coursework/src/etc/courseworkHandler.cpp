/*!
 * @brief Runs the coursework
 *
 * @date 07/11/18
 */
#include <omp.h>

#include <cstdint>
#include <openssl/rand.h>
#include <iostream>
#include <cstring>
#include <openssl/ssl.h>

#include "key/key.h"
#include "ssl/cipherDoer.h"
#include "courseworkHandler.h"
#include "ssl/decipherDoer.h"

namespace etc {

CourseworkHandler::CourseworkHandler()
    : _plaintextInitial((uint8_t *) "MARY HAD A LITTLE LAMB1234567890") // 32 characters lopng
      , _plaintextInitialLength(0)
      , _encryptedText(new uint8_t[16384])
      , _encLength(0)
      , _iv(nullptr)
{
    // Setup and init _iv
    _iv = new uint8_t[AES_BLOCK_SIZE];
    RAND_bytes(_iv,
               sizeof(_iv));

    // The key we're encrypting with
    //! @todo foreach
    for(int i = 0; i < 17; ++i)
    {
        _definedKey[i] = 0x00;
    }

    _definedKey[0] = 0x01;
    _definedKey[1] = 0x02;
    _definedKey[2] = 0x03;

    _plaintextInitialLength = (int) std::strlen((char *) _plaintextInitial);

    etc::ssl::decipher::CipherDoer::EncipherText(_definedKey,
                                                 _iv,
                                                 _plaintextInitial,
                                                 &_encryptedText,
                                                 &_encLength,
                                                 &_plaintextInitialLength);
}

void CourseworkHandler::StartSerial()
{
    // Where to store the final decrypted file
    auto plaintextFinal = new uint8_t[AES_BLOCK_SIZE * 2];

    etc::key::key key;
    int success = 0;
    do
    {
        int plaintextLengthSerial = 0;
        success = etc::ssl::decipher::CipherDoer::DecipherText(key.getStringNorm(),
                                                               _iv,
                                                               &plaintextFinal,
                                                               &_encryptedText,
                                                               &plaintextLengthSerial,
                                                               &_encLength);
        key.incrementStringNorm();

        if(success)
        {
            if(std::strcmp((char *) plaintextFinal,
                           (char *) _plaintextInitial) != 0)
            {
                //False decryption
                success = 0;
            }
        }
    }
    while(!success);

    std::cout << "Final: " << plaintextFinal << std::endl;

    delete[] plaintextFinal;
}

void CourseworkHandler::StartOpenMP()
{
    omp_lock_t threadLock;
    etc::threadHandler::ThreadHandler th(threadLock);
    etc::solutionHandler::SolutionHandler sh(4,
                                             th);

    // Start generating solutions
#pragma omp sections nowait
    {
        sh.GenUsingHandler();
    };

    std::vector<etc::ssl::decipherDoer *> doers;
    for(int i = 0; i < 4; ++i)
    {
        doers[i] = new etc::ssl::decipherDoer(i,
                                              _iv,
                                              _encryptedText,
                                              _encLength,
                                              _plaintextInitial,
                                              _plaintextInitialLength,
                                              th,
                                              sh.getQueue(i));
    }

#pragma omp parallel num_threads(4)
    {
        int threadNum = omp_get_thread_num();
        (doers[threadNum])->startDecrypting();
    }
}

} /* namespace etc */