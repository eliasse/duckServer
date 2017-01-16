//
// Created by Elias Strandell on 03/01/17.
//

#ifndef ASYNCSERVER_CHATSERVER_HPP
#define ASYNCSERVER_CHATSERVER_HPP

#include "chat_message.hpp"
#include "chatServer.hpp"
#include "chatRoom.hpp"
#include "chatSession.hpp"
#include <boost/asio.hpp>
#include <set>
#include <list>
#include <iostream>
#include <deque>
#include <cstdlib>

class serialSession;

class chat_server
{
public:
    chat_server(boost::asio::io_service& io_service,
                const boost::asio::ip::tcp::endpoint& endpoint)
            : acceptor_(io_service, endpoint),
              socket_(io_service)
    {
        do_accept();
    }

    void deliver(const chat_message &msg);

    // Elias
    void addParticipantToRoom(std::shared_ptr<chat_participant> &participant);

private:
    void do_accept();

    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::socket socket_;
    chat_room room_;
};

#endif //ASYNCSERVER_CHATSERVER_HPP