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
- The SchoolDatabaseApi project [Git link](https://github.com/DStalytenkoDev/SchoolDatabaseApi.git)
- Qt Framework

### Build guide
1. Download repo
2. Download and build API's repo
2. Do auto confiuration with Qt Creator and GCC or MSVC compiler
3. In the project configuration setup environment varibales of API proj such as:
    - SCHOOLDATABASEAPILIBPATH for the binaries
    - SCHOOLDATABASEAPIHEADERSPATH for the headers
4. build

Note that binary variable includes the name of the bin file  
and the other one includes only root directory of API repo

**Should re-run cmake configuration after setting up variables.**

---

**Or just use some already compiled stuff for the release if it exists))**

## Releases
### v1.0.0
uses v1.0.0 of the API
