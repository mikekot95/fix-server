#include "fixserver.h"

#include <iostream>
#include <iomanip>
#include <ctime>

#include <quickfix/fix44/Logon.h>
#include <quickfix/fix44/Reject.h>

template < typename T1, typename T2 >
    void log(T1 txt, T2 msg) {
        std::time_t now = std::time(nullptr);
        std::tm * localTime = std::localtime( & now);
        std::cout << "[" << std::put_time(localTime, "%Y-%m-%d %H:%M:%S") << "] " << txt << " : " << msg << std::endl;
    }

void MyFIXServer::onCreate(const FIX::SessionID & sessionID) {
    log("Session created for", sessionID);
}

void MyFIXServer::onLogon(const FIX::SessionID & sessionID) {
    log("Logon successful for session:", sessionID);
    is_logged = true; // Устанавливаем состояние при успешном логине
}

void MyFIXServer::onLogout(const FIX::SessionID & sessionID) {
    log("Logged out for", sessionID);
    is_logged = false; // Сбрасываем состояние при выходе
}

void MyFIXServer::toAdmin(FIX::Message & message,
    const FIX::SessionID & sessionID) {
    log("Outgoing admin message:", message.toString());
}

void MyFIXServer::fromAdmin(const FIX::Message & message,
    const FIX::SessionID & sessionID)
EXCEPT(FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon) {
    try {
        crack(message, sessionID);
        log("Incoming admin message:", message.toString());
    } catch (const FIX::FieldNotFound & e) {
        std::cerr << "Field not found: " << e.what() << std::endl;
    } catch (const FIX::IncorrectDataFormat & e) {
        std::cerr << "Incorrect data format: " << e.what() << std::endl;
    } catch (const FIX::IncorrectTagValue & e) {
        std::cerr << "Incorrect tag value: " << e.what() << std::endl;
    } catch (const FIX::RejectLogon & e) {
        std::cerr << "Reject Logon: " << e.what() << std::endl;
    } catch (const std::exception & e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }
}

void MyFIXServer::toApp(FIX::Message & message,
    const FIX::SessionID & sessionID)
EXCEPT(FIX::DoNotSend) {
    try {
        log("Outgoing application message:", message.toString());
    } catch (const FIX::DoNotSend & e) {
        std::cerr << "Message must not be sent: " << e.what() << std::endl;
    } catch (const std::exception & e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }
}

void MyFIXServer::fromApp(const FIX::Message & message,
    const FIX::SessionID & sessionID)
EXCEPT(FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType) {
    try {
        crack(message, sessionID);
        log("Incoming application message:", message.toString());
    } catch (const FIX::FieldNotFound & e) {
        std::cerr << "Field not found: " << e.what() << std::endl;
    } catch (const FIX::IncorrectDataFormat & e) {
        std::cerr << "Incorrect data format: " << e.what() << std::endl;
    } catch (const FIX::IncorrectTagValue & e) {
        std::cerr << "Incorrect tag value: " << e.what() << std::endl;
    } catch (const FIX::UnsupportedMessageType & e) {
        std::cerr << "Unsupported message type: " << e.what() << std::endl;
    } catch (const std::exception & e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }
}

void MyFIXServer::onMessage(const FIX44::Logon & message,
    const FIX::SessionID & sessionID)
EXCEPT(FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::InvalidMessage, , FIX::UnsupportedMessageType) {
    try {
        log("Received Logon message:", message.toString());
        if (is_logged) {
            std::cout << "Session already logged in. Rejecting new logon request." << std::endl;
            FIX44::Reject reject_response;
            reject_response.setField(FIX::FIELD::MsgType, "3"); // MsgType = Reject
            reject_response.setField(FIX::FIELD::Text, "Already logged in");
            FIX::Session::sendToTarget(reject_response, sessionID);
        } else {
            std::cout << "Processing logon..." << std::endl;

            // Отправляем подтверждение успешного логина
            FIX44::Logon logon_response;
            logon_response.setField(FIX::FIELD::MsgType, "A"); // MsgType = Logon
            logon_response.setField(FIX::FIELD::HeartBtInt, "30"); // Heartbeat interval
            FIX::Session::sendToTarget(logon_response, sessionID);
            is_logged = true; // Устанавливаем состояние логина
        }
    } catch (const FIX::FieldNotFound & e) {
        std::cerr << "Field not found: " << e.what() << std::endl;
    } catch (const FIX::IncorrectDataFormat & e) {
        std::cerr << "Incorrect data format: " << e.what() << std::endl;
    } catch (const FIX::IncorrectTagValue & e) {
        std::cerr << "Incorrect tag value: " << e.what() << std::endl;
    } catch (const FIX::InvalidMessage & e) {
        std::cerr << "Invalid message: " << e.what() << std::endl;
    } catch (const FIX::UnsupportedMessageType & e) {
        std::cerr << "Unsupported message type: " << e.what() << std::endl;
    } catch (const std::exception & e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }
}
