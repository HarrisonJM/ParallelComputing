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
#include <cstdint>

namespace etc
{
namespace ssl
{
namespace decipher
{

class CipherDoer
{
public:
    CipherDoer() = default;
    ~CipherDoer() = default;

    static int EncipherText(const uint8_t* key
                            , const uint8_t* iv
                            , const uint8_t* plaintext
                            , uint8_t** encipheredText
                            , int* outLen
                            , int* inLen);
    static int DecipherText(const uint8_t* key
                            , const uint8_t* iv
                            , uint8_t** out
                            , uint8_t** in
                            , int* outLen
                            , int* inLen);

private:

    static void _handleOpenSSLErrors();
};
}
}
} /* NAMESPACE etc::decipher */

#endif /*PROTOCOLDEVELOPER_DECIPHERAGENT_H*/
