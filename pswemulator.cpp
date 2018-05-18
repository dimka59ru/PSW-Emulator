#include "pswemulator.h"
#include <QDebug>

const char REQUEST_FOR_SEARCH          = 0xe0;
const char RESPONSE_TO_SEARCH_REQUEST  = 0xe1;
const int PORT                         = 6123;
const int LENGTH_MESSAGE               = 444;

PswEmulator::PswEmulator() {
    socket = new QUdpSocket(this);
    socket->bind(PORT);
    connect(socket, SIGNAL(readyRead()), SLOT(slotProcessDatagrams()));
    qDebug() << "Waiting for requests from the search program.\n";
}

void PswEmulator::slotProcessDatagrams() {
    QByteArray datagram;
    QHostAddress senderAddress;
    quint16 senderPort;


    do {
        datagram.resize(socket->pendingDatagramSize());
        socket->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);
        char request = datagram.front();

        if (request == REQUEST_FOR_SEARCH) {
            //            qDebug() << "Received a request from " << senderAddress.toIPv4Address();
            qDebug() << "Received a request from " << QHostAddress(senderAddress.toIPv4Address()).toString();


            for (int i = 1; i < 2; i++)
                sendAnswer(senderAddress, PORT, i);
        }
    } while(socket->hasPendingDatagrams());
}

void PswEmulator::sendAnswer(QHostAddress senderAddress, int port, int i) {
    QByteArray datagram;


    datagram.fill(0, LENGTH_MESSAGE);
    datagram[0] = RESPONSE_TO_SEARCH_REQUEST;

    datagram[1] = i % 11; // Модель

    //ip
    datagram[2] = 192;
    datagram[3] = 168;
    datagram[4] = 0;
    datagram[5] = i;

    //mac
    datagram[6] = 0xc0;
    datagram[7] = 0x11;
    datagram[8] = 0xa6;
    datagram[9] = 0x00+i;
    datagram[10] = 0xff-i;
    datagram[11] = 0xff;

    //mask
    datagram[276] = 255;
    datagram[277] = 255;
    datagram[278] = 255;
    datagram[279] = 0;

    //firmware
    datagram[272] = i % 11;
    datagram[273] = 2;
    datagram[274] = 0;


    QByteArray description("Описание устройства");
    datagram.replace(12, description.size(), description);

    QByteArray location("Местоположение устройства");
    datagram.replace(140, location.size(), location);

    socket->writeDatagram(datagram, senderAddress, port);

    qDebug() << "Answer sent to" << QHostAddress(senderAddress.toIPv4Address()).toString() << "\n";
}



