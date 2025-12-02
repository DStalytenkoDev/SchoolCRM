# SchooliesCave is GUI application for School

**It manages School database and it aims to be a platform for teachers, students and their school life.**

The project serves the porpose of creating a GUI app for School. Also it uses its own API layer over the Sql.  
That API that is SchoolDatabaseApi now is a nearly solid part of the project. And will have lesser effect on the main project.

## What can you expect from SchooliesCave?

You can connect the app to an sql server. And **manage these kinds of data**:
- Roles (Positions of employees)
- The list of every person and some data associated with it
- The list of all subjects
- The lists of subjects associated with a particular teacher
- Ability to manage all grades, eg. create/delete grade, add/remove students, add/remove subjects
- The journal to manage students scores.
- **See the photo [assets](assets/MEDIA.md)**

## Stack used
- **Qt Framework 6.9.3** with no modifications as a linked git module under the **GPL-3.0-only**
- **MariaDB** is now the database for the project
- **LibreSSL 4.2.1** under the licenses that are OpenSSL License, Original SSLeay Licence and ICS License

## License

> The **SchooliesCave** is released under the **GNU General Public License, version 3 only**. (GPL-3.0-only)
> **Please see the LICENSE and pay attention to copyright and license notices in all project's files**
  
> **Qt Framework 6.9.3** under the GNU General Public License, version 3 only (GPL-3.0-only)
> All licenses of **Qt** can be found in it's linked module
  
> **LibreSSL** under the licenses that are OpenSSL License, Original SSLeay Licence and ICS License
> All licenses of **LibreSSL** can be found in it's linked module
>   
> This product includes software developed by the OpenSSL Project for  
> use in the OpenSSL Toolkit (http://www.openssl.org/)
>   
> This product includes cryptographic software written by Eric Young
> (eay@cryptsoft.com). This product includes software written by Tim
> Hudson (tjh@cryptsoft.com)

## Build guide

---

    git clone https://github.com/DStalytenkoDev/SchooliesCave.git
    
    mkdir SchooliesCave/build
    mkdir SchooliesCave/build/YourNameCmakeConfiguration
    cd SchooliesCave/build/YourNameCmakeConfiguration

    cmake ../../
    cmake --build .

> please aknowledge the `proj/cmake/UserConfig.cmake`
> There are some properties you can edit like:
>   
> - `USER_BUILD_TYPE` should be {release | debug}, it affects only dependencies built by the project
> - `QT6_FORCE_SYSTEM` should be {ON | OFF}, it forces the use of system installed Qt6
> - `QT6_FORCE_BUILD` should be {ON | OFF}, it forces the Qt6 to be build despite the one was built before
> - `SSL_FORCE_SYSTEM` should be {ON | OFF}, it forces the use of system installed LibreSSL 
> - `SSL_FORCE_BUILD` should be {ON | OFF}, it forces the LibreSSL to be build despite the one was built before

### The project should be **Qt Creator** compatible

However you might need to manage `*_FORCE_SYSTEM` or `*_FORCE_BUILD` properties.

You definitely need to set the `QT6_FORCE_SYSTEM` to `ON`

You might need to do one normal plain cmake configuration to let all dependecies to be build and installed  
in case your system does not have them installed.


## Depoyment notice

1. Install MariaDB as you wish, [MariaDB Docs](https://mariadb.com/docs/server)
2. Edit in **src/SchoolApi_v2.0.0/create_school_database.sql** the `use SchooliesCave;` to the name of the db you created
3. Run the content of the script in the admin panel to create the database

## Platform support

**The goal is Unix and Windows**

- Windows 10, 11 (should build, the new **Init.cmake** approach is not tested)
- Linux (should build, **tested**)

> **The project was dialed for MariaDB**

## Contribute rules
- Any big or unfinished changes should go through  `dev`  branch
- When anything from  `dev`  seems working it should migrate to  `beta`  branch
- When  `beta`  becomes stable it migrates to  `master`  branch
- `master`  branch should be instantly affected only by major or minor updates or critical patches

> **Any push to  `master`  branch is implicit realese**  
> **`beta`  branch should be in one instance**  

> in file **/version_log** should be written the very last version for the branch  
> **tag** should be craeted by the minor update or by the contributor`s judgement  
> **tag** should not having a ***dev, beta or prod*** attributes in its name
