#ifndef NODE_H
#define NODE_H

#include <QtCore>

class Node{
    bool isMaster = false, isActive;
    quint64 id;
    qint64 heartbeatTimestamp;

public:
    quint64 getId(){
        return id;
    }

    void setHertbeatTimestamp(qint64 timestamp){
        heartbeatTimestamp = timestamp;
    }

    qint64 getHeartbeatTimestamp(){
        return heartbeatTimestamp;
    }

    void setMaster(bool isMaster){
        this->isMaster = isMaster;
    }

    bool getIsMaster(){
        return isMaster;
    }

    void setActive(bool isActive){
        this->isActive = isActive;
    }

    bool getIsActive(){
        return isActive;
    }
};

#endif // NODE_H
