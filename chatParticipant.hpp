//
// Created by Elias Strandell on 03/01/17.
//

#ifndef ASYNCSERVER_CHATPARTICIPANT_HPP
#define ASYNCSERVER_CHATPARTICIPANT_HPP

#include "chat_message.hpp"
#include "chatParticipant.hpp"
#include <deque>

class chat_participant;

typedef std::shared_ptr<chat_participant> chat_participant_ptr;

class chat_participant
{
public:
    virtual ~chat_participant() {}
    virtual void deliver(const chat_message& msg) = 0;
    virtual bool isSerialPort() { return false; }
private:
};

#endif //ASYNCSERVER_CHATPARTICIPANT_HPP
