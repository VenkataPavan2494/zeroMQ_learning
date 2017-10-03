// This is part of the program to demonstrate a simple PUSH-PULL model for the
// zmq message processing.
// Task ventilator is something that pushes the required info to the sub-tasks
// that run concurrently. These sub-tasks do their corresponding jobs in
// parallel.
// Header for the zmq
#include <zmq.hpp>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <ctime>
#include <iostream>


#define within(num) (int) ((float) num * random () / (RAND_MAX + 1.0))

// The main function..
int main(int argc, char ** argv) {

  // Create a context..
  zmq::context_t context {1};

  // Socket to send messages on.. for a given context. The messages are sent to
  // the workers through this socket.
  zmq::socket_t sender {context, ZMQ_PUSH};

  // Bind the socket to an address.
  sender.bind("tcp://*:5557");

  // Make this 'Task ventilator' wait till the workers start...
  std::cout << "Press enter when the workers are ready!!!" << std::endl;

  getchar();

  std::cout << "Sending tasks to the workers..." << std::endl;

  // The first message that should sent is for the sink, which indicates it
  // that a batch of parallel workers run to-gether.

  // Another socket for the sink.
  zmq::socket_t sink {context, ZMQ_PUSH};

  // Connect to the address on which the sink's socket is binded to..
  sink.connect("tcp://localhost:5558");

  zmq::message_t msg_to_send {2}; // Message to be sent to the sink..

  std::memcpy(msg_to_send.data(), "0", 1); // Send just '0' to the sink, to
                                           // to indicate the start of the
                                           // batch.

  sink.send(msg_to_send);   // Send the message to the sink..

  // Initialize random number generator...
  std::srand(std::time(NULL));

  // Send 100 tasks out..
  int task_nb = 0;

  int total_msec = 0; // Total amount of work in terms of duration to be done.

  for(task_nb = 0; task_nb != 100; task_nb++) {
    int work_load = 0; // The work load interms of the milli seconds to be done
                       // for each worker..

    work_load = within(100) + 1;

    total_msec += work_load;

    // Reuse the same message tha was used to send to the sink. We just have to
    // 'rebuild' it for the same.
    msg_to_send.rebuild(10);

    // reset the contents of the message..
    std::memset(msg_to_send.data(), '\0', 10);



  }

  return 0;
}
