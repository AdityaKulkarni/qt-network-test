#include "client.hpp"
#include <QNetworkDatagram>
#include <QHostAddress>

Client::Client(QObject *parent) : QObject(parent)
{
    tcpSocket = new QTcpSocket(this);
    udpSocket = new QUdpSocket(this);

    heartbeatTimer = new QTimer(this);
    connect(heartbeatTimer, &QTimer::timeout, this, &Client::sendHeartbeat);
    heartbeatTimer->start(1000);

    connect(tcpSocket, &QTcpSocket::readyRead, this, &Client::readDataFromServer);
	connect(tcpSocket, &QTcpSocket::connected, this, &Client::sendDataToServer);
	connect(tcpSocket, &QTcpSocket::disconnected, this, &Client::onServerDisconnected);
    connect(tcpSocket, &QTcpSocket::errorOccurred, this, &Client::onErrorOccured);


	tcpSocket->connectToHost("10.0.0.149", 1234);
    udpSocket->connectToHost("10.0.0.149", 1235);
}

void Client::readDataFromServer()
{
	QByteArray data = tcpSocket->readAll();
	qDebug() << "Received:" << data;
}

void Client::sendDataToServer()
{

    if (tcpSocket->state() == QTcpSocket::ConnectedState)
    {
        Message* message = new Message("Hello from client", tcpSocket->localAddress().toString(), tcpSocket->peerAddress().toString(), MessageType::DATA);
        tcpSocket->write(message->toString().toUtf8());
	}
	else
	{
		qDebug() << "Client is not connected to server.";
	}
}

void Client::sendHeartbeat(){
    Message* message = new Message("Heartbeat", udpSocket->localAddress().toString(), udpSocket->peerAddress().toString(), MessageType::HEARTBEAT);
    QNetworkDatagram nwDatagram(message->toString().toUtf8(), QHostAddress("10.0.0.149"), 1235);
    udpSocket->writeDatagram(nwDatagram);
}

void Client::onServerDisconnected()
{
	qDebug() << "Server has disconnected.";
	exit(-1);
}

void Client::onErrorOccured(QAbstractSocket::SocketError socketError){
    qDebug() << "Socket error occured: " << socketError;
}
