//
// Created by Elias Strandell on 03/01/17.
//

#ifndef ASYNCSERVER_CHATROOM_HPP
#define ASYNCSERVER_CHATROOM_HPP

#include "chatParticipant.hpp"
#include "serialSession.hpp"
#include "chat_message.hpp"
#include <boost/asio.hpp>
#include <set>
#include <list>
#include <iostream>
#include <deque>
#include <cstdlib>
#include <boost/property_tree/json_parser.hpp>
#include "chatRoom.hpp"

class serialSession;
typedef std::deque<chat_message> chat_message_queue;

class chat_room
{
public:
    void join(std::shared_ptr<chat_participant> participant)
    {
        participants_.insert(participant);
        for (auto msg: recent_msgs_)
            participant->deliver(msg);
    }

    void leave(std::shared_ptr<chat_participant> participant)
    {
        participants_.erase(participant);
    }

    void deliver(const chat_message& msg)
    {
        recent_msgs_.push_back(msg);
        while (recent_msgs_.size() > max_recent_msgs)
            recent_msgs_.pop_front();

        for (auto participant: participants_) {

            if (participant->isSerialPort() && msg.isSerialPortData())
            {
                //std::cout << "AVOIDING LOOPING SERIAL PORT DATA\n";
                continue;
            }

            participant->deliver(msg);
        }
    }

private:
    std::set<std::shared_ptr<chat_participant>> participants_;
    enum { max_recent_msgs = 10 };
    chat_message_queue recent_msgs_;
};

#endif //ASYNCSERVER_CHATROOM_HPP