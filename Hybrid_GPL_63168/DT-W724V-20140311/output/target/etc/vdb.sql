CREATE TABLE [mobilesetting] (
[id] INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL,
[mobileID] INTEGER  NULL,
[incomAll] INTEGER  NULL,
[outgoLine] INTEGER  NULL,
[onlinePhone] INTEGER  NULL,
[emailList] VARCHAR(64)  NULL,
[rssList] VARCHAR(64)  NULL,
[callWait] INTEGER  NULL,
[cfu] INTEGER  NULL,
[cfb] INTEGER  NULL,
[cfnr] INTEGER  NULL,
[hansetType] INTEGER  NULL
);

CREATE TABLE [lanmap] (
[id] INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL,
[name] VARCHAR(64)  NULL,
[mac] VARCHAR(32)  NULL,
[flag] INTEGER  NULL,
[setflag] INTEGER  NULL
);

CREATE TABLE [callrecord] (
[id] INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL,
[number] VARCHAR(64)  NULL,
[name] VARCHAR(64)  NULL,
[datetime] TIMESTAMP  NULL,
[linename] VARCHAR(64)  NULL,
[lineid] INTEGER  NULL,
[flags] INTEGER  NULL
);

CREATE TABLE [contact] (
[id] INTEGER  NOT NULL PRIMARY KEY AUTOINCREMENT,
[name] VARCHAR(256)  NULL,
[fstname] VARCHAR(256)  NULL,
[num1] VARCHAR(64)  NULL,
[num1flag] CHAR  NULL,
[num2] VARCHAR(64)  NULL,
[num2flag] CHAR  NULL,
[num3] VARCHAR(64)  NULL,
[num3flag] CHAR  NULL,
[numreserved] VARCHAR(5)  NULL,
[melody] INTEGER  NULL,
[lineid] INTEGER  NULL
);

CREATE TABLE [dectsetting] (
[id] INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL,
[flags] INTEGER  NULL,
[pin] VARCHAR(64)  NULL,
[addr] VARCHAR(64)  NULL,
[netmask] VARCHAR(64)  NULL,
[gateway] VARCHAR(64)  NULL,
[dnsserver] VARCHAR(64)  NULL,
[firmwareVer] VARCHAR(64)  NULL,
[eepromVer] VARCHAR(64)  NULL,
[hardwareVer] VARCHAR(64)  NULL
);

CREATE TABLE [internalnames] (
[id] INTEGER  NOT NULL PRIMARY KEY AUTOINCREMENT,
[number] INTEGER  NULL,
[name] VARCHAR(64)  NULL
);

CREATE TABLE [linesetting] (
[id] INTEGER  NOT NULL PRIMARY KEY AUTOINCREMENT,
[name] VARCHAR(64)  NULL,
[lineid] INTEGER  NULL,
[usable] INTEGER  NULL,
[bitmap] INTEGER  NULL,
[dialprefix] VARCHAR(32)  NULL,
[melody] INTEGER  NULL,
[fpvolume] INTEGER  NULL,
[blocked] VARCHAR(252)  NULL,
[mulcallmode] INTEGER  NULL,
[intrucall] INTEGER  NULL,
[clirValue] INTEGER  NULL,
[clirActcode] VARCHAR(32)  NULL,
[clirDeActcode] VARCHAR(32)  NULL,
[cfuValue] INTEGER  NULL,
[cfuActcode] VARCHAR(32)  NULL,
[cfuDeActcode] VARCHAR(32)  NULL,
[cfuNum] VARCHAR(64)  NULL,
[cfnrValue] INTEGER  NULL,
[cfnrSec] INTEGER  NULL,
[cfnrActcode] VARCHAR(32)  NULL,
[cfnrDeActcode] VARCHAR(32)  NULL,
[cfnrNum] VARCHAR(64)  NULL,
[cfbValue] INTEGER  NULL,
[cfbActcode] VARCHAR(32)  NULL,
[cfbDeActcode] VARCHAR(32)  NULL,
[cfbNum] VARCHAR(64)  NULL
);

CREATE TABLE [onlinecontact] (
[id] INTEGER  NOT NULL PRIMARY KEY AUTOINCREMENT,
[name] VARCHAR(32)  NULL,
[benutzername] VARCHAR(256)  NULL,
[password] VARCHAR(256) NULL
);

CREATE TABLE [dectemailaddr] (
[id] INTEGER  NOT NULL PRIMARY KEY AUTOINCREMENT,
[enable] INTEGER  NULL,
[emailAccount] VARCHAR(32)  NULL,
[serverType] INTEGER  NULL,
[serverConnct] INTEGER  NULL,
[mailServerAddr] VARCHAR(256)  NULL,
[serverPort] INTEGER  NULL,
[userName] VARCHAR(256)  NULL,
[passWord] VARCHAR(512)  NULL
);

CREATE TABLE [dectemailinterval] (
[id] INTEGER  NOT NULL PRIMARY KEY AUTOINCREMENT,
[intervalId] INTEGER  NULL
);

CREATE TABLE [dectrssinfo] (
[id] INTEGER  NOT NULL PRIMARY KEY AUTOINCREMENT,
[instid] INTEGER  NULL,
[rssName] VARCHAR(64)  NULL,
[addrURL] VARCHAR(256)  NULL,
[rssInfo] INTEGER  NULL
);

CREATE TABLE [decterssinterval] (
[id] INTEGER  NOT NULL PRIMARY KEY AUTOINCREMENT,
[rssIntervalId] INTEGER  NULL
);
CREATE TABLE [calllist] (
[id] INTEGER  NOT NULL PRIMARY KEY AUTOINCREMENT,
[phoneNum] VARCHAR(32)  NULL
);
CREATE VIEW [allcall] AS 
select * from callrecord;

CREATE VIEW [inaccepcall] AS 
select * from callrecord where flags=0;

CREATE VIEW [missedcall] AS 
select * from callrecord where flags=2 or flags=3;

CREATE VIEW [outcall] AS 
select * from callrecord where flags=1;
