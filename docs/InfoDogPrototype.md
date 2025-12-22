# SchooliesCave presents InfoDog
It should be a server that manages all data beetween the clients.  
It should be RESTfull like API, but it will what it will. At first it must  
meet the app demand, and then 'REST'

# Idea is:
The server should deliver the:
- data access control
- ownership management
- users management
- data API

I don`t wanna use only Qt Framework. I am not sure that the 100% dependency on Qt is a good thing.
It lets you think and understand less of the real world.

# Stack to be used:
- LibreSSL in order to have tls
- RapidXML XML parser
- libiconv for unicode

# Architecture

## Rules:

- All connections are tls.
- Everything after the handshake is XML formatted with no 'XML prolog'

- root user must have access only through localhost

- **AccessGroup** lets the user to have a paricular level of access
    - means that a certain person is added in a group eg. admin, student, homeroomTeacher
    - the number of groups is strictly provided by the architecture
    - they might be:
        - admin
        - teacher
        - stuff
        - student
- **Person**, is a person and user
    - has firstName, secondName, role, birthday, description, photo, password hash, username, accessGroup
    - must first be created as an incomplete and non-accessible profile and completion code for the user to complete profile with the username and password
- **Subject**, is subject to be studied
- **Teacher**, has subjects list, person's id
- **Class**
    - has name, homeroomTeacher(teacher's id), students, subjects

## Proto Define client request

    <root>
        <action>%ACTION</action>
        
        %REQPROPERTIES
    </root>

### Furthere see in the link [Action`s reference](InfoDogActions.md)

## Interaction rules
