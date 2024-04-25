#include "client.hpp"

Client::Client(QObject *parent) : QObject(parent)
{
	tcpSocket = new QTcpSocket(this);

	connect(tcpSocket, &QTcpSocket::readyRead, this, &Client::readDataFromServer);
	connect(tcpSocket, &QTcpSocket::connected, this, &Client::sendDataToServer);
	connect(tcpSocket, &QTcpSocket::disconnected, this, &Client::onServerDisconnected);

	tcpSocket->connectToHost("10.0.0.149", 1234);
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
		tcpSocket->write("Hello from client");
	}
	else
	{
		qDebug() << "Client is not connected to server.";
	}
}

void Client::onServerDisconnected()
{
	qDebug() << "Server has disconnected.";
	exit(-1);
}
