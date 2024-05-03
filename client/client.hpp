#include <QTcpSocket>
#include <QUdpSocket>
#include <QTimer>
#include "../payload/Message.h"

class Client : public QObject
{
	Q_OBJECT
public:
	Client(QObject *parent = nullptr);

private slots:
	void readDataFromServer();
	void onServerDisconnected();
    void onErrorOccured(QAbstractSocket::SocketError error);

private:
	QTcpSocket *tcpSocket = nullptr;
    QUdpSocket *udpSocket = nullptr;
    QTimer* heartbeatTimer;

	void sendDataToServer();
    void sendHeartbeat();
};
