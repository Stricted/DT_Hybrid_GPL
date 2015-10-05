CREATE TABLE [r_record] (
[totalsize]  INTEGER NOT NULL,
[foldername] VARCHAR(4096) UNIQUE NOT NULL,
[backuptime] INTEGER NOT NULL
);

CREATE TABLE [r_backdir] (
[dirpath] VARCHAR(4096) UNIQUE NOT NULL,
[privacy] INTEGER NOT NULL,
[dirid]   INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT
);

CREATE TABLE [r_backfile] (
[dirid]     INTEGER NOT NULL,
[filename]  VARCHAR(4096) NOT NULL,
[newname]   VARCHAR(4096) NOT NULL,
[filetype]  INTEGER NOT NULL,
[mtime]     INTEGER NOT NULL,
[privacy]   INTEGER NOT NULL  
);
