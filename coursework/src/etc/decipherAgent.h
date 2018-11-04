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

namespace etc::decipher
{

class decipherAgent
{
public:
    decipherAgent(int agentID
                      , threadHandler::ThreadHandler &th
                      , unsigned char *encipheredText
                      , int encLength
                      , void (*_errorHandle)()
                      , etc::solutionHandler::SolutionHandler &sh);

    ~decipherAgent() = default;

    void startCracking();

private:

    const int _ID;

    const unsigned char _iv[128];
    unsigned char* _encipheredText[16384];
    int _encLength;
    unsigned char *_plaintext;
    threadHandler::ThreadHandler &_th;

    void (*_handleOpenSSLErrors)(void);
    std::queue<std::string> *_keyToTry;

    const char *_AccessKey();
    void _PerformCracking();

    void _InitContext(EVP_CIPHER_CTX *ctx);
    void _InitDecrypt(EVP_CIPHER_CTX *ctx
                      , const unsigned char *key);
    void _DecryptUpdate(EVP_CIPHER_CTX *ctx
                            , int plaintext_length);
    void _FinaliseDecryption(EVP_CIPHER_CTX *ctx
                             , long plaintext_length
                             , int lengthLeft);
};
} /* NAMESPACE etc::decipher */

#endif /*PROTOCOLDEVELOPER_DECIPHERAGENT_H*/
