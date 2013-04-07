#include "tx_oracommon.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

// Loads Conn_settings structure from the specified XML file
void Conn_settings::load(const string &filename)
{
    using boost::property_tree::ptree;
    ptree pt;

    read_xml(filename, pt);

    username   = pt.get<string>("connection.username");
    password   = pt.get<string>("connection.password");
	connString = pt.get<string>("connection.connString");
};