CREATE TABLE [ACTORS] (
[exID] INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL,
[name] VARCHAR(64)  UNIQUE NOT NULL
);

CREATE TABLE [ALBUMS] (
[exID] INTEGER  NOT NULL PRIMARY KEY AUTOINCREMENT,
[name] VARCHAR(64)  UNIQUE NOT NULL
);

CREATE TABLE [ARTISTS] (
[exID] INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL,
[name] VARCHAR(64)  UNIQUE NOT NULL
);

CREATE TABLE [AUTHORS] (
[exID] INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL,
[name] VARCHAR(64)  UNIQUE NOT NULL
);

CREATE TABLE [BOOKMARK] (
[id] INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL,
[parentID] INTEGER  NOT NULL,
[itemID] INTEGER  NOT NULL,
[devUDN] VARCHAR(256)  NULL,
[devUDNSrvType] VARCHAR(256)  NULL,
[devUDNSrvId] VARCHAR(256)  NULL,
[curState] TEXT  NULL,
[curStateName] VARCHAR(64)  NULL
);

CREATE TABLE [CREATORS] (
[exID] INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL,
[name] VARCHAR(64)  UNIQUE NOT NULL
);

CREATE TABLE [DIRECTORS] (
[exID] INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL,
[name] VARCHAR(64)  UNIQUE NOT NULL
);

CREATE TABLE [GENRES] (
[exID] INTEGER  NOT NULL PRIMARY KEY AUTOINCREMENT,
[name] VARCHAR(64)  UNIQUE NOT NULL
);

CREATE TABLE [ITEM] (
[id] INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL,
[parentID] INTEGER  NOT NULL,
[flags] INTEGER  NOT NULL,
[title] VARCHAR(256)  NULL,
[fileName] VARCHAR(4096)  NULL,
[size] INTEGER  NULL,
[date] DATE  NULL,
[bitsPerSample] INTEGER  NULL,
[duration] INTEGER  NULL,
[nrAudioChannels] INTEGER  NULL,
[resolution] INTEGER  NULL,
[sampleFrequency] INTEGER  NULL,
[bitrate] INTEGER  NULL,
[lastModify] TIME  NULL,
[originalTrackNumber] INTEGER  NULL,
[exID_ACTOR] INTEGER,
[exID_ALBUM] INTEGER,
[exID_ARTIST] INTEGER,
[exID_AUTHOR] INTEGER,
[exID_CREATOR] INTEGER,
[exID_DIRECTOR] INTEGER,
[exID_GENRE] INTEGER,
[exID_PRODUCER] INTEGER,
[exID_PUBLISHER] INTEGER
);

CREATE TABLE [ITEM_PLAYLIST] (
[id] INTEGER  NOT NULL,
[exID] INTEGER  NOT NULL
);

CREATE TABLE [PRODUCERS] (
[exID] INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL,
[name] VARCHAR(64)  UNIQUE NOT NULL
);

CREATE TABLE [PUBLISHERS] (
[exID] INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL,
[name] VARCHAR(64)  UNIQUE NOT NULL
);

CREATE INDEX [IDX_ITEM_] ON [ITEM](
[parentID]  ASC,
[fileName]  ASC
);

CREATE TRIGGER [DELETE_ITEM]
AFTER DELETE ON [ITEM]
FOR EACH ROW
BEGIN

delete from ACTORS
where ACTORS.exID = OLD.exID_ACTOR and not exists(
           select ITEM.id
           from  ITEM
           where ITEM.exID_ACTOR =OLD.exID_ACTOR );

delete from ALBUMS
where ALBUMS.exID = OLD.exID_ALBUM and not exists(
           select ITEM.id
           from  ITEM
           where ITEM.exID_ALBUM =OLD.exID_ALBUM );

delete from ARTISTS
where ARTISTS.exID = OLD.exID_ARTIST and not exists(
           select ITEM.id
           from  ITEM
           where ITEM.exID_ARTIST =OLD.exID_ARTIST );

delete from AUTHORS
where AUTHORS.exID = OLD.exID_AUTHOR and not exists(
           select ITEM.id
           from  ITEM
           where ITEM.exID_AUTHOR =OLD.exID_AUTHOR );

delete from CREATORS
where CREATORS.exID = OLD.exID_CREATOR and not exists(
           select ITEM.id
           from  ITEM
           where ITEM.exID_CREATOR =OLD.exID_CREATOR );

delete from DIRECTORS
where DIRECTORS.exID = OLD.exID_DIRECTOR and not exists(
           select ITEM.id
           from  ITEM
           where ITEM.exID_DIRECTOR =OLD.exID_DIRECTOR );

delete from GENRES
where GENRES.exID = OLD.exID_GENRE and not exists(
           select ITEM.id
           from  ITEM
           where ITEM.exID_GENRE =OLD.exID_GENRE );

delete from PRODUCERS
where PRODUCERS.exID = OLD.exID_PRODUCER and not exists(
           select ITEM.id
           from  ITEM
           where ITEM.exID_PRODUCER =OLD.exID_PRODUCER );

delete from PUBLISHERS
where PUBLISHERS.exID = OLD.exID_PUBLISHER and not exists(
           select ITEM.id
           from  ITEM
           where ITEM.exID_PUBLISHER =OLD.exID_PUBLISHER );

END;

CREATE VIEW [DLNA] AS
select ITEM.id as id, ITEM.parentID as parentID, ITEM.flags as flags, ITEM.title as title,
ITEM.size as size, ITEM.date as date, ITEM.bitsPerSample as bitsPerSample,
ITEM.bitrate as bitrate, ITEM.duration as duration, ITEM.nrAudioChannels as nrAudioChannels,
ITEM.sampleFrequency as sampleFrequency, ITEM.resolution as resolution,
ACTORS.name as actor, ALBUMS.name as album, ARTISTS.name as artist, AUTHORS.name as author,
CREATORS.name as creator, DIRECTORS.name as director, GENRES.name as genre,
PRODUCERS.name as producer, PUBLISHERS.name as publisher, ITEM.fileName as fileName,
ITEM.originalTrackNumber as originalTrackNumber,
ITEM.exID_ACTOR as actor_id,
ITEM.exID_ALBUM as album_id,
ITEM.exID_ARTIST as artist_id,
ITEM.exID_AUTHOR as author_id,
ITEM.exID_CREATOR as creator_id,
ITEM.exID_DIRECTOR as director_id,
ITEM.exID_GENRE as genre_id,
ITEM.exID_PRODUCER as producer_id,
ITEM.exID_PUBLISHER as publisher_id
FROM ITEM 
LEFT JOIN ACTORS on ITEM.exID_ACTOR = ACTORS.exID
LEFT JOIN ALBUMS on ITEM.exID_ALBUM = ALBUMS.exID
LEFT JOIN ARTISTS on ITEM.exID_ARTIST = ARTISTS.exID
LEFT JOIN AUTHORS on ITEM.exID_AUTHOR = AUTHORS.exID
LEFT JOIN CREATORS on ITEM.exID_CREATOR = CREATORS.exID
LEFT JOIN DIRECTORS on ITEM.exID_DIRECTOR = DIRECTORS.exID
LEFT JOIN GENRES on ITEM.exID_GENRE = GENRES.exID
LEFT JOIN PRODUCERS on ITEM.exID_PRODUCER = PRODUCERS.exID
LEFT JOIN PUBLISHERS on ITEM.exID_PUBLISHER = PUBLISHERS.exID
