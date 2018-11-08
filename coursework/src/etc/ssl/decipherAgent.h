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

#include "threadHandler.h"
#include "solutionHandler.h"

namespace etc::ssl::decipher
{

class decipherAgent
{
public:
    decipherAgent(int agentID
                  , threadHandler::ThreadHandler &th
                  , uint8_t *encipheredText
                  , int encLength
                  , etc::solutionHandler::SolutionHandler &sh);

    ~decipherAgent() = default;

    void startCrackingSerial();
    void startCrackingWithOpenMP();
    void startCrackingWithMPI();

private:

    const int _ID;

    const uint8_t *_iv;
    const uint8_t *_encipheredText;
    const int _encLength;

    uint8_t *_plaintext;
    threadHandler::ThreadHandler &_th;

    std::queue<const uint8_t *> *_keyToTry;

    const EVP_CIPHER *_cipherType;
    int _PerformCrackingSerial(solutionHandler::SolutionHandler &sh);

    int _PerformCrackingOpenMP();
    int _PerformCrackingMPI();
    void _InitContext(EVP_CIPHER_CTX **ctx);
    void _InitDecrypt(EVP_CIPHER_CTX *ctx
                      , const uint8_t *key);

    void _DecryptUpdate(EVP_CIPHER_CTX *ctx
                        , uint8_t **ptOut
                        , int *outLength
                        , int numBytes);
    int _FinaliseDecryption(EVP_CIPHER_CTX *ctx
                            , uint8_t **ptOut
                            , int *lengthLeft);

    const uint8_t *_AccessKey() const;
    const void _handleOpenSSLErrors() const;
    const bool _accessThreadHandler(bool* update) const;

};
} /* NAMESPACE etc::decipher */

#endif /*PROTOCOLDEVELOPER_DECIPHERAGENT_H*/
