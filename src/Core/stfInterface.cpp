#include <iostream>
#include <cstdio>
#include <string>

class StockfishEngine {
private:
    FILE* stockfish_input;
    FILE* stockfish_output;
    
public:
    bool startEngine(const std::string& path_to_stockfish) {
        std::string command = path_to_stockfish;
        
        // Open pipes to communicate with Stockfish
        stockfish_input = popen(command.c_str(), "w");
        stockfish_output = popen(command.c_str(), "r");
        
        if (!stockfish_input || !stockfish_output) {
            return false;
        }
        
        // Initialize Stockfish
        sendCommand("uci");
        
        return true;
    }
    
    void sendCommand(const std::string& command) {
        if (stockfish_input) {
            fprintf(stockfish_input, "%s\n", command.c_str());
            fflush(stockfish_input);
        }
    }
    
    std::string readOutput() {
        std::string result;
        char buffer[256];
        
        if (stockfish_output && fgets(buffer, sizeof(buffer), stockfish_output)) {
            result = buffer;
            // Remove newline
            if (!result.empty() && result.back() == '\n') {
                result.pop_back();
            }
        }
        
        return result;
    }
    
    std::string getBestMove(const std::string& fen, int depth = 15) {
        sendCommand("position fen " + fen);
        sendCommand("go depth " + std::to_string(depth));
        
        std::string output;
        std::string bestMove;
        
        // Read until we find the best move
        while ((output = readOutput()) != "") {
            if (output.find("bestmove") != std::string::npos) {
                size_t pos = output.find("bestmove");
                if (pos != std::string::npos) {
                    bestMove = output.substr(pos + 9); // "bestmove " is 9 chars
                    // Extract just the move (in case there's ponder info)
                    size_t space_pos = bestMove.find(' ');
                    if (space_pos != std::string::npos) {
                        bestMove = bestMove.substr(0, space_pos);
                    }
                    break;
                }
            }
        }
        
        return bestMove;
    }
    
    ~StockfishEngine() {
        if (stockfish_input) {
            sendCommand("quit");
            pclose(stockfish_input);
        }
        if (stockfish_output) {
            pclose(stockfish_output);
        }
    }
};