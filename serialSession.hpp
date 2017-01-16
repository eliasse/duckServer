//
// Created by Elias Strandell on 04/01/17.
//

#ifndef ASYNCSERVER_SERIALSESSION_HPP
#define ASYNCSERVER_SERIALSESSION_HPP

#include "chatServer.hpp"
#include "chatParticipant.hpp"
#include "SimpleSerial.hpp"

class serialSession :
        public chat_participant,
        public std::enable_shared_from_this<serialSession> {

public:
    SimpleSerial &serialPort;
    std::deque<chat_message, std::allocator<chat_message>> write_msgs_;
    serialSession(SimpleSerial &port) : serialPort(port) {
        serialPort.setReadLineCallback(this->readLineHandler);
        readLine();
    }
    static void readLineHandler(std::string);
    bool isSerialPort();

private:
    void deliver(const chat_message &msg);
    void do_write();
    void readLine();
};


#endif //ASYNCSERVER_SERIALSESSION_HPP
