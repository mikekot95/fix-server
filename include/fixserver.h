#ifndef FIXSERVER_H
#define FIXSERVER_H

#include <quickfix/Application.h>
#include <quickfix/MessageCracker.h>

class MyFIXServer: public FIX::Application, public FIX::MessageCracker {
    private: bool is_logged = false;
    public: void onCreate(const FIX::SessionID & sessionID) override;
    void onLogon(const FIX::SessionID & sessionID) override;
    void onLogout(const FIX::SessionID & sessionID) override;
    void toAdmin(FIX::Message & message,
        const FIX::SessionID & sessionID) override;
    void fromAdmin(const FIX::Message & message,
        const FIX::SessionID & sessionID) override
    EXCEPT(FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon);
    void toApp(FIX::Message & message,
        const FIX::SessionID & sessionID) override
    EXCEPT(FIX::DoNotSend);
    void fromApp(const FIX::Message & message,
        const FIX::SessionID & sessionID) override
    EXCEPT(FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType);
    void onMessage(const FIX44::Logon & message,
        const FIX::SessionID & sessionID) override
    EXCEPT(FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::InvalidMessage, , FIX::UnsupportedMessageType);
};
 
#endif
