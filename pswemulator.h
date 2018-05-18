#ifndef PSWEMULATOR_H
#define PSWEMULATOR_H
#include <QObject>
#include <QtNetwork>

class PswEmulator: public QObject {
Q_OBJECT
private:
    QUdpSocket* socket;

public:
    PswEmulator();

private:
    void sendAnswer(QHostAddress senderAddress, int port, int i);

private slots:
    void slotProcessDatagrams();
};

#endif // PSWEMULATOR_H
