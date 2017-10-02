#include <zmq.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#define within(num) (int) ((float) num * random () / (RAND_MAX + 1.0))

int main () {

    //  Prepare our context and publisher
    zmq::context_t context {1};

    // Create a socket for the communication. This should be of type 'PUB'
    // (Publisher). That is, it is a publisher of climate information.
    zmq::socket_t publisher {context, ZMQ_PUB};

    // Now bind the socket to a port.
    publisher.bind("tcp://*:5556");

    //publisher.bind("ipc://weather.ipc");                // Not usable on Windows.

    //  Initialize random number generator
    srandom ((unsigned) time (NULL));

    // weather publication counter...
    //uint64_t publication_count = 0;

    while (1) {

        int zipcode, temperature, relhumidity;

        //  Get values that will fool the boss
        zipcode     = within (100000);
        temperature = within (215) - 80;
        relhumidity = within (50) + 10;

        //  Send message to all subscribers
        zmq::message_t message {20};
        snprintf ((char *) message.data(), message.size() ,
            "%05d %d %d", zipcode, temperature, relhumidity);
        publisher.send(message);

        // Try to print the weather publication count..
        //std::cout << "Weather publication nb:" << publication_count++ << std::endl;

    }
    return 0;
}