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
    CipherAgent(int agentID
                    , threadHandler::ThreadHandler &th
                    , solutionHandler::SolutionHandler &sh);

    explicit CipherAgent(threadHandler::ThreadHandler &th);

    ~CipherAgent() = default;

    void startCrackingSerial(const uint8_t* encipheredText
                             , uint8_t** plaintext
                             , int encLength
                             , int* plaintextLength);
    void startCrackingWithOpenMP();
    void startCrackingWithMPI();

    void EncipherText(const uint8_t* key
                      , const uint8_t* iv
                      , const uint8_t* plaintext
                      , uint8_t** encipheredText
                      , int* outLen
                      , int* inLen) const;

private:
    const int _ID;

    const uint8_t* _iv;

    threadHandler::ThreadHandler &_th;

    std::queue<const uint8_t*>* _keyToTry;

    const EVP_CIPHER* _cipherType;
    int _PerformCrackingSerial(solutionHandler::SolutionHandler &sh
                               , const uint8_t* in
                               , uint8_t** out
                               , int* outLength
                               , int inLength);

    int _PerformCrackingOpenMP();
    int _PerformCrackingMPI();
    void _InitContext(EVP_CIPHER_CTX** ctx);
    void _InitDecrypt(EVP_CIPHER_CTX* ctx
                      , const uint8_t* key);

    void _DecryptUpdate(EVP_CIPHER_CTX* ctx
                        , uint8_t** out
                        , int* outLength
                        , int inLength
                        , const uint8_t* in);
    int _FinaliseDecryption(EVP_CIPHER_CTX* ctx
                            , uint8_t** out
                            , int* outLength);

    const uint8_t* _AccessKey() const;

    void _handleOpenSSLErrors() const;
    const bool _accessThreadHandler(bool* update) const;
};
} /* NAMESPACE etc::decipher */

#endif /*PROTOCOLDEVELOPER_DECIPHERAGENT_H*/
