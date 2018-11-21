/*!
 * @brief Performs the decrpytion
 *
 * @date 04/11/18
 */


#ifndef PROTOCOLDEVELOPER_DECIPHERAGENT_H
#define PROTOCOLDEVELOPER_DECIPHERAGENT_H

#include <string>
#include <queue>
#include <openssl/ssl.h>
#include <openssl/aes.h>
#include <openssl/evp.h>

#include "threadHandler.h"
#include "solutionHandler.h"

namespace etc::ssl::decipher
{

class CipherAgent
{
public:
    explicit CipherAgent(const uint8_t* iv);
    ~CipherAgent() = default;

    static int EncipherText(const uint8_t* key
                            , const uint8_t* iv
                            , uint8_t** plaintext
                            , uint8_t** encipheredText
                            , int* outLen
                            , int* inLen);
    static int DecipherText(const uint8_t* key
                            , const uint8_t* iv
                            , uint8_t** plaintext
                            , uint8_t** encipheredText
                            , int* plaintextLength
                            , int* encipheredTextLength);

private:
    const uint8_t* _iv;

    static void _handleOpenSSLErrors();
    static int _CipherText(int mode
                           , const uint8_t* key
                           , const uint8_t* iv
                           , uint8_t** in
                           , uint8_t** out
                           , int* outLen
                           , int* inLen);
};
} /* NAMESPACE etc::decipher */

#endif /*PROTOCOLDEVELOPER_DECIPHERAGENT_H*/
