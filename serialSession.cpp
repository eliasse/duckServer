//
// Created by Elias Strandell on 04/01/17.
//
#include "serialSession.hpp"
#include <boost/iostreams/stream.hpp>

extern std::list<chat_server> servers;

void serialSession::deliver(const chat_message& msg) {
    bool write_in_progress = !write_msgs_.empty();
    // Find NMEA sentences in the message and create a new message for each sentence
    std::string json = msg.body();

    std::cout << "=======================================\n";
    std::cout << json;
    std::cout << "\n=======================================\n";

    boost::iostreams::stream<boost::iostreams::array_source> json_stream(json.c_str(), json.size());

    boost::property_tree::ptree ptree;
    try {
        boost::property_tree::json_parser::read_json(json_stream, ptree);
        for (boost::property_tree::ptree::value_type &entry : ptree.get_child("NMEA_SENTENCES")) {
            std::string name = entry.first;
            std::string data = (std::basic_string<char, std::char_traits<char>, std::allocator<char>> &&) entry.second.data();
            chat_message out_msg;
            out_msg.body_length(std::strlen(data.c_str()));
            std::memcpy(out_msg.body(), data.c_str(), msg.body_length());
            out_msg.encode_header();
            write_msgs_.push_back(out_msg);

            std::cout << "Found: " << name << " = " << data;
            std::cout << "Messages in queue: " << write_msgs_.size() << std::endl;
        }
    } catch (boost::property_tree::json_parser_error parser_error) {
        std::cout << parser_error.what() << std::endl;
    }

    if (!write_in_progress)
    {
        do_write();
    }
}

void serialSession::do_write()
{
    std::cout << "Writing to SerialPort: " << write_msgs_.front().body() << std::endl;
    serialPort.write(*write_msgs_.front().body(), write_msgs_.front().body_length());
    write_msgs_.pop_front();

    if (!write_msgs_.empty()) {
        do_write();
    }
}

bool serialSession::isSerialPort() {
    return true;
}

void serialSession::readLineHandler(std::string str) {
    using namespace std::chrono;

    milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    std::time_t time_ = std::time(nullptr);
    char *time = std::ctime(&time_);
    boost::property_tree::ptree ptree;
    ptree.put("SOURCE", "ARDUINO");
    ptree.put("TIME_UTC", time);
    ptree.put("TIME_UNIX", ms.count());
    ptree.put("MESSAGES.NMEA", str.c_str());
    std::stringstream json;
    boost::property_tree::write_json(json, ptree);
    std::cout << json.str() << std::endl;

    std::string json_string;
    json_string = json.str();

    chat_message msg;
    msg.body_length(std::strlen(json_string.c_str()));
    std::memcpy(msg.body(), json_string.c_str(), msg.body_length());
    msg.encode_header();
    msg.setIsSerialPortData();
    std::for_each(std::begin(servers), std::end(servers), [msg](typename std::list<chat_server>::reference server){ server.deliver(msg); });
}

void serialSession::readLine() {
    serialPort.asyncReadLine();
}
