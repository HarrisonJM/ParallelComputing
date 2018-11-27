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
#include <iomanip>

#include "key/key.h"
#include "ssl/cipherDoer.h"
#include "courseworkHandler.h"

namespace etc
{

CourseworkHandler::CourseworkHandler()
    : _plaintextInitial((uint8_t*) "MARY HAD A LITTLE LAMB1234567890") // 32 characters lopng
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
    for (int i = 0; i < 17; ++i)
    {
        _definedKey[i] = 0x00;
    }

    _definedKey[0] = 0xFF;
    _definedKey[1] = 0x10;
    _definedKey[2] = 0xFF;
//    _definedKey[4] = 0x10;
//    _definedKey[5] = 0x03;

    _plaintextInitialLength = (int) std::strlen((char*) _plaintextInitial);

    etc::ssl::decipher::CipherDoer::EncipherText(_definedKey,
                                                 _iv,
                                                 _plaintextInitial,
                                                 &_encryptedText,
                                                 &_encLength,
                                                 &_plaintextInitialLength);
}

CourseworkHandler::~CourseworkHandler()
{
    delete _iv;
    delete[] _encryptedText;
}

void CourseworkHandler::StartSerial()
{
    // Where to store the final decrypted file
    auto plaintextFinal = new uint8_t[AES_BLOCK_SIZE*2];
    auto solution = new uint8_t[AES_128_KEY_SIZE];

    etc::key::key key;
    int success = 0;
    do
    {
        int plaintextLengthSerial = 0;
        key.getStringNorm(&solution);
        success = etc::ssl::decipher::CipherDoer::DecipherText(solution,
                                                               _iv,
                                                               &plaintextFinal,
                                                               &_encryptedText,
                                                               &plaintextLengthSerial,
                                                               &_encLength);
        key.incrementStringNorm();

        if (success)
        {
            if (std::strcmp((char*) plaintextFinal,
                            (char*) _plaintextInitial) != 0)
            {
                //False decryption
                success = 0;
            }
        }
    } while (!success);

    std::cout << "Final: " << plaintextFinal << std::endl;
    _printKey(solution,
              AES_128_KEY_SIZE);

    delete[] plaintextFinal;
}

void CourseworkHandler::StartOpenMP()
{
    // Where to store the final decrypted file

    etc::key::key key;
    int success = 0;
    bool finish = false;
    omp_lock_t lck;
    omp_init_lock(&lck);

#pragma omp parallel for shared(finish, key) private(success)
    for (int i = 0; i < 20; ++i)
    {
        auto solution = new uint8_t[AES_128_KEY_SIZE];
        auto plaintextFinal = new uint8_t[AES_BLOCK_SIZE*2];
        auto pr_iv = new uint8_t[AES_BLOCK_SIZE];
        auto pr_encT = new uint8_t[16384];
        auto pr_pt = new uint8_t[16384];

        // Create thread specific personal copies
        omp_set_lock(&lck);
        std::memcpy(pr_iv,
                    _iv,
                    AES_BLOCK_SIZE);
        std::memcpy(pr_encT,
                    _encryptedText,
                    16384);
        std::memcpy(pr_pt,
                    _plaintextInitial,
                    _plaintextInitialLength);
        int pr_encL = _encLength;
        omp_unset_lock(&lck);

        while (!finish)
        {
            int plaintextLengthSerial = 0;

            // Get a new key
            omp_set_lock(&lck);
            key.getStringNorm(&solution);
            key.incrementStringNorm();
            omp_unset_lock(&lck);

            // Use the key
            success = etc::ssl::decipher::CipherDoer::DecipherText(solution,
                                                                   pr_iv,
                                                                   &plaintextFinal,
                                                                   &pr_encT,
                                                                   &plaintextLengthSerial,
                                                                   &pr_encL);
            // Verify it is a valid decrpytion
            //! @todo tidy up
            if (success)
            {
                if (std::strcmp((char*) plaintextFinal,
                                (char*) pr_pt) != 0)
                {
                    //False decryption
                    success = 0;
                }
                else
                {
                    // Successful
                    std::cout <<
                              "Final OpenMP: " << plaintextFinal << std::endl;
                    _printKey(solution,
                              AES_128_KEY_SIZE);
                    success = 1;
                    // Alert other threads to finish
                    finish = true;

                    break;
                }
            }
        }

//            delete[] pr_iv;
        delete[] solution;
        delete[] pr_encT;
        delete[] pr_pt;
        delete[] plaintextFinal;
    }
}

void CourseworkHandler::startMPI()
{

}

void CourseworkHandler::_printKey(const uint8_t* key
                                  , const int length)
{
    std::cout << "         Key: ";
    for (int i = 0; i < length; ++i)
    {
        if ((i%60) == 0)
        {
            std::cout << std::endl;
        }

        std::cout << std::hex << std::setfill('0') << std::setw(2) << (int) key[i] << " ";
    }

    std::cout << std::endl;
}

} /* namespace etc */