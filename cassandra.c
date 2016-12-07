#include"cassandra.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


extern int yylex(); 
typetoken token;
boolean follow_token=false;
typetoken _lire_token(){ 
	if(follow_token){
	follow_token=false;//remise a zero du marqueur de surlecture
	return token;//renvoie le token déjà lu
	}
	else return (typetoken) yylex(); 
} 

//cql_type ::=  native_type | collection_type | user_defined_type | tuple_type | custom_type

boolean cql_type(){

				boolean check=false;

				if(native_type()){check=true;}
				else if(collection_type()){check=true;}
				else if(tuple_type()){check=true;}
				//else if(custom_type()){check=true;}
				//else if(user_defined_type()){check=true;}
				
				return  check;
}

//native_type ::=  ASCII| BIGINT | BLOB| BOOLEAN| COUNTER| DATE| DECIMAL| DOUBLE| FLOAT| INET| INT| SMALLINT| TEXT| TIME| TIMESTAMP| TIMEUUID| TINYINT| UUID| VARCHAR | VARINT

boolean native_type(){

		boolean native=false;

		if(token==ASCII){native=true;}
		else if(token==BIGINT){native=true;}
		else if(token==BLOB){native=true;}
		else if(token==BOOLEAN){native=true;}
		else if(token==COUNTER){native=true;}
		else if(token==DATE){native=true;}
		else if(token==DECIMAL){native=true;}
		else if(token==DOUBLE){native=true;}
		else if(token==FLOAT){native=true;}
		else if(token==INET){native=true;}
		else if(token==INT){native=true;}
		else if(token==SMALLINT){native=true;}
		else if(token==TEXT){native=true;}
		else if(token==TIME){native=true;}
		else if(token==TIMESTAMP){native=true;}
		else if(token==TIMEUUID){native=true;}
		else if(token==TINYINT){native=true;}
		else if(token==UUID){native=true;}
		else if(token==VARCHAR){native=true;}
		else if(token==VARINT){native=true;}
/* switch(token) {
case ASCII :
	return true;
	break;
case BIGINT:
	return true;
	break;
case BOOLEAN :
	return true;
	break;
case COUNTER :
	return true;
	break;
case DATE :
	return true;
	break;
case DECIMAL :
	return true;
	break;
case DOUBLE :
	return true;
	break;
case FLOAT :
	return true;
	break;
case INET :
	return true;
	break;
case INT :
	return true;
	break;
case SMALLINT :
	return true;
	break;
case TEXT :
	return true;
	break;
case TIME :
	return true;
	break;
case TIMESTAMP :
	return true;
	break;
case TIMEUUID :
	return true;
	break;
case TINYINT :
	return true;
	break;
case UUID :
	return true;
	break;
case VARCHAR :
	return true;
	break;
case VARINT :
	return true;
	break;
default : 
	return false;
	break;
}
*/
		
		return native;

}

//idf ::= quoted_idf | unquoted_idf
boolean IDF(){

		boolean idf=false;
		if(token==QUOTED_IDF){idf=true;}
		else if(token==UNQUOTED_IDF){idf=true;}
		return idf;
}

//constant ::=  string | integer | float | boolean | uuid | blob | NULL
boolean _const(){

	boolean con=false;
	if(token==STRING_TOKEN){con=true;}
	else if(token==INUMBER){con=true;}
	else if(token==DNUMBER){con=true;}
	else if(token==TRUE){con=true;}
	else if(token==FALSE){con=true;}
	else if(token==BLOB_TOKEN){con=true;}
	else if(token==UUID_TOKEN){con=true;}
	else if(token==HEX){con=true;}
	return con;
}

//term ::=  constant | literal | function_call | type_hint | bind_marker
boolean term(){

		boolean ter=false;
		if(_const()){ter=true;}
		else if(literal()){ter=true;}
		else if(function_call()){ter=true;}
		else if(type_hint()){ter=true;}
		else if(bind_marker()){ter=true;}
		
		return ter;
}

//literal ::=  collection_literal | udt_literal | tuple_literal

boolean literal(){

	boolean lit=false;
	if(collection_literal()){lit=true;}
	//else if(udt_literal()){lit=true;}
	else if(tuple_literal()){lit=true;}
	return lit;
}

//function_call ::=  identifier '(' [ term (',' term)* ] ')'
boolean function_call(){

		boolean fun=false;
		if(IDF()){  printf("idffonction");
					token=_lire_token();
					if(token==POPEN){
						//printf("(");
						token=_lire_token();
							if(term()){
								//printf("term");
								token=_lire_token();
								if(isterm()){
									fun=true;
								}
							}
					}
				}
		
		return fun;
}

//isterm ::= ,term | )
boolean isterm(){
	boolean is=false;
	if(token==VIRG){
		token=_lire_token();
		if(term()){
				token=_lire_token();
				is=isterm();
		}
	}
	else if(token==PCLOSE){is=true;}
  return is;
}

//type_hint ::=  '(' cql_type `)` term
boolean type_hint(){

		boolean hint=false;
		if(token==POPEN){
			token=_lire_token();
			if(cql_type()){
				token=_lire_token();
				if(token=PCLOSE){
					hint=true;
				}
			}
		}		
	    return hint;
}

//bind_marker   ::=  '?' | ':' identifier
boolean bind_marker(){

	boolean bind=false;
	if(token==QST){
		bind=true;
	}
	else{ if(token==TWOP){
		token=_lire_token();
		if(IDF()){
			bind=true;
		}}
	}
	return bind;
}

//collection_type ::=  MAP '<' cql_type ',' cql_type '>'| SET '<' cql_type '>'| LIST '<' cql_type '>'
boolean collection_type(){
	boolean col=false;
	if(token==MAP){
		token=_lire_token();
		if(token==LESSER){
			token=_lire_token();
			if(cql_type()){
				token=_lire_token();
				if(token==VIRG){
					token=_lire_token();
					if(cql_type()){
						token=_lire_token();
						if(token==BIGGER){
									col=true;
						}
					}
				}
			}
		}
	}

	
	else {if(token==SET){
		token=_lire_token();
		if(token==LESSER){
			token=_lire_token();
			if(cql_type()){
				token=_lire_token();
				if(token==BIGGER){
					col=true;
				}
			}
		}
	}

	else { if(token==LIST){
			token=_lire_token();
			if(token==LESSER){
			token=_lire_token();
				if(cql_type()){
				token=_lire_token();
					if(token==BIGGER){
					col=true;
					}
				}
			}
		  }
		}

   }
		return col;
}
// map_literal ::=  '{' [ term ':' term (',' term : term)* ] '}'
boolean map_literal(){
		boolean maplit=false;
		if(token==ACOLO){
			token=_lire_token();
			if(term()){
				token=_lire_token();
				if(token==TWOP){
					token=_lire_token();
					if(term()){
						token=_lire_token();
						if(islit()){
								maplit=true;	    
					    }
                    }
                }
            } 
        }
		return maplit;
}
//islit ::= ,term : term | } 
boolean islit(){

		boolean ismlit=false;
		if(token==VIRG){
			token=_lire_token();
			if(term()){
				token=_lire_token();
				if(token==TWOP){
					token=_lire_token();
					if(term()){
						token=_lire_token();
						ismlit=islit();
				
				    }
			    }
			}
		}

			
		else if(token==ACOLF){
			ismlit=true;
		}
		
		return ismlit;
}
//list_literal ::=  '[' [ term (',' term)* ] ']'
boolean list_literal(){
	boolean listlit=false;
	if(token==CROPEN){
		token=_lire_token();
		if(term()){
				token=_lire_token();
		        listlit=islist();
		}
	}		
	return listlit;
}
//islist ::= ,term | ] 
boolean islist(){
	boolean islis=false;
	if(token==VIRG){
		token=_lire_token();
		if(term()){
			token=_lire_token();
			islis=islist();
		}
	}
	else if(token==CRCLOSE){
		islis=true;
	}
	return islis;
}
//set_literal ::=  '{' [ term (',' term)* ] '}'
boolean set_literal(){
	boolean listlit=false;
	if(token==ACOLO){ printf("{");
		token=_lire_token();
		if(term()){ printf("term");
				token=_lire_token();
		        listlit=isset();
		}
	}		
	return listlit;
}
//islist ::= ,term | ] 
boolean isset(){
	boolean islis=false;
	if(token==VIRG){ printf(",");
		token=_lire_token();
		if(term()){printf("term\n");
			token=_lire_token();
			islis=isset();
		}
	}
	else if(token==ACOLF){printf("}\n");
		islis=true;
	}
	return islis;
}

//collection_literal ::=  map_literal | set_literal | list_literal | udt_literal
boolean  collection_literal(){
	boolean collit=false;
	typetoken c= mapset_literal();
	if(c==MAP_LIT){
		collit=true;
	}
	else if(c==SET_LIT)
		{printf("hani hna map");
		collit=true;
		}
	else if(c=UDT_LIT){collit=true;}
	else if(list_literal()){
		printf("hani hna");
		collit=true;
		}
	
	
	return collit;
}

typetoken mapset_literal(){
		typetoken mapset=FALSE;
		if(token==ACOLO){ printf("{");
			token=_lire_token();
			if(IDF()){
				token=_lire_token();
				if(token=TWOP){ printf(":\n");
					token=_lire_token();
					if(term()){printf("term\n");
						token=_lire_token();
							if(is_udt()) mapset=UDT_LIT;	
					}
				}
				else if(token==POPEN){token=_lire_token();
							if(term()){
								//printf("term");
								token=_lire_token();
								if(isterm()){
									token=_lire_token();
									if(token==TWOP){
										token=_lire_token();
										if(term()){printf("Term 2() ");
											token=_lire_token();
											if(islit()){printf("Hani f is MAP "); mapset=MAP_LIT;}
                						}	
              						}

									else if(token==VIRG){ 
											follow_token=true; 
											token=_lire_token();
											if(isset()){ printf("Hani f iss set"); mapset=SET_LIT;}
										}
									else if(token==ACOLF) mapset=SET_LIT;
                
								}
							}
					}

			}
			else if(term()){printf("Term 1() ");
				token=_lire_token();
				if(token==TWOP){
					token=_lire_token();
					if(term()){printf("Term 2() ");
						token=_lire_token();
						if(islit()){printf("Hni f is lit "); mapset=MAP_LIT;return mapset;}
                	}
               }

				else if(token==VIRG){ 
					follow_token=true; 
					token=_lire_token();
					if(isset()){ printf("Hani f iss set"); mapset=SET_LIT;return mapset;}
				
				else if(token==ACOLF) mapset=SET_LIT;
                
            	}
        	}
        }
		return mapset;
}



/*
//udt_literal ::=  '{' identifier ':' term is_udt

boolean udt_literal(){
	boolean udtl=false;
	if(IDF()){ printf("idf");
        		token=_lire_token();
        		if(token=TWOP){ printf(":\n");
					token=_lire_token();
					if(term()){printf("term\n");
						token=_lire_token();
							if(is_udt()) udtl=true;	
					}
				}
        	}
    return udtl;    	
}*/


//udt_literal ::=  '{' identifier ':' term is_udt

boolean udt_literal(){
	boolean udtl=false;
	typetoken b;
	b=mapset_literal();
	if(b==UDT_LIT) udtl=true;
    return udtl;    	
}

// is_udt::= ',' identifier ':' term  | '}'
boolean is_udt(){
	boolean isu=false;
	if(token==VIRG){
		token=_lire_token();
		if(IDF()){
			token=_lire_token();
			if(token==TWOP){
				token=_lire_token();
				if(term()){
					token=_lire_token();
					isu=is_udt();
				}
			}
		}
	}
	else if(token==ACOLF) {printf("}");isu=true;}
	return isu;
}

//tuple_type    ::=  TUPLE '<' cql_type ( ',' cql_type )* '>'
boolean tuple_type(){
	boolean tup=false;
	if(token==TUPLE){ 
		token=_lire_token();
		if(token==LESSER){
			token=_lire_token();
			if(cql_type()){ 
				token=_lire_token();
 				tup=tuple_type_aux();
			}
		}
	}
	return tup;
 }

//tuple_type_aux ::= ,cql_type | >
boolean tuple_type_aux(){
	boolean ttp=false;
	if(token==VIRG){
		token=_lire_token();
		if(cql_type()){
			token=_lire_token();
			ttp=tuple_type_aux();
		}
	}
	else if(token==BIGGER){
		ttp=true;
	}
	return ttp;
}

//tuple_literal ::=  '(' term ( ',' term )* ')'
boolean tuple_literal(){
  boolean tup=false;
  if(token==POPEN){ 
  	token=_lire_token();
  	if(term()){ 
  		token=_lire_token();
  		tup=tuple_literal_aux();
  	}
  }
  return tup;
}
//tuple_literal_aux ::= ,term | )
boolean tuple_literal_aux(){
	boolean ttp=false;
	if(token==VIRG){ 
		token=_lire_token();
		if(term()){ 
			token=_lire_token();
			ttp=tuple_literal_aux();
		}
	}
	else if(token==PCLOSE){ 
		ttp=true;
	}
	return ttp;
}


/*
ALTER ( KEYSPACE | SCHEMA ) keyspace_name
TM
WITH REPLICATION = map
| ( WITH DURABLE_WRITES = ( true | false ))
AND ( DURABLE_WRITES = ( true | false )) */

boolean alter_key_space(){

	if(token==KEYSPACE || token==SCHEMA){
		token=_lire_token();
		if(IDF()){
			token=_lire_token();
			if(token==WITH_REPLICATION){
				token=_lire_token();
				if(token==EQ){
					token=_lire_token();
					if(map_literal()){
					token=_lire_token();								
					if(token==AND){
					token=_lire_token();
					while(token==DURABLE_WRITES){
						token=_lire_token();
						if(token==EQ){
							token=_lire_token();
							if(token==TRUE || token==FALSE){
								token=_lire_token();
								if(token==PVIRG)
									return true;
								}
								
							}
						}
					}
					else if(token==PVIRG)
						return true;						
						}
					}


				}else if(token==WITH) {
					token=_lire_token();
					while(token==DURABLE_WRITES){
						token=_lire_token();
						if(token==EQ){
							token=_lire_token();
							if(token==TRUE || token==FALSE){
								token=_lire_token();
								if(token==PVIRG)
									return true;
								
								}
								
							}
						}
					
				}
		
			}
					
		}
	
return false;
}



// if_existing : IF NOT EXISTS
boolean if_existing(){
if(token==IF){
			token=_lire_token();
			if(token==NOT_EXISTS)
				return true;
}
return false;
}

/*
CREATE ( KEYSPACE | SCHEMA ) IF NOT EXISTS IDF
WITH REPLICATION = map_lit
AND DURABLE_WRITES = boolean */




boolean Keyspace_aux(){
		if(IDF()){
					token=_lire_token();
					if(token==WITH_REPLICATION){
						token=_lire_token();
						if(token==EQ){
							token=_lire_token();
							if(map_literal()){
								token=_lire_token();								
								if(token==AND){
									token=_lire_token();
									if(token==DURABLE_WRITES){
										token=_lire_token();
										if(token==EQ){
											token=_lire_token();
											if(token==TRUE || token==FALSE){
												token=_lire_token();
												if(token==PVIRG)
													return true;
												}
												
											}
										}
									}
									else if(token==PVIRG)
													return true;					
								}
							}


						}
		
					}
return false;
}

boolean create_key_space(){

	if(token==KEYSPACE || token==SCHEMA){
		token=_lire_token();
		if(if_existing()){
			token=_lire_token();
			return Keyspace_aux();
			
			}else return Keyspace_aux();		
		}
	
return false;
}



/*


partition_key is:

column_name
| ( column_name1
        , column_name2, column_name3 ... )
| ((column_name1*, column_name2*), column3*, column4* . . . )
      

column_name1 is the partition key.

column_name2, column_name3 ... are clustering columns.

column_name1*, column_name2* are partitioning keys.

column_name3*, column_name4* ... are clustering columns.

*/
// Partition_aux: IDF ',' Partition_aux | IDF ')'
boolean partition_key_aux(){
	if(IDF()){	
		token=_lire_token();
		if(token==VIRG){
			token=_lire_token();
			return partition_key_aux();
			}	
		if(token==PCLOSE)
		return true;
	}		
	

	return false;		
			
		
}
//Partition Key: IDF | '(' Partition_aux
boolean partition_key(){
	if(IDF()) 
		{return true;}
	if(token==POPEN){
		token=_lire_token();
		return partition_key_aux();
		}
return false;

}









//clustering_columns : Parition_key_aux

boolean clustering_columns(){
	if(IDF()){	
		token=_lire_token();
		if(token==VIRG){
			token=_lire_token();
			return clustering_columns();
			}	
	}		
	if(token==PCLOSE)
		return true;

	return false;			
}



//PRI_KEY_AUX : ',' clustering_columns 
boolean pri_key_aux(){
if(token==VIRG){
	token=_lire_token();
	return clustering_columns();
}
return false;
}


//PRI_KEY: '(' Partition_key ')' | '(' Partition_key PRI_KEY_AUX

boolean pri_key(){
	
	if(token==POPEN){
		token=_lire_token();
		if(partition_key()){
			token=_lire_token();
			if(token==PCLOSE)
				return true;
			else
				return pri_key_aux();		
		}		
	}
return false;
}




/*
column_definition is:

column_name cql_type
| column_name cql_type PRIMARY KEY
| PRIMARY KEY ( partition_key )
| column_name collection_type

*/



//column_def_aux: cql_type PRIMARY_KEY | collection_type




boolean column_def_aux(){
	if(cql_type()){		
		token=_lire_token();
		if(token==PRIMARY_KEY){
			return true;
		}
		if(token==PCLOSE ||token==VIRG){
			follow_token=true;	
			return true;
			}
	}
	if(collection_type())
	{	return true;	}
return false;
}


//column_def_part: IDF column_def_aux | PRIMARY_KEY PRI_KEY

boolean column_def_part(){

	if(IDF()){
		token=_lire_token();
		return column_def_aux();
		}
	if(token==PRIMARY_KEY)
		return pri_key();

return false;

}

//column_definition: '(' column_def_part ')' | '(' column_def_part ',' column_def_part   
boolean column_definition(){
	if(column_def_part()){
		token=_lire_token();
		if(token==PCLOSE)
		{
			follow_token=true;
			return true;
		}	
		if(token==VIRG){
			token=_lire_token();
			return column_definition();
		}
	
	}
return false;
}

//Keyspace_per_default: idf.idf|idf
boolean Keyspace_per_default(){

if(IDF()){
			token=_lire_token();
					if(token==POINT){
						//On a specifié un keyspace
						token=_lire_token();
						if(IDF())
							{
								return true;
							}
					}
					else 
					 {//keyspace par defaut
					 		follow_token=true;
							return true;}
	}
return false;
}

/*create_table_aux_1: keyspace_name.table_name 
( column_definition, column_definition, ...) */
boolean create_table_aux_1(){
if(Keyspace_per_default()){
							token=_lire_token();
							if(token==POPEN){
								token=_lire_token();
								if(column_definition())
									return true;
									
								
							}
	}
				
				return false;
}
/*
CREATE TABLE keyspace_name.table_name 
( column_definition, column_definition, ...)
WITH property AND property ...(pour l'instant j'ai pas fait cette ligne)

*/
//with_property_aux: AND with_property_aux | ;
//boolean




//with_property : with property with_property_aux | ;


boolean create_table(){

	if(token==TABLE){
		token=_lire_token();
			if(if_existing()){
				token=_lire_token();
				if(create_table_aux_1()){
					token=_lire_token();
					if(token==PCLOSE)
					{
						return true;
					}
				}
			}else  
				if(create_table_aux_1()){
				
					token=_lire_token();
					if(token==PCLOSE)
					{
						return true;
					}
				}
		}
	
	return false;
}

//create_table_keyspace : CREATE TABLE create_table() | CREATE KEYSPACE create_key_space()
boolean create_table_keyspace(){
	boolean response=false;
if(token==CREATE){
	token=_lire_token();
	if(token==TABLE)
		response=create_table();
	else if(token==KEYSPACE||token==SCHEMA){
		response=create_key_space();
	}

}
return response;
}

/*ALTER TABLE keyspace_name.table_name instruction


| ( WITH property AND property ... )

*/
//ALTER column_name TYPE cql_type
boolean alter_table_inst_alter(){
	if(token==ALTER)
	{	token=_lire_token();
		if(IDF()){
			token=_lire_token();
			if(token==TYPE){
				token=_lire_token();
				if(cql_type())
					return true;
			}
		}
	}
return false;
}
//ADD column_name cql_type

boolean alter_table_inst_add(){
		if(token==ADD)
	{	token=_lire_token();
		if(IDF()){
			token=_lire_token();
				if(cql_type())
					return true;
		}
	}
return false;
}

//DROP column_name 

boolean alter_table_inst_drop(){
		if(token==DROP)
	{	token=_lire_token();
		if(IDF())
					return true;
	}
return false;
}

//RENAME column_name TO column_name 
boolean alter_table_inst_rename(){
	if(token==RENAME)
	{	token=_lire_token();
		if(IDF()){
			token=_lire_token();
				if(token==TO_TOKEN){
					token=_lire_token();
					if(IDF())
						return true;
				}
		}
	}
return false;
}

/*instruction is:

ALTER column_name TYPE cql_type
| ( ADD column_name cql_type )
| ( DROP column_name )
| ( RENAME column_name TO column_name )
| ( WITH property AND property ... )
*/
boolean alter_table_instruction(){
	if( alter_table_inst_add())
		return true;
	if( alter_table_inst_alter())
		return true;
	if( alter_table_inst_rename())
		return true;
	if( alter_table_inst_drop())
		return true;
return false;
}

//ALTER TABLE keyspace_name.table_name instruction ;
boolean alter_table(){
	if(token==TABLE){
				token=_lire_token();
				if(Keyspace_per_default()){
					token=_lire_token();
					if(alter_table_instruction())
					{	token=_lire_token();
						if(token==PVIRG)
						return true;
					}
				}
			
}
return false;
}

boolean alter_table_keyspace(){
	boolean response=false;
if(token==ALTER){
	token=_lire_token();
	if(token==TABLE)
		response=alter_table();
	else if(token==KEYSPACE||token==SCHEMA){
		response=alter_key_space();
	}

}
return response;
}


/*//tuple_names_literal ::=  '(' column_name ( ',' column_name )* ')'
boolean _names()
{
  boolean tup=false;
  if(token==POPEN){printf("(\n" );
  	token=_lire_token();
  	if(IDF()){printf("col1\n");
  		token=_lire_token();
  		tup=tuple_names_aux();
  	}
  }
  return tup;
}

//tuple_names_aux ::= , idf | )
boolean tuple_names_aux(){
	boolean ttp=false;
	if(token==VIRG){ printf(",\n");
		token=_lire_token();
		if(IDF()){ printf("column2\n");
			token=_lire_token();
			ttp=tuple_names_aux();
		}
	}
	else if(token==PCLOSE){ printf(")\n");
		ttp=true;
	}
	return ttp;
}
*/

int main(){
	boolean d;
	token=_lire_token();
	d=alter_table_keyspace();
	if(d==true){printf("good\n");}
	else {printf("nope\n");}
	return 0;
}
