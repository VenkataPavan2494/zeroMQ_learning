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
  // The following is the Server Hellow World context.
  // Please note the argument 1 to the context indicates the number of IO
  // threads it is associated with.

  zmq::context_t server_hello_wld_context {1};

  // Create a socket for the context; to enable communication between sever
  // and client.
  // Since this is a server type, the mode should be of 'REP' (Response) type

  zmq::socket_t server_socket {server_hello_wld_context, ZMQ_REP};

  // Now bnid the socket with an address.
  server_socket.bind("tcp://*:5555");

  // Run an infinite while loop.
  // The server here, first receives the message from the client and then
  // responds.
  // First Client says hello to the server and the server responds with
  // world.
  while(1) {

    // Create a instance for the message that is to tbe received from the
    // client.
    zmq::message_t msg_from_client;

    // Wait for the message from the client through the socket.
    server_socket.recv(&msg_from_client);

    // After receiving the message from the client... print the following.
    std::cout << "Received Hello from client..." << std::endl;

    // sleep for a second to see the difference.
    sleep(1);

    // Now send a message to the client.

    // We need to send "World" to the client.
    std::string cli_msg {"World"};

    // A messge instance to send to client.
    // Please note that we are indicating that the message would of the size
    // of 'World' at max.
    zmq::message_t msg_to_client {cli_msg.size()};

    // Copy the cli_msg to the zmq message to be sent to client.
    std::memcpy(msg_to_client.data(), cli_msg.c_str(), cli_msg.size());

    // Now send the message to the client over socket.

    server_socket.send(msg_to_client);
  }

  return 0;

}