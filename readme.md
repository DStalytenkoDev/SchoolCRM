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
1. Clone repo
2. Open **SchoolCRM/CMakeLists.txt** by Qt creator
3. Do auto confiuration with Qt Creator and GCC or MSVC compiler
4. build

---

**Or just use some already compiled stuff for the release if it exists))**

## Releases
### v1.4.2-beta
uses v2.0.0 of the API  
bug fix  
new error system  
new connection manager  
models hold on all data work  
multi-selection in item based entities is deprecated
### v1.1.2
uses v2.0.0 of the API  
patched bugs
### v1.1.1
uses v2.0.0 of the API  
patched bugs  
changed configure and build method
### v1.0.0
uses v1.0.0 of the API
