CREATE TABLE [version_record] (
[version]    INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
[totalsize]  INTEGER NOT NULL,
[foldername] VARCHAR(4096) NOT NULL,
[backuptime] INTEGER NOT NULL
);

CREATE TABLE [version_backdir] (
[version] INTEGER NOT NULL,
[dirpath] VARCHAR(4096) NOT NULL,
[privacy] INTEGER NOT NULL,
[dirid]   INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT
);

CREATE TABLE [version_backfile] (
[version]    INTEGER NOT NULL,
[dirid]      INTEGER NOT NULL,
[filename]   VARCHAR(4096) NOT NULL,
[onlinename] VARCHAR(4096) default NULL,
[filetype]   INTEGER NOT NULL,
[mtime]      INTEGER NOT NULL,
[privacy]    INTEGER NOT NULL,
[action]     INTEGER NOT NULL 
);

CREATE TABLE [version_foldername] (
[version]     INTEGER NOT NULL PRIMARY KEY,
[foldername]  VARCHAR(4096) NOT NULL,
[backdate]    VARCHAR(128) NOT NULL
);