#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <iostream>
#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = net::ip::tcp;

// URL encode function to properly encode special characters
std::string encodeFen(const std::string& fen) {
    std::string result;

    for(char c : fen)
    {
        switch (c)
        {
        case ' ':
            result += "%20";
            break;

        case '/':
            result += "%2F";
            break;
        
        default:
            result += c;
            break;
        }
    }
    return result;
}

// Synchronous HTTPS GET request with SSL/TLS
std::string httpsGet(const std::string& host, const std::string& port, 
                     const std::string& target) {
    try {
        std::cout << "Connecting to " << host << ":" << port << target << " (HTTPS)" << std::endl;
        
        // Create I/O context
        net::io_context ioc;

        // SSL context
        ssl::context ctx(ssl::context::tlsv12_client);
        
        // Verify the remote server's certificate (set to default paths)
        ctx.set_default_verify_paths();
        ctx.set_verify_mode(ssl::verify_none);  // For testing; use verify_peer in production

        // Resolver for looking up domain names
        tcp::resolver resolver(ioc);
        
        // SSL stream
        beast::ssl_stream<beast::tcp_stream> stream(ioc, ctx);
        
        // Set SNI Hostname (many servers need this)
        if(!SSL_set_tlsext_host_name(stream.native_handle(), host.c_str())) {
            beast::error_code ec{static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()};
            throw beast::system_error{ec};
        }

        // Set timeout for operations (30 seconds)
        beast::get_lowest_layer(stream).expires_after(std::chrono::seconds(30));

        // Look up the domain name
        std::cout << "Resolving DNS..." << std::endl;
        auto const results = resolver.resolve(host, port);
        std::cout << "DNS resolved, connecting..." << std::endl;

        // Make the connection
        beast::get_lowest_layer(stream).connect(results);
        std::cout << "TCP connected, performing SSL handshake..." << std::endl;
        
        // Perform SSL handshake
        stream.handshake(ssl::stream_base::client);
        std::cout << "SSL handshake complete!" << std::endl;

        // Set up the HTTP GET request
        http::request<http::string_body> req{http::verb::get, target, 11};
        req.set(http::field::host, host);
        req.set(http::field::user_agent, "ChessClient/V0.1");
        req.set(http::field::accept, "application/json");
        req.set(http::field::connection, "keep-alive");

        // Send the HTTP request 
        std::cout << "Sending request..." << std::endl;
        http::write(stream, req);
        std::cout << "Request sent, waiting for response..." << std::endl;

        // Buffer for reading the response
        beast::flat_buffer buffer;

        // Declare a container to hold the response
        http::response<http::string_body> res;

        // Receive the HTTP response
        http::read(stream, buffer, res);
        std::cout << "Response received! Status: " << res.result_int() << std::endl;

        // Gracefully close the stream
        beast::error_code ec;
        stream.shutdown(ec);
        
        // ec can contain errors but that's ok for some servers
        if(ec == net::error::eof || ec == ssl::error::stream_truncated) {
            // These are expected when Connection: close is used
            ec = {};
        }
        if(ec) {
            std::cout << "Shutdown warning: " << ec.message() << std::endl;
        }

        // Return the response body
        return res.body();

    } catch (std::exception const& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return "";
    }
}


int main() {
    std::cout << "=== HTTPS GET Example with Stockfish API ===" << std::endl;
    
    // The FEN string needs to be URL encoded
    std::string fen = "rn1q1rk1/pp2b1pp/2p2n2/3p1pB1/3P4/1QP2N2/PP1N1PPP/R4RK1 b - - 1 11";
    std::string depth = "12";
    
    // Encode the FEN string
    std::string encodedFen = encodeFen(fen);
    
    // Build the full path with encoded parameters
    std::string path = "/api/s/v2.php?fen=" + encodedFen + "&depth=" + depth;
    
    std::cout << "Original FEN: " << fen << std::endl;
    std::cout << "Encoded FEN: " << encodedFen << std::endl;
    std::cout << "Full path: " << path << std::endl << std::endl;
    
    std::string response = httpsGet("stockfish.online", "443", path);
    std::cout << "Response body:\n" << response << std::endl;

    return 0;
}