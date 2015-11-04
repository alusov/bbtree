#ifndef RING_BUFFER_H
#define RING_BUFFER_H

namespace BBTree
{

template<int SIZE, class DATA_T>
class RingBuffer
{
public:
  typedef uint32_t INDEX_T;

private:

  DATA_T data[SIZE];
  INDEX_T readCount = 0;
  INDEX_T writeCount = 0;
  static const INDEX_T mask = SIZE - 1;

public:
  void Write(const DATA_T &value)
  {
    data[writeCount++ & mask] = value;
  }

  void Read(DATA_T &value)
  {
    value = data[readCount++ & mask];
  }

  INDEX_T Count()const
  {
    return writeCount - readCount;
  }

  inline bool IsEmpty()const
  {
    return writeCount == readCount;
  }

  inline bool IsFull()const
  {
    return ((INDEX_T)(writeCount - readCount) & (INDEX_T)~(mask)) != 0;
  }
};

}
#endif /* RING_BUFFER_H */
