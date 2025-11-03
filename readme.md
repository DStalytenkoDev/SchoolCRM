# SchoolCRM is GUI application to manage School database

The project serves the porpose of creating a GUI app for a Database API, thogh more or less  
it gets rid of the API and does not respresent it directly. At some points it combines  
certain entities into high level widgets.

## Its structure
The interface consists mostly of the left bar and main widget takes all of the right space.  
Main widgets called modules. They just swicth each other 'cause user interactions.

### The stuff is:
- List-like entities:
        - Persons
        - Roles
        - Subjects
- List-of-lists-like entities:
        - Subjects of a teacher
- The Classes
- The Journal

### Stack used
- Qt Framework 6.9.3
- The SchoolDatabaseApi (will remain as a part of this project as of now) [DStalytenkoDev/SchoolDatabaseApi.git](https://github.com/DStalytenkoDev/SchoolDatabaseApi.git)
- MariaDB as the database

### Build guide
1. Clone repo
2. Run `cmake -DBUILDTYPE=debug -P Init.cmake`, you can change `debug` to `release`
3. Run `cmake --build proj/build/MainCmakeConf`

> the Init.cmake initiates qt6 and builds it
> and creates the main projects's configuration
> then you still need to build the main project and rebuild it when-ever you need
===
> Unfortunately you can encounter some driver problems in the **QSQLDatabase** on linux yet so far.
> I will fix that as soon as possible, though if driver issues are not your problem than you good to go))
> just build the database right way, and then everything's simple.
----

### Platform support so far...
- Windows 10, 11 with MS SQL SERVER and QODBC driver [yes]
- Linux (driver issues are there)

#### Tip: You can try changing driver in the APIs files:
> in the file `SchoolApi_v2.0.0/SchoolApi/Connection.h` in the line **26**
> change `QSqlDatabase database = QSqlDatabase::addDatabase("QODBC");`
> And as an example here it is: `QSqlDatabase database = QSqlDatabase::addDatabase("Other driver...");`

### Contribute
- Any big or unfinished changes should go through  `dev`  branch
- When anything from  `dev`  seems working it should migrate to  `beta`  branch
- When  `beta`  becomes stable it migrates to  `master`  branch
- `master`  branch should be instantly affected only by major or minor updates or critical patches

> **Any push to  `master`  branch is implicit realese**  
> **`beta`  branch should be in one instance**  

> in file **/version_log** should be written the very last version for the branch  
> **tag** should be craeted by the minor update or by the contributor`s judgement  
> **tag** should not having a ***dev, beta or prod*** attributes in its name
