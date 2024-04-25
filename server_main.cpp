#include <iostream>
#include <QCoreApplication>
#include "./server/server.hpp"
#include "./client/client.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	Server server;

	return a.exec();
}