#include "TestConnection.h"
#include "TestRole.h"
#include "TestPerson.h"
#include "TestClass.h"
#include "TestClassmate.h"
#include "TestStudent.h"
#include "TestSubject.h"
#include "TestStudentMark.h"
#include "TestClassSubjectsList.h"
#include "TestTeacherSubjectsList.h"

int main(int argc, char** args)
{
    QTest::qExec(new TestConnection, argc, args);
    QTest::qExec(new TestRole, argc, args);
    QTest::qExec(new TestPerson, argc, args);
    QTest::qExec(new TestClass, argc, args);
    QTest::qExec(new TestClassmate, argc, args);
    QTest::qExec(new TestStudent, argc, args);
    QTest::qExec(new TestSubject, argc, args);    
    QTest::qExec(new TestStudentMark, argc, args);
    QTest::qExec(new TestClassSubjectsList, argc, args);
    QTest::qExec(new TestTeacherSubjectsList, argc, args);

    return 0;
}
