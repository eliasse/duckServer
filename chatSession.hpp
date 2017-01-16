//
// Created by Elias Strandell on 03/01/17.
//

#ifndef ASYNCSERVER_CHATSESSION_HPP
#define ASYNCSERVER_CHATSESSION_HPP

#include "chatParticipant.hpp"
#include "chatSession.hpp"
#include "chat_message.hpp"
#include "chatRoom.hpp"
#include <boost/asio.hpp>
#include <set>
#include <list>
#include <iostream>
#include <deque>
#include <cstdlib>
#include <chrono>

class chat_session
        : public chat_participant,
          public std::enable_shared_from_this<chat_session>
{
public:
    chat_session(boost::asio::ip::tcp::socket socket, chat_room& room)
            : socket_(std::move(socket)),
              room_(room)
    {
    }

    void start()
    {
        room_.join(shared_from_this());
        std::cout << "\033[1;32Connection from: \033[0m\n" << socket_.remote_endpoint() << std::endl;
        do_read_header();
    }

    void deliver(const chat_message& msg)
    {
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (!write_in_progress)
        {
            do_write();
        }
    }

private:
    void do_read_header()
    {
        auto self(shared_from_this());
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.data(), chat_message::header_length),
                                [this, self](boost::system::error_code ec, std::size_t /*length*/)
                                {
                                    if (!ec && read_msg_.decode_header())
                                    {
                                        do_read_body();
                                    }
                                    else
                                    {
                                        room_.leave(shared_from_this());
                                    }
                                });
    }

    void do_read_body()
    {
        auto self(shared_from_this());
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
                                [this, self](boost::system::error_code ec, std::size_t /*length*/)
                                {
                                    if (!ec)
                                    {
                                        room_.deliver(read_msg_);
                                        do_read_header();
                                    }
                                    else
                                    {
                                        room_.leave(shared_from_this());
                                    }
                                });
    }

    void do_write()
    {
        auto self(shared_from_this());
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(write_msgs_.front().data(),
                                                     write_msgs_.front().length()),
                                 [this, self](boost::system::error_code ec, std::size_t /*length*/)
                                 {
                                     if (!ec)
                                     {
                                         write_msgs_.pop_front();
                                         if (!write_msgs_.empty())
                                         {
                                             do_write();
                                         }
                                     }
                                     else
                                     {
                                         room_.leave(shared_from_this());
                                     }
                                 });
    }

    boost::asio::ip::tcp::socket socket_;
    chat_room& room_;
    chat_message read_msg_;
    std::deque<chat_message, std::allocator<chat_message>> write_msgs_;
};

#endif //ASYNCSERVER_CHATSESSION_HPP
