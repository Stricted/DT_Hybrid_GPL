CREATE TABLE [rsssubtitlelist] (
[id] INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL,
[channelID] INTEGER  NULL,
[pubdateTime] VARCHAR(64)  NULL,
[sqlDateTime] INTEGER  NULL,
[sublistTitle] VARCHAR(128)  NULL,
[sublistText] VARCHAR(512)  NULL,
[sublistUrl] VARCHAR(256)  NULL,
[readStatus] INTEGER  NULL
);

CREATE TABLE [rssgetfileinfo] (
[id] INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL,
[rssurl] VARCHAR(256)  NULL,
[lastgettime] INTEGER  NULL,
[sessionidlist] VARCHAR(256)  NULL
);

CREATE TABLE [emailsub](
[id] INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL,
[account] INTEGER NOT NULL,
[mailnumber] INTEGER NOT NULL,
[UID] VARCHAR(128)  NULL,
[MailDatetime] VARCHAR(64)      NULL,
[emailAddr] VARCHAR(64)  NULL,
[subject] NVARCHAR(128)  NULL,
[text] NVARCHAR(650)  NULL);

CREATE TABLE [emailstatus] (
[id] INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL,
[account] INTEGER  NULL,
[UID] VARCHAR(128)  NULL,
[readstatus] INTEGER  NULL
);