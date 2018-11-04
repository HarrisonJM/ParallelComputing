/*!
 * @brief holds an individual character for the key
 *
 * @addtogroup
 *
 * @date 04/11/18
 */


#ifndef PROTOCOLDEVELOPER_KEYSEGMENT_H
#define PROTOCOLDEVELOPER_KEYSEGMENT_H

namespace etc::key
{

class keySegment
{
public:
    explicit keySegment(int segNum);
    ~keySegment();

    void incrementC();
    char getCharacter();
    keySegment *getNextSegment();

private:
    int _segNum;
    char _c;
    keySegment *_nextSegment;
};
} /* NAMEPSACE etc::key */

#endif /*PROTOCOLDEVELOPER_KEYSEGMENT_H*/
