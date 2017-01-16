//
// Created by Elias Strandell on 16/01/17.
//

#ifndef ASYNCSERVER_CONFIGREADER_HPP
#define ASYNCSERVER_CONFIGREADER_HPP

#include <boost/property_tree/json_parser.hpp>
#include <iostream>

struct configReader {
    configReader(std::string &&filename) {
        try {
            boost::property_tree::read_json("configuration.json", conf);
        } catch (boost::property_tree::file_parser_error &e) {
            std::cerr << e.what() << std::endl;
        }
        catch (boost::property_tree::json_parser_error &e) {
            std::cerr << e.what() << std::endl;
        }
    }

    std::string getPortName() {
        return conf.get<std::string>("SERIALPORT");
    }

    int getBaudRate() {
        return conf.get<int>("BAUDRATE");
    }

    std::string getName() {
        return conf.get<std::string>("VEHICLENAME");
    }

    std::list<int> getPorts() {
        std::list<int> portNumbers;
        for (boost::property_tree::ptree::value_type &cell : conf.get_child("SERVER_PORTS")) {
            portNumbers.push_back(atoi(cell.second.data().c_str()));
        }
        return portNumbers;
    }

private:
    boost::property_tree::ptree conf;
};

#endif //ASYNCSERVER_CONFIGREADER_HPP
