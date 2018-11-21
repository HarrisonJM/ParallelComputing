/*!
 * @brief A class that defines a single instance of deciphering
 *
 * @date 21/11/18
 */


#ifndef PROTOCOLDEVELOPER_DECIPHERDOER_H
#define PROTOCOLDEVELOPER_DECIPHERDOER_H

#include <queue>

// For controlling the threads
#include "../threadHandler.h"

// For deciphering the encText
#include "cipherDoer.h"

namespace etc::ssl
{

class decipherDoer
{
public:
    decipherDoer(int agentID
                 , uint8_t* iv
                 , uint8_t* encText
                 , int encLength
                 , uint8_t* plainText
                 , int plainTextLength
                 , etc::threadHandler::ThreadHandler &th
                 , std::queue<uint8_t*> &solutionQueue);

    ~decipherDoer() = default;

    void startDecrypting();
private:
    int _agentID;
    uint8_t* _iv;
    uint8_t* _encryptedText;
    int _encryptedTextLength;
    uint8_t* _plainTextcmp;
    decipher::CipherDoer _decrypter;
    etc::threadHandler::ThreadHandler &_th;
    std::queue<uint8_t*> &_solutions;
};
}

#endif /*PROTOCOLDEVELOPER_DECIPHERDOER_H*/