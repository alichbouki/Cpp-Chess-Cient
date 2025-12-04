#include <botHandler.hpp>

string httpsGet(const string& host, const string& port, const string& target) {
    try {

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

        // Look up the domain name
        auto const results = resolver.resolve(host, port);

        // Make the connection
        beast::get_lowest_layer(stream).connect(results);
        
        // Perform SSL handshake
        stream.handshake(ssl::stream_base::client);

        // Set up the HTTP GET request
        http::request<http::string_body> req{http::verb::get, target, 11};
        req.set(http::field::host, host);
        req.set(http::field::user_agent, "ChessClient/V0.1");
        req.set(http::field::accept, "application/json");
        req.set(http::field::connection, "keep-alive");

        // Send the HTTP request 
        // cout << "Sending request..." << endl;
        http::write(stream, req);
        // cout << "Request sent, waiting for response..." << endl;

        // Buffer for reading the response
        beast::flat_buffer buffer;

        // Declare a container to hold the response
        http::response<http::string_body> res;

        // Receive the HTTP response
        http::read(stream, buffer, res);
        // cout << "Response received! Status: " << res.result_int() << endl;

        // Gracefully close the stream
        beast::error_code ec;
        stream.shutdown(ec);
        
        // ec can contain errors but that's ok for some servers
        if(ec == net::error::eof || ec == ssl::error::stream_truncated) {
            // These are expected when Connection: close is used
            ec = {};
        }
        if(ec) {
            cout << "Shutdown warning: " << ec.message() << endl;
        }

        // Return the response body
        return res.body();

    } catch (exception const& e) {
        cout << "HTTP Request Error: " << e.what() << endl;
        return "";
    }
}

string encodeFen(const string& fen) {
    string result;

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

void getBotMove(ChessBoard* board){
    // The FEN string needs to be URL encoded
    string depth = "12";
    
    // Encode the FEN string
    string encodedFen = encodeFen(board->boardToFEN());
    cout << "FEN : " << board->boardToFEN() << endl;
    
    // Build the full path with encoded parameters
    string path = "/api/s/v2.php?fen=" + encodedFen + "&depth=" + depth;
    
    string response = httpsGet("stockfish.online", "443", path);
    auto responseJson = json::parse(response);

    string botMoveStr = (responseJson["bestmove"].is_null() ? "none" : responseJson["bestmove"]);
    float eval = (responseJson["evaluation"].is_null() ? -3 : (responseJson["evaluation"].is_number_float() ? responseJson["evaluation"].get<float>() : -2));
    string mate = (responseJson["mate"].is_null() ? "none" : responseJson["mate"]);
    bool successState = responseJson["success"];

    string nextMove = botMoveStr.substr(botMoveStr.find("bestmove ") + 9);
    nextMove = nextMove.substr(0, 4);

    cout << "Bot Move: " << nextMove << endl;
    cout << "Evaluation: " << eval << endl;
    cout << "Mate in: " << mate << endl;
    cout << "Success: " << (successState ? "true" : "false") << endl;

    board->movePiece(
        nextMove[0] - 'a',
        nextMove[1] - '1',
        nextMove[2] - 'a',
        nextMove[3] - '1'
    );

    // Move move = board->parseStrMove(nextMove);
    board->eval = eval;
    board->isMate = (mate != "none");
    // board->selectPieceMove(&move);
}