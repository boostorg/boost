drop table [test-log]
go
CREATE TABLE [test-log] 
    (
    [runner] [varchar] (32) NOT NULL ,
	[timestamp] [varchar] (32) NOT NULL ,
	[test-name] [varchar] (64) COLLATE SQL_Latin1_General_CP1_CI_AS NULL ,
	[test-run] [varchar] (32) COLLATE SQL_Latin1_General_CP1_CI_AS NULL ,
	[test-type] [varchar] (16) COLLATE SQL_Latin1_General_CP1_CI_AS NULL ,
	[test-program] [varchar] (1024) COLLATE SQL_Latin1_General_CP1_CI_AS NULL ,
	[target-directory] [varchar] (1024) NOT NULL ,
    [library] varchar(64)
    PRIMARY KEY ( runner, timestamp, [target-directory] )
    ) 
GO

drop table [test-run] 
go
CREATE TABLE [test-run] (
	[runner] [varchar] (32) NOT NULL ,
	[timestamp] [varchar] (32) NOT NULL ,
	[source] [varchar] (32) COLLATE SQL_Latin1_General_CP1_CI_AS NULL ,
	[tag] [varchar] (32) COLLATE SQL_Latin1_General_CP1_CI_AS NULL ,
	[platform] [varchar] (32),
	[run-type] [varchar] (32) COLLATE SQL_Latin1_General_CP1_CI_AS NULL
    PRIMARY KEY ( runner, timestamp )
    ) 
GO

drop table [compile] 
go
create table [compile]
    (
    [runner] [varchar] (32) COLLATE SQL_Latin1_General_CP1_CI_AS NULL ,
	[target-directory] [varchar] (1024) COLLATE SQL_Latin1_General_CP1_CI_AS NULL ,
    [output] [text],
	[timestamp] [varchar] (32) NULL ,
    result [varchar] (16)
    
    )    


select * from [test-run]
select * from [test-log]
select * from compile