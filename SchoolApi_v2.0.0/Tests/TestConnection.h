#ifndef TESTCONNECTION_H
#define TESTCONNECTION_H


#include <QtTest>
#include <SchoolApi/Connection.h>


class TestConnection : public QObject
{
    Q_OBJECT

public:
    TestConnection();

private slots:
    void testData();
    void testTrustedConnection();
    void testSecureConnection();
    void testValid();
};


#endif // TESTCONNECTION_H
