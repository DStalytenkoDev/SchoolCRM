/*

Copyright (C) 2025 AUTHORS of SchooliesCave
SPDX-License-Identifier: GPL-3.0-only

This file is part of SchooliesCave.

SchooliesCave is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation, version 3
of the License only.

SchooliesCave is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with SchooliesCave.
If not, see <https://www.gnu.org/licenses/>.

*/


use [School]

create table [Roles]
(
	[id] int identity(1, 1) unique, -- key
	[name] nvarchar(100) not null
);

create table [Persons]
(
	[id] int identity(1, 1) unique, -- key
	[role_id] int not null, -- ref
	[first_name] nvarchar(100) not null,
	[second_name] nvarchar(100) not null,
	[birthday] int not null,
	
	foreign key (role_id) references Roles(id)     
);

create table [Classes]
(
	[id] int identity(1, 1) unique, -- key
	[name] nvarchar(100) not null,
);

create table [Classmates]
(
	[person_id] int, -- key, ref
	[class_id] int not null, -- ref

	unique (person_id),
	foreign key (person_id) references Persons(id),
	foreign key (class_id) references Classes(id)
);

create table [Students]
(
	[person_id] int, -- key, ref
	[grade_id] int not null, -- ref

	unique (person_id),
	foreign key (person_id) references Persons(id),
	foreign key (grade_id) references Classes(id) 
);

create table [Subjects]
(
	[id] int identity(1, 1) unique,
	[name] nvarchar(100) not null,
);

create table [TeachersSubjects]
(
	[person_id] int not null references Persons(id),
	[subject_id] int not null references Subjects(id),
);

create table [StudentMarks]
(
        [id] int identity(1, 1) unique,
        [teacher_id] int not null references Persons(id),
        [student_id] int not null references Persons(id),
        [subject_id] int not null references Subjects(id),
        [type] tinyint not null default 0,
        [mark] tinyint not null,
        [date] date not null,
);

create table [ClassesSubjects]
(
	[class_id] int not null references Classes(id),
	[subject_id] int not null references Subjects(id)
);



