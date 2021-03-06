#include "server.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QDebug>
#include <string>

QT_USE_NAMESPACE

Server::Server(quint16 port, bool debug, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Server"),
                                            QWebSocketServer::NonSecureMode, this)),
    m_clients(),
    m_debug(debug)
{
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        if (m_debug)
            qDebug() << "Server listening on port" << port;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &Server::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &Server::closed);
    }
}

Server::~Server()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

void Server::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &Server::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &Server::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &Server::socketDisconnected);

    m_clients << pSocket;
}

void Server::processTextMessage(QString message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    QSqlQuery query("SELECT players.first_name, players.last_name, players.date_of_birth FROM players LIMIT 0,10" , db.myDatabase);
/*
 * Pseudo code (frontend):
 * recieve JSon object
 * present JSon object to the user
 *
 * TODO:
 *
 * Frontend:
 * Get a input more usable than a string,
 * maybe through a table (generated by a hardcoded query) made clickable through JQuery
 *
 * Backend:
 * If the Pseudo code above is complete it should be pretty much done.
 * Pretty up the code, fix the Databasemanager class, it is pretty useless atm.
*/
    QJsonObject myJsonObject;
    QString tArray[3] = {"players.first_name", "players.last_name", "players.date_of_birth "};

    QJsonArray myArray[3];

    while(query.next())
        for(int i=0 ; i<3 ; i++)
        {
            myArray[i].push_back(query.value(i).toString());
        }
    for(int i = 0 ; i<3 ; i++)
        myJsonObject.insert(tArray[i], myArray[i]);
    QJsonDocument myDoc(myJsonObject);
    if (pClient) {
        qDebug() << myDoc.toJson();
        pClient->sendTextMessage(myDoc.toJson());
    }
}

void Server::processBinaryMessage(QByteArray message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        qDebug() << "Binary Message received:" << message;
    if (pClient) {
        pClient->sendBinaryMessage(message);
    }
}

void Server::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        qDebug() << "socketDisconnected:" << pClient;
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

