/*!
 * @brief
 *
 * @addtogroup
 *
 * @date 07/11/18
 */


#ifndef PROTOCOLDEVELOPER_ENCIPHEREDTEXTREADER_H
#define PROTOCOLDEVELOPER_ENCIPHEREDTEXTREADER_H

#include <string>
#include <fstream>
#include <memory>

namespace etc::reader
{

class encipheredTextReader
{
public:
    explicit encipheredTextReader(std::string fileName);
    ~encipheredTextReader() = default;

    std::unique_ptr<uint8_t> _GetNextBlock();
private:
    std::string _fileName;
    std::fstream _if;
};
}

#endif /*PROTOCOLDEVELOPER_ENCIPHEREDTEXTREADER_H*/
