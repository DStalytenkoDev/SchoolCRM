#include "TestConnection.h"
#include "Config.h"

TestConnection::TestConnection() {}

void TestConnection::testData()
{
    dbapi::Connection connection("SERVER", "DATABASE");

    QCOMPARE(connection.serverName(), "SERVER");
    QCOMPARE(connection.databaseName(), "DATABASE");

    connection = dbapi::Connection("SERVER", "USER", "PASS",  "DATABASE");

    QCOMPARE(connection.userName(), "USER");
    QCOMPARE(connection.password(), "PASS");

    connection = dbapi::Connection();

    connection.setServerName("SERVER");
    connection.setUserName("USER");
    connection.setPassword("PASS");
    connection.setDatabaseName("DATABASE");
    connection.setPort(9090);

    QCOMPARE(connection.serverName(), "SERVER");
    QCOMPARE(connection.databaseName(), "DATABASE");
    QCOMPARE(connection.userName(), "USER");
    QCOMPARE(connection.password(), "PASS");
    QCOMPARE(connection.port(), 9090);

    QCOMPARE(dbapi::Connection::defaultConnection(), nullptr);
    auto p_connection = new dbapi::Connection;

    p_connection->makeDefault();
    QCOMPARE(dbapi::Connection::defaultConnection(), p_connection);

    delete p_connection;
    QCOMPARE(dbapi::Connection::defaultConnection(), nullptr);
}

void TestConnection::testTrustedConnection()
{
    dbapi::Connection connection;
    connection.setDatabaseName(Config::DATABASE_NAME);
    connection.setServerName(Config::SERVER_NAME);
    connection.setTrustedConnection();

    QVERIFY(connection.open());

    connection.close();
}

void TestConnection::testSecureConnection()
{
    dbapi::Connection connection;
    connection.setDatabaseName(Config::DATABASE_NAME);
    connection.setServerName(Config::SERVER_NAME);
    connection.setUserName(Config::USERNAME);
    connection.setPassword(Config::PASSWORD);

    QVERIFY(connection.open());

    connection.close();
}

void TestConnection::testValid()
{
    dbapi::Connection connection;
    connection.setDatabaseName(Config::DATABASE_NAME);
    connection.setServerName(Config::SERVER_NAME);
    connection.setUserName(Config::USERNAME);
    connection.setPassword(Config::PASSWORD);

    connection.open();

    QVERIFY(connection.valid());
    QVERIFY((bool)connection);

    connection.close();

    QVERIFY(not connection.valid());
    QVERIFY(not (bool)connection);
}
