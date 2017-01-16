//
// Created by Elias Strandell on 04/01/17.
//

#include "chatServer.hpp"

void chat_server::deliver(const chat_message &msg) {
    room_.deliver(msg);
}

void chat_server::addParticipantToRoom(std::shared_ptr<chat_participant> &participant) {
    room_.join(participant);
}

void chat_server::do_accept()
{
    acceptor_.async_accept(socket_,
                           [this](boost::system::error_code ec)
                           {
                               if (!ec)
                               {
                                   std::__1::make_shared<chat_session>(std::__1::move(socket_), room_)->start();
                               }

                               do_accept();
                           });
}