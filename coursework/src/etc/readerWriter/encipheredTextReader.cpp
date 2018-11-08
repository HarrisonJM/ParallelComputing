/*!
 * @brief
 *
 * @addtogroup
 *
 * @date 07/11/18
 */

#include "encipheredTextReader.h"

namespace etc::reader
{

etc::reader::encipheredTextReader::encipheredTextReader(std::string fileName)
: _fileName(std::move(fileName))
{
    _if.open(_fileName, std::fstream::in | std::fstream::out);
}
std::unique_ptr<uint8_t> encipheredTextReader::_GetNextBlock()
{
    uint8_t block;
    _if >> std::noskipws >> block;
    auto block_p = std::make_unique<uint8_t>(block);

    return std::move(block_p);
}

} /* namespace etc::readerWriter */

