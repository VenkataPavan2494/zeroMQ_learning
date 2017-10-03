// This is a program to demonstrate the simple PUSH-PULL pattern of the
// zmq messaging.
#include <zmq.hpp>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <ctime>
#include <iostream>
#include <sstream>

int main (int argc, char *argv[])
{
    zmq::context_t context(1);

    //  Socket to receive messages on
    // The following socket is for getting the workload from the task ventilator
    zmq::socket_t receiver(context, ZMQ_PULL);
    receiver.connect("tcp://localhost:5557");

    //  Socket to send messages to. This is for sending the notification to the
    // sink about the current task being done..
    zmq::socket_t sender(context, ZMQ_PUSH);
    sender.connect("tcp://localhost:5558");

    //  Process tasks forever
    while (1) {

        zmq::message_t message;
        int workload;           //  Workload in msecs

        receiver.recv(&message);
        std::string smessage(static_cast<char*>(message.data()), message.size());

        std::istringstream iss {smessage};
        iss >> workload;

        //  Do the work
        sleep(workload);

        //  Send results to sink
        message.rebuild();
        sender.send(message);

        //  Simple progress indicator for the viewer
        std::cout << "." << std::flush;
    }
    return 0;
}
