#include "server.hpp"

Server::Server(QObject *parent) : QObject(parent)
{
	tcpServer = new QTcpServer(this);

	// Start listening for incoming connections
	if (!tcpServer->listen(QHostAddress::Any, 1234))
	{
		qDebug() << "Server could not start: " << tcpServer->errorString();
	}
	else
	{
		qDebug() << "Server started!";
	}

	connect(tcpServer, &QTcpServer::newConnection, this, &Server::onClientConnected);
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
	QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
	if (socket)
	{
		clients.removeAll(socket);
		socket->deleteLater();
		qDebug() << "Client disconnected.";
	}
}

void Server::readClientData()
{
	QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
	if (socket)
	{
		QByteArray data = socket->readAll();
		qDebug() << "Data received: " << data;
		socket->write("Hello from server!");
		socket->flush();
		socket->waitForBytesWritten(3000);
	}
}
