#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>
#include <QList>
#include <QDebug>

enum class MessageType{
    HEARTBEAT,
    DATA
};

class Message{
private:
    QString text, owner, destination;
    MessageType type;

public:
    Message(){};
    Message(QString text, QString owner, QString destination, MessageType type): text(text), owner(owner), destination(destination), type(type){};

    void setText(QString text){
        this->text = text;
    }

    QString getText(){
        return this->text;
    }

    void setOwner(QString owner){
        this->owner = owner;
    }

    QString getOwner(){
        return this->owner;
    }

    void setDestination(QString destination){
        this->destination = destination;
    }

    QString getDestination(){
        return this->destination;
    }

    void setMessage(MessageType type){
        this->type = type;
    }

    MessageType getType(){
        return this->type;
    }

    QString toString(){
        return text + "__" + owner + "__" + destination + "__" + QString::number(static_cast<int>(type));
    }

    static Message* decode(QString message){
        try{
            QList<QString> tokens = message.split("__");
            if(tokens.size() != 4){
                throw new std::runtime_error("Malformed message!");
            }else{
                return new Message(tokens.at(0), tokens.at(1), tokens.at(2), static_cast<MessageType>(tokens.at(3).toInt()));
            }
        }catch(std::exception e){
            qDebug() << e.what();
            throw new std::runtime_error("Malformed message!");
        }
    }
};

#endif // MESSAGE_H
