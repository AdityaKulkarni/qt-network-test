#include "server.hpp"

Server::Server(QObject *parent) : QObject(parent)
{
    tcpServer = new QTcpServer(this);
    udpServer = new QUdpSocket(this);

    // Start listening for incoming connections
    if (!tcpServer->listen(QHostAddress::Any, 1234))
    {
        qDebug() << "Server could not start: " << tcpServer->errorString();
    }
    else
    {
        qDebug() << "Server started!";
        connect(tcpServer, &QTcpServer::newConnection, this, &Server::onClientConnected);
    }

    // Setup for UDP server
    if (!udpServer->bind(QHostAddress("10.0.0.149"), 1235)) // Optionally check if bind is successful
    {
        qDebug() << "UDP Server could not bind: " << udpServer->errorString();
    }
    else
    {
        connect(udpServer, &QUdpSocket::readyRead, this, &Server::readUdpHeartbeat);
    }

    heartbeatTimer = new QTimer(this);
    heartbeatTimer->start(5000);
    connect(heartbeatTimer, &QTimer::timeout, this, &Server::checkHeartbeats);
}

void Server::onClientConnected()
{
	QTcpSocket *socket = tcpServer->nextPendingConnection();
	clients.append(socket);

	connect(socket, &QTcpSocket::disconnected, this, &Server::onClientDisconnected);
	connect(socket, &QTcpSocket::readyRead, this, &Server::readClientData);

	qDebug() << "New client connected: " << socket->peerAddress().toString();
}

void Server::onClientDisconnected()
{
    // QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    // if (socket)
    // {
    // 	clients.removeAll(socket);
    // 	socket->deleteLater();
    // 	qDebug() << "Client disconnected.";
    // }
}

void Server::readClientData()
{
	QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
	if (socket)
	{
		QByteArray data = socket->readAll();
        Message* decodedMessages = Message::decode(data);
        if(decodedMessages != nullptr && decodedMessages->getType() == MessageType::DATA){
            socket->write("Hello from server!");
            socket->flush();
            socket->waitForBytesWritten(3000);
        }
	}
}

void Server::checkHeartbeats() {
    qint64 now = QDateTime::currentSecsSinceEpoch();
    for(auto node : lastHeartbeat){
        if(now - node.second->getHeartbeatTimestamp() >  3){
            if(node.second->getIsActive()){
                qDebug() << "Node: " << node.first->first.toString() << " is inavtive";
                node.second->setActive(false);

                if(node.second->getIsMaster()){
                    isMasterSelected = false;
                }
            }
        }
    }
}

void Server::readUdpHeartbeat(){
    while (udpServer->hasPendingDatagrams()) {
        QByteArray datagram;
        QHostAddress senderAddress;
        quint16 senderPort;
        datagram.resize(int(udpServer->pendingDatagramSize()));
        udpServer->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);

        Message* decodedMessages = Message::decode(datagram);

        if(decodedMessages != nullptr && decodedMessages->getType() == MessageType::HEARTBEAT){
            qDebug() << "Heartbeat from: " << decodedMessages->getOwner();

            try{
                QPair<QHostAddress, quint16>* key = new QPair<QHostAddress, quint16>(senderAddress, senderPort);
                Node* node;

                if(lastHeartbeat.find(key) == lastHeartbeat.end()){
                    node = new Node();
                }else{
                    node = lastHeartbeat[key];
                }

                node->setHertbeatTimestamp(QDateTime::currentSecsSinceEpoch());
                if(!node->getIsActive()){
                    node->setActive(true);
                }

                if(!isMasterSelected){
                    qDebug() << "Selected new master: " << senderAddress.toString();
                    node->setMaster(true);
                    isMasterSelected = true;
                }

                lastHeartbeat[key] = node;
            }catch(std::exception e){
                qDebug() << e.what();
            }
        }
    }
}
