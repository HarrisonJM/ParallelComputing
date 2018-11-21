/*!
 * @brief
 *
 * @addtogroup
 *
 * @date 04/11/18
 */
#include <openssl/err.h>
#include <iostream>

#include "cipherAgent.h"

#ifndef KILOBYTE16
#define KILOBYTE16 16384
#endif

namespace etc::ssl::decipher
{

static int counter = 1;

/*!
 * @brief constructor
 * @param iv The IV
 */
CipherAgent::CipherAgent(const uint8_t* iv)
    : _iv(iv)
{
}
/*!
 * @brief handles errors by just quitting
 */
void CipherAgent::_handleOpenSSLErrors()
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
 * @param plaintext The text to encrypt
 * @param encipheredText Where to store the encrypted data
 * @param outLen The size of the storage pointer
 * @param inLen The size of the plaintext
 */
int CipherAgent::EncipherText(const uint8_t* key
                              , const uint8_t* iv
                              , uint8_t** plaintext
                              , uint8_t** encipheredText
                              , int* outLen
                              , int* inLen)
{
    return _CipherText(1,
                       key,
                       iv,
                       plaintext,
                       encipheredText,
                       outLen,
                       inLen);
}
/*!
 * @brief enciphers a text for use in decrpytion
 * @param mode 1 for encrypt, 0 for decrypt
 * @param key The key to encrypt with
 * @param iv The initializaiton vector
 * @param in The inny
 * @param out The outy
 * @param outLen The size of the storage pointer
 * @param inLen The size of the plaintext
 */
int CipherAgent::_CipherText(int mode
                             , const uint8_t* key
                             , const uint8_t* iv
                             , uint8_t** in
                             , uint8_t** out
                             , int* outLen
                             , int* inLen)
{
    int success = 0;

    EVP_CIPHER_CTX* ctx = nullptr;

    ctx = EVP_CIPHER_CTX_new();
//    EVP_CIPHER_CTX_set_padding(ctx, 16);

    if (!ctx)
    {
        _handleOpenSSLErrors();
    }
    else
    {
        success = EVP_CipherInit_ex(ctx,
                                    EVP_aes_128_cbc(),
                                    nullptr,
                                    key,
                                    iv,
                                    mode);
    }

    if (!success)
    {
        _handleOpenSSLErrors();
    }

//    for (int i = 0; i < 4; ++i)
    if (success)
    {
        success = EVP_CipherUpdate(ctx,
                                   *out,
                                   outLen,
                                   *in,
                                   *inLen);
    }

    if (success)
        success = EVP_CipherFinal_ex(ctx,
                                     *out,
                                     outLen);

    if (!success)
    {
        fprintf(stderr,
                "ERROR: EVP_CipherFinal_ex failed. OpenSSL error: %s\n",
                ERR_error_string(ERR_get_error(),
                                 nullptr));
    }

    EVP_CIPHER_CTX_cleanup(ctx);

    std::cout << "outLength: " << *outLen << std::endl;
    std::cout << "inLength: " << *inLen << std::endl;
    std::cout << "KEY: " << key << std::endl;
    std::cout << "IV: " << iv << std::endl;

    return success;
}
/*!
 * @brief Decrypts a thing
 * @param key The key
 * @param iv The initilisation vector
 * @param plaintext The plaintext to decipher
 * @param encipheredText The enciphered text
 * @param plaintextLength the size of the plaintext buffer
 * @param encipheredTextLength The size of the enciphered text buffer
 */
int CipherAgent::DecipherText(const uint8_t* key
                              , const uint8_t* iv
                              , uint8_t** plaintext
                              , uint8_t** encipheredText
                              , int* plaintextLength
                              , int* encipheredTextLength)
{
    int success = 0;

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
                                    *plaintext,
                                    plaintextLength,
                                    *encipheredText,
                                    *encipheredTextLength);
    }

    if (success)
        success = EVP_DecryptFinal_ex(ctx,
                                      *plaintext + *plaintextLength,
                                      plaintextLength);

    if (!success)
    {
        fprintf(stderr,
                "ERROR: EVP_CipherFinal_ex failed. OpenSSL error: %s\n",
                ERR_error_string(ERR_get_error(),
                                 nullptr));
    }

    EVP_CIPHER_CTX_cleanup(ctx);

    std::cout << "outLength: " << *plaintextLength << std::endl;
    std::cout << "inLength: " << *encipheredTextLength << std::endl;
    std::cout << "KEY: " << key << std::endl;
    std::cout << "IV: " << iv << std::endl;
    return success;

//    return _CipherText(0,
//                       key,
//                       iv,
//                       encipheredText,
//                       plaintext,
//                       plaintextLength,
//                       encipheredTextLength);
}
} /* NAMESPACE etc::ssl::decipher */