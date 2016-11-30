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
	else if(c==UDT_LIT){collit=true;}      //manque==
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
	else if(token==ACOLF) {isu=true;}
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
/***********************************************/

/*********** MN hna badi code dyali ismail_melili ;p *******************/
/******************** name *************/
boolean name(){
							
							boolean name=false;
							if(token==QUOTED_IDF){name=true;}
							else if(token==UNQUOTED_IDF){name=true;}
							
							return name;
							}	
/****************  drop_keyspace_statement ::=  DROP KEYSPACE [ IF EXISTS ] keyspace_name*************/
boolean drop_keyspace_statement(){
				boolean drk=false;
				
					if(token=KEYSPACE){
						token=_lire_token();
					if(token==IF){
						token=_lire_token();
						if(token==EXISTS){
							token=_lire_token();
							if(name()){
								token=_lire_token();
								if(token==PVIRG){
									drk=true;
										}
										}
										}	
						}
					else {if(name()){
							token=_lire_token();
								if(token==PVIRG){
									drk=true;
												}	
										}
					}
						
					}
				
				
return drk;		

}	
/************ table_name*******************/
boolean table_name(){
	boolean tab=false;
	if(name())
		{token=_lire_token();
			if(token==POINT)
				{token=_lire_token();
					if(name())
						{
							token=_lire_token();
							if(token==PVIRG)
								{tab=true;
								}
								
						}
						
				}
		if(token==PVIRG)
				{tab=true;
				
				}					
	}
	
	
return tab;
}
/************** drop_table_statement::=  DROP TABLE [ IF EXISTS ] table_name;******************/
boolean drop_table_statement(){
		boolean drtab=false;
		
				if(token==TABLE){
					token=_lire_token();
					if(token==IF){
						token=_lire_token();
						if(token==EXISTS){
							token=_lire_token();
							if(table_name())
							{	
										drtab=true;
								
						}
					}
					}
					else{
						if(table_name())
							{	
										drtab=true;
					}	
				}
				
		}
	
		

return drtab;		
}			
/**************** simple_selection ******************/
boolean simple_selection(){
	boolean s=false;
	if(token!=FROM){
	if(IDF())
		{token=_lire_token();
			if(token==FROM)
				{s=true;}
			
			if(token==VIRG)
					{
				token=_lire_token();
				if(IDF())
					{if(token==FROM)
						{s=true;}
						else{simple_selection();}
					}
					
					}
			
		
			if(token==CROPEN){
					token=_lire_token();
					if(term()){
						token=_lire_token();
						if(token==CRCLOSE)
							{token=_lire_token();
								if(token==FROM)
									{s=true;}
								else{simple_selection();}
							}
										
					}
						

			
		}
			if(token==POINT){
					token=_lire_token();
						if(name()){
							if(token==FROM)
									{s=true;}
							else{simple_selection();}	

							}							
											
					}
			
			
				
	

}
else{s=true;}
return s;
}
}
/******************* update_parametre *********************/
boolean update_parametre(){
	boolean para=false;
	if(token==TTL || token==TIMESTAMP)
	{
		token=_lire_token();
		if(token==INUMBER || bind_marker())
		{	para=true;
		}
		
	}
	
return para;	

}
/******************** using_delete ******************/
boolean using_delete(){
boolean us=false;
int  p=1;
if(token==USING){
	token=_lire_token();
	if(update_parametre())
	{
		while(p==1){
			token=_lire_token();
			if(token==AND){
				token=_lire_token();
				if(update_parametre()){
					token=_lire_token();
					if(token==WHERE)
					{p==0;us=true;

					}
					else{p=1;}
				}
				else{p=0;}
			}
			else if(token==WHERE){
				p=0;us=true;
			}
			else{p=0;}
		}

	}
	
}
	
return us;
}
/*******************operateur ********************/
boolean operateur(){
boolean op=false;
if(token==BIGGEREQ || token==LESSEREQ || token==DIFF || token==LESSER || token==BIGGER || token==CONTAINS || token==IN){
op=true;
}
if(token==CONTAINS){
	token=_lire_token();
		if(token==KEY){
			op=true;
		}
}
return op;

}
/************************ ( ',' column_name )*  ************************/
boolean iscol(){
	boolean i=false;
	if(token==VIRG){
		token=_lire_token();
			if(IDF()){
				token=_lire_token();
				if(token==PCLOSE){
					i=true;
								}
				else{iscol();}


}

}

return i;
}

/***************** drop_index_statement::=  DROP INDEX [ IF EXISTS ] index_name; ******************/
boolean drop_index_statement(){
	boolean drin=false;
	
		if(token==INDEX){
			token=_lire_token();
			if(token==IF){
			token=_lire_token();
			if(token==EXISTS){
				token=_lire_token();
			if(token==UNQUOTED_NAME){
				
				token=_lire_token();
					if(token==PVIRG)
						{	
							drin=true;}			
					}
				}

				}
				else{
						if(token==UNQUOTED_NAME){
				
						token=_lire_token();
							if(token==PVIRG)
						{	
							drin=true;}			
					}
				}				
			
		}		
		
	

return drin;	
}
/**************drop_materialized_view_statement ::=  DROP MATERIALIZED VIEW [ IF EXISTS ] view_name;****************/
boolean drop_materialized_view_statement(){
	boolean drv=false;
	
		if(token==MATERIALIZED){
			token=_lire_token();
			if(token==VIEW){
				token=_lire_token();
					if(token==IF){
						token=_lire_token();
						if(token==EXISTS){
							token=_lire_token();
					if(token==UNQUOTED_IDF){
						token=_lire_token();
							if(token==PVIRG){
								drv=true;
							}
							}
						}
					}
					else{
						if(token==UNQUOTED_IDF){
						token=_lire_token();
							if(token==PVIRG){
								drv=true;
							}
					}
					
			}
			
	}
	
}


return drv;
}
/****************** role_name ******************/
boolean role_name(){
	boolean rn=false;
	if(IDF() || token==STRING_TOKEN){
		rn=true;
	}
return rn;
}
/**************************  drop_role_statement ::=  DROP ROLE [ IF EXISTS ] role_name ;****************/

boolean drop_role_statement(){
boolean dr=false;

	if(token==ROLE){
		token=_lire_token();
		if(token==IF){
			token=_lire_token();
			if(token==EXISTS){
				token=_lire_token();
		if(role_name())
		{	token=_lire_token();
			if(token==PVIRG){
				dr=true;
			}
			}
		}
		}
		else{
			if(role_name())
		{	token=_lire_token();
			if(token==PVIRG){
				dr=true;
			}
		
	}
}
	
}


return dr;
}
/************* drop_user_statement::=  DROP USER [ IF EXISTS ] role_name ;*****************************/
boolean drop_user_statement(){
	boolean drus=false;
	
		if(token==USER){
			token=_lire_token();
			if(token==IF){
				token=_lire_token();
				if(token==EXISTS){
					token=_lire_token();
			if(role_name())
			{	token=_lire_token();
				if(token==PVIRG)
				{
					drus=true;
				}
				}
			}
		}
		else{
			if(role_name())
			{	
				token=_lire_token();
				if(token==PVIRG)
				{
					drus=true;
				}
				}
		}
		}
		
		
	
	
return drus;
}
/************** arguments_signature ****************/
boolean arguments_signature(){
	boolean arg=false;
	int p=1;
	if(cql_type()){
		token=_lire_token();
		while(p==1){
			if(token==VIRG){
			token=_lire_token();
			if(cql_type()){
				token=_lire_token();
				if(token==PCLOSE){
					arg=true;
					p=0;
				}
				else{p=1;}
			}
			else{p=0;}
		}
		else if(token==PCLOSE){
			arg=true;p=0;
		}
		else{p=0;}
	}
}

return arg;
}
/***************table_name_funct*************************/
boolean table_name_funct(){
	boolean tab=false;
	if(name())
		{token=_lire_token()
			if(token==POINT)
				{token=_lire_token();
					if(name())
						{
							token=_lire_token();
							if(token==POPEN){
								token=_lire_token();
							if(arguments_signature()){
								
									token=_lire_token();
									if(token==PVIRG){
									tab=true;
													}
												}
											}

					else{if(token==PVIRG)
						{printf(";\n");
							tab=true;
				
							}					
						
				}
							}

						}
	
				else{	if(token==POPEN){
								token=_lire_token();
							if(arguments_signature()){
								
									token=_lire_token();
									if(token==PVIRG){
									tab=true;
													}
												}
											}
										
							
									
			else{if(token==PVIRG)
						{printf(";\n");
							tab=true;
				
							}					
						
				}
				
	}
	
}	
return tab;
}

/***********************drop_function_statement::=  DROP FUNCTION [ IF EXISTS ] function_name [ '(' arguments_signature ')' ];*****************************/
boolean drop_function_statement(){
	boolean drfc=false;
	
		if(token==FUNCTION){
			token=_lire_token();
			if(token==IF){
				token=_lire_token();
				if(token==EXISTS){
					token=_lire_token();
			if(table_name_funct()){
				drfc=true;
				
						
					}
					
				}
				
			}
			else{
				if(table_name_funct()){
				drfc=true;
						
					}
			}
			
		}
		
	
	
	return drfc;
}
/***********************drop_aggregate_statement ::=  DROP AGGREGATE [ IF EXISTS ] function_name [ '(' arguments_signature ')' ];*****************************/
boolean drop_aggregate_statement(){
	boolean dragg=false;
	
		if(token==AGGREGATE){
			token=_lire_token();
			if(token==IF){
				token=_lire_token();
				if(token==EXISTS){
					token=_lire_token();
					if(table_name_funct()){
				
								dragg=true;
							}
							}
						}
					
			else{
				if(table_name_funct()){
				
								dragg=true;
							}
							}
						}
						
						
					

	
	return dragg;
}
/******************* drop() ****************************/
boolean drop(){
	boolean drp=false;
	if(token==DROP){
		token=_lire_token();
		if(token==KEYSPACE) drp=drop_keyspace_statement();
		if(token==TABLE)	drp=drop_table_statement();
		if(token==INDEX)	drp=drop_index_statement();
		if(token==MATERIALIZED) drp=drop_materialized_view_statement();
		if(token==ROLE)		drp=drop_role_statement();
		if(token==USER)		drp=drop_user_statement();
		if(token==FUNCTION)	drp=drop_function_statement();
		if(token==AGGREGATE) drp=drop_aggregate_statement();	
		}
		return drp;
}

/*************************main *************************************************/
int main(){
	boolean d;
	token=_lire_token();
	d=drop();
	if(d==true){printf("good\n");}
	else {printf("nope\n");}
	return 0;
}