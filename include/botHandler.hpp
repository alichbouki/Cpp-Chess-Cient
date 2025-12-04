#ifndef BOT_HANDLER_HPP
#define BOT_HANDLER_HPP

#include <chess.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <json.hpp>

#include <iostream>


namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;

using tcp = net::ip::tcp;
using namespace std;
using namespace nlohmann;

// Function declarations
string encodeFen(const string& fen);
string httpsGet(const string& host, const string& port, const string& target);
void getBotMove(ChessBoard* Board);

#endif