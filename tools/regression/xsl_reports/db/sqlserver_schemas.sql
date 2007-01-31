drop table [test-log]
go
drop table [test-run] 
go
CREATE TABLE [test-run] (
	[runner] [varchar] (32) NOT NULL ,
	[timestamp] [varchar] (32) NOT NULL ,
	[source] [varchar] (32) COLLATE SQL_Latin1_General_CP1_CI_AS NULL ,
	[tag] [varchar] (32) COLLATE SQL_Latin1_General_CP1_CI_AS NULL ,
	[platform] [varchar] (32),
	[run-type]         [varchar] (32) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[comment]          [text] NULL,
	[toolset]          varchar (32) not null
    PRIMARY KEY ( runner, timestamp )
    ) 
GO

go
CREATE TABLE [test-log] 
    (
    [runner] [varchar] (32) NOT NULL ,
	[timestamp] [varchar] (32) NOT NULL ,
	[test-name] [varchar] (64) COLLATE SQL_Latin1_General_CP1_CI_AS NULL ,
	[test-run] [varchar] (32) COLLATE SQL_Latin1_General_CP1_CI_AS NULL ,
	[test-type] [varchar] (16) COLLATE SQL_Latin1_General_CP1_CI_AS NULL ,
	[test-program] [varchar] (512) COLLATE SQL_Latin1_General_CP1_CI_AS NULL ,
	[target-directory] [varchar] (512) NOT NULL ,
    [library] varchar(64)
    PRIMARY KEY ( runner, timestamp, [target-directory] )
    ) 
GO

alter table [test-log]  add CONSTRAINT parent FOREIGN KEY ( runner, timestamp ) REFERENCES [test-run] ( runner, timestamp )

go
drop table [compile] 
go
create table [compile]
    (
    [runner]           [varchar](32) NOT NULL ,
	[target-directory] [varchar](512) NOT NULL ,
    [output]           [text],
	[timestamp]        [varchar](32) NULL ,
    result             [varchar](16)
    )    

drop table [link] 
go
create table [link]
    (
    [runner]           [varchar](32) NOT NULL ,
	[target-directory] [varchar](512) NOT NULL ,
    [output]           [text],
	[timestamp]        [varchar](32) NULL ,
    result             [varchar](16)
    )    

drop table [run] 
go
create table [run]
    (
    [runner]           [varchar](32) NOT NULL ,
	[target-directory] [varchar](512) NOT NULL ,
    [output]           [text],
	[timestamp]        [varchar](32) NULL ,
    result             [varchar](16)
    )    

drop table [note]
go
create table [note]
    (
    [runner]           [varchar](32) NOT NULL ,
	[target-directory] [varchar](512) NOT NULL ,
    [note]             [text]
    )


select * from [test-run]
select * from [test-log]
select * from run
select * from note