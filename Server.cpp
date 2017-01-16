// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/asio.hpp>
#include "chatServer.hpp"
#include "configReader.hpp"

using boost::asio::ip::tcp;

class SimpleSerial;
SimpleSerial *serial = 0;

std::list<chat_server> servers;

int main(int argc, char* argv[])
{
    configReader conf("configuration.json");
    std::cout << "Vehicle name: " << conf.getName() << std::endl;

    try
    {
        boost::asio::io_service io_service;

        try {
            serial = new SimpleSerial(io_service, conf.getPortName(), conf.getBaudRate());
        }
        catch (boost::system::system_error e) {
            std::cerr << "Serial port error: " << e.what() << std::endl;
        }

        std::list<int> portNumbers = conf.getPorts();
        for (int num : portNumbers)
        {
            tcp::endpoint endpoint(tcp::v4(), num);
            servers.emplace_back(io_service, endpoint);
        }

        std::shared_ptr<chat_participant> serial_session = std::make_shared<serialSession>(*serial);

        for (auto& server: servers) {
            server.addParticipantToRoom(serial_session);
        }

        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
