%{
	#include<stdio.h>
	#include"cassandra.h"
	

%}

%option noyywrap
hex [0-9a-fA-F]*
unquoted_identifier [a-zA-Z][a-zA-Z0-9_]*
quoted_identifier ["][a-zA-Z][a-zA-Z0-9_]*(["]{2})*[a-zA-Z0-9_]*["]

num [0-9][0-9]*
float   -?[0-9]+(\.[0-9]*)?([eE][+-]?[0-9+])?
uuid hex{8}-hex{4}-hex{4}-hex{4}-hex{12}
blob     0[xX]{hex}+
string ['][^']*([']{2})*[^']*[']
%%
["/"]["/"][ ]*[^\n]*		{}
["\-"]["\-"][^\n]*		{}
["/"]["*"][^]*[\n]*[^]*["*"]["/"]   {}       
"ALTER"				{return ALTER;}                                                                                
"WITH REPLICATION"		{return WITH_REPLICATION;}
"WITH"				{return WITH;}
"DURABLE_WRITES"		{return DURABLE_WRITES;}                             
"WHERE"					{return WHERE;}
"FROM"					{return FROM;}                                        
"DELETE"			{ return DELETE;}
"CREATE"			{ return CREATE;}
"PUT"				{ return PUT;}
"DROP"				{ return DROP;}
"SELECT"			{return SELECT;}
"INSERT"			{ return INSERT;}
"INTO"				{ return INTO;}	
"UPDATE"			{ return UPDATE;}
"RENAME"			{ return RENAME;}
"KEYSPACE"			{ return KEYSPACE;}	
"SCHEMA"			{ return SCHEMA;}
"ALLOW FILTERING"	{return ALLOW;}
"LIMIT"				{return LIMIT;}
"PER PARTITION LIMIT"	{return PER;}
"DISTINCT"			{return DISTINCT;}
"TABLE"				{return TABLE;}
"not exists"			{ return NOT_EXISTS;}
"AND"				{ return AND;}
"ADD"				{return ADD;}
"values"			{ return VALUES;}		
"if"				{ return IF;}	
"exists"	                { return EXISTS;}
"primary key"			{ return PRIMARY_KEY;}		
"boolean"			{ return BOOLEAN;}
"counter"			{ return COUNTER;}
"date"		 	        { return DATE;}
"decimal"			{ return DECIMAL;}
"double"			{ return DOUBLE;}
"float"			        { return FLOAT;}
"inet"			        { return INET;}
"int"			        { return INT;}
"blob"					{return BLOB;}
"string"				 {return STRING;}
"smallint"			{ return SMALLINT;}
"text"			        { return TEXT;}					
"to"					{return TO_TOKEN;}
"time"			        { return TIME;}
"bigint"			{return BIGINT;}
"timestamp"			{ return TIMESTAMP;}
"timeuuid"			{ return TIMEUUID;}
"tinyint"			{ return TINYINT;}
"uuid"			        { return UUID;}
"varint" 			{ return VARINT;}
"varchar"			{ return VARCHAR;}
"set"				{return SET;}
"type" 				{ return TYPE;}
"list"				{ return LIST;}
"map"				{ return MAP;}
"tuple"				{ return TUPLE;}
"true"				{ return TRUE;}
"false"				{ return FALSE;}
"ttl"				{ return TTL;}
"using"				{ return USING;}
"KEY"				{return KEY;}	
"VIEW"				{return VIEW;}	
"ROLE"				{return ROLE;}
"USER"				{return USER;}
"USE"				{return USE;}
"FUNCTION"			{return FUNCTION;}
"MATERIALIZED"		{return MATERIALIZED;}
"AGGREGATE"			{return AGGREGATE;}
"contains"		{return CONTAINS;}
"contains key"	{return CONT_KEY;}
"group by"		{return GROUP;}
"order by"		{return ORDER;}
"token"			{return TOKEN;}
"JSON"				{return JSON;}/* lexical dial oussama*/
"AS"				{return AS;}  //14/12/2016
"CAST"				{ return CAST;} 
"COUNT"				{ return COUNT;}
"ASC"				{return ASC;}
"DESC"				{return DESC;}
{num}				{ return INUMBER;}
{float}				{ return DNUMBER;}
{uuid}				{ return UUID_TOKEN;}
{blob}				{ return BLOB_TOKEN;}
{unquoted_identifier}		{ return UNQUOTED_IDF;}
{quoted_identifier}		{return QUOTED_IDF;}
{string}			{return STRING_TOKEN;}
":"				{ return TWOP;}
"="				{return EQ;}
"?"					{return QST;}
"\'"				{ return APOST;}
","				{ return VIRG;}
";"				{ return PVIRG;}
"."				{return POINT;}
"{"				{ return ACOLO;}
"}"				{ return ACOLF;}
"("				{return POPEN;}
")"				{return PCLOSE;}
"["				{return CROPEN;}
"]"				{return CRCLOSE;}
"<"			        { return LESSER;}
"!="				{return DIFF;}
">="				{return BIGGEREQ;}
"<="				{return LESSEREQ;}
">"			        { return BIGGER;}
"*"				{ return MULT;}
"+"				{return PLUS;}
"-"				{return MINUS;}
[\n]				{return ENTER;}
[ \t\n]		        	/* caractère vide */
.				{return ERRORLEX;}

%%

