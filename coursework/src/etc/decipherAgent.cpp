/*!
 * @brief
 *
 * @addtogroup
 *
 * @date 04/11/18
 */



#include <cstring>
#include <openssl/err.h>

#include "decipherAgent.h"
#include "solutionHandler.h"

namespace etc::decipher
{
/*!
 * @brief constructor
 * @param agentID ID of the agent (thread number)
 * @param th thread handler reference
 * @param encipheredText The text that is enciphered
 * @param encLength The length of the encoded data in bytes
 * @param sh The solution handler that contains the queue we want to use
 *
 */
decipherAgent::decipherAgent(int agentID
                             , threadHandler::ThreadHandler &th
                             , uint8_t *encipheredText
                             , int encLength
                             , solutionHandler::SolutionHandler &sh)
    :
    _ID(agentID)
    , _iv()
    , _encipheredText(encipheredText)
    , _encLength(encLength)
    , _plaintext(new unsigned char[encLength])
    , _th(th)
    , _keyToTry(sh.getQueue(_ID))
    , _cipherType(EVP_aes_128_ctr())
{
}
/*!
 * Kicks off the cracking serially
 */
void decipherAgent::startCrackingSerial()
{
    // Will run the cracking in serial
    while (_PerformCrackingSerial());
}
/*!
 * Kicks off the cracking using openMP
 */
void decipherAgent::startCrackingWithOpenMP()
{
}
/*!
 * Kicks off the cracking using MPI
 */
void decipherAgent::startCrackingWithMPI()
{
}
/*!
 * @brief performs the actual decryption
 */
int decipherAgent::_PerformCrackingSerial()
{
    EVP_CIPHER_CTX *ctx;

    _InitContext(&ctx);
    _InitDecrypt(ctx,
                 _AccessKey());

    int plaintextLength;
    int readBytes = 1;

    for (int i = 0; i < _encLength; ++i)
    {
        _DecryptUpdate(ctx,
                       &_plaintext,
                       &plaintextLength,
                       readBytes);
    }

    return _FinaliseDecryption(ctx,
                               &_plaintext,
                               &plaintextLength);
}

int decipherAgent::_PerformCrackingOpenMP()
{
    int res;
    do
    {
        EVP_CIPHER_CTX *ctx;

        _InitContext(&ctx);
        _InitDecrypt(ctx,
                     _AccessKey());

        int plaintextLength;
        int readBytes = 1;

        for (int i = 0; i < _encLength; ++i)
        {
            _DecryptUpdate(ctx,
                           &_plaintext,
                           &plaintextLength,
                           readBytes);
        }

        res = _FinaliseDecryption(ctx,
                                  &_plaintext,
                                  &plaintextLength);
        if (0 == res)
        {
            bool update = true;
            _accessThreadHandler(&update);
        }

        // If done is true, exit
        if (_accessThreadHandler(nullptr))
            abort();
    } while (res);

    return res;
}

int decipherAgent::_PerformCrackingMPI()
{
    return 1;
}

/*!
 * @brief Initialises the decryption context
 * @param ctx
 */
void decipherAgent::_InitContext(EVP_CIPHER_CTX **ctx)
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
void decipherAgent::_InitDecrypt(EVP_CIPHER_CTX *ctx
                                 , const uint8_t *key)
{
    int res = EVP_CipherInit_ex(ctx,
                                nullptr,
                                nullptr,
                                key,
                                _iv,
                                0); // Indicate that we are DECRYPTING

    if (1 != res)
    {
        _handleOpenSSLErrors();
    }
}
/*!
 * @brief Performs an update on the decryption, decrypting more blocks
 * @param ctx The Context of the decrpytion
 * @param plaintext_length The Max length of the output
 * @param ptOut Where to output to
 * @param outLength The length written
 * @param numBytes The length in
 */
void decipherAgent::_DecryptUpdate(EVP_CIPHER_CTX *ctx
                                   , uint8_t **ptOut
                                   , int *outLength
                                   , int numBytes)
{
    int res = EVP_CipherUpdate(ctx,
                               *ptOut,
                               outLength,
                               _encipheredText,
                               numBytes);

    if (res != 1)
        _handleOpenSSLErrors();
}
/*!
 * @brief Performs the final step of the decryption
 * @param ctx The context
 * @param plaintext_length The length of plaintext so far
 * @param lengthLeft How much length is left to decrpyt
 */
int decipherAgent::_FinaliseDecryption(EVP_CIPHER_CTX *ctx
                                       , uint8_t **ptOut
                                       , int *lengthLeft)
{
    int res = EVP_CipherFinal_ex(ctx,
                                 *ptOut,
                                 lengthLeft);

    if (1 != res)
    {
        ERR_print_errors_fp(stderr);
    }

    return res;
}

/*!
 * @brief Grabs the key from the front of the queue and pops it off
 * @return A pointer to the key
 */
const uint8_t *decipherAgent::_AccessKey() const
{
    auto *key = new uint8_t[16];

    memcpy(key,
           _keyToTry->front(),
           16);

    _keyToTry->pop();

    return key;
}

/*!
 * @brief handles errors by just quitting
 */
const void decipherAgent::_handleOpenSSLErrors() const
{
    ERR_print_errors_fp(stderr);
    abort();
}
/*!
 * @brief checks or updates the ThreadHandler
 * @param update IF not NULL updates the threadhandler with the value pointed to
 * @return Whether another thread has signified that it has completed
 */
const bool decipherAgent::_accessThreadHandler(bool *update) const
{
    if (update)
        _th.setDone(*update);

    return _th.getDone();
}
} /* NAMESPACE etc::decipher */