#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include <iostream>
#include <memory>

#include "gen-cpp/CalculatorService.h"
#include "gen-cpp/calculator_types.h"

using ::apache::thrift::protocol::TBinaryProtocol;
using ::apache::thrift::protocol::TProtocol;
using ::apache::thrift::protocol::TTransport;
using ::apache::thrift::transport::TBufferedTransport;
using ::apache::thrift::transport::TSocket;
using namespace std;

int main(int argc, char** argv) {
  shared_ptr<TTransport> socket(new TSocket("localhost", 8080));
  shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  Calculator::CalculatorServiceClient client(protocol);

  transport->open();
  Calculator::AddRequest request;
  request.num1 = 2;
  request.num2 = 3;
  cout << client.Add(request) << "\n";
  transport->close();

  // TODO: Play with the timeout on the server side and on the client side.

  return 0;
}
