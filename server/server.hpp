#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QDateTime>
#include <QUdpSocket>
#include <QPair>
#include "../Node.h"
#include "../payload/Message.h"

struct HostAddressComparator {
    bool operator() (const QPair<QHostAddress, quint16>* lhs, const QPair<QHostAddress, quint16>* rhs) const {
        if (lhs->first.toString() < rhs->first.toString()) {
            return true;
        } else if (lhs->first.toString() == rhs->first.toString()) {
            return lhs->second < rhs->second;
        }
        return false;
    }
};

class Server : public QObject
{
	Q_OBJECT
public:
	Server(QObject *parent = nullptr);

private slots:
	void onClientConnected();
	void onClientDisconnected();
	void readClientData();

private:

    bool isMasterSelected = false;

	QTcpServer *tcpServer = nullptr;
    QUdpSocket *udpServer = nullptr;
	QList<QTcpSocket *> clients;
    std::map<QPair<QHostAddress, quint16>*, Node*, HostAddressComparator> lastHeartbeat;
    QTimer* heartbeatTimer;

    void checkHeartbeats();
    void readUdpHeartbeat();
};
