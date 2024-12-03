#include "fixserver.h"

#include <iostream>

#include <quickfix/FileStore.h>
#include <quickfix/SocketAcceptor.h>

int main(int argc, char * argv[]) {
    if (argc < 2) {
        std::cerr << "No input file!";
        return 1;
    }

    try {
        MyFIXServer application;
        FIX::SessionSettings settings(argv[1]);
        FIX::FileStoreFactory storeFactory(settings);
        FIX::SocketAcceptor acceptor(application, storeFactory, settings);
        acceptor.start();
        std::cout << "Type Ctrl-C to quit" << std::endl;

        while (true) {
            // Сервер работает
        }

        acceptor.stop();
        return 0;
    } catch (const FIX::ConfigError & e) {
        std::cerr << "Configuration error: " << e.what() << std::endl;
        return 1;
    } catch (const FIX::SessionNotFound & e) {
        std::cerr << "Session not found: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception & e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 1;
    }
}
