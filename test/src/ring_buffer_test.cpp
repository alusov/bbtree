#include <limits.h>
#include "gtest/gtest.h"
#include "ring_buffer.h"

using namespace BBTree;
 
class RingBufferTest : public ::testing::Test {
 protected:
  virtual void SetUp() 
  {
  }
 
  virtual void TearDown() 
  {
  }
};
	 
TEST_F(RingBufferTest,IsFullBuffer)
{
    RingBuffer<4,int> ringBuffer;  
    
    ringBuffer.Write(1);
    ringBuffer.Write(2);
    ringBuffer.Write(3);  
    ASSERT_FALSE(ringBuffer.IsFull());
    ringBuffer.Write(4);
    ASSERT_TRUE(ringBuffer.IsFull());
    EXPECT_EQ(1,ringBuffer.Read_());
    ASSERT_FALSE(ringBuffer.IsFull());      
}
