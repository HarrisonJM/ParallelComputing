/*!
 * @brief Runs the coursework
 *
 * @date 07/11/18
 */
#include <omp.h>
#include <mpi.h>

#include <cstdint>
#include <openssl/rand.h>
#include <iostream>
#include <cstring>
#include <openssl/ssl.h>
#include <iomanip>

#include "key/key.h"
#include "ssl/cipherDoer.h"
#include "courseworkHandler.h"

/* MPI defines */
#define INITTAG 1
#define SUCCESSTAG 2
#define KEYTAG 3
#define REQTAG 4

namespace etc
{

CourseworkHandler::CourseworkHandler()
    : _plaintextInitial((uint8_t*) "MARY HAD A LITTLE LAMB1234567890") // 32 characters lopng
      , _plaintextInitialLength(0)
      , _encryptedText(new uint8_t[16384])
      , _encLength(0)
      , _iv(nullptr)
{
    // Setup and init _iv
    _iv = new uint8_t[AES_BLOCK_SIZE];
    RAND_bytes(_iv,
               sizeof(_iv));

    // The key we're encrypting with
    //! @todo foreach
    for (int i = 0; i < 17; ++i)
    {
        _definedKey[i] = 0x00;
    }

    _definedKey[0] = 0xFF;
    _definedKey[1] = 0x10;
    _definedKey[2] = 0xFF;
//    _definedKey[4] = 0x10;
//    _definedKey[5] = 0x03;

    _plaintextInitialLength = (int) std::strlen((char*) _plaintextInitial);

    etc::ssl::decipher::CipherDoer::EncipherText(_definedKey,
                                                 _iv,
                                                 _plaintextInitial,
                                                 &_encryptedText,
                                                 &_encLength,
                                                 &_plaintextInitialLength);
}

CourseworkHandler::~CourseworkHandler()
{
    delete _iv;
    delete[] _encryptedText;
}

void CourseworkHandler::StartSerial()
{
    // Where to store the final decrypted file
    auto plaintextFinal = new uint8_t[AES_BLOCK_SIZE*2];
    auto solution = new uint8_t[AES_128_KEY_SIZE];

    etc::key::key key;
    int success = 0;
    do
    {
        int plaintextLengthSerial = 0;
        key.getStringNorm(&solution);
        success = etc::ssl::decipher::CipherDoer::DecipherText(solution,
                                                               _iv,
                                                               &plaintextFinal,
                                                               &_encryptedText,
                                                               &plaintextLengthSerial,
                                                               &_encLength);
        key.incrementStringNorm();

        if (success)
        {
            success = _SolutionCheck(plaintextFinal,
                                     _plaintextInitial,
                                     solution,
                                     "Serial");
        }
    } while (!success);

    delete[] plaintextFinal;
}

void CourseworkHandler::StartOpenMP()
{
    // Where to store the final decrypted file
    etc::key::key key;
    int success = 0;
    bool finish = false;
    omp_lock_t lck;
    omp_init_lock(&lck);

        std::cout << " Before OMP." << std::endl;
#pragma omp parallel num_threads(20) shared(finish, key) private(success)
    {
        std::cout << " Before thingy." << std::endl;
        auto solution = new uint8_t[AES_128_KEY_SIZE];
        auto plaintextFinal = new uint8_t[AES_BLOCK_SIZE*2];
        auto pr_iv = new uint8_t[AES_BLOCK_SIZE];
        auto pr_encT = new uint8_t[16384];
        auto pr_pt = new uint8_t[16384];

        std::cout << " After thingy." << std::endl;

        // Create thread specific personal copies
        omp_set_lock(&lck);
        std::memcpy(pr_iv,
                    _iv,
                    AES_BLOCK_SIZE);
        std::memcpy(pr_encT,
                    _encryptedText,
                    16384);
        std::memcpy(pr_pt,
                    _plaintextInitial,
                    _plaintextInitialLength);
        int pr_encL = _encLength;
        omp_unset_lock(&lck);
        std::cout << "ID: " << omp_get_thread_num() << " After Lock." << std::endl;
        while (!finish)
        {
            int plaintextLengthSerial = 0;

            // Get a new key
            omp_set_lock(&lck);
            key.getStringNorm(&solution);
            key.incrementStringNorm();
            omp_unset_lock(&lck);

            // Use the key
            success = etc::ssl::decipher::CipherDoer::DecipherText(solution,
                                                                   pr_iv,
                                                                   &plaintextFinal,
                                                                   &pr_encT,
                                                                   &plaintextLengthSerial,
                                                                   &pr_encL);
            std::cout << "ID: " << omp_get_thread_num() << " After decipher" << std::endl;
            // Verify it is a valid decrpytion
            //! @todo tidy up
            if (success)
            {
                omp_set_lock(&lck);
                finish = _SolutionCheck(plaintextFinal,
                                        pr_pt,
                                        solution,
                                        "OpenMP");
                omp_unset_lock(&lck);
            }
        }

//            delete[] pr_iv;
        delete[] solution;
        delete[] pr_encT;
        delete[] pr_pt;
        delete[] plaintextFinal;
    }
}

/*!
 * First the masterthread sends data to all the other threads
 *
 * The master thread provides keys in an as-needed policy
 * At the start, each thread will check to see if the others are done by
 * requesting that information from the master
 *
 * When a worker thread is complete/a solution is found, it will instead send the
 * Master a "done" signal (as opposed to a note done signal). Which the master willt hen send out when requested
 */
void CourseworkHandler::StartMPI()
{
    int procNum = 0;
    int ID = 0;

    // Init the MPI comms, split
    MPI::Init();
    // How many workers do we have?
    procNum = MPI::COMM_WORLD.Get_size();
    // _our_ particular ID
    ID = MPI::COMM_WORLD.Get_rank();

    bool safe = true;

    if (ID == 0)
    {
        safe = _MasterWork(procNum);
    }
    else
    {
        _WorkerWork();
    }

    if (safe)
        MPI::Finalize();
}

void CourseworkHandler::_printKey(const uint8_t* key
                                  , const int length)
{
    std::cout << "         Key: ";
    for (int i = 0; i < length; ++i)
    {
        if ((i%60) == 0)
        {
            std::cout << std::endl;
        }

        std::cout << std::hex << std::setfill('0') << std::setw(2) << (int) key[i] << " ";
    }

    std::cout << std::endl;
}
/*!
 * Performs the work of the master Thread
 * @param procNum The number of processors
 */
bool CourseworkHandler::_MasterWork(int procNum)
{
    // We only have a single processor!
    if (procNum < 2)
    {
        std::cout << "ONLY 1 PROCESSOR!" << std::endl;
        return false;
    }

    for (int i = 1; i < procNum; ++i)
    {
        std::cout << "Sending IV to: " << i << std::endl;
        // Send the IV
        MPI::COMM_WORLD.Send(_iv, // buf
                             AES_BLOCK_SIZE*2, // count
                             MPI_INTEGER, // dataType
                             i, //dest
                             INITTAG); //tag

        std::cout << "Sending encrypted text to: " << i << std::endl;
        // Send the encrypted Text
        MPI::COMM_WORLD.Send(_encryptedText, // buf
                             16384, // count
                             MPI_INTEGER, // dataType
                             i, //dest
                             INITTAG); //tag

        std::cout << "Sending enc length to: " << i << std::endl;
        // Send the encrypted Text length
        MPI::COMM_WORLD.Send(&_encLength, // buf
                             1, // count
                             MPI_INTEGER, // dataType
                             i, //dest
                             INITTAG); //tag

        std::cout << "Sending plaintext for comparison to: " << i << std::endl;
        // Send the initial plaintext
        MPI::COMM_WORLD.Send(_plaintextInitial, // buf
                             _plaintextInitialLength, // count
                             MPI_INTEGER, // dataType
                             i, //dest
                             INITTAG); //tag

        std::cout << "Sending _plaintextInitialLength to: " << i << std::endl;
        // Send the length of the plaintext
        MPI::COMM_WORLD.Send(&_plaintextInitialLength, // buf
                             1, // count
                             MPI_INTEGER, // dataType
                             i, //dest
                             INITTAG); //tag

    }

    etc::key::key key;
    auto solution = new uint8_t[AES_128_KEY_SIZE];

    // Will be used by allgather so it knows when to end
    bool done = false;
    while (!done)
    {
        // Dish out keys
        for (int i = 1; i < procNum; ++i)
        {
            bool keyRequest = false;

            //Fetch whether the key needs fetching
            MPI::COMM_WORLD.Irecv(&keyRequest, // buf
                                  1, // count
                                  MPI_CXX_BOOL, // dataType
                                  i, //dest
                                  REQTAG); // tag

            if (true == keyRequest)
            {
                key.getStringNorm(&solution);
                MPI::COMM_WORLD.Send(solution, // buf
                                     AES_128_KEY_SIZE, // count
                                     MPI_UINT8_T, // dataType
                                     i, //dest
                                     KEYTAG); // tag
                key.incrementStringNorm();
            }
        }

        // Gather completion from all threads
        MPI::COMM_WORLD.Allreduce(&done,
                                  &done,
                                  1,
                                  MPI_CXX_BOOL,
                                  MPI_LOR);
    }

    return true;
}
/*!
 * Performs the work of the worker thread
 */
void CourseworkHandler::_WorkerWork()
{
    auto solution = new uint8_t[AES_128_KEY_SIZE];
    auto plaintextFinal = new uint8_t[AES_BLOCK_SIZE*2];
    auto pr_iv = new uint8_t[AES_BLOCK_SIZE];
    auto pr_encT = new uint8_t[16384];
    auto pr_pt = new uint8_t[16384];
    int pr_encL = 0;

    // receive parameters
    {
        // Receive the IV
        MPI::COMM_WORLD.Recv(pr_iv, // buf
                             AES_BLOCK_SIZE*2, // count
                             MPI_UINT8_T, // dataType
                             0, //src
                             INITTAG); //tag

        // Receive the encrypted Text
        MPI::COMM_WORLD.Recv(pr_encT, // buf
                             16384, // count
                             MPI_UINT8_T, // dataType
                             0, //src
                             INITTAG); //tag

        // Receive the encrypted Text length
        MPI::COMM_WORLD.Recv(&pr_encL, // buf
                             1, // count
                             MPI_INTEGER, // dataType
                             0, //src
                             INITTAG); //tag

        // Receive the initial plaintext
        MPI::COMM_WORLD.Recv(pr_pt, // buf
                             _plaintextInitialLength, // count
                             MPI_UINT8_T, // dataType
                             0, //src
                             INITTAG); //tag

        // Receive the length of the plaintext
        MPI::COMM_WORLD.Recv(&_plaintextInitialLength, // buf
                             1, // count
                             MPI_INTEGER, // dataType
                             0, //src
                             INITTAG); //tag
    }

    bool done = false;
    const bool request = true;
    int success = false;

    while (!done)
    {
        // Send request to master for a key
        MPI::COMM_WORLD.Send(&request,
                             1,
                             MPI_CXX_BOOL,
                             0,
                             REQTAG);
        // Get the key
        MPI::COMM_WORLD.Recv(&solution,
                             AES_128_KEY_SIZE,
                             MPI_UINT8_T,
                             0,
                             KEYTAG);

        int plaintextLengthSerial = 0;
        success = etc::ssl::decipher::CipherDoer::DecipherText(solution,
                                                               pr_iv,
                                                               &plaintextFinal,
                                                               &pr_encT,
                                                               &plaintextLengthSerial,
                                                               &pr_encL);

        // Verify it is a valid decrpytion
        //! @todo tidy up
        if (success)
        {
            done = _SolutionCheck(plaintextFinal,
                                  pr_pt,
                                  solution,
                                  "MPI");
        }

        // Gather completion from all threads
        MPI::COMM_WORLD.Allreduce(&done,
                                  &done,
                                  1,
                                  MPI_CXX_BOOL,
                                  MPI_LOR);
    }

    delete[] solution;
    delete[] pr_encT;
    delete[] pr_pt;
    delete[] plaintextFinal;
}
bool CourseworkHandler::_SolutionCheck(const uint8_t* unencText
                                       , const uint8_t* plainText
                                       , const uint8_t* key
                                       , const char* method)
{
    bool ret = false;

    if (std::strcmp((char*) unencText,
                    (char*) plainText) == 0)
    {
        // Successful
        std::cout <<
                  "Final " << method << ": " << unencText << std::endl;
        _printKey(key,
                  AES_128_KEY_SIZE);
        // Alert other threads to finish
        ret = true;
    }

    return ret;
}
} /* namespace etc */
