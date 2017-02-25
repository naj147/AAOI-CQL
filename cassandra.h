

typedef enum {QUOTED_IDF,UNQUOTED_IDF,UNQUOTED_NAME,QUOTED_NAME,INDEX_NAME,VIEW,VIEW_NAME,MATERIALIZED,USE,USER,ROLE,FUNCTION,IN,CONTAINS,AGGREGATE,BIGGEREQ,LESSEREQ,DIFF,
CREATE, INSERT, UPDATE, DELETE, INTO, MULT, PLUS, DROP, ALTER, SET, WHERE,WITH,DURABLE_WRITES,WITH_REPLICATION, FROM,VALUES, IF, EXISTS,BATCH, BEG_IN, PUT,RENAME,KEYSPACE,SCHEMA,TABLE,NOT_EXISTS,AND, APPLY, EQ,ADD, MINUS,DIV,INDEX, ASCII,BIGINT,BLOB, COUNTER,DECIMAL,DOUBLE,FLOAT,INET,INT,TEXT,TIMEUUID,UUID,INUMBER, DNUMBER,STRING,BLOB_TOKEN,UUID_TOKEN,
VARCHAR,VARINT,LIST, MAP,SET_T,TUPLE,DESCRIBE,TYPE,TYPES,PRIMARY_KEY,ALL,KEY,POPEN,PCLOSE,BOPEN,BCLOSE,VIRG,PVIRG,LESSER,TTL,TIMESTAMP,
BIGGER,TWOP,ACOLO,ACOLF,CROPEN,CRCLOSE,ENTER,POINT,USING,QST,TRUE,APOST,SET_LIT,MAP_LIT,UDT_LIT,FALSE,TO_TOKEN ,SELECT,HEX,BOOLEAN,SMALLINT,DATE,ERRORLEX,TIME,TINYINT,STRING_TOKEN,
GROUP,ORDER,TOKEN,PER,LIMIT,CONT_KEY,ALLOW,/*TOKENS OUSSAMA 14/12/2016*/DISTINCT,JSON,AS,CAST,COUNT,ASC,DESC} typetoken;
typedef enum{SimpleStr,NTS}class_types;
typedef enum{false=0,true=1}boolean;

//THE NEW STRUCTURE

typedef struct ___map* _map;
typedef struct ___setlist* _setlist;
//typedef enum{MapLit,SetLit,ListLit,String,Integer,Blob,Bool,Float,Uuid}cqlTypes;

typedef struct data
{
typetoken type;
char * value;
struct data *next;
}table_data;

typedef struct _terms_types{
 union {
	_map Map;
	_setlist List;
	_setlist Set;
	table_data* data;
	}term;
 	typetoken vartype;
}terms_types;

typedef struct ___map{
	terms_types *key;
	terms_types *value;
	_map next;
}__map;
typedef struct ___setlist{
	terms_types * terms;
	_setlist  next;
}__setlist;





typedef struct prim{
	int partition;//1 : partition key ; 0 : clustering key
	char *name;
	struct prim* next;
}primary;

typedef struct table{

	char* name;
	terms_types* type;//char * is going to be replaced by the new type
	primary* primary;
	struct table* next;
}table_options;

typedef struct tab{
	char*  keyspace_name;
	char* name;
	table_options* fields;
	struct tab* next;
}tab_op;


typedef struct keyspace_options{
		char* name;
		typetoken durable_writes;
		boolean class_check;
		char* class;
		int replication_factor;
		int success;
		tab_op * tables;
}keyspace_options;







typetoken token;
extern int yylex();
boolean native_type();
boolean collection_type();
boolean tuple_type();
boolean custom_type();
boolean Keyspace_per_create();
boolean user_defined_type();
boolean cql_type();
boolean map_keyspace();
boolean IDF();
boolean _const();
boolean term();
boolean literal();
boolean  function_call();
boolean type_hint();
boolean  bind_marker();
boolean collection_literal();
boolean udt_literal();
boolean tuple_literal();
boolean isterm();
boolean cql_statement();
boolean statemet();
boolean ddl_statemet();
boolean dml_statemet();
boolean secondary_index_statement();
boolean materialized_view_statement();
boolean role_or_permission_statement();
boolean udf_statement();
boolean udt_statement();
boolean trigger_statement();
boolean use_statement();
boolean create_keyspace_statement();
boolean alter_keyspace_statement();
boolean drop_keyspace_statement();
boolean create_table_statement();
boolean alter_table_statement();
boolean drop_table_statement();
boolean truncate_statement();
boolean  select_statement();
boolean insert_statement();
boolean update_statement();
boolean delete_statement();
boolean batch_statement();
boolean create_index_statement();
boolean drop_index_statement();
boolean create_materialized_view_statement();
boolean drop_materialized_view_statement();
boolean create_role_statement();
boolean drop_role_statement();
boolean grant_role_statement();
boolean revoke_role_statement();
boolean list_roles_statement();
boolean grant_permission_statement();
boolean revoke_permission_statement();
boolean create_user_statement();
boolean alter_user_statement();
boolean drop_user_statement();
boolean list_users_statement();
boolean create_function_statement();
boolean drop_function_statement();
boolean create_aggregate_statement();
boolean drop_aggregate_statement();
boolean create_type_statement();
boolean alter_type_statement();
boolean map_literal();
boolean list_literal();
boolean set_literal();
boolean isset();
boolean islit();
boolean islist();
boolean udt_name();
boolean create_type_statement();
boolean alter_type_statement();
boolean alter_type_modification();
boolean name();
boolean is_field();
boolean field_definition();
boolean is_alt();
boolean is_udt();
boolean drop_type_statement();
//boolean tuple_names_aux();
boolean keyspace_name();
typetoken mapset_literal();
boolean tuple_type_aux();
boolean tuple_literal_aux();
boolean names_values();
boolean _names();
boolean tuple_names_aux();
boolean update_parameter();
boolean update_parameter_aux();
boolean _using_parameter();
boolean insert_statement();
boolean iscol();
boolean operateur();
//boolean using_delete();
boolean update_parametre();
boolean table_name_funct();
boolean relation();
boolean isrel();
boolean group_by();
boolean group_by_clause();
boolean where();
boolean where_clause();
boolean USIN();
int simple_selection();
boolean issel();
boolean isselec();
boolean selector();
boolean select_clause();
boolean isidf();
boolean allow();
boolean limit();
boolean partition();
boolean order_by();
boolean  type_imput();
boolean select_statement();
boolean column_name();
boolean table_name();
boolean name();
boolean IF_CLAUSE();
boolean assigement();
boolean isass();
boolean iscond();
boolean isupd();
boolean update_statement();
boolean  function_name();
boolean ascdesc();
boolean isorder();
boolean order_by_clause();
boolean iscolgr();
boolean operator();
void read_json(char *);
boolean addTable_Keyspace();
void write_table();
void create_json_keyspace();
void afficherhadchi(table_options * n);
char * type_interp_aux(typetoken tok);
typetoken char_to_token(char * tok);
char* type_interp(terms_types * t);
table_options *read_table(char * table_name, char *keyspace_name,table_options* champs);
int nbr_primary_key(table_options* p);
