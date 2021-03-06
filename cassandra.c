#include "cassandra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <ctype.h>
#include <jansson.h>
#include "error.h"
#include "murmur3.h"
#include <stdint.h>

extern char* yytext;
char* prt;
int T[4]={-2147483648,0,1073741823,2147483647};
primary *pr=NULL;
table_options *used=NULL;
tab_op *tables=NULL;
table_options *current=NULL;
typetoken current_type=FALSE;
int insert_ordre=0;
FILE *file;
keyspace_options memtable;
char* text;
int cursor;
data_error* errs=NULL;
data_error* errors;
table_options *c;
primary*tr;
primary *ttt;
json_t *root;
json_t *data;
json_t *parser;
json_error_t error;
//extern int yylex(); d
boolean follow_token=false;
table_data *values;
table_data* remplir_values(table_data* v,char *value,typetoken tk){
table_data *p=NULL;
	if(v==NULL){
		v=(table_data*)malloc(sizeof(table_data));
		v->value=(char*)malloc(sizeof(char));
		v->type=tk;
		v->next=NULL;
		strcpy(v->value,value);
	}else{
		p=v;
		while(p->next)
			p=p->next;
		p->next=(table_data*)malloc(sizeof(table_data));
		p=p->next;
		p->value=(char*)malloc(sizeof(char));
		p->type=tk;
		p->next=NULL;
		strcpy(p->value,value);
	}
	return v;
}
//H :tableau hashé, T le tableau des valeurs max, k_H parcoureur du tableau H, l : longueur du tableau H
int which_file(int H[],int k_H,int l)
{
	//int k_H=0;
	while(k_H<l){
	for(int m=0;m<4;m++){
		if(H[k_H]<T[m]) return m+1;
		else if(H[k_H]==T[m]){ k_H++;
	 	 return which_file(H,k_H,l);}
	 	}
	}
return -1;
}
typetoken _lire_token(){
	if(follow_token){
	follow_token=false;//remise a zero du marqueur de surlecture
	return token;//renvoie le token déjà lu
	}
	else return (typetoken) yylex();
}
void write_insert(){
	//printf("FFFFFF");
	table_options *t=current;

	primary *p=NULL;
	json_t* row=json_object();
	char * value=(char*)malloc(100*sizeof(char));
	char * hexstring=(char*)malloc(100*sizeof(char));
	int i=0,k=0,j=0,l;

char * path;
	int hash_keys[31]={0};
	uint32_t hex[4];
	uint32_t seed = 42;
	char * filename=(char*)malloc(100*sizeof(char));
	uint32_t number;
	//writing all the
	while(t)
	{
		if(t->type->term.data->value){
				json_object_set(row,t->name,json_string(t->type->term.data->value));
				printf("my value is %s",t->type->term.data->value);
		}
		if(t->primary) {
			p=t->primary;
			//printf("%s\n and partition = %d",p->name,p->partition);
		}
		t=t->next;
	}
	//determine which file to write to by testing the partition keys
		//here we get the hashed values
	while(p!=NULL && p->partition==1 && j<31)
	{
			strcpy(value,json_string_value(json_object_get(row,p->name)));
			printf("\n%s",value);
		MurmurHash3_x64_128(value,sizeof(value),seed,hex);
	//	number = (int)strtol((const char *)hexstring[0],NULL,16);
	/*	sprintf(hexstring,"%lx",hex);
		printf("\n%s",hexstring);
		number = (long int)strtol(hexstring,NULL,16);*/
		sprintf(hexstring,"%08x%08x%08x%08x" ,hex[0],hex[1],hex[2],hex[3]);

			//printf("NUMBER : %d\n",number);
			number = strtol(hexstring,NULL,32);
		//	printf("\n%s",hexstring);
		hash_keys[j]=number;
		printf("\n%ld",number);
			j++;
			p=p->next;
	}
	//here we determine which file to write to
	int r=0;
		l=which_file(hash_keys,r,j);
		printf("\n%d\n",l);
	sprintf(filename,"table%d.txt",l);
	//printf("%s", memtable.name);
		path=(char*)malloc(sizeof(char)*1024);
		strcpy(path, memtable.name);
		strcat(path,"/");
		strcat(path,memtable.tables->name);
		strcat(path,"/");
		strcat(path,filename);
		printf("\n%s",path);

		json_t* rows=json_array();
		rows=json_load_file(path,0,&error);
		if(rows==NULL)
		printf("FUCK YOU AFAF\n");
		json_array_append(row,rows);
		json_dump_file(row,path,0);

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



//Navigate the type

terms_types * navigate_type(terms_types* type){
terms_types *utype;//the type in the next function call
	if(type!=NULL){
	switch(type->vartype){
		case MAP:
		case MAP_LIT:
			utype=type->term.Map->key;
			if(navigate_type(utype)==NULL || (navigate_type(utype)->vartype!=MAP && navigate_type(utype)->vartype!=MAP_LIT && navigate_type(utype)->vartype!=SET && navigate_type(utype)->vartype!=SET_LIT && navigate_type(utype)->vartype!=LIST))
				return type;
			else
				return navigate_type(type);
			break;
		case SET :
		case  SET_LIT :
			utype=type->term.Set->terms;
			if(navigate_type(utype)==NULL || (navigate_type(utype)->vartype!=MAP && navigate_type(utype)->vartype!=MAP_LIT && navigate_type(utype)->vartype!=SET && navigate_type(utype)->vartype!=SET_LIT && navigate_type(utype)->vartype!=LIST))
				return type;
			else
				return navigate_type(type);
			break;
		case LIST :
			utype=type->term.List->terms;
		if(navigate_type(utype)==NULL || (navigate_type(utype)->vartype!=MAP && navigate_type(utype)->vartype!=MAP_LIT && navigate_type(utype)->vartype!=SET && navigate_type(utype)->vartype!=SET_LIT && navigate_type(utype)->vartype!=LIST))
				return type;
			else
				return navigate_type(type);
			break;
		default :
			return type;
			break;
		}
	}
		return NULL;
}

void * remplire_type_aux(typetoken tok, terms_types* t_t){
	printf("Segmentation fault here REMPLIRE_AUX BEGIN\n");
				switch(t_t->vartype){
					case MAP :
					case MAP_LIT:
						if(t_t->term.Map->key==NULL){
							t_t->term.Map->key=(terms_types*)malloc(sizeof(terms_types));
							t_t->term.Map->key->vartype=tok;
						}else
						{
							if(t_t->term.Map->value==NULL){
							t_t->term.Map->value=(terms_types*)malloc(sizeof(terms_types));
							t_t->term.Map->value->vartype=tok;
							}
						}
						break;
					case SET :
					case  SET_LIT :
						t_t->term.Set->terms=(terms_types*)malloc(sizeof(terms_types));
						t_t->term.Set->terms->vartype=tok;
						break;
					case LIST:
						t_t->term.List->terms=(terms_types*)malloc(sizeof(terms_types));
						t_t->term.List->terms->vartype=tok;
						break;
					default :
						t_t->vartype=tok;
						t_t->term.data=NULL;
						break;
				}
				printf("Segmentation fault here REMPLIRE_AUX FIN\n");
}

//remplir type
void * remplire_type(typetoken tok, table_options * T){
	terms_types * t_t=NULL;
	table_options * p=T;
	printf("Segmentation fault here REMPLIRE BEGIN\n");
				switch(tok){
					case MAP :
					case MAP_LIT:
					if(p->type==NULL){
						p->type=(terms_types*)malloc(sizeof(terms_types));
						p->type->vartype=tok;
						p->type->term.Map=(_map)malloc(sizeof(__map));
						p->type->term.Map->key=NULL;
						p->type->term.Map->value=NULL;
					}else{
						t_t=navigate_type(p->type);//BS
						remplire_type_aux(tok,t_t);
					}
						break;
					case SET :
					case  SET_LIT :
					if(p->type==NULL){
						p->type=(terms_types*)malloc(sizeof(terms_types));
						p->type->vartype=tok;
						p->type->term.Set=(_setlist)malloc(sizeof(__setlist));
					}else{
						t_t=navigate_type(p->type);//BS
						remplire_type_aux(tok,t_t);

					}
						break;
					case LIST:
					if(p->type==NULL){
						p->type=(terms_types*)malloc(sizeof(terms_types));
						p->type->vartype=tok;
						p->type->term.List=(_setlist)malloc(sizeof(__setlist));
						}
					else{
						t_t=navigate_type(p->type);//BS
						remplire_type_aux(tok,t_t);
					}
						break;
					default :
					if(p->type==NULL){
						p->type=(terms_types*)malloc(sizeof(terms_types));
						p->type->vartype=tok;
						p->type->term.data=NULL;
						}else{
							printf("Noooo\n");
						t_t=navigate_type(p->type);

						remplire_type_aux(tok,t_t);
						}
						break;
				}
				printf("Segmentation fault here REMPLIRE FINAL\n");
				//printf("name : %s and type : %s  ",p->name,type_interp_aux(p->type->vartype));
}

//THE PROBLEM IS NESTING MAPS
//native_type ::=  ASCII| BIGINT | BLOB| BOOLEAN| COUNTER| DATE| DECIMAL| DOUBLE| FLOAT| INET| INT| SMALLINT| TEXT| TIME| TIMESTAMP| TIMEUUID| TINYINT| UUID| VARCHAR | VARINT

boolean native_type(){
	printf("Segmentation fault here NATIVE TYPE BEGIN\n");
		table_options * p=tables->fields;
		boolean native=false;
		if(token==ASCII){native=true;}
		else if(token==BIGINT){native=true;}
		else if(token==BLOB){native=true;}//binary
		else if(token==BOOLEAN){native=true;}
		else if(token==COUNTER){native=true;}
		else if(token==DATE){native=true;}
		else if(token==DECIMAL){native=true;}
		else if(token==DOUBLE){native=true;}
		else if(token==FLOAT){native=true;}
		else if(token==INET){native=true;}//ignore
		else if(token==INT){native=true;}
		else if(token==SMALLINT){native=true;}
		else if(token==TEXT){native=true;}
		else if(token==TIME){native=true;}//ignore
		else if(token==TIMESTAMP){native=true;}//ignore
		else if(token==TIMEUUID){native=true;}//ignore
		else if(token==TINYINT){native=true;}
		else if(token==UUID){native=true;}//8 4 4 12
		else if(token==VARCHAR){native=true;}
		else if(token==VARINT){native=true;}
		if(native==true){
		while(p->next){
				p=p->next;
		}
		remplire_type(token,p);
		/*	if(p->type==NULL){
				p->type=(terms_types*)malloc(sizeof(terms_types));
				p->type->vartype=token;
				p->type->term.data=NULL;
		}else{
				t_t=navigate_type(p->type);
				switch(t_t->vartype){
					case MAP :
					case MAP_LIT:
						if(t_t->term.Map->key==NULL){
							t_t->term.Map->key=(terms_types*)malloc(sizeof(terms_types));
							t_t->term.Map->key->vartype=token;
							t_t->term.Map->key->term.data=NULL;
						}else
						{
							if(t_t->term.Map->value==NULL){
							t_t->term.Map->value=(terms_types*)malloc(sizeof(terms_types));
							t_t->term.Map->value->vartype=token;
							t_t->term.Map->value->term.data=NULL;
							}
						}
						break;
					case SET :
					case  SET_LIT :
						t_t->term.Set->terms=(terms_types*)malloc(sizeof(terms_types));
						t_t->term.Set->terms->vartype=token;
						t_t->term.Set->terms->term.data=NULL;
						break;
					case LIST:
						t_t->term.List->terms=(terms_types*)malloc(sizeof(terms_types));
						t_t->term.List->terms->vartype=token;
						t_t->term.List->terms->term.data=NULL;
						break;
					default :
						t_t->vartype=token;
						break;

				}
		}*/
		}
		printf("Segmentation fault here NATIVE TYPE FINISH\n");
		return native;

}

//idf ::= quoted_idf | unquoted_idf
boolean IDF(){
		boolean idf=false;
		if(token==QUOTED_IDF){
			idf=true;
		}
		else if(token==UNQUOTED_IDF){
			idf=true;
		}
		if(idf==true){
			printf("its true wtf");
		}
		if(idf==false){
			printf("it's false wth");
		}
		return idf;
}
//VERIFY LE TOKEN TOK DES DONNEES ET SON COMPATIBLITER AVEC LE TOKEN DU TYPE DU VARIABLE
boolean type_interpreter(typetoken tok,typetoken type){
	switch(tok){
		case INUMBER:
			if(type==INT||type==SMALLINT||type==BIGINT||type==DECIMAL||type==TINYINT)
				return true;
			break;
		case DNUMBER :
			if(type==FLOAT||type==DOUBLE)
				return true;
			break;
		case TRUE :
		case FALSE:
			if(type==BOOLEAN)
				return true;
			break;
		case HEX:
		case UUID_TOKEN:
		case STRING_TOKEN:
			if(type==TEXT||type==VARCHAR)
				return true;
			break;
		case BLOB:
			if(type==BLOB)
				return true;
			break;
		default :
			return false;
			break;
	}
	return false;
}

//INT ordre signifie dans quelle variable va t'on ecrire la valeur car on peut avoir insert values({first tuple of values},{second tuple of values}...)
boolean copy_data(typetoken tok,int ordre,char *dt){
	terms_types * t_t=NULL;
	table_options * p=current;
	char * data=(char*)malloc(sizeof(char));
	strcpy(data,dt);
	int i=ordre;
	_map mp;
	_setlist setlist;
	printf("\n\n\nORDRE IS %d",i);
	while(p && i>1){
		i--;
		p=p->next;
	}

	t_t=navigate_type(p->type);

				switch(t_t->vartype){
					case MAP :
					case MAP_LIT:

							mp=t_t->term.Map;
							while(mp->next)
								mp=mp->next;

								//t_t->term.Map->key->vartype est toujours remplit d'apres les regles qu'on a etablit en haut
							if(type_interpreter(tok,t_t->term.Map->key->vartype)==true){
								printf("I ENTER KEY\n");

								if(mp->key==NULL){
									printf("I ENTRER1 \n");
									mp->key=(terms_types*)malloc(sizeof(terms_types));
									mp->key->vartype=t_t->term.Map->key->vartype;
									mp->key->term.data=(table_data*)malloc(sizeof(table_data));
									mp->key->term.data->value=(char*)malloc(sizeof(char)*100);
									strcpy(mp->key->term.data->value,data);
									mp->key->term.data->next=NULL;

									return true;
								}else{
									if(mp->key->term.data==NULL){
										printf("I ENTRER2 \n");
										mp->key->term.data=(table_data*)malloc(sizeof(table_data));
										mp->key->term.data->value=(char*)malloc(sizeof(char));
										strcpy(mp->key->term.data->value,data);
										mp->value->term.data->next=NULL;


										}
									else{
										if(mp->value==NULL){
											printf("I ENTRER3 \n");
											mp->value=(terms_types*)malloc(sizeof(terms_types));
											mp->value->vartype=t_t->term.Map->key->vartype;
											mp->value->term.data=(table_data*)malloc(sizeof(table_data));
											mp->value->term.data->value=(char*)malloc(sizeof(char));
											strcpy(mp->value->term.data->value,data);
											mp->value->term.data->next=NULL;

											return true;
										}else
										{	if(mp->value->term.data)
											{
												printf("I ENTRER4 \n");
												mp->value->term.data=(table_data*)malloc(sizeof(table_data));
												mp->value->term.data->value=(char*)malloc(sizeof(char));
												strcpy(mp->value->term.data->value,data);
												mp->value->term.data->next=NULL;

												return true;
											}else
											{
												printf("I ENTRER5 \n");
												mp->next=(_map)malloc(sizeof(__map));
												mp=mp->next;
												mp->key=(terms_types*)malloc(sizeof(terms_types));
												mp->key->vartype=t_t->term.Map->key->vartype;
												mp->key->term.data=(table_data*)malloc(sizeof(table_data));
												mp->key->term.data->value=(char*)malloc(sizeof(char));
												strcpy(mp->key->term.data->value,data);
												mp->key->term.data->next=NULL;

												return true;
											}
										}
									}
								}
							}else if(type_interpreter(tok,t_t->term.Map->value->vartype)==true){
								if(mp->value==NULL){
											printf("I ENTRER6 \n");
											mp->value=(terms_types*)malloc(sizeof(terms_types));
											mp->value->vartype=t_t->term.Map->key->vartype;
											mp->value->term.data=(table_data*)malloc(sizeof(table_data));
											mp->value->term.data->value=(char*)malloc(sizeof(char));
											strcpy(mp->value->term.data->value,data);
											mp->value->term.data->next=NULL;

											return true;
										}else
										{	if(mp->value->term.data)
											{
												printf("I ENTRER7 \n");
												mp->value->term.data=(table_data*)malloc(sizeof(table_data));
												mp->value->term.data->value=(char*)malloc(sizeof(char));
												strcpy(mp->value->term.data->value,data);
												mp->value->term.data->next=NULL;

												return true;
											}else
											{
												printf("I ENTRER8 \n");
												mp->next=(_map)malloc(sizeof(__map));
												mp=mp->next;
												mp->key=(terms_types*)malloc(sizeof(terms_types));
												mp->key->vartype=t_t->term.Map->key->vartype;
												mp->key->term.data=(table_data*)malloc(sizeof(table_data));
												mp->key->term.data->value=(char*)malloc(sizeof(char));
												strcpy(mp->key->term.data->value,data);
												mp->key->term.data->next=NULL;
												return true;
											}
										}
							}
						return false;
						break;
					case SET :
					case  SET_LIT :
						setlist=t_t->term.Set;
						while(setlist->next){
								setlist=setlist->next;
						}
						if(type_interpreter(tok,t_t->term.Set->terms->vartype)){
						if(setlist->terms!=NULL){
							setlist->terms=(terms_types*)malloc(sizeof(terms_types));
							setlist->terms->term.data=(table_data*)malloc(sizeof(table_data));
							setlist->terms->term.data->value=(char*)malloc(sizeof(char));
							strcpy(setlist->terms->term.data->value,data);
							setlist->terms->term.data->next=NULL;
						}
						if(setlist->terms->term.data==NULL){
							setlist->terms->term.data=(table_data*)malloc(sizeof(table_data));
							setlist->terms->term.data->value=(char*)malloc(sizeof(char));
							strcpy(setlist->terms->term.data->value,data);
							setlist->terms->term.data->next=NULL;
							return true;
						}
						else{
							setlist->next=(_setlist)malloc(sizeof(__setlist));
							setlist=setlist->next;
							setlist->terms->term.data=(table_data*)malloc(sizeof(table_data));
							setlist->terms->term.data->value=(char*)malloc(sizeof(char));
							strcpy(setlist->terms->term.data->value,data);
							setlist->terms->term.data->next=NULL;
							return true;
						}
						}
						return false;
						break;
					case LIST:

						setlist=t_t->term.List;
						while(setlist->next){
								setlist=setlist->next;
						}
						if(type_interpreter(tok,t_t->term.List->terms->vartype)){
							if(setlist->terms!=NULL){
								setlist->terms=(terms_types*)malloc(sizeof(terms_types));
								setlist->terms->term.data=(table_data*)malloc(sizeof(table_data));
								setlist->terms->term.data->value=(char*)malloc(sizeof(char));
								strcpy(setlist->terms->term.data->value,data);
								setlist->terms->term.data->next=NULL;
							}
						if(setlist->terms->term.data==NULL){
							setlist->terms->term.data=(table_data*)malloc(sizeof(table_data));
							setlist->terms->term.data->value=(char*)malloc(sizeof(char));
							strcpy(setlist->terms->term.data->value,data);
							setlist->terms->term.data->next=NULL;
							return true;
						}
						else{
							setlist->next=(_setlist)malloc(sizeof(__setlist));
							setlist=setlist->next;
							setlist->terms->term.data=(table_data*)malloc(sizeof(table_data));
							setlist->terms->term.data->value=(char*)malloc(sizeof(char));
							strcpy(setlist->terms->term.data->value,data);
							setlist->terms->term.data->next=NULL;
							return true;
						}
					}
						return false;
						break;
					default :
						if(type_interpreter(tok,t_t->vartype)){
							printf("\nDATA%s",data);
						t_t->term.data=(table_data*)malloc(sizeof(table_data));
						t_t->term.data->value=(char*)malloc(sizeof(char));
						strcpy(t_t->term.data->value,data);
						t_t->term.data->next=NULL;
						return true;
					}
						return false;
						break;
	}

	/*
	int i=1;
	if(p){
		if(p->data && p->next && p->next->data==NULL)
			if(p->data->value==NULL){
				p->data->value=(char*)malloc(sizeof(char));

	while(p && i<ordre){
		p=p->next;
		i++;
	}
	if(p && type_interpreter(tok,p->type)){
		if(p->data){
			if(p->data->value){
				strcat(p->data->value,yytext);
			}else
			{
				p->data->value=(char*)malloc(sizeof(char));
				strcpy(p->data->value,yytext);
			}
		}
	}*/
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
	if(con==true)
	values=remplir_values(values,yytext,token);
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
	if(IDF()){
		printf("idffonction");
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
					token=_lire_token();
					if(term()) hint=true;
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
	table_options * p=tables->fields;
	boolean col=false;
	if(token==MAP){
	printf("Segmentation fault here MAP BEGIN\n");
		while(p->next){
				p=p->next;
		}
		remplire_type(token,p);
		token=_lire_token();
		if(token==LESSER){
		//	strcat(p->type,yytext);
			token=_lire_token();
			if(cql_type()){
				token=_lire_token();
				if(token==VIRG){
					//strcat(p->type,yytext);
					token=_lire_token();
					if(cql_type()){
						token=_lire_token();
						if(token==BIGGER){
						//	strcat(p->type,yytext);
							col=true;
							printf("Segmentation fault here MAP FINISH\n");
						}
					}
				}
			}
		}
	}


	else {if(token==SET){
		while(p->next){
				p=p->next;
			}
			remplire_type(token,p);
		token=_lire_token();
		if(token==LESSER){
			//strcat(p->type,yytext);
			token=_lire_token();
			if(cql_type()){
				token=_lire_token();
				if(token==BIGGER){
			//		strcat(p->type,yytext);
					col=true;
				}
			}
		}
	}

	else { if(token==LIST){
		while(p->next){
				p=p->next;
			}
			remplire_type(token,p);
			token=_lire_token();
			if(token==LESSER){
			//	strcat(p->type,yytext);
				token=_lire_token();
				if(cql_type()){
				token=_lire_token();
					if(token==BIGGER){
			//			strcat(p->type,yytext);
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

boolean map_keyspace(){
	memtable.class=(char*)malloc(sizeof(char));
	boolean maplit=false;
		if(token==ACOLO){
			cursor++;
			token=_lire_token();
			if(term()){
				cursor++;
				if(strcmp("\'class\'",yytext)!=0){
						errs=creer_semantic_error(CIC,cursor,errs);
				}
				token=_lire_token();
				if(token==TWOP){
					cursor++;
					token=_lire_token();
					if(term()){printf("\n%s",yytext);
						printf("\n %d\n",strcmp(yytext,"\'SimpleStrategy\'"));
						cursor++;
						if((strcmp(yytext,"\'SimpleStrategy\'")==0)||(strcmp(yytext,"\'NetworkTopologyStrategy\'")==0)){
						strcpy(memtable.class,yytext);
						//printf("my class is %s\n",memtable.class);
							token=_lire_token();
						if(token==VIRG){
							token=_lire_token();
							if(term()){
								if(strcmp(yytext,"\'replication_factor\'")==0){
									if(strcmp(memtable.class,"\'SimpleStrategy\'")!=0){
										errs=creer_semantic_error(CIR,cursor,errs);
										//errs=errs->next;
									}
									token=_lire_token();
									if(token==TWOP){
										token=_lire_token();
										if(token==INUMBER){
											memtable.replication_factor=atoi(yytext);
											printf("%d replication_factor \n",memtable.replication_factor);
											token=_lire_token();
											if(token==ACOLF){
												maplit=true;
											}
										}
									}
								}else {

								}
								//maplit=true;
							}
							else if(token==ACOLF){
								maplit=true;
							}
					    }
					    else if(token==ACOLF){
								maplit=true;
							}
                    }else{printf("error now");
						errs=creer_semantic_error(CIV,cursor,errs);
						//errs=errs->next;
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
		islis=true;
	}
	return islis;
}
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
	if(collit==true)
	{
		current_type=c;
	}
	return collit;
}

//I IGNORED UDT_LIT AND FUNCTIONS, TO BE CHECKED LATER
//remember that you created a values Global variable that has all variables with their types in it
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
				//case it's a function
				else if(token==POPEN){token=_lire_token();
							if(term()){
								//printf("term");
								token=_lire_token();
								if(isterm()){
									token=_lire_token();
									if(token==TWOP){
										token=_lire_token();
										if(term()){
											token=_lire_token();
											if(islit()){ mapset=MAP_LIT;}
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
			else if(term()){
				printf("Term 1() ");
				token=_lire_token();
				if(token==TWOP){
					token=_lire_token();
					if(term()){
						printf("Term 2() ");
						token=_lire_token();
						if(islit()){ mapset=MAP_LIT;
							}
                	}
               }
				else if(token==VIRG){
					follow_token=true;
					token=_lire_token();
					if(isset()){ printf("Hani f iss set"); mapset=SET_LIT;}

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
	table_options * p=tables->fields;
	boolean tup=false;
	if(token==TUPLE){
		while(p->next){
				p=p->next;
			}
			if(p->type==NULL){
			//	p->type=(char*)malloc(sizeof(char)*48);
			//	strcpy(p->type,yytext);
		}else{
			//	strcat(p->type,yytext);
		}
		token=_lire_token();
		if(token==LESSER){
			//strcat(p->type,yytext);
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
	table_options * p=tables->fields;
	while(p->next){
				p=p->next;
			}
	boolean ttp=false;
	if(token==VIRG){
	//	strcat(p->type,yytext);
		token=_lire_token();
		if(cql_type()){
			token=_lire_token();
			ttp=tuple_type_aux();
		}
	}
	else if(token==BIGGER){
	//	strcat(p->type,yytext);
		ttp=true;
	}
	return ttp;
}
//tuple_literal ::=  '(' term ( ',' term )* ')'
boolean tuple_literal(){
  boolean tup=false;
  if(token==POPEN){printf("(");
  	token=_lire_token();
  	if(term()){
  		while(values){
  			printf("LA VALEUR %s\n",values->value);
        	if(copy_data(values->type,insert_ordre,values->value)){
        		values=values->next;
        	}else{
        		errs=creer_semantic_error(ICT,cursor,errs);
        		return FALSE;
        	}
       		}

  		 printf("term");
  		token=_lire_token();
  		tup=tuple_literal_aux();
  	}
  }
  return tup;
}
boolean keyspace_name(){
	//printf("\ntest keyspace");
	if(name()){
		//printf("name works");
		return true;
	}
	return false;
}
//tuple_literal_aux ::= ,term | )
boolean tuple_literal_aux(){
	boolean ttp=false;
	if(token==VIRG){ printf(",\n");
		token=_lire_token();
		insert_ordre++;
		if(term()){ printf("t2\n");
			while(values){
				printf("LA VALEUR %s\n",values->value);
        	if(copy_data(values->type,insert_ordre,values->value)){
        		values=values->next;
        	}else{
        		errs=creer_semantic_error(ICT,cursor,errs);
        		return FALSE;
        	}
       		}
			token=_lire_token();
			ttp=tuple_literal_aux();
		}
	}
	else if(token==PCLOSE){ printf(")\n");
		ttp=true;
	}
	return ttp;
}
/************************************************************************************/

//names_values     ::=  names VALUES tuple_literal

boolean names_values(){
 boolean n=false;

 printf("names & values\n");
  	printf("LE NBR DES PR : %d\n",nbr_primary_key(memtable.tables->fields));
 if(_names()){

					/*pri=pr;
					while(pri){
						printf("\nPRINT %s\n",pri->name);
						pri=pri->next;
					}*/

   printf("namesvalid\n");
   printf("\n\n\n\nCURRENT\n\n");
	//afficherhadchi(current);
   token=_lire_token();
   if(token==VALUES){ printf("VALUES\n");
   	token=_lire_token();
   	insert_ordre=1;
   	if(tuple_literal()) n=true;}

   }
 //  afficherhadchi(current);
return n;
}
//values fonction
//tuple







//fills the current list with the mentionned variables and the primary keys
boolean search_field(char*field){
	tab_op*p;
	boolean b=false;
	table_options* fields=NULL;
	table_options*temp=NULL;
	primary*n=pr,*m=NULL,*r=NULL;
	if(memtable.name){
		p=memtable.tables;

	//	afficherhadchi(p->fields);
		printf("THE CURRENT FIELD %s \nnbr_primary_key(p) : %d ",field,nbr_primary_key(p->fields));
		if(p->fields)
		{
			fields=p->fields;
			while(fields){
				printf("OKI1\n");
				if(fields->name!=NULL && strcmp(fields->name,field)==0){
					if(current==NULL){
						current=(table_options*)malloc(sizeof(table_options));
						current->name=(char*)malloc(sizeof(char));
						current->type=fields->type;
						current->primary=NULL;
						strcpy(current->name,field);
						current->next=NULL;

					}
					else{
						temp=current;
						while(temp->next){
							printf("OKI2\n");
							temp=temp->next;
						}
						temp->next=(table_options*)malloc(sizeof(table_options));
						temp=temp->next;
						temp->name=(char*)malloc(sizeof(char));
						temp->type=fields->type;
						temp->primary=NULL;
						strcpy(temp->name,field);
						temp->next=NULL;
					}
					b=true;break;
				}
				fields=fields->next;
			}
				if(fields->primary){
					printf("PRIMARY haha\n");
					if(current==NULL){
						current=(table_options*)malloc(sizeof(table_options));
						if(fields->name){
						current->name=(char*)malloc(sizeof(char));
						current->type=fields->type;
						strcpy(current->name,field);
						}
						current->primary=fields->primary;
						temp=current;
					}else{
						temp=current;
						if(temp){printf("NBR OF PRS %d\n",nbr_primary_key(p->fields));
							if(nbr_primary_key(p->fields)==1){
								while(temp && strcmp(temp->name,fields->primary->name)!=0){
									printf("OKI3\n");
									temp=temp->next;
								}
								if(temp!=NULL){
									temp->primary=(primary *)malloc(sizeof(primary));
									temp->primary->partition=fields->primary->partition;
									temp->primary->name=(char*)malloc(sizeof(char));
									strcpy(temp->primary->name,fields->primary->name);
									temp->primary->next=NULL;
								}
							}else{
								while(temp->next && temp->primary==NULL){printf("OKI4\n");
									temp=temp->next;
								}
								if(temp->primary==NULL){
									temp->next=(table_options*)malloc(sizeof(table_options));
									temp=temp->next;
									temp->primary=(primary *)malloc(sizeof(primary));
									temp->primary->partition=fields->primary->partition;
									temp->primary->name=(char*)malloc(sizeof(char));
									strcpy(temp->primary->name,fields->primary->name);
								}else{
									r=temp->primary;
									while(r->next){printf("OKI5\n");
										r=r->next;
									}
									r->next=(primary *)malloc(sizeof(primary ));
									r=r->next;
									r->partition=fields->primary->partition;
									r->name=(char*)malloc(sizeof(char));
									strcpy(r->name,fields->primary->name);
								}
							}
						}
			}
					r=NULL;
					if(temp)
						m=temp->primary;
					if(n!=NULL)
						while(m){printf("OKI6\n");
							while(n){printf("OKI7\n");
								if(strcmp(n->name,m->name)==0){
									if(r==NULL)
									{
										n=n->next;
										pr=pr->next;
									}else
									{
										r->next=n->next;
										n=n->next;
									}
									break;
								}else{
									r=n;
									n=n->next;
								}

							}
							m=m->next;
						}
		}

return b;
}
}
//afficherhadchi(current);
}
int nbr_primary_key(table_options* p){
	table_options* n=p;
	primary *pri=NULL;
	int i=0;
	while(n){
		if(n->primary){
			pri=n->primary;
		//	printf("\n\nPRI : %s\n",pri->name);
			while(pri){
		//		printf("\n\nPRI : %s\n",pri->next->name);
				i++;
				pri=pri->next;
			}
			return i;
		}
		n=n->next;
	}

}
//_names ::=  '(' column_name ( ',' column_name )* ')' ::= ( idf tuple_names_aux
boolean _names(){
char * name=(char*)malloc(sizeof(char));
  boolean tup=false;
  if(token==POPEN){
  	token=_lire_token();
  	strcpy(name,yytext);

  	if(search_field(name)){
  		printf("SUCCESS\n\n");
  		token=_lire_token();
  		tup=tuple_names_aux();
  	}
  }
  return tup;
}

//tuple_literal_aux ::= ,term | )
boolean tuple_names_aux(){
	char * name=(char*)malloc(sizeof(char));
	boolean ttp=false;
	if(token==VIRG){ printf("VIRGULE ,\n");
		token=_lire_token();
		strcpy(name,yytext);
  		if(search_field(name)){
  			printf("\nSUCCESS");
  			printf("column2\n");
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

/*insert_statement ::=  INSERT INTO table_name  names_values [ IF NOT EXISTS ]
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
				if(table_name()){

					token=_lire_token();
				if(names_values()){

					if(pr){
						errs=creer_semantic_error(APM,cursor,errs);
						return false;
					}
					afficherhadchi(current);
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
				else if(token==PVIRG||token==ENTER) {
					printf(";");
					ins=true;}

			}

		}
		}
	}}
	if(ins==true){
		printf("\n\n\n\n\nWTFFFFF\n");
		write_insert();
	}
    return ins;
}

//INSERT
/*
insert_statement ::=  INSERT INTO table_name names_values
                      [ IF NOT EXISTS ]
                      [ USING update_parameter ( AND update_parameter )* ]
names_values     ::=  names VALUES tuple_literal
names            ::=  '(' column_name ( ',' column_name )* ')'
column_name :: = idf */


/*
column_definition is:

column_name cql_type
| column_name cql_type PRIMARY KEY
| PRIMARY KEY ( partition_key )
| column_name collection_type

*/



/******************** name *************/
boolean name(){

							boolean n=false;
							char * name=(char*)malloc(100*sizeof(char));
							if(IDF()){

							}

							return n;
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
tab_op* make_first(tab_op* t,char * nom_table){
tab_op* p=t;
tab_op* k=p;
if(strcmp(t->name,nom_table)==0){
		return t;
	}
while(p){
	if(strcmp(p->name,nom_table)==0){
		k->next=p->next;
		p->next=t;
		t=p;
		return t;
	}
		k=p;
		p=p->next;
}


}
/************ table_name*******************/
boolean table_name(){
	        char* keys=(char*)malloc(sizeof(char));
			char* table_name=(char*)malloc(48*sizeof(char));
			tab_op * m=NULL;
			strcpy(keys,yytext);
			tab_op* l=NULL;
			table_options*tp=NULL;
		    primary* pri=NULL;
			token=_lire_token();
					if(token==POINT){
						token=_lire_token();
						strcpy(table_name,yytext);
						if(IDF())
							{
								strcat(keys,"/");
								strcat(keys,"keyspace.txt");
								read_json(keys);
								printf("seg\n");
								memtable.tables=make_first(memtable.tables,table_name);
								l=memtable.tables;
								printf("fails\n");
								printf("le nom du table : %s\n", memtable.tables->name);
								printf("la table dans l %s\n ",l->name);
								m=memtable.tables;
								while(m && strcmp(m->name,table_name)!=0)
									m=m->next;
								if(m){
								tables=m;
								}else{
									errs=creer_semantic_error(TDE,cursor,errs);
									return false;
								}
								tables->fields=read_table(tables->name,tables->keyspace_name,tables->fields);//Pour l'instant je veux la premiere
 								if(tables->fields)
  							//	afficherhadchi(tables->fields);
								while(l && strcmp(l->name,table_name)!=0){
										l=l->next;
									}
									if(l && strcmp(l->name,table_name)==0){
										tp=l->fields;
										while(tp){
											printf("IN1 loled\n");
											if(tp->primary!=NULL){
												pr=tp->primary;
												printf("OUT1\n");

												return true;
											}
											tp=tp->next;
										}

									}
								}
									errs=creer_semantic_error(TDE,cursor,errs);
							}

					else
					 {//keyspace par defaut
					 	l=memtable.tables;
								while(l && strcmp(l->name,table_name)==0){
										l=l->next;

									}
									if(l && strcmp(l->name,table_name)==0){
										tp=l->fields;
										while(tp){
											printf("IN2\n");
											if(tp->primary){
												pr=tp->primary;
												printf("OUT2\n");
												return true;
											}
											tp=tp->next;
										}

									}
									errs=creer_semantic_error(TDE,cursor,errs);

                   		}
        return false;
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
							{	token=_lire_token();
							if(token==PVIRG)
								{
										drtab=true;
								}
						}
					}
				 	}
					else{
						if(table_name())
							{
								token=_lire_token();
							if(token==PVIRG)
								{
										drtab=true;
								}
							}
						}

				}



  return drtab;
}


/******************* update_parametre *********************/
/*
//update_parameter ::=  ( TIMESTAMP | TTL ) ( integer | bind_marker )
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
/******************** using_delete *****************
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

}*/
/*******************operateur ********************/
/*boolean operateur(){
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

}*/
/************************ ( ',' column_name )*  ************************/
/*boolean iscol(){
	boolean i=false;
	if(token==VIRG){
		token=_lire_token();
			if(IDF()){
				token=_lire_token();
				if(token==PCLOSE){
					i=true;
				}
				else{i=iscol();}
 			}

 	}

 return i;
}/*

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
		{token=_lire_token();
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
void use_aux(char * logpath)
{			printf("WTF\n");
		strcat(logpath,"/keyspace.txt");
		read_json(logpath);

}
boolean us_e (){
	cursor=1;
	if(IDF()){
		cursor++;
		char* logpath=(char*)malloc(sizeof(char));
		strcpy(logpath,yytext);
		use_aux(logpath);
  		token=_lire_token();
		if(token==PVIRG){
				return true;
						}
					}
	return false;
	}


boolean custom_type(){
	boolean custom;
	if(token==STRING_TOKEN){
			custom=true;
	}
	else{custom=false;}
	return custom;
}
/*boolean name(){
	boolean na;
	if(token==QUOTED_IDF){na=true;}
	else if(token=UNQUOTED_IDF){na=true;}
	else{na=false;}
}*/
boolean column_name(){
	boolean col;
	if(IDF()){
		col=true;

	}
	else{col=false;}
	return col;
}
/*boolean table_name(){
	boolean tab=false;
	if(keyspace_name()){
		printf("\n -->keyspace");
		token=_lire_token();
		//printf("\njohn");
				if(token==POINT){
						token=_lire_token();
						if(name()){
							tab=true;

						}
						//else{tab=false;}


				}
				else {printf("geni");follow_token=true;tab=true;}
				//else{tab=false;}
	}
	//else if(name()){printf("\n-->name");tab=true;}
	//else{tab=false;}
	return tab;
}*/
boolean select_statement(){
	printf("select_test");

	boolean select=false;
	if(token==SELECT){
		printf("\nSELECT:");
		token=_lire_token();
		if(type_imput()){

			token=_lire_token();
			if((select_clause())||(token==MULT)){
				printf("\n-->select clause works:");
			token=_lire_token();
			if(token==FROM){
					printf("from");
				token=_lire_token();
				if(table_name()){
					token=_lire_token();
				    if(token==PVIRG){printf("select woks");select=true;}
					else if(where()){
						token=_lire_token();
						if(token==PVIRG){select=true;}
						else if(group_by()){
							token=_lire_token();
							if(token==PVIRG){select=true;}

							else if(order_by()){
								token=_lire_token();
								if(token==PVIRG){select=true;}
								else if(partition()){
									token=_lire_token();
									if(token==PVIRG){select=true;}
									else if(limit()){
										token=_lire_token();
										if(token==PVIRG){select=true;}
										else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

										else{select=false;}

									}
									else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

									else{select=false;}


								}
								else if(limit()){
										token=_lire_token();
										if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}
										else if(token==PVIRG){select=true;}
										else{select=false;}

									}
								else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

								else{select=false;}

							}
							else if(partition()){
									token=_lire_token();
									if(token==PVIRG){select=true;}
									else if(limit()){
										token=_lire_token();
										if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}
										else if(token==PVIRG){select=true;}
										else{select=false;}

									}
									else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}
									else{select=false;}


								}
							else if(limit()){
										token=_lire_token();
										if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}
										else if(token==PVIRG){select=true;}
										else{select=false;}

									}
							else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

							else{select=false;}

						}
						else if(order_by()){
								token=_lire_token();
								if(token==PVIRG){select=true;}
								else if(partition()){
									token=_lire_token();
									if(token==PVIRG){select=true;}
									else if(limit()){
										token=_lire_token();
										if(token==PVIRG){select=true;}
										else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

										else{select=false;}

									}
									else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

									else{select=false;}


								}
								else if(limit()){
										token=_lire_token();
										if(token==PVIRG){select=true;}
										else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

										else{select=false;}

									}
								else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

								else{select=false;}

							}
						else if(partition()){
									token=_lire_token();
									if(token==PVIRG){select=true;}
									else if(limit()){
										token=_lire_token();
										if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}
										else if(token==PVIRG){select=true;}
										else{select=false;}

									}
									else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

									else{select=false;}


								}
						else if(limit()){
										token=_lire_token();
										if(token==PVIRG){select=true;}
										else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

										else{select=false;}

									}
						else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

						else{select=false;}
							}



					else if(group_by()){
							token=_lire_token();
							if(token==PVIRG){select=true;}
							else if(order_by()){
								token=_lire_token();
								if(token==PVIRG){select=true;}
								else if(partition()){
									token=_lire_token();
									if(limit()){
										token=_lire_token();
									    if(token==PVIRG){select=true;}
										if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

										else{select=false;}

									}
									else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

									else{select=false;}


								}
								else if(limit()){
										token=_lire_token();
										if(token==PVIRG){select=true;}
										else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

										else{select=false;}

									}
								else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

								else{select=false;}

							}
							else if(partition()){
									token=_lire_token();
									if(token==PVIRG){select=true;}
									else if(limit()){
										token=_lire_token();
										if(token==PVIRG){select=true;}
										else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

										else{select=false;}

									}
									else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

									else{select=false;}


								}
							else if(limit()){
										token=_lire_token();
										if(token==PVIRG){select=true;}
										else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

										else{select=false;}

									}
							else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

							else{select=false;}

						}
					else if(order_by()){
								token=_lire_token();
								if(token==PVIRG){select=true;}
								else if(partition()){
									token=_lire_token();
									if(token==PVIRG){select=true;}
									else if(limit()){
										token=_lire_token();
										if(token==PVIRG){select=true;}
										else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

										else{select=false;}

									}
									else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

									else{select=false;}


								}
								else if(limit()){
										token=_lire_token();
										if(token==PVIRG){select=true;}
										else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

										else{select=false;}

									}
								else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

								else{select=false;}

							}
					else if(partition()){
									token=_lire_token();
									if(token==PVIRG){select=true;}
									else if(limit()){
										token=_lire_token();
										if(token==PVIRG){select=true;}
										else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

										else{select=false;}

									}
									else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

									else{select=false;}


								}
					else if(limit()){
										token=_lire_token();
									    if(token==PVIRG){select=true;}
										else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

										else{select=false;}

									}
					else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

					else{select=false;}

				}
			}
			else{printf("From doesn't work");select=false;}


		}
		}
		else if((select_clause())||(token==MULT)){
				printf("\n-->select clause works:");
			token=_lire_token();
			if(token==FROM){
				printf("from works");
				token=_lire_token();
				if(table_name()){
					printf("table name works");
					token=_lire_token();
				    if(token==PVIRG){select=true;}
					else if(where()){
						token=_lire_token();
						if(token==PVIRG){select=true;}
						else if(group_by()){
							token=_lire_token();
							if(token==PVIRG){select=true;}

							else if(order_by()){
								token=_lire_token();
								if(token==PVIRG){select=true;}
								else if(partition()){
									token=_lire_token();
									if(token==PVIRG){select=true;}
									else if(limit()){
										token=_lire_token();
										if(token==PVIRG){select=true;}
										else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

										else{select=false;}

									}
									else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

									else{select=false;}


								}
								else if(limit()){
										token=_lire_token();
										if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}
										else if(token==PVIRG){select=true;}
										else{select=false;}

									}
								else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

								else{select=false;}

							}
							else if(partition()){
									token=_lire_token();
									if(token==PVIRG){select=true;}
									else if(limit()){
										token=_lire_token();
										if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}
										else if(token==PVIRG){select=true;}
										else{select=false;}

									}
									else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}
									else{select=false;}


								}
							else if(limit()){
										token=_lire_token();
										if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}
										else if(token==PVIRG){select=true;}
										else{select=false;}

									}
							else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

							else{select=false;}

						}
						else if(order_by()){
								token=_lire_token();
								if(token==PVIRG){select=true;}
								else if(partition()){
									token=_lire_token();
									if(token==PVIRG){select=true;}
									else if(limit()){
										token=_lire_token();
										if(token==PVIRG){select=true;}
										else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

										else{select=false;}

									}
									else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

									else{select=false;}


								}
								else if(limit()){
										token=_lire_token();
										if(token==PVIRG){select=true;}
										else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

										else{select=false;}

									}
								else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

								else{select=false;}

							}
						else if(partition()){
									token=_lire_token();
									if(limit()){
										token=_lire_token();
										if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}
										else if(token==PVIRG){select=true;}
										else{select=false;}

									}
									else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}
									else if(token==PVIRG){select=true;}
									else{select=false;}


								}
						else if(limit()){
										token=_lire_token();
										if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}
										else if(token==PVIRG){select=true;}
										else{select=false;}

									}
						else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

						else{select=false;}
							}



					else if(group_by()){
							token=_lire_token();
							if(token==PVIRG){select=true;}
							else if(order_by()){
								token=_lire_token();
								if(token==PVIRG){select=true;}
								else if(partition()){
									token=_lire_token();
									if(limit()){
										token=_lire_token();
									    if(token==PVIRG){select=true;}
										if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

										else{select=false;}

									}
									else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

									else{select=false;}


								}
								else if(limit()){
										token=_lire_token();
										if(token==PVIRG){select=true;}
										else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

										else{select=false;}

									}
								else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

								else{select=false;}

							}
							else if(partition()){
									token=_lire_token();
									if(token==PVIRG){select=true;}
									else if(limit()){
										token=_lire_token();
										if(token==PVIRG){select=true;}
										else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

										else{select=false;}

									}
									else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

									else{select=false;}


								}
							else if(limit()){
										token=_lire_token();
										if(token==PVIRG){select=true;}
										else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

										else{select=false;}

									}
							else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

							else{select=false;}

						}
					else if(order_by()){
								token=_lire_token();
								if(token==PVIRG){select=true;}
								else if(partition()){
									token=_lire_token();
									if(token==PVIRG){select=true;}
									else if(limit()){
										token=_lire_token();
										if(token==PVIRG){select=true;}
										else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

										else{select=false;}

									}
									else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

									else{select=false;}


								}
								else if(limit()){
										token=_lire_token();
										if(token==PVIRG){select=true;}
										else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

										else{select=false;}

									}
								else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

								else{select=false;}

							}
					else if(partition()){
									token=_lire_token();
									if(token==PVIRG){select=true;}
									else if(limit()){
										token=_lire_token();
										if(token==PVIRG){select=true;}
										else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

										else{select=false;}

									}
									else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

									else{select=false;}


								}
					else if(limit()){
										token=_lire_token();
									    if(token==PVIRG){select=true;}
										else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

										else{select=false;}

									}
					else if(allow()){
											token=_lire_token();
											if(token==PVIRG){
												select=true;
											}
											else{select=false;}
										}

					else{select=false;}

				}
			}
			else{select=false;}


		}
		else{select=false;}
	}
	return select;
 }
boolean  type_imput(){
 	boolean imp;
 	if((token==DISTINCT)||(token==JSON)){
 		imp=true;
 	}
 	else{imp=false;}
 	return imp;
 }
boolean where(){
 	boolean wh;
 	if(token==WHERE){
 		printf("\n-->where");
 		token=_lire_token();
 		if(where_clause()){
 			wh=true;
 		}
 		else{printf("its doesnt where");wh=false;}
 	}
 	else{wh=false;}
 	return wh;

 }
boolean group_by(){
 	boolean grp;
 	if(token==GROUP){
 		token=_lire_token();
 		if(group_by_clause()){
 			grp=true;
 		}
 		else{grp=false;}
 	}
 	else{grp=false;}
 	return grp;
 }


boolean order_by(){
	boolean order;
	if(token==ORDER){
		printf("\n-->order by test");
		token=_lire_token();
		if(order_by_clause()){
			printf("\n--> ordr by clause works ");
			order=true;
		}
		else{order=false;}
	}
	else{order=false;}
	return order;
}
boolean partition(){
 	boolean par;
 	if(token==PER){
 		token=_lire_token();
 		if((token==INUMBER)||(bind_marker())){
 			par=true;
 		}
 		else{par=false;}

 	}
 	else{par=false;}
 	return par;
 }
boolean limit(){
	boolean lim;
	if(token==LIMIT){
		token=_lire_token();
		if((token==INUMBER)||(bind_marker())){
			lim=true;
		}
		else{lim=false;}
	}
	else{lim=false;}
	return lim;
}
boolean allow(){
	boolean all;
	if(token==ALLOW){
		all=true;
	}
	else{all=false;}
return all;
}
boolean isidf(){
	boolean isid;
	if(token==AS){
		token=_lire_token();
		if(IDF()){isid=true;}
		else{isid=false;}
	}
	else{isid=false;}
	return isid;

}
boolean select_clause(){
	boolean selecl=false;
	printf("-->select clause test");
	if(selector()){
		printf("Selector works");
		token=_lire_token();
		if(isidf()){
			token=_lire_token();
			if(token==VIRG){
					token=_lire_token();
					if(selector()){
					token=_lire_token();
					if(isidf()){
						token==_lire_token();
						if(token==FROM){
						follow_token=true;
						selecl=true;
					}
					}
					else if(token==FROM){
						printf("its from test");
						follow_token=true;
						selecl=true;
					}
				}
			}
			else if(token==FROM){
				follow_token=true;
				selecl=true;
			}

		}
		else if(token==FROM){
			follow_token=true;
			selecl=true;
		}
		else if(token==VIRG){

					token=_lire_token();
					if(selector()){
					token=_lire_token();
					if(isidf()){
						token=_lire_token();
					if(token==FROM){
						follow_token=true;
						selecl=true;
					}

					}
					else if(token==FROM){
							follow_token=true;
								selecl=true;
							}
				}
			}



		}
		return selecl;
}
boolean selector(){
	boolean tor=false;
	printf("selector test");
	if(column_name()){
		token=_lire_token();
		if(token==POPEN){
			printf("\npopen");
			token=_lire_token();
			if(selector()){
				printf("\nselector");
				token=_lire_token();
				if(isselec()){
					printf("\n---> in ");
					tor=true;
				}
			}
		}
			else{follow_token=true;tor=true;}
	}
	else if(term()){tor=true;}
	else if(token==CAST){
		token=_lire_token();
		if(token==POPEN){
			token=_lire_token();
			if(selector()){
				token=_lire_token();
				if(token==AS){
					token=_lire_token();
					if(cql_type()){
						token=_lire_token();
						if(token==PCLOSE){
							tor=true;
						}
					}
				}
			}
		}
	}
	else if(function_name()){
		printf("\nfunction name enter");
		token=_lire_token();
		if(token==POPEN){
			printf("\npopen");
			token=_lire_token();
			if(selector()){
				printf("\nselector");
				token=_lire_token();
				if(isselec()){
					printf("\n---> in ");
					tor=true;
				}
			}
		}

	}
	else if(token==COUNT){
		token=_lire_token();
		if(token==POPEN){
			token=_lire_token();
			if(token==MULT){
				token=_lire_token();
				if(token==PCLOSE){
					tor=true;
				}
			}

		}
	}
	return tor;
}
boolean isselec(){
	boolean iss;
	if(token==VIRG){
		token=_lire_token();
		if(selector()){
			token=_lire_token();
			iss=isselec();
		}
		else{iss=false;}
	}
	else if(token==PCLOSE){
		iss=true;
	}
	else{iss=false;}
	return iss;
}

boolean issel(){
	boolean isss=false;
	if(token==VIRG){
		token=_lire_token();
		if(selector()){
			token=_lire_token();
			if(isidf()){
				token=_lire_token();
				if(token==FROM){
				follow_token=true;
				isss=true;

			}
			else if(token==FROM){
				follow_token=true;
				isss=true;
			}
		}
	}
	//return isss;
 }
 return isss;
}

boolean where_clause(){
	boolean wherecl=false;
	if(relation()){
		printf("\n-->relation rest valid");
		token=_lire_token();
		if(isrel()){
			wherecl=true;
		}
	}
	return wherecl;
}
boolean isrel(){
	boolean isr=false;
	if(token==AND){
		printf("\n-->AND");
		token=_lire_token();
		if(relation()){
			printf("\n--> next relation valid");
			token=_lire_token();
			isr=isrel();

		}
	}
	else {
		follow_token=true;
		isr=true;
	}

}
boolean relation(){
	boolean rela=false;
	if(column_name()){
		printf("\n-->columm name");
		token=_lire_token();
		if(operator()){
			token=_lire_token();
			if(term()){
				printf("\n-->its a valid relation");
				rela=true;
			}
		}
		else{rela=false;}
	}
	else if(token==POPEN){
		token=_lire_token();
		if(column_name()){
			token=_lire_token();
			if(iscol()){
				token=_lire_token();
				if(token==PCLOSE){
					token=_lire_token();
					if(operator()){
						token=_lire_token();
					if(tuple_literal()){
						rela=true;

					}
				}
				}
			}
		}
	}
	else if(token==TOKEN){
		token=_lire_token();
		if(column_name()){
			token=_lire_token();
			if(iscol()){
				token=_lire_token();
				if(token==PCLOSE){
					token=_lire_token();
					if(operator()){
						token=_lire_token();

					if(term()){
						rela=true;

					}
				}
				}
			}
		}
	}

				return rela;
}
/*
group_by_clause  ::=  column_name ( ',' column_name )*

*/
boolean group_by_clause(){
	boolean groub=false;
	if(column_name()){
		token=_lire_token();
		if(iscolgr()){
			groub=true;
		}
	}
	return groub;
}
boolean iscol(){
	boolean isclo=false;
	if(token==VIRG){
		token=_lire_token();
		if(column_name()){
			token=_lire_token();
			isclo=iscol();
		}

	}
	else if(token==PCLOSE){
		follow_token=true;
		isclo=true;
	}
	return isclo;
}
boolean iscolgr(){
	boolean isclo=false;
	if(token==VIRG){
		token=_lire_token();
		if(column_name()){
			token=_lire_token();
			isclo=iscol();
		}

	}
	else if((token==PVIRG)||(token==ORDER)||(token==PER)||(token==LIMIT)||(token==ALLOW)){
		follow_token=true;
		isclo=true;
	}
	return isclo;

}
/*
ordering_clause  ::=  column_name [ ASC | DESC ] ( ',' column_name [ ASC | DESC ] )*
*/
boolean order_by_clause(){
	boolean ordby=false;
	if(column_name()){
		token=_lire_token();
		if(ascdesc()){
			printf("\n--> ASC DESC");
			token=_lire_token();
			if(isorder()){
				ordby=true;
			}

		}
		else{
			if(isorder()){
				ordby=true;
			}
		}
	}
	return ordby;


}
boolean isorder(){
	boolean nisor=false;
	if(token==VIRG){
		token=_lire_token();
		if(column_name()){
			token=_lire_token();
			if(ascdesc()){
					nisor=isorder();
			}
			else{nisor=isorder();}
		}
	}
	else if((token==PVIRG)||(token==ORDER)||(token==PER)||(token==LIMIT)||(token==ALLOW)){
		follow_token=true;
		nisor=true;
	}
	return nisor;
}
//ascdesc ::= ASC|DESC
boolean ascdesc(){
	boolean asc;
	if((token==ASC)||(token==DESC)){
		asc=true;

	}
	else{asc=false;}
	return asc;

}
//function name : IDF
boolean  function_name(){
	boolean fun;
	if(IDF()){
		fun=true;
	}
	else{fun=false;}
}
/*
update_statement ::=  UPDATE table_name
                      [ USING update_parameter ( AND update_parameter )* ]
                      SET assignment ( ',' assignment )*
                      WHERE where_clause
                      [ IF ( EXISTS | condition ( AND condition )*) ]
update_parameter ::=  ( TIMESTAMP | TTL ) ( integer | bind_marker )
assignment       ::=  simple_selection '=' term
                     | column_name '=' column_name ( '+' | '-' ) term
                     | column_name '=' list_literal '+' column_name
simple_selection ::=  column_name
                     | column_name '[' term ']'
                     | column_name '.' `field_name
condition        ::=  simple_selection operator term
*/
/*
update_statement ::=  UPDATE table_name
                      [ USIN ]
                      SET assignment ( ',' assignment )*
                      where
                      [IF_CLAUSE]
*/
boolean update_statement(){
	boolean upd=false;
	if(token==UPDATE){
		printf("\nUPDATE");
		token=_lire_token();
		if(table_name()){
			printf("\nTable");
			token=_lire_token();
			if(USIN()){
				token=_lire_token();
				if(token==SET){
				token=_lire_token();
				if(assigement()){
					token=_lire_token();
					if(isass()){
						token=_lire_token();
						if(token==WHERE){
							token==_lire_token();
							if(where_clause()){
								token=_lire_token();
								if(IF_CLAUSE()){
									if(token==PVIRG){
										upd=true;
									}

								}
								else{
									if(token==PVIRG){
										upd=true;
									}
								}
							}
						}
					}

				}
			}

			}
			else if(token==SET){
				printf("\nset");
				token=_lire_token();
				if(assigement()){
					printf("\n-->assig");
					token=_lire_token();

					if(isass()){
						printf(" \n>>>> its isass");
						token=_lire_token();
						if(where()){
							printf("\n------> where valid");



								token=_lire_token();
								if(IF_CLAUSE()){
									printf("\n--------->if clause");
									token=_lire_token();
									if(token==PVIRG){
										upd=true;
									}

								}
								else{
									if(token==PVIRG){
										upd=true;
									}
								}

						}
					}

				}
			}
		}
	}
		return upd;
}
/*
USIN::=USING update_parameter ( AND update_parameter )*
*/
boolean USIN(){
	boolean US=false;
	if(token==USING){
			token=_lire_token();
			if(update_parameter()){
				token=_lire_token();
				if(isupd()){
					token=_lire_token();
					if(token==WHERE){
						follow_token=true;
						US=true;
					}
				}
			}

	}
	return US;
}
boolean isupd(){
	boolean isp=false;
	if(token==AND){
		token=_lire_token();
		if(update_parameter()){
			token=_lire_token();
			isp=isupd();
		}
	}
	else if(token==WHERE){
		follow_token=true;
		isp=true;
	}
	return isp;
}

/*
condition        ::=  simple_selection operator term
*/
boolean condition(){
	boolean cond=false;
	if(simple_selection()){
		token=_lire_token();
		if(operator()){
			token=_lire_token();
			if(term()){
						cond=true;
			}
		}
	}
	return cond;
}

//IF_CLAUSE ::=IF ( EXISTS | condition ( AND condition )*)
boolean IF_CLAUSE(){
	boolean ifcl=false;
	if(token==IF){
		printf("\n->if");
		token=_lire_token();
		if(token==EXISTS){
			printf("\n->exists");
					ifcl=true;
		}
		else if(condition()){
			token=_lire_token();
			if(iscond()){
					ifcl=true;
			}
		}
	}
	return ifcl;
}
boolean iscond(){
	boolean isc=false;
	if(token==AND){
		token=_lire_token();
		if(condition()){
			token=_lire_token();
			isc=iscond();
		}

	}
	else if(token==PVIRG){
		follow_token=true;
		isc=true;
	}
	return isc;
}

boolean isass(){
	boolean isas=false;
	if(token==VIRG){
		token=_lire_token();
		if(assigement()){
			isas=isass();
		}
	}
	else if(token==WHERE){
		follow_token=true;
		isas=true;
	}
	return isas;
}
/*
assignment       ::=  simple_selection '=' term
                     | column_name '=' column_name ( '+' | '-' ) term
                     | column_name '=' list_literal '+' column_name
*/
boolean assigement(){
		boolean assi=false;
	int a=simple_selection();
	if((a!=3)&&(a!=0)){
		printf("\n-->simple");
		token=_lire_token();
		if(token==EQ){
			printf("\n--->eq");
			token=_lire_token();
			if(term()){
				printf("\n-->term");
				assi=true;
			}
		}
	}
	else if((a==3)){
		printf("\n---> thing");
		token=_lire_token();
		if(token==EQ){
			printf("\n--->eq");
			token=_lire_token();
			if(list_literal()){
				token=_lire_token();
				if(token==PLUS){
					token=_lire_token();
					if(column_name()){
						assi=true;
					}
				}
			}
			else if(column_name()){
				printf("it's column name ");
				token=_lire_token();
				if((token==MINUS)||(token==PLUS)){
					printf("its plus");
					token=_lire_token();
					if(term()){
						assi=true;
					}
				}
			}
			else if(term()){assi=true;}
		}

	}
		return assi;
}


boolean operator(){
	boolean ope=false;
	printf("\n--->operator test");
	if(token==IN){ope=true;}
	else if(token==EQ){ope=true;}
	else if(token==LESSEREQ){ope=true;}
	else if(token==BIGGEREQ){ope=true;}
	else if(token==CONTAINS){ope=true;}
	else if(token==CONT_KEY){ope=true;}
	else if(token==LESSER){ope=true;}
	else if(token==BIGGER){ope=true;}
	else if(token==DIFF){ope=true;}
	return ope;
}

boolean field_definition(){
	if(IDF()){return true;}
	else{return false;}
}

int simple_selection(){
	int simp=0;
	if(column_name()){
		printf("\n-->colmumn valid");
			token=_lire_token();
			if(token==CROPEN){
				token=_lire_token();
				if(term()){
					token=_lire_token();
					if(token==CRCLOSE){
						simp=1;
					}
				}
			}
			else if(token==POINT){
				token=_lire_token();
				if(field_definition()){
					simp=2;
				}
			}
			else{
				if(token==EQ){
				printf("\n--->jessus");}
				follow_token=true;
				simp=3;
			}
	}
	return simp;
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
				else if(token==WITH) {
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
					cursor++;


								memtable.name=(char*)malloc(48*sizeof(char));
								strcpy(memtable.name,yytext);
								printf("\n%s DAMN BOYYYYY\n",memtable.name);


					token=_lire_token();
					if(token==WITH_REPLICATION){
						cursor=cursor+2;
						token=_lire_token();
						if(token==EQ){
							cursor++;
							token=_lire_token();
							if(map_keyspace()){
								cursor++;
								token=_lire_token();
								if(token==AND){
									cursor++;
									token=_lire_token();
									if(token==DURABLE_WRITES){
										cursor++;
										token=_lire_token();
										if(token==EQ){
											cursor++;
											token=_lire_token();
											if(token==TRUE || token==FALSE){
												cursor++;
												memtable.durable_writes=token;
												token=_lire_token();
												if(token==PVIRG){
													cursor++;
													memtable.success=1;
													return true;}
												}
											}
										}
									}
									else if(token==PVIRG){
													memtable.success=1;
													return true;	}
								}
							}


						}

					}
memtable.success=0;
return false;
}

boolean create_key_space(){
	cursor=1;
	if(token==KEYSPACE || token==SCHEMA){
		cursor++;
		token=_lire_token();
		if(if_existing()){
			cursor++;
			token=_lire_token();
			if(Keyspace_aux()){
				if(memtable.success==1){
					char* keys_name=(char*)malloc(48*sizeof(char));
					strcpy(keys_name,yytext);
					if(fopen(keys_name,"r")!=NULL){
								errs=creer_semantic_error(KAE,cursor,errs);

					}
					int i=nbr_semantic_errors(errs);
					if(i>1){
						afficher_semantic_error(errs);
						return false;
					}
					else{
						create_json_keyspace();
					}
					return true;
				}

			}
			}else if(Keyspace_aux()){
					if(memtable.success==1){
				     int i=nbr_semantic_errors(errs);
						printf("my class is %s loled %s\n",memtable.class,memtable.name);
						afficher_semantic_error(errs);
					if((i==0)&&(memtable.success==1)){
						 create_json_keyspace();
							return true;
					}

				}

			}
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
	table_options * p = tables->fields;
	primary * pri=NULL;
	if(IDF()){
		while(p->next)
		 	p=p->next;
			pri=p->primary;
			while(pri->next)
				pri=pri->next;
			pri->next=(primary*)malloc(sizeof(primary));
			pri=pri->next;
			pri->partition=1;
			pri->name=(char*)malloc(sizeof(char));
			strcpy(pri->name,yytext);
			pri->next=NULL;
			token=_lire_token();
		if(token==VIRG){
				token=_lire_token();
				return partition_key_aux();
				}
		if(token==PCLOSE){
					return true;

	}
}
	return false;
}

//Partition Key: IDF | '(' Partition_aux
boolean partition_key(){

	table_options* p=tables->fields;

	if(IDF())
		{	while(p->next && p->primary==NULL){
		//	 	p->data=NULL;
			 	p=p->next;
			 }

				if(p->primary){
					errs=creer_semantic_error(DPK,cursor,errs);
					return false;
				}

			p->primary=(primary*)malloc(sizeof(primary));
			p->primary->partition=1;
			p->primary->name=(char*)malloc(sizeof(char));
			strcpy(p->primary->name,yytext);
			p->primary->next=NULL;
			return true;
		}
	if(token==POPEN){
			while(p->next&& p->primary==NULL){
			//	p->data=NULL;
			 	p=p->next;
			}

			if(p){
				if(p->primary){
					errs=creer_semantic_error(DPK,cursor,errs);
					return false;
				}
			}
			p->primary=(primary*)malloc(sizeof(primary));
			p->primary->partition=1;
			p->primary->name=(char*)malloc(sizeof(char));
			strcpy(p->primary->name,yytext);
			p->primary->next=NULL;
		token=_lire_token();
		return partition_key_aux();
		}
return false;

}



//clustering_columns : Parition_key_aux
//PRI_KEY_AUX : ',' clustering_columns

boolean clustering_columns(){
	table_options* p=tables->fields;
	primary * pri=NULL;
	if(IDF()){
		while(p->next && p->primary==NULL)
			 	p=p->next;
			}
			pri=p->primary;
			while(pri->next){
				pri=pri->next;
			}
			pri->next=(primary*)malloc(sizeof(primary));
			pri=pri->next;
			pri->partition=0;
			pri->name=(char*)malloc(sizeof(char));
			strcpy(pri->name,yytext);
			pri->next=NULL;
		token=_lire_token();
		if(token==VIRG){
			token=_lire_token();
			return clustering_columns();
		}
	if(token==PCLOSE)
		return true;

	return false;
}




//PRI_KEY: '(' Partition_key ')' | '(' Partition_key PRI_KEY_AUX

boolean pri_key(){

	token=_lire_token();
	if(token==POPEN){
		token=_lire_token();
		if(partition_key()){
			token=_lire_token();
			if(token==PCLOSE)
				return true;
			else{
				if(token==VIRG){
					token=_lire_token();
					return clustering_columns();
				}
			}
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
	boolean b=true;
	table_options *p=tables->fields;
	if(cql_type()){

		if(token==BIGGER){
			b=false;//= true????
		}
		token=_lire_token();
		if(token==PRIMARY_KEY){
			while(p->next && p->primary==NULL)
				p=p->next;
			if(p->primary){
					errs=creer_semantic_error(DPK,cursor,errs);
					return false;
				}
			p->primary=(primary*)malloc(sizeof(primary));
			p->primary->partition=1;
			p->primary->name=(char*)malloc(sizeof(char));
			strcpy(p->primary->name,p->name);
			p->primary->next=NULL;
			b=true;
		}
		if(token==PCLOSE ||token==VIRG){
			follow_token=true;
			b=true;
			}
	}

return b;
}

//column_def_part: IDF column_def_aux | PRIMARY_KEY PRI_KEY
//CATCH THE TYPES OF VARIABLES
boolean column_def_part(){
	table_options*p;
	if(IDF()){
		if(tables->fields==NULL){
			tables->fields=(table_options*)malloc(sizeof(table_options));
			p=tables->fields;
		}
		else{
			p=tables->fields;
			while(p->next){
				p=p->next;
			}
			p->next=(table_options*)malloc(sizeof(table_options));
			p=p->next;
		}
			p->name=(char*)malloc(sizeof(char)*48);
			p->type=NULL;
			p->primary=NULL;
			p->next=NULL;
			strcpy(p->name,yytext);
		token=_lire_token();
		return column_def_aux();
		}
	if(token==PRIMARY_KEY){
		p->name=NULL;
			p->type=NULL;
			p->primary=(primary*)malloc(sizeof(primary));
			p->next=NULL;
		return pri_key();
	}

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



boolean Keyspace_per_create(){
if(IDF()){
			char* keys=(char*)malloc(48*sizeof(char));
			char* key=(char*)malloc(48*sizeof(char));
			strcpy(keys,yytext);
			strcpy(key,yytext);
			tab_op* l=(tab_op*)malloc(sizeof(tab_op));
			l->fields=NULL;
			token=_lire_token();
					if(token==POINT){
						token=_lire_token();
						printf("this is the text %s\n",yytext);
						if(true)
							{
								strcat(keys,"/");
								strcat(keys,"keyspace.txt");
								file=fopen(keys,"r");
								char *testagain=(char*)malloc(48*sizeof(char));
								if(file==NULL){
									errs=creer_semantic_error(KDE,cursor,errs);
								}else{
									use_aux(key);

									l->keyspace_name=(char*)malloc(48*sizeof(char));
									l->name=(char*)malloc(48*sizeof(char));
					 				strcpy(l->keyspace_name,key);
					 				strcpy(l->name,yytext);
					 				strcpy(testagain,l->keyspace_name);
					 				strcat(testagain,"/");
					 				strcat(testagain,l->name);
					 				fclose(file);
					 				file=fopen(testagain,"r");
					 				if(file!=NULL){
					 					errs=creer_semantic_error(TAE,cursor,errs);
					 				}
					 				else{
					 					l->next=tables;
		    							tables=l;
					 					return true;
					 				}
								}
								//return true;
							}
					}
					else
					 {//keyspace par defaut
					 	char *testagain=(char*)malloc(sizeof(char));
					 		l->name=(char*)malloc(sizeof(char));
					 		l->keyspace_name=(char*)malloc(sizeof(char));
					 		if(memtable.name!=NULL){
					 			strcpy(l->keyspace_name,memtable.name);
					 			strcpy(l->name,yytext);
					 			strcpy(testagain,memtable.name);
					 			strcat(testagain,"/");
					 			strcat(testagain,l->name);
					 			file=fopen(testagain,"r");
					 			if(file!=NULL){
					 				errs=creer_semantic_error(TAE,cursor,errs);
					 				//		errs=errs->next;
					 			}
					 			else{
		    						follow_token=true;
		    						fclose(file);
		    						l->next=tables;
		    						tables=l;
									return true;}
								}else{
									errs=creer_semantic_error(UK,cursor,errs);
								}
                   		}
	}printf("FALSE\n");
return false;
}

/*create_table_aux_1: keyspace_name.table_name
( column_definition, column_definition, ...) */
boolean create_table_aux_1(){
if(Keyspace_per_create()){
							token=_lire_token();
							if(token==POPEN){
								token=_lire_token();
								if(column_definition()){

									return true;
								}


							}
	}

				return false;
}

boolean create_table(){
	int i;
	boolean b=false;
	if(token==TABLE){
		token=_lire_token();
			if(if_existing()){
				token=_lire_token();
				if(create_table_aux_1()){
					token=_lire_token();
					if(token==PCLOSE)
					{

						b=true;
					}
				}
			}else{printf("no use?");
				if(create_table_aux_1()){

					token=_lire_token();
					if(token==PCLOSE)
					{
						b=true;
					}
				}
				}
		}
	if(b==true){
		if(addTable_Keyspace()==true){
		write_table();
		char * filename=(char*)malloc(100*sizeof(char));
		char * path=(char*)malloc(100*sizeof(char));
		strcpy(path, memtable.name);
		strcat(path,"/");
		strcat(path,memtable.tables->name);
		FILE * f;
		for(i=1;i<5;i++){

			strcpy(path, memtable.name);
			strcat(path,"/");
			strcat(path,memtable.tables->name);
			strcat(path,"/");
			//printf("my path is %s\n\n",path);
			sprintf(filename,"table%d.txt",i);
			printf("%s\n",filename);
			strcat(path,filename);
			f=fopen(path,"w");
			fclose(f);
		}
		memtable.tables=tables;
	}
		else
			b=false;

	}
	return b;
}





//create_table_keyspace : CREATE TABLE create_table() | CREATE KEYSPACE create_key_space()
boolean create_table_keyspace(){
	boolean response=false;
if(token==CREATE){
	////printf("create ");
	token=_lire_token();
	if(token==TABLE){
		response=create_table();
	}
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
/*
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
*/

boolean Analyse_syntaxique(){
 if(token==INSERT){
	return insert_statement();}
 if(token==DROP){
	return drop();}
 if(token==CREATE){
return create_table_keyspace();}
 /*if(token==ALTER){
 	return alter_table_keyspace();}*/
 if(token==USE){token=_lire_token();
    return us_e();}
 if(select_statement()){return true;}

 if(update_statement()){return true;}
 return false;
}



/****JASON FUNCTIONS****/

//READING A KEYSPACE FROM A PATH  AND CREATE A SEMANTIC ERREUR IF KEYSPACE NOT FOUND
void read_json(char * logpath){
tab_op*p;
root = json_load_file(logpath,0, &error);
if(root==NULL)
{
 	errs=creer_semantic_error(KDE,cursor,errs);
}
else{
  	data=json_array_get(root,0);
 	memtable.name=(char*)malloc(sizeof(char));
  	strcpy(memtable.name,json_string_value(json_object_get(data,"keyspace name")));
 	memtable.class=(char*)malloc(sizeof(char));
  	strcpy(memtable.class,json_string_value(json_object_get(data,"class")));
  	json_t * imthetable=json_array_get(root,1);
  	imthetable=json_object_get(imthetable,"tables");

  	size_t k=json_array_size(imthetable);

  	for(int i=0;i<k;i++)
  	{
  		if(i==0 && tables==NULL){

  			tables=(tab_op*)malloc(sizeof(tab_op));
  			tables->fields=NULL;
  			tables->keyspace_name=(char*)malloc(sizeof(char));
  			tables->name=(char*)malloc(sizeof(char));
  			strcpy(tables->name,json_string_value(json_array_get(imthetable,i)));
  			strcpy(tables->keyspace_name,memtable.name);
  			tables->next=NULL;

  		}
  		else{
  			p->fields=NULL;
  			p=(tab_op*)malloc(sizeof(tab_op));
  			p->keyspace_name=(char*)malloc(sizeof(char));
  			p->name=(char*)malloc(sizeof(char));
  			strcpy(p->name,json_string_value(json_array_get(imthetable,i)));
  			strcpy(p->keyspace_name,memtable.name);
  			p->next=tables;
  			tables=p;
  		}
  	}
		memtable.tables=tables;
}
}
//ADDING A TABLE TO THE JSON KEYSPACE AND CREATING ITS FILE IF NOT ( IT DEPENDS ON WHETHER WE USE THE OPTION  IF EXISTS OR NOT)
boolean addTable_Keyspace(){
	json_t*tab=json_array_get(root,1);
		json_t*tab_values=json_object_get(tab,"tables");
		if(tab_values==NULL)
			tab_values=json_array();
		printf("the table's name :%s\n",tables->name);
		size_t index;
		json_t * value;
		json_array_foreach(tab_values,index,value){
			if(strcmp(json_string_value(value),tables->name)==0){
				errs=creer_semantic_error(TAE,cursor,errs);
				return false;
				}
		}
		json_array_append_new(tab_values,json_string(tables->name));
		json_object_set(tab,"tables",tab_values);
		json_array_set(root,1,tab);
		char* logpath=(char*)malloc(sizeof(char));
		char *keyspacee=(char*)malloc(sizeof(char));
		strcpy(logpath,memtable.name);
		strcat(logpath,"/");
		strcpy(keyspacee,logpath);
		strcat(logpath,tables->name);
		mkdir(logpath,0777);
		strcat(keyspacee,"keyspace.txt");
		json_dump_file(root,keyspacee,0);
		memtable.tables=tables;
		return true;
}

//creation de keyspace

void create_json_keyspace(){
		json_t*T;
		root=json_array();
						T=json_object();
						data=json_object();
						json_object_set_new(T,"tables",json_array());
						json_object_set_new(data,"keyspace name",json_string(memtable.name));
						json_object_set_new(data,"class",json_string(memtable.class));
						if(strcmp(memtable.class,"\'SimpleStrategy\'")==0)
								json_object_set_new(data,"replication_factor",json_integer(memtable.replication_factor));
							char* logpath=(char*)malloc(sizeof(char));
							strcpy(logpath,memtable.name);
							mkdir(logpath,0777);
							strcat(logpath,"/keyspace.txt");
							json_array_append(root,data);
							json_array_append(root,T);
							json_dump_file(root,logpath,0);
							memtable.name=NULL;
							memtable.class=NULL;
							memtable.success=0;
							memtable.tables=NULL;
}



typetoken char_to_token(char * tok){
	if(strcmp(tok,"MAP")==0){return MAP;}
	if(strcmp(tok,"SET")==0){return SET;}
	if(strcmp(tok,"LIST")==0){return LIST;}
	if(strcmp(tok,"ascii")==0){return ASCII;}
	if(strcmp(tok,"bigint")==0){return BIGINT;}
	if(strcmp(tok,"blob")==0){return BLOB;}
	if(strcmp(tok,"boolean")==0){return BOOLEAN;}
	if(strcmp(tok,"counter")==0){return COUNTER;}
	if(strcmp(tok,"date")==0){return DATE;}
	if(strcmp(tok,"decimal")==0){return DECIMAL;}
	if(strcmp(tok,"double")==0){return DOUBLE;}
	if(strcmp(tok,"float")==0){return FLOAT;}
	if(strcmp(tok,"inet")==0){return INET;}
	if(strcmp(tok,"int")==0){return INT;}
	if(strcmp(tok,"smallint")==0){return SMALLINT;}
	if(strcmp(tok,"text")==0){return TEXT;}
	if(strcmp(tok,"timestamp")==0){return TIMESTAMP;}
	if(strcmp(tok,"timeuuid")==0){return TIMEUUID;}
	if(strcmp(tok,"tinyint")==0){return TINYINT;}
	if(strcmp(tok,"uuid")==0){return UUID;}
	if(strcmp(tok,"varchar")==0){return VARCHAR;}
	if(strcmp(tok,"varint")==0){return VARINT;}
return FALSE;
}


//Interpreter Tokens
char * type_interp_aux(typetoken tok){
	switch(tok){
		case ASCII:
			return "ascii";
			break;
		case BIGINT :
			return "bigint";
			break;
		case BLOB :
			return "blob";
			break;
		case BOOLEAN:
			return "boolean";
			break;
		case COUNTER:
			return "counter";
			break;
		case DATE:
			return "date";
			break;
		case DECIMAL:
			return "decimal";
			break;
		case DOUBLE:
			return "double";
			break;
		case FLOAT:
			return "float";
			break;
		case INET:
			return "inet";
			break;
		case INT:
			return "int";
			break;
		case SMALLINT:
			return "smallint";
			break;
		case TEXT:
			return "text";
			break;
		case TIME:
			return "time";
			break;
		case TIMESTAMP:
			return "timestamp";
			break;
		case TIMEUUID:
			return "timeuuid";
			break;
		case TINYINT:
			return "tinyint";
			break;
		case UUID:
			return "uuid";
			break;
		case VARCHAR:
			return "varchar";
			break;
		case VARINT:
			return "varint";
			break;
	}
	return NULL;
}

char * type_interp(terms_types * t){
	char * type=type=(char*)malloc(sizeof(char));
	switch(t->vartype){
		case MAP:
		case MAP_LIT:
			strcpy(type,"MAP<");
			strcat(type,type_interp(t->term.Map->key));
			strcat(type,",");
			strcat(type,type_interp(t->term.Map->value));
			strcat(type,">");
			break;
		case SET:
		case SET_LIT:
			strcpy(type,"SET<");
			strcat(type,type_interp(t->term.Set->terms));
			strcat(type,">");
			break;
		case LIST:
			strcpy(type,"LIST<");
			strcat(type,type_interp(t->term.List->terms));
			strcat(type,">");
			break;
		default:
			strcpy(type,type_interp_aux(t->vartype));
			break;
	}
	return type;
}
void * read_type_aux(char * type, table_options * t){
	char * mot;
	char * mot2;
	if(strstr(type,",")!=NULL){
		mot=strtok(type,",");
		while(mot!=NULL){
			if(strstr(mot,">")!=NULL){
				mot2=strtok(mot,">");
				while(mot2!=NULL){
					remplire_type(char_to_token(mot2),t);
					mot2=strtok(NULL,">");
				}
			}
			else
				remplire_type(char_to_token(mot),t);

			mot=strtok(NULL,",");
		}

	}else
	{	printf("\ntype for the god damn %s \n",type);
		remplire_type(char_to_token(type),t);
	}
}
void * read_type(char * type, table_options * t){
char * mot;
	if(strstr(type,"MAP")!=NULL || strstr(type,"SET")!=NULL || strstr(type,"LIST")!=NULL){
		printf("WTFTFFTFTFT\n");
		mot=strtok(type,"<");
		while(mot!=NULL){
			printf("type strtok : %s type mot : %s\n",type,mot);
			read_type_aux(mot,t);
			mot=strtok(NULL,"<");
		}
	}
	else{
	remplire_type(char_to_token(type),t);
	}


}

//to be modified
void write_table(){
	terms_types* t_t=NULL;
	table_options*p=tables->fields;
	primary *pri=NULL;
	json_t* root_table=json_array();
	json_t* table_fields=json_object();
	json_t* primary_fields=json_object();
	while(p){
		if(p->name){
			t_t=p->type;
			printf("name :%s type : %s\n",p->name,type_interp(t_t));
			json_object_set_new(table_fields,p->name,json_string(type_interp(t_t)));
		}
		if(p->primary){
			pri=p->primary;
			while(pri){
				printf("name : %s Pt :%d\n",pri->name,pri->partition);
			json_object_set_new(primary_fields,pri->name,json_integer(pri->partition));
				pri=pri->next;
			}
		}
		p=p->next;
	}
	json_array_append(root_table,table_fields);
	json_array_append(root_table,primary_fields);
	char *path=(char*)malloc(sizeof(char)*100);
	strcpy(path,memtable.name);
	strcat(path,"/");
	strcat(path,tables->name);
	strcat(path,"/table.txt");
	printf("%s",path);
	json_dump_file(root_table,path,0);
}
//read table from name
table_options *read_table(char * table_name, char *keyspace_name,table_options* champs){
	table_options* p=NULL;
	primary * r=NULL;
	char *path=(char*)malloc(sizeof(char)*100);
	char * mot=(char*)malloc(sizeof(char));
	strcpy(path,keyspace_name);
	strcat(path,"/");
	strcat(path,table_name);
	printf("table name is %s \n",table_name);
	strcat(path,"/table.txt");
	root = json_load_file(path,0, &error);
	if(root!=NULL){
  	json_t * fields=json_array_get(root,0);
  	const char *key;
	json_t *value;
  	json_object_foreach(fields, key, value){
  		if(champs==NULL){
  			strcpy(mot,json_string_value(value));
  			//printf("key %s values %s \n",key,mot);
  			champs=(table_options*)malloc(sizeof(table_options));
  			champs->name=(char*)malloc(sizeof(char));
  			champs->type=NULL;
  		//	printf("key %s values %s \n",key,mot);
  			strcpy(champs->name,key);
  		//	strcpy(champs->type,json_string_value(value));
  			printf("LOLhahah\n");
  			read_type(mot,champs);
  			champs->next=NULL;
  		//	champs->data=NULL;//Cachh me ousside how abou dat?
  			champs->primary=NULL;
  		//	printf("CHAMPS : \n name: %s value: %s\n",champs->name,type_interp(champs->type));
  		//	printf("DAAAAAAAAAAAAAAASSS\n\ninterpretation %s\n",type_interp(champs->type));

  		}else{
  			p=champs;
  			while(p->next){

  				p=p->next;
  			}
  			p->next=(table_options*)malloc(sizeof(table_options));
  			p=p->next;
  			p->name=(char*)malloc(sizeof(char));
  			p->type=NULL;
  			strcpy(mot,json_string_value(value));
  		//p->next->type=(char*)malloc(sizeof(char));
  			strcpy(p->name,key);
  			printf("key %s values %s \n",key,mot);
  			read_type(mot,p);
  			p->next=NULL;
  		//p->next->data=NULL;//Cachh me ousside how abou dat?
  	//		printf("\n\ninterpretation %s\n",type_interp(p->type));
  			p->primary=NULL;
  		//	printf("CHAMPS : \n name: %s value: %s",p->name,type_interp(p->type));
  		}

  	}
  	 fields=json_array_get(root,1);
  	 int i=json_object_size(fields);
  	 if(i<=1 && i>=0 && champs){
  	 	p=champs;
  	 	while(p){
  	 		if(p->name && json_object_get(fields,p->name)!=NULL){
  	 			p->primary=(primary*)malloc(sizeof(primary));
  	 			p->primary->name=(char*)malloc(sizeof(char));
  	 			strcpy(p->primary->name,p->name);
  	 			p->primary->next=NULL;
  	 			p->primary->partition=1;
  	 		}
  	 		p=p->next;
  	 }
  	 }
  	 else if(i>1 && champs){
  	 	p=champs;
  	 	while(p->next)
  	 		p=p->next;
  	 		p->next=(table_options*)malloc(sizeof(table_options));
  	 		p=p->next;
  	 		p->name=NULL;
  	 		p->type=NULL;
  	 		p->next=NULL;
  	 		p->primary=NULL;
  	 	json_object_foreach(fields, key, value){
  	 		if(p->primary==NULL){
  	 			p->primary=(primary*)malloc(sizeof(primary));
  	 			p->primary->name=(char*)malloc(sizeof(char));
  	 			strcpy(p->primary->name,key);
  	 			p->primary->partition=(int)json_integer_value(value);
  	 			p->primary->next=NULL;
  	 		}else
  	 		{
  	 			r=p->primary;
  	 			while(r->next!=NULL)
  	 				r=r->next;
  	 			r->next=(primary*)malloc(sizeof(primary));
  	 			r=r->next;
  	 			r->name=(char*)malloc(sizeof(char));
  	 			strcpy(r->name,key);
  	 			r->partition=(int)json_integer_value(value);
  	 			r->next=NULL;
  	 		}
  	 	}

  	 }
  	 printf("lol1\n");
  	 //afficherhadchi(champs);
  	 return champs;
  	}//printf("NULLED\n");
  	return NULL;
}

void afficherhadchi(table_options * n){
	table_options* p =n;
	primary * pri=NULL;
	printf("ENTRED\n");
	while(p){
		if(p->name)
		{
			switch(p->type->vartype){
			case MAP :
			printf("\n-----------------------------\nname : %s\n type %s\n",p->name,type_interp(p->type));
			if(p->type->term.Map->key->term.data!=NULL){
				printf("%s",p->type->term.Map->key->term.data->value);
				printf("%s",p->type->term.Map->value->term.data->value);
				}
			break;
			default:
			printf("\n-----------------------------\nname : %s\n type %s\n",p->name,type_interp(p->type));
			if(p->type->term.data!=NULL)
				printf("%s\n",p->type->term.data->value);
			break;
			}
		}
		printf("SEG\n");
		pri=p->primary;
		while(pri!=NULL){
			printf("%s",pri->name);
			pri=pri->next;
		}
		p=p->next;
	}
}

int main(){
memtable.name=NULL;
memtable.class=NULL;
	boolean d;

	/*ta=(table_options*)malloc(sizeof(table_options));
	curs=ta;
	c=ta;
	te=ta;
	primarys=(primary*)malloc(sizeof(primary));
	primary_o=primarys;
	pr=primarys;
	tr=pr;
	ttt=pr;
	errors=(data_error*)malloc(sizeof(data_error));
	errs=errors;*/
	token=_lire_token();
	d=Analyse_syntaxique();
	if(d==true){
		//afficher_semantic_error(errors);
	//	afficherhadchi();
	//	afficher_semantic_error(errs);
	//	printf("KEYSPACE NAME %s",memtable.name);

	}
	else{printf("it doesn't\n");

	afficher_semantic_error(errs);}
}
