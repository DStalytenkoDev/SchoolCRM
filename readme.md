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
- The SchoolDatabaseApi project [DStalytenkoDev/SchoolDatabaseApi.git](https://github.com/DStalytenkoDev/SchoolDatabaseApi.git)
- Qt Framework

### Build guide
1. Clone repo
2. Open **SchoolCRM/CMakeLists.txt** by Qt creator
3. Do auto confiuration with Qt Creator and GCC or MSVC compiler
4. build

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
