# InfoDog Action's reference

## ACTIONs:
- CREDENTIALS & SESSION
    - createSession
- USER
    - createPerson
    - completePerson
    - deletePerson
    - updatePerson
    - updateUsername
    - resetPassword
    - getAllPersons
- OWNERSHIP
    - createAdmin
    - deleteAdmin
    - getAllAdmins
- SUBJECT
    - createSubject
    - deleteSubject
    - getAllSubjects
- TEACHER
    - createTeacher
    - deleteTeacher
    - getAllTeachers
    - assignSubjectToTeacher
    - unassignSubjectFromTeacher
    - getSubjectsOfTeacher
- Class
    - createClass
    - deleteClass
    - updateHomeroomTeacher
    - getAllClasses
    - getClass
        - SUBJECT
            - assignSubjectToClass
            - unassignSubjectFromClass
            - getSubjectsOfClass
        - STUDENT
            - assignStudentToClass
            - unassignStudentFromClass
            - getStudentsOfClass

## Definitions of actions

### CREDENTIALS & SESSION

%ACTION: createSession  
%REQPROPERTIES:

    username:string
    password:string

### USER
%ACTION: createPerson  
%REQPROPERTIES:
    
    session:string

    firstName:string
    secondName:string
    role:string
    birthday:int
    description:text
    photo:jpg
 
%ACTION: completePerson  
%REQPROPERTIES:

    completionCode:string
    username:string
    password:string

%ACTION: deletePerson  
%REQPROPERTIES:

    session:string
    personId:string

%ACTION: updatePerson  
%REQPROPERTIES:

    session:string
    personId:int
    firstName:string
    secondName:string
    role:string
    birthday:int
    description:text
    photo:jpg

%ACTION: updateUsername  
%REQPROPERTIES:

    session:string
    username:string
    

%ACTION: resetPassword  
%REQPROPERTIES:  

    session:string
    password:string

%ACTION: getAllPersons  
%REQPROPERTIES:  

    session:string

### OWNERSHIP
%ACTION: createAdmin  
%REQPROPERTIES:

    session:string
    personId:int

%ACTION: deleteAdmin  
%REQPROPERTIES:

    session:string
    personId

%ACTION: getAllAdmins  
%REQPROPERTIES:

    session:string

### SUBJECT
%ACTION: createSubject  
%REQPROPERTIES:

    session:string
    name:string

%ACTION: deleteSubject  
%REQPROPERTIES:

    session:string
    subjectId:int

%ACTION: getAllSubjects  
%REQPROPERTIES:

    session:string

### TEACHER
%ACTION: createTeacher  
%REQPROPERTIES:

    session:string
    personId:int
    

%ACTION: deleteTeacher  
%REQPROPERTIES:

    session:string
    personId:int

%ACTION: getAllTeachers  
%REQPROPERTIES:

    session:string

%ACTION: assignSubjectToTeacher  
%REQPROPERTIES:

    session:string
    personId:int
    subjectId:int

%ACTION: unassignSubjectFromTeacher  
%REQPROPERTIES:

    session:string
    personId:int
    subjectId:int

%ACTION: getSubjectsOfTeacher  CREDENTIALS & SESSION
%REQPROPERTIES:

    session:string
    personId:int

### Class
%ACTION: createClass
%REQPROPERTIES:

    session:string
    name:string
    homeroomTeacher:personId

%ACTION: deleteClass
%REQPROPERTIES:

    session:string
    classId:int

%ACTION: updateHomeroomTeacher
%REQPROPERTIES:

    session:string
    classId:int
    homeroomTeacher:personId

%ACTION: getAllClasses
%REQPROPERTIES:

    session:string

%ACTION: getClass
%REQPROPERTIES:

    session:string
    classId:int

#### SUBJECT
%ACTION: assignSubjectToClass
%REQPROPERTIES:

    session:string
    classId:int
    subjectId:int

%ACTION: unassignSubjectFromClass
%REQPROPERTIES:

    session:string
    classId:int
    subjectId:int

%ACTION: getSubjectsOfClass
%REQPROPERTIES:

    session:string
    classId:int

#### STUDENT
%ACTION: assignStudentToClass
%REQPROPERTIES:

    session:string
    classId:int
    personId:int

%ACTION: unassignStudentFromClass
%REQPROPERTIES:

    session:string
    classId:int
    personId:int

%ACTION: getStudentsOfClass
%REQPROPERTIES:

    session:string
    classId:int
