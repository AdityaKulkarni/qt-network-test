#include <QTcpServer>
#include <QTcpSocket>

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
	QTcpServer *tcpServer = nullptr;
	QList<QTcpSocket *> clients;
};
