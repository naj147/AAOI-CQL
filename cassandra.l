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
{num}				{ return INUMBER;}

{float}				{ return DNUMBER;}
{uuid}				{ return UUID_TOKEN;}

{blob}				{ return BLOB_TOKEN;}
{unquoted_identifier}		{ return UNQUOTED_IDF;}
{quoted_identifier}		{return QUOTED_IDF;}
{string}			{return STRING_TOKEN;}
":"				{ return TWOP;}
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
">"			        { return BIGGER;}
[\n]				{return ENTER;}
[ \t\n]		        	/* caractère vide */
.				{return ERRORLEX;}

%%

