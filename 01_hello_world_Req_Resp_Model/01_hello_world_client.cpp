// Simple example demonstrating the server and client example using ZMQ

// Include the header file for the Zero MQ
#include <zmq.hpp>
#include <cstring>
#include <iostream>
#include <unistd.h>


// The main program
int main(int argc, char ** argv) {

  // Try to print the ZMQ version..
  // The version information is a tuple..
  std::tuple<int, int, int> zmq_version_info {zmq::version()};

  std::cout << "The ZMQ version:" << std::get<0>(zmq_version_info) << "."
                                  << std::get<1>(zmq_version_info) << "."
                                  << std::get<2>(zmq_version_info) << std::endl;

  // Importantly we need to create a context for the communication between
  // two entities: server and client.
  // The following is the Client Hello World context.
  // Please note the argument 1 to the context indicates the number of IO
  // threads it is associated with.

  zmq::context_t client_hello_wld_context {1};

  // Create a socket for the context; to enable communication between sever
  // and client.
  // Since this is a server type, the mode should be of 'REQ' (Request) type
  zmq::socket_t client_socket {client_hello_wld_context, ZMQ_REQ};

  // Now attempt to conect to the server..
  std::cout << "Attempting to connect to the server..." << std::endl;

  // Now connect the socket with an address.
  client_socket.connect("tcp://localhost:5555");

  // Send 10 requests to the sever.. waiting for responses for each one of them.
  for(int idx = 0; idx != 10; idx++) {

    // String to sent to the server..
    std::string string_to_server {"Hello"};

    // Create a instance for the message that is to be sent to the client.
    zmq::message_t msg_to_client {string_to_server.size()};

    // Pack the string into the message to be sent to the server..
    std::memcpy(msg_to_client.data(), string_to_server.c_str(),
                                                      string_to_server.size());

    std::cout << "Sending Hello to the server..." << std::endl;

    client_socket.send(msg_to_client);

    // Wait for the reply from server...
    zmq::message_t reply_from_server;
    client_socket.recv(&reply_from_server);

    // After receiving the message from the client... print the following.
    std::cout << "Received world...  " << idx << std::endl;
  }

  return 0;

}