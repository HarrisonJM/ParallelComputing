/*!
 * @brief
 *
 * @addtogroup
 *
 * @date 04/11/18
 */
#include <cstring>
#include <openssl/err.h>
#include <iostream>

#include "cipherAgent.h"
#include "solutionHandler.h"

#ifndef KILOBYTE16
#define KILOBYTE16 16384
#endif

namespace etc::ssl::decipher
{
/*!
 * @brief constructor
 * @param agentID ID of the agent (thread number)
 * @param th thread handler reference
 * @param sh The solution handler that contains the queue we want to use
 * @param encipher True to encipher text
 */
CipherAgent::CipherAgent(int agentID
                         , threadHandler::ThreadHandler &th
                         , solutionHandler::SolutionHandler &sh)
    : _ID(agentID)
      , _iv()
      , _th(th)
      , _keyToTry(sh.getQueue(_ID))
      , _cipherType(EVP_aes_128_ctr())
{
}
/*!
 * @brief mroe basic constructor for enciphering
 * @param th The thread handler that we _must_ provide
 */
CipherAgent::CipherAgent(threadHandler::ThreadHandler &th)
    : _ID(0)
      , _th(th)
{
}
/*!
 * Kicks off the cracking serially
 */
void CipherAgent::startCrackingSerial(const uint8_t* encipheredText
                                      , uint8_t** plaintext
                                      , int encLength
                                      , int* plaintextLength)
{
    solutionHandler::SolutionHandler sh(1,
                                        _th);
    // Will run the cracking in serial
    int success = 1;
    while (success)
    {
        int ptLen = 0;
        success = _PerformCrackingSerial(sh,
                                         encipheredText,
                                         plaintext,
                                         &ptLen,
                                         encLength);
    }

    std::cout << "AAAAAA" << std::endl;
}
/*!
 * Kicks off the cracking using openMP
 */
void CipherAgent::startCrackingWithOpenMP()
{
}
/*!
 * Kicks off the cracking using MPI
 */
void CipherAgent::startCrackingWithMPI()
{
}
/*!
 * @brief performs the actual decryption
 * @param sh a reference to the solution handler that can dish out
 * @param in The text that is encrypted
 * @param out Where to store the decrypted information
 * @param outLength The length of the plaintext storage
 * @param inLength The length of the encrypted text
 * @return The result code of the final function
 */
int CipherAgent::_PerformCrackingSerial(solutionHandler::SolutionHandler &sh
                                        , const uint8_t* in
                                        , uint8_t** out
                                        , int* outLength
                                        , int inLength)
{
    EVP_CIPHER_CTX* ctx;

    _InitContext(&ctx);
//    _InitDecrypt(ctx,
//                 sh.GetOneSolution());
    _InitDecrypt(ctx,
                 (uint8_t*) "123");

    _DecryptUpdate(ctx,
                   out,
                   outLength,
                   inLength,
                   in);

    return _FinaliseDecryption(ctx,
                               out,
                               outLength);
}

int CipherAgent::_PerformCrackingOpenMP()
{
    int res = 0;
//    do
//    {
//        EVP_CIPHER_CTX *ctx;
//
//        _InitContext(&ctx);
//        _InitDecrypt(ctx,
//                     _AccessKey());
//
//        int plaintextLength;
//        int readBytes = 1;
//
//        for (int i = 0; i < _encLength; ++i)
//        {
//            _DecryptUpdate(ctx,
//                           &_plaintext,
//                           &plaintextLength,
//                           readBytes,
//                           nullptr);
//        }
//
//        res = _FinaliseDecryption(ctx,
//                                  &_plaintext,
//                                  &plaintextLength);
//        if (0 == res)
//        {
//            bool update = true;
//            _accessThreadHandler(&update);
//        }
//
//        // If done is true, exit
//        if (_accessThreadHandler(nullptr))
//            abort();
//    } while (res);

    return res;
}

int CipherAgent::_PerformCrackingMPI()
{
    return 1;
}

/*!
 * @brief Initialises the decryption context
 * @param ctx
 */
void CipherAgent::_InitContext(EVP_CIPHER_CTX** ctx)
{
    *ctx = EVP_CIPHER_CTX_new();

    if (!*ctx)
    {
        _handleOpenSSLErrors();
    }
}
/*!
 * @brief Initialises the decryption
 * @param ctx The context
 * @param key Th key we'll try
 */
void CipherAgent::_InitDecrypt(EVP_CIPHER_CTX* ctx
                               , const uint8_t* key)
{
    int res = EVP_DecryptInit_ex(ctx,
                                 EVP_aes_128_cbc(),
                                 nullptr,
                                 key,
                                 _iv);

    if ((key[0] == '1') && (key[1] == '2') && (key[2] == '3'))
    {
        std::cout << "KEY Used:" << key << std::endl;
    }

    if (1 != res)
    {
        _handleOpenSSLErrors();
    }
}
/*!
 * @brief Performs an update on the decryption, decrypting more blocks
 * @param ctx The Context of the decrpytion
 * @param plaintext_length The Max length of the output
 * @param out The plaintext
 * @param outLength The length written
 * @param inLength The length in
 */
void CipherAgent::_DecryptUpdate(EVP_CIPHER_CTX* ctx
                                 , uint8_t** out
                                 , int* outLength
                                 , int inLength
                                 , const uint8_t* in)
{
    std::cout << "Before up" << std::endl;
    int res = EVP_DecryptUpdate(ctx,
                                *out,
                                outLength,
                                in,
                                inLength);

    std::cout << "After up" << std::endl;

    if (res != 1)
        _handleOpenSSLErrors();
}
/*!
 * @brief Performs the final step of the decryption
 * @param ctx The context
 * @param plaintext_length The length of plaintext so far
 * @param outLength How much length is left to decrpyt
 */
int CipherAgent::_FinaliseDecryption(EVP_CIPHER_CTX* ctx
                                     , uint8_t** out
                                     , int* outLength)
{
    std::cout << "before fin" << std::endl;
    int success = EVP_DecryptFinal_ex(ctx,
                                      *out,
                                      outLength);

    if (success != 1)
    {
        fprintf(stderr,
                "Fin Error: %s",
                ERR_error_string(stderr, ERR_get_error()));
    }

    std::cout << "after fin" << std::endl;
    //1 on success
    return !success;
}
/*!
 * @brief Grabs the key from the front of the queue and pops it off
 * @return A pointer to the key
 */
const uint8_t* CipherAgent::_AccessKey() const
{
    auto* key = new uint8_t[16];

    memcpy(key,
           _keyToTry->front(),
           16);

    _keyToTry->pop();

    return key;
}

/*!
 * @brief handles errors by just quitting
 */
void CipherAgent::_handleOpenSSLErrors() const
{
    ERR_print_errors_fp(stderr);
    abort();
}
/*!
 * @brief checks or updates the ThreadHandler
 * @param update IF not NULL updates the threadhandler with the value pointed to
 * @return Whether another thread has signified that it has completed
 */
const bool CipherAgent::_accessThreadHandler(bool* update) const
{
    if (update)
        _th.setDone(*update);

    return _th.getDone();
}
/*!
 * @brief enciphers a text for use in decrpytion
 * @param key The key to encrypt with
 * @param iv The initializaiton vector
 * @param plaintext The text to encrypt
 * @param encipheredText Where to store the encrypted data
 * @param outLen The size of the storage pointer
 * @param inLen The size of the plaintext
 */
void CipherAgent::EncipherText(const uint8_t* key
                               , const uint8_t* iv
                               , const uint8_t* plaintext
                               , uint8_t** encipheredText
                               , int* outLen
                               , int* inLen) const
{
    int counter = 0; // Track loops
    const int bufSize = KILOBYTE16;

    EVP_CIPHER_CTX* ctx = nullptr;
    if (!(ctx = EVP_CIPHER_CTX_new()))
        _handleOpenSSLErrors();

    if (!EVP_CipherInit_ex(ctx,
                           EVP_aes_128_cbc(),
                           nullptr,
                           key,
                           iv,
                           1))
    {
        fprintf(stderr,
                "ERROR: EVP_CipherInit_ex failed. OpenSSL error: %s\n",
                ERR_error_string(ERR_get_error(),
                                 nullptr));
        EVP_CIPHER_CTX_cleanup(ctx);
    }

    while (counter < *inLen)
    {
        // Read in data in blocks until EOF. Update the ciphering with each read.
        if (!EVP_CipherUpdate(ctx,
                              (*encipheredText) + counter,
                              outLen,
                              plaintext,
                              *inLen))
        {
            break;
        }

        // Move pointer along
        counter++;
    }
}
} /* NAMESPACE etc::decipher */