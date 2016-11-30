

typedef enum { QUOTED_IDF,UNQUOTED_IDF,UNQUOTED_NAME,QUOTED_NAME,TTL,USING,INDEX_NAME,MATERIALIZED,VIEW,VIEW_NAME,USER,ROLE,FUNCTION,IN,CONTAINS,AGGREGATE,BIGGEREQ,LESSEREQ,
CREATE, INSERT, UPDATE, DELETE, INTO, DROP, ALTER, SET, WHERE, FROM,VALUES, IF, EXISTS,BATCH, BEG_IN, PUT,RENAME,KEYSPACE,SCHEMA,NOT_EXISTS,AND, APPLY, EQ,ADD, MINUS, PLUS, MULT, DIV,INDEX, ASCII,BIGINT,BLOB, COUNTER,DECIMAL,DOUBLE,FLOAT,INET,INT,TEXT,TIMESTAMP,TIMEUUID,UUID,INUMBER, DNUMBER,STRING,BLOB_TOKEN,UUID_TOKEN,
VARCHAR,VARINT,LIST, MAP,SET_T,TUPLE,DESCRIBE,TYPE,TYPES,PRIMARY_KEY,ALL,POPEN,PCLOSE,BOPEN,BCLOSE,VIRG,PVIRG,LESSER,TABLE,KEY,DIFF,
BIGGER,TWOP,ACOLO,ACOLF,CROPEN,CRCLOSE,ENTER,POINT,QST,TRUE,APOST,SET_LIT,MAP_LIT,UDT_LIT,FALSE,TO_TOKEN ,SELECT,HEX,BOOLEAN,SMALLINT,DATE,ERRORLEX,TIME,TINYINT,STRING_TOKEN} typetoken; 
 
typedef enum{false=0,true=1}boolean;

boolean native_type();
boolean collection_type();
boolean tuple_type();
boolean custom_type();
boolean user_defined_type();
boolean cql_type();
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
boolean istuple();
boolean istuplit();
boolean tuple_names_aux();
boolean keyspace_name();
typetoken mapset_literal();
boolean tuple_type();
boolean tuple_type_aux();
boolean tuple_literal_aux();
boolean iscol();
boolean operateur();
boolean using_delete();
boolean update_parametre();
boolean simple_selection();
boolean table_name_funct();