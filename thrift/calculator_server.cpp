#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TServerSocket.h>

#include <iostream>
#include <memory>

#include "gen-cpp/CalculatorService.h"
#include "gen-cpp/calculator_types.h"

using ::apache::thrift::server::TServerTransport;
using ::apache::thrift::server::TTransportFactory;
using ::apache::thrift::transport::TServerSocket;

using ::apache::thrift::TProcessor;
using ::apache::thrift::protocol::TBinaryProtocolFactory;
using ::apache::thrift::protocol::TProtocolFactory;
using ::apache::thrift::server::TSimpleServer;
using ::apache::thrift::transport::TBufferedTransportFactory;

using namespace std;

class ServicesImpl : public Calculator::CalculatorServiceIf {
 public:
  ServicesImpl() = default;

  int64_t Add(const Calculator::AddRequest& request) override {
    int64_t sum = request.num1 + request.num2;
    return sum;
  }
};

int main() {
  constexpr int PORT = 8080;

  shared_ptr<TServerTransport> serverTransport(new TServerSocket(PORT));
  shared_ptr<TTransportFactory> transportFactory(
      new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
  shared_ptr<ServicesImpl> services(new ServicesImpl());
  shared_ptr<TProcessor> processor(
      new Calculator::CalculatorServiceProcessor(services));
  TSimpleServer server(processor, serverTransport, transportFactory,
                       protocolFactory);

  cout << "Starting server ..." << endl;
  server.serve();
  cout << "Server stopped." << endl;

  return EXIT_SUCCESS;
}
