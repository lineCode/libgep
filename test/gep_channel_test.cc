#include <stdio.h>  // for remove
#include <string.h>  // for memset
#include <unistd.h>  // for usleep

#include "gep_client.h"
#include "gep_test_lib.h"
#include "test_protocol.h"

#include "gtest/gtest.h"

class GepChannelTest : public GepTest {
};

TEST_F(GepChannelTest, SetSocket) {
  // push message in the client
  GepChannel *gc = client_->GetGepChannel();
  int socket = gc->GetSocket();
  gc->SetSocket(socket);
  gc->SendMessage(command1_);

  // push message in the server
  GepChannelArray *gca = server_->GetGepChannelArray();
  gca->SendMessage(command3_);

  WaitForSync(2);
}

TEST_F(GepChannelTest, RecvDataInvalidSocket) {
  // set an invalid socket
  GepChannel *gc = client_->GetGepChannel();
  gc->SetSocket(-1);
  EXPECT_EQ(-1, gc->RecvData());
}


TEST_F(GepChannelTest, RecvDataBufferFull) {
  // add too much read data
  GepChannel *gc = client_->GetGepChannel();
  gc->SetLen(GepProtocol::kMaxMsgLen + 1);
  EXPECT_EQ(-1, gc->RecvData());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
