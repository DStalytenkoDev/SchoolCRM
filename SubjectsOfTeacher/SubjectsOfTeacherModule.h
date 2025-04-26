#ifndef SUBJECTSOFTEACHERMODULE_H
#define SUBJECTSOFTEACHERMODULE_H

#include <QWidget>

namespace Ui {
class SubjectsOfTeacherModule;
}

class SubjectsOfTeacherModule : public QWidget
{
    Q_OBJECT

public:
    explicit SubjectsOfTeacherModule(QWidget *parent = nullptr);
    ~SubjectsOfTeacherModule();

private:
    Ui::SubjectsOfTeacherModule *ui;
};

#endif // SUBJECTSOFTEACHERMODULE_H
