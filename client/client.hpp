#include <QTcpSocket>

class Client : public QObject
{
	Q_OBJECT
public:
	Client(QObject *parent = nullptr);

private slots:
	void readDataFromServer();
	void onServerDisconnected();

private:
	QTcpSocket *tcpSocket = nullptr;

	void sendDataToServer();
};
