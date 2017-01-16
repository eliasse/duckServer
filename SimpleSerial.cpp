//
// Created by Elias Strandell on 04/01/17.
//

#include "SimpleSerial.hpp"

boost::asio::streambuf serialInBuffer;

void SimpleSerial::writeString(std::string s)
{
    boost::asio::async_write(serial,
                             boost::asio::buffer(s.c_str(),s.size()),
                             boost::bind(&SimpleSerial::handleWrite,
                                         this,
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred()));
}

void SimpleSerial::write(const char &data, std::size_t n_bytes) {
    std::cout << "Writing " << n_bytes << " bytes\n";
    boost::asio::async_write(serial,
                             boost::asio::buffer(&data, n_bytes),
                             boost::bind(&SimpleSerial::handleWrite,
                                         this,
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred()));
}

void SimpleSerial::handleWrite(boost::system::error_code ec, std::size_t bytes_transferred) {
    if (!ec) {
        std::cout << "Write success: " << bytes_transferred << " bytes transferred." << std::endl;
    }
    else{
        std::cout << "Write failed! " << std::endl;
    }
}


void SimpleSerial::asyncReadLine() {
    boost::asio::async_read_until(serial, serialInBuffer, '\n', boost::bind(&SimpleSerial::readHandler,
                                                                            this,
                                                                            boost::asio::placeholders::error,
                                                                            boost::asio::placeholders::bytes_transferred));//, '\n', &readHandler);
}

void SimpleSerial::readHandler(boost::system::error_code e, std::size_t bytes_read) {

    if (e) {
        std::cout << "Failed to read from serial port!!!! X(" << std::endl;
        return;
    } else
    {

        if (callback_ == nullptr) {
            std::cerr << "Error: Callback function for readLine is not specified [SimpleSerial] \n";
            return;
        }
        else {
            std::cout << "I read: " << bytes_read << " bytes." << std::endl;
            std::string str(boost::asio::buffers_begin(serialInBuffer.data()), boost::asio::buffers_begin(serialInBuffer.data()) + bytes_read);
            std::cout << str << std::endl;
            callback_(str);
            serialInBuffer.consume(bytes_read);
        }
    }
    asyncReadLine();
}

void SimpleSerial::setReadLineCallback(ReadLineCallback callback) {
    callback_ = callback;
}
