#ifndef _SIMPLE_SERIAL_
#define _SIMPLE_SERIAL_

#include <boost/asio.hpp>
#include <iostream>
#include <boost/bind.hpp>
#include <list>

typedef void (*ReadLineCallback)(std::string);

class SimpleSerial
{
public:
    /**
     * Constructor.
     * \param port device name, example "/dev/ttyUSB0" or "COM4"
     * \param baud_rate communication speed, example 9600 or 115200
     * \throws boost::system::system_error if cannot open the
     * serial device
     */
    SimpleSerial(boost::asio::io_service &io_srv, std::string port, unsigned int baud_rate)
            : io(&io_srv), serial(*io,port)
    {
        serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
    }

    void writeString(std::string s);
    void write(const char &data, std::size_t n_bytes);
    void handleWrite(boost::system::error_code ec, std::size_t bytes_transferred);
    void asyncReadLine();
    void setReadLineCallback(ReadLineCallback);
    void readHandler(boost::system::error_code e, std::size_t bytes_read);


private:
    boost::asio::io_service *io;
    boost::asio::serial_port serial;
    ReadLineCallback callback_ = nullptr;
};

#endif