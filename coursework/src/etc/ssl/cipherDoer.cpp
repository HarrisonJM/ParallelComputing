/*!
 * @brief
 *
 * @addtogroup
 *
 * @date 04/11/18
 */
#include <openssl/err.h>
#include <iostream>
#include <cstdint>

#include "cipherDoer.h"

#ifndef KILOBYTE16
#define KILOBYTE16 16384
#endif

namespace etc::ssl::decipher
{

/*!
 * @brief handles errors by just quitting
 */
void CipherDoer::_handleOpenSSLErrors()
{
    char errorMessage[16384];
    fprintf(stderr,
            "Fin Error: %s\n",
            ERR_error_string(ERR_get_error(),
                             errorMessage));
}

/*!
 * @brief enciphers a text for use in decrpytion
 * @param key The key to encrypt with
 * @param iv The initializaiton vector
 * @param in The text to encrypt
 * @param out Where to store the encrypted data
 * @param outLen The size of the storage pointer
 * @param inLen The size of the plaintext
 */
int CipherDoer::EncipherText(const uint8_t* key
                             , const uint8_t* iv
                             , const uint8_t* in
                             , uint8_t** out
                             , int* outLen
                             , int* inLen)
{
    int success = 0;
    int finalLen = 0;
    int outLenBkp = *outLen;

    EVP_CIPHER_CTX* ctx = nullptr;

    ctx = EVP_CIPHER_CTX_new();

    if (!ctx)
    {
        _handleOpenSSLErrors();
    }
    else
    {
        success = EVP_EncryptInit_ex(ctx,
                                     EVP_aes_128_cbc(),
                                     nullptr,
                                     key,
                                     iv);
    }

    if (!success)
    {
        _handleOpenSSLErrors();
    }

    if (success)
    {
        success = EVP_EncryptUpdate(ctx,
                                    *out,
                                    &outLenBkp,
                                    in,
                                    *inLen);
    }

    if (success)
        success = EVP_EncryptFinal_ex(ctx,
                                      (*out) + outLenBkp,
                                      &finalLen);

    *outLen = *inLen + finalLen;

    EVP_CIPHER_CTX_cleanup(ctx);

    return success;
}
/*!
 * @brief Decrypts a thing
 * @param key The key
 * @param iv The initilisation vector
 * @param out The plaintext to decipher
 * @param in The enciphered text
 * @param outLen the size of the plaintext buffer
 * @param inLen The size of the enciphered text buffer
 */
int CipherDoer::DecipherText(const uint8_t* key
                             , const uint8_t* iv
                             , uint8_t** out
                             , uint8_t** in
                             , int* outLen
                             , int* inLen)
{
    int success = 0;
    int finalLen = 0;
    int outLenBkp = *outLen;

    EVP_CIPHER_CTX* ctx = nullptr;

    ctx = EVP_CIPHER_CTX_new();
//    EVP_CIPHER_CTX_set_padding(ctx, 16);

    if (!ctx)
    {
        _handleOpenSSLErrors();
    }
    else
    {
        success = EVP_DecryptInit_ex(ctx,
                                     EVP_aes_128_cbc(),
                                     nullptr,
                                     key,
                                     iv);
    }

    if (!success)
    {
        _handleOpenSSLErrors();
    }

    if (success)
    {
        success = EVP_DecryptUpdate(ctx,
                                    *out,
                                    &outLenBkp,
                                    *in,
                                    *inLen);
    }

    if (success)
        success = EVP_DecryptFinal_ex(ctx,
                                      (*out) + outLenBkp,
                                      &finalLen);

    *outLen = outLenBkp + finalLen;

    EVP_CIPHER_CTX_cleanup(ctx);

    *((*out) + *outLen) = '\0';

    return success;
}
} /* NAMESPACE etc::ssl::decipher */