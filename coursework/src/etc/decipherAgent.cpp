/*!
 * @brief
 *
 * @addtogroup
 *
 * @date 04/11/18
 */



#include <cstring>
#include "decipherAgent.h"
#include "solutionHandler.h"

namespace etc::decipher
{
/*!
 * @brief constructor
 * @param agentID ID of the agent (thread number)
 * @param th thread handler reference
 * @param encipheredText The text that is enciphered
 * @param _errorHandle A function pointer to an error handling method
 */
decipherAgent::decipherAgent(int agentID
                             , threadHandler::ThreadHandler &th
                             , unsigned char *encipheredText
                             , int encLength
                             , void (*_errorHandle)()
                             , solutionHandler::SolutionHandler &sh)
    :
    _ID(agentID)
    , _iv()
    , _encipheredText()
    , _encLength(encLength)
    , _plaintext(new unsigned char[encLength])
    , _th(th)
    , _handleOpenSSLErrors(_errorHandle)
    , _keyToTry(sh.getQueue(_ID))
{
    for(int i = 0; i < _encLength; ++i)
    {
//        _encipheredText[i] = encipheredText[i];
    }
}
/*!
 * Kicks off the cracking
 */
void decipherAgent::startCracking()
{
}
/*!
 * @brief Grabs the key from the front of the queue and pops it off
 * @return A pointer to the key
 */
const char *decipherAgent::_AccessKey()
{
    char *key = new char[100];
    std::strcpy(key,
                _keyToTry->front().c_str());

    _keyToTry->pop();
    return key;
}
/*!
 * @brief performs the actual decryption
 */
void decipherAgent::_PerformCracking()
{
    EVP_CIPHER_CTX *ctx;
}
/*!
 * @brief Initialises the decryption context
 * @param ctx
 */
void decipherAgent::_InitContext(EVP_CIPHER_CTX *ctx)
{
    ctx = EVP_CIPHER_CTX_new();

    if (!ctx)
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
                                 , const unsigned char *key)
{
    int res = EVP_DecryptInit_ex(ctx,
                                 EVP_aes_256_cbc(),
                                 NULL,
                                 key,
                                 _iv);

    if (1 != res)
    {
        _handleOpenSSLErrors();
    }
}
/*!
 * @brief Updates the decryption
 * @param ctx
 * @param plaintext_length
 */
void decipherAgent::_DecryptUpdate(EVP_CIPHER_CTX *ctx
                                   , int plaintext_length)
{
    int res = EVP_DecryptUpdate(ctx,
                                _plaintext,
                                &plaintext_length,
                                _encipheredText,
                                _encipheredText.length());

    if (1 != res)
        _handleOpenSSLErrors();
}
/*!
 * @brief Performs the final step of the decryption
 * @param ctx
 * @param plaintext_length
 * @param lengthLeft
 */
void decipherAgent::_FinaliseDecryption(EVP_CIPHER_CTX *ctx
                                        , long plaintext_length
                                        , int lengthLeft)
{
}
} /* NAMESPACE etc::decipher */