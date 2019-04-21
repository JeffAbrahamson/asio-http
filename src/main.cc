/*
  Copyright 2019  Jellybooks

  Originally based on example code from Boost ASIO, which bore the
  following copyright notice:

  Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include <asio.hpp>
#include <boost/program_options.hpp>
#include <exception>
#include <iostream>
#include <string>

#include "server.h"

using std::cout;
using std::endl;
using std::string;
using std::to_string;

namespace BPO = boost::program_options;
class usage_exception : public std::exception {};
class help_exception : public std::exception {};

void Usage(const string &binary_name, const BPO::options_description &options,
           const BPO::variables_map &opt_map) {
  cout << "Usage: " << binary_name << " options" << endl << endl;
  cout << options << endl;
  if (opt_map.count("help") == 0) {
    cout << "Bad invocation:  ";
  }
  cout << endl;
  cout << "Usage: http_server <address> <port> <doc_root>" << endl;
  cout << "  For IPv4, try:" << endl;
  cout << "    receiver 0.0.0.0 80 ." << endl;
  cout << "  For IPv6, try:" << endl;
  cout << "    receiver 0::0 80 ." << endl;

  if (opt_map.count("help") > 0)
    throw help_exception();
  throw usage_exception();
}

void FlagsValidate(int argc, char *argv[], BPO::options_description &options,
                   BPO::positional_options_description &positional_options,
                   BPO::variables_map &opt_map) {
  try {
    BPO::store(BPO::command_line_parser(argc, argv)
                   .options(options)
                   .positional(positional_options)
                   .run(),
               opt_map);
  } catch (BPO::required_option &e) {
    cout << "Missing required argument: " << e.what() << endl;
    throw usage_exception();
  } catch (BPO::error &e) {
    cout << "Invocation error: " << e.what() << endl;
    Usage(argv[0], options, opt_map);
    throw usage_exception();
  } catch (...) {
    cout << "Unknown error." << endl;
    throw usage_exception();
  }
  if (opt_map.count("help")) {
    Usage(argv[0], options, opt_map);
  }
  BPO::notify(opt_map);
}

void FlagsValidate(int argc, char *argv[], BPO::options_description &options,
                   BPO::variables_map &opt_map) {
  BPO::positional_options_description positional_options;
  FlagsValidate(argc, argv, options, positional_options, opt_map);
}

BPO::variables_map ParseOptions(int argc, char *argv[]) {
  BPO::options_description general("General");
  general.add_options()("listen-address,l",
                        BPO::value<string>()->default_value("localhost"),
                        "Interface on which to listen")(
      "port,p", BPO::value<int>()->default_value(9999))(
      "document-root", BPO::value<string>()->default_value("."));

  BPO::options_description options("Available options");
  options.add(general);
  options.add_options()("help,h", "Produce this help message");

  BPO::variables_map opt_map;
  FlagsValidate(argc, argv, options, opt_map);
  return opt_map;
}

int main(int argc, char *argv[]) {
  BPO::variables_map options;
  try {
    options = ParseOptions(argc, argv);
  } catch (help_exception) {
    return 0;
  } catch (usage_exception) {
    return 1;
  }
  try {

    // Initialise the server.
    const string address = options["listen-address"].as<string>();
    const string port = to_string(options["port"].as<int>());
    const string root = options["document-root"].as<string>();
    http::server::Server s(address, port, root);

    // Run the server until stopped.
    s.Run();
  } catch (std::exception &e) {
    std::cerr << "exception: " << e.what() << "\n";
  }

  return 0;
}
