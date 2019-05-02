#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <vector>
#include <omnetpp.h>
#include "Packet_m.h"

using namespace omnetpp;

class TrustChainElement
{
public:
    int partnerId, partnerSeqNum, transactionValue; // values limited to +- 2147483647

    TrustChainElement(int partnerId, int partnerSeqNum, int transactionValue)
    {
        this->partnerId = partnerId;
        this->partnerSeqNum = partnerSeqNum;
        this->transactionValue = transactionValue;
    }
};

class LogDatabaseElement
{
public:
    int UserAId, UserASeqNum, UserBId, UserBSeqNum, transactionValue; // values limited to +- 2147483647

    LogDatabaseElement(int UserAId, int UserASeqNum, int UserBId, int UserBSeqNum, int transactionValue)
    {
        this->UserAId = UserAId;
        this->UserASeqNum = UserASeqNum;
        this->UserBId = UserBId;
        this->UserBSeqNum = UserBSeqNum;
        this->transactionValue = transactionValue;
    }
};

class App : public omnetpp::cSimpleModule
{
private:
    // configuration
    int myAddress;
    std::vector<int> destAddresses;
    cPar *sendIATime;
    cPar *packetLengthBytes;

    // state
    //cMessage *generatePacket;
    cMessage *timerThread;
    long pkCounter;
    long chainTotalValue;
    bool amIEvil;

    int tempBlockID;
    int tempPartnerSeqNum;
    int tempBlockTransaction;

    // signals
    simsignal_t endToEndDelaySignal;
    simsignal_t hopCountSignal;
    simsignal_t sourceAddressSignal;

    //TrustChain Storage
    std::vector<TrustChainElement> trustChain;
    std::vector<LogDatabaseElement> logDatabase;

public:
    App();
    virtual ~App();

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    //Utility functions
    virtual void threadFunction();
    virtual void receiveMessage(cMessage *msg);
    virtual int randomNodeAddressPicker();
    virtual void registerNewChainNode(int id, int seqNum, int value);
    virtual void calculateChainValue();

    //Messaging
    virtual void createTransactionMessage();
    virtual void createChainRequestMessage();
    virtual void createChainLogMessage();
    virtual void createAckMessage();
    virtual void createDisseminationMessage(int userXID, int userXSeqNum, int userYID, int userYSeqNum, int transactionValue);
    virtual void createBusyMessage(int destAddress);

    virtual bool verificationTransactionChain(Packet *pk);
    virtual void logTransactionChain(Packet *pk);
    virtual bool isAlreadyPresentInDb(LogDatabaseElement *element);
    virtual bool isNodeEvil();
};