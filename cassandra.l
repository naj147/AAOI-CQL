%{
	#include<stdio.h>
	#include"cassandra.h"
	

%}

%option noyywrap
hex [0-9a-fA-F]*
unquoted_identifier [a-zA-Z][a-zA-Z0-9_]*
quoted_identifier ["][a-zA-Z][a-zA-Z0-9_]*(["]{2})*[a-zA-Z0-9_]*["]
unquoted_name      [a-zA-Z_0-9]{1,48}
quoted_name			["][a-zA-Z_0-9]{1,48}["]
num [0-9][0-9]*
float   -?[0-9]+(\.[0-9]*)?([eE][+-]?[0-9+])?
uuid hex{8}-hex{4}-hex{4}-hex{4}-hex{12}
blob     0[xX]{hex}+
string ['][^']*([']{2})*[^']*['] | ^(\$\$)([^][^(\$\$)][^])*(\$\$)$ 
%%
["/"]["/"][ ]*[^\n]*		{}
["\-"]["\-"][^\n]*		{}
["/"]["*"][^]*[\n]*[^]*["*"]["/"]   {}                                                                                     
"DELETE"			{ return DELETE;}
"from"				{return FROM;}
"ASC"				{return ASC;}
"COUNT"				{return COUNT;}
"DESC"				{return DESC;}
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
"GROUP BY"			{return GROUP;}
"where"				{return WHERE;}
"order by"			{return ORDER;}
"exists"	                { return EXISTS;}
"primary key"			{ return PRIMARY_KEY;}		
"boolean"			{ return BOOLEAN;}
"ALLOW FILTERING"	{return ALLOW;}
"LIMIT"				{return LIMIT;}
"PER PARTITION LIMIT"	{return PER;}
"counter"			{ return COUNTER;}
"DISTINCT"			{return DISTINCT;}
"JSON"				{return JSON;}
"AS"				{return AS;}
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
"in"			{return IN;}
"CAST"		{return CAST;}
"contains"		{return CONTAINS;}
"contains key"	{return CONT_KEY;}
"true"				{ return TRUE;}
"false"				{ return FALSE;}
{num}				{ return INUMBER;}

{float}				{ return DNUMBER;}
{uuid}				{ return UUID_TOKEN;}

{blob}				{ return BLOB_TOKEN;}
{unquoted_identifier}		{ return UNQUOTED_IDF;}
{quoted_identifier}		{return QUOTED_IDF;}
{quoted_name}			{return QUOTED_NAME;}
{unquoted_name}			{return UNQUOTED_NAME;}
{string}			{return STRING_TOKEN;}
":"				{ return TWOP;}
"?"					{return QST;}
"\'"				{ return APOST;}
","				{ return VIRG;}
";"				{ return PVIRG;}
"."				{return POINT;}
"*"				{return MULT;}
"{"				{ return ACOLO;}
"}"				{ return ACOLF;}
"("				{return POPEN;}
")"				{return PCLOSE;}
"["				{return CROPEN;}
"]"				{return CRCLOSE;}
"<"			        { return LESSER;}
">"			        { return BIGGER;}
"="				{return EQ;}
"<="			{return LESSEQ;}
">="			{return BIGEQ;}

"!="			{return NOEQ;}


[\n]				{return ENTER;}
[ \t\n]		        	/* caractère vide */
.				{return ERRORLEX;}

%%


