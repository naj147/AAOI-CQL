#include"cassandra.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


//extern int yylex(); 
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
				if(token==PCLOSE){
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
	else if(c==UDT_LIT){collit=true;}
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
  if(token==POPEN){printf("("); 
  	token=_lire_token();
  	if(term()){ printf("term");
  		token=_lire_token();
  		tup=tuple_literal_aux();
  	}
  }
  return tup;
}
//tuple_literal_aux ::= ,term | )
boolean tuple_literal_aux(){
	boolean ttp=false;
	if(token==VIRG){ printf(",\n");
		token=_lire_token();
		if(term()){ printf("t2\n");
			token=_lire_token();
			ttp=tuple_literal_aux();
		}
	}
	else if(token==PCLOSE){ printf(")\n");
		ttp=true;
	}
	return ttp;
}

/*
int main(){
	boolean d;
	token=_lire_token();
	d=literal();
	if(d==true){printf("good\n");}
	else {printf("nope\n");}
	return 0;
}*/
//names_values     ::=  names VALUES tuple_literal

boolean names_values()
{
boolean n=false;
if(_names()){ printf("namesvalid\n");
   token=_lire_token();
   if(token==VALUES){ printf("VALUES\n");
   	token=_lire_token();
   	if(tuple_literal()) n=true;}

   }
return n;   
}


//_names ::=  '(' column_name ( ',' column_name )* ')'
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

//tuple_literal_aux ::= ,term | )
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




//update_parameter ::=  ( TIMESTAMP | TTL ) ( integer | bind_marker )
boolean update_parameter()
{
	boolean up=false;
	if(token==TIMESTAMP||token==TTL){
		token=_lire_token();
		if(bind_marker()||token==INUMBER) up=true;
	}
	return up;
}



//update_parameter_aux::= ( AND update_parameter )* || ';'
boolean update_parameter_aux()
{
	boolean up=false;
	if(token==AND){
		token=_lire_token();
		if(update_parameter()) {token=_lire_token(); up=update_parameter_aux();}
	}
	else if(token==PVIRG){ follow_token=true;up=true;}
	return up;
}


/*   [ USING update_parameter ( AND update_parameter )* ] */
boolean _using_parameter()
{
	boolean up=false;
	if(token==USING){
		 token=_lire_token();
		 if(update_parameter())
		 	token=_lire_token();
		 	up=update_parameter_aux();
	}


}

/*insert_statement ::=  INSERT INTO table_name ( names_values | json_clause )
                      [ IF NOT EXISTS ]
                      [ USING update_parameter ( AND update_parameter )* ]
                      */

boolean insert_statement()
{
	boolean ins=false;
	if(token==INSERT){ printf("insert\n");
		token=_lire_token();
		if(token==INTO){printf("into\n");
			token=_lire_token();
			if (IDF())
			{
				token=_lire_token();
			
				if(names_values()){printf("columns\n");
				token=_lire_token();
				if(token==IF){
					token=_lire_token();
					if(token==NOT_EXISTS)
					{
						token=_lire_token();
						if(token==USING)
						{
							follow_token=true;
							token=_lire_token();
							ins=_using_parameter();
						}
						else if(token==PVIRG) ins=true;
					}
				}
				else if(token==USING)
						{
							follow_token=true;
							token=_lire_token();
							ins=_using_parameter();
						}
				else if(token==PVIRG||token==ENTER) {printf(";");ins=true;}

			}
		}
	}}
    return ins;
}

//INSERT
/*
insert_statement ::=  INSERT INTO table_name ( names_values | json_clause )
                      [ IF NOT EXISTS ]
                      [ USING update_parameter ( AND update_parameter )* ]
names_values     ::=  names VALUES tuple_literal
json_clause      ::=  JSON string [ DEFAULT ( NULL | UNSET ) ]
names            ::=  '(' column_name ( ',' column_name )* ')'
column_name :: = idf */


int main(){
	boolean d;
	token=_lire_token();
	d=insert_statement();
	if(d==true){printf("it works\n");}
	else{printf("it doesn't\n");}
}

