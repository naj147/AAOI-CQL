#include"cassandra.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


extern int yylex(); 
typetoken token;
boolean follow_token=false;
typetoken _lire_token(){ 
if(follow_token==true)
{
	follow_token=false;//remise a zero du marqueur de surlecture
	return token;//renvoie le token déjà lu
}
else return (typetoken) yylex(); 
} 

boolean cql_type(){

				boolean check;

				if(native_type()){check=true;}
				else if(collection_type()){check=true;}
				else if(tuple_type()){check=true;}
				else if(custom_type()){check=true;}
				else if(user_defined_type()){check=true;}
				else {check=false;}
				return  check;
}

boolean native_type(){

		boolean native;
		printf("native\n");
		if(token==STRING){native=true;}
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
		else if(token==VARCHAR){printf("its varchar \n");native=true;}
		else if(token==VARINT){native=true;}
		else {native=false;}
		return native;

}
boolean IDF(){
		printf("\n-->test idf");
		boolean idf;
		if(token==QUOTED_IDF){idf=true;}
		else if(token==UNQUOTED_IDF){idf=true;}
		else{printf("\nits not a idf");idf=false;}
		return idf;
}
boolean _const(){
	//printf("\nConst");
	boolean con;
	if(token==STRING_TOKEN){printf("\nString valid");
	con=true;}
	else if(token==INUMBER){printf("\nInt valid");
	con=true;}
	else if(token==DNUMBER){printf("\n double valid");
	con=true;}
	else if(token==TRUE){printf("\nboolean true valid;");
	con=true;}
	else if(token==FALSE){printf("\nboolean false valid");
	con=true;}
	else if(token==BLOB_TOKEN){printf("\nBloc valid");
	con=true;}
	else if(token==UUID_TOKEN){printf("\nuid valid");
	con=true;}
	else{printf("\nit's not a const");
	con=false;}
	return con;
}

boolean term(){
		
		boolean ter;
		if(_const()){printf("\nIt's a const");
		ter=true;}

		else if(lit()!=0){printf("\nit's a literal");
		
		ter=true;}

		else if(function_call()){printf("\nit's a function call");
		ter=true;}

		else if(type_hint()){printf("\nit's a typehint");
		ter=true;}

		else if(bind_marker()){printf("\nit's a bind marker");
		ter=true;}

		else{ter=false;}

		return ter;
}
boolean literal(){
	printf("\nliteral test ");
	boolean lit=false;

	if(collection_literal()){lit=true;}

	else if(udt_literal()){lit=true;}

	else if(tuple_literal()){lit=true;}

	else{printf("\nit's not a literal");lit=false;}
	return lit;
}
boolean li(){
	boolean l;
	if(token==ACOLO){
		
	}
}
boolean function_call(){
		printf("\ntest function call ");
		boolean fun;
		if(IDF()){
					token=_lire_token();
					if(token==POPEN){
						
						
									token=_lire_token();
									if(term()){
										token=_lire_token();
										if(isterm()){
											token=_lire_token();
											if(token==PCLOSE){
															fun=true;		
											}
											else{fun=false;}
										}
										else{fun=false;}
									}
									else{fun=false;}
						
					
					}
					else{fun=false;}
		}
		else{printf("\n its not a function call");
		fun=false;}
		return fun;
}
boolean isterm(){
	boolean is;
	if(token==VIRG){
		token=_lire_token();
		if(term()){
				if(token==PCLOSE){
					follow_token=true;
					is=true;

				}
				else{
			token=_lire_token();

					is=isterm();}
		}
		else{is=false;}
	}
	else if(token==PCLOSE){
		follow_token=true;
		is=true;
	}
	else {is=false;}
	return is;

}
boolean type_hint(){
		printf("\ntest type hint");
		boolean hint;
		if(token==POPEN){
			token=_lire_token();
			if(cql_type()){
				token=_lire_token();
				if(token=PCLOSE){
					if(term()){
					hint=true;}
					else{hint=false;}
				}
				else{printf("\nits not a type hint");
				hint=false;}
			}

			else{hint=false;}
		}
		else{printf("\nit's not a type hint");
		hint=false;}
		return hint;
}
boolean bind_marker(){
	printf("\ntest bind marker");
	boolean bind;
	if(token==QST){
		bind=true;
	}
	else if(token==TWOP){
		token=_lire_token();
		if(IDF()){
			bind=true;
		}
		else{printf("\nits not a bind marker");
		bind=false;}
	}
	else{printf("\nit's not a bind marker");
	bind=false;}
	return bind;
}
boolean collection_type(){
	printf("collection");
	boolean col;
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
						else{col=false;}
					}
					else{col=false;}
				}
				else{col=false;}
			}
			else{col=false;} 
		}
		else{col=false;}
	}
	else if(token==SET){
		token=_lire_token();
		if(token==LESSER){
			token=_lire_token();
			if(cql_type()){
				token=_lire_token();
				if(token==BIGGER){
					//printf("its in \n");
					col=true;
				}
				else{col=false;}
			}
			else{col=false;}
		}

		else{col=false;}
	}
	else if(token==LIST){
		token=_lire_token();
		if(token==LESSER){
			token=_lire_token();
			if(cql_type()){
				token=_lire_token();
				if(token==BIGGER){
					col=true;
				}
				else{col=false;}
			}
			else{col=false;}
		}

		else{col=false;}
	}
	else{col=false;}
		return col;
}

boolean  collection_literal(){
	printf("\ncollection");
	boolean collit;
	if(map_literal()){
		printf("\nmapvalid");
		collit=true;
	}
	else if(set_literal()){
		printf("\nset valid");
		collit=true;
	}
	else if(list_literal()){
		printf("\nlist literral");
		collit=true;
	}
	else{collit=false;}
	return collit;
}
boolean map_literal(){
		printf("-->map\n");
		boolean maplit=false;
		if(token==ACOLO){
			printf("\nit's acol");
			token=_lire_token();
			if(term()){
				printf("\nits a term");
				token=_lire_token();
				if(token==TWOP){
					printf("\nits a :");
					token=_lire_token();
					printf("h");
					if(term()){
					printf("\nits another term");
						token=_lire_token();
						if(islit()){
							printf("\nits a lit");
								token=_lire_token();
								if(token==ACOLF){
											maplit=true;

								}
								else{maplit=false;}
						}

						else{maplit=false;}
					}
					else{maplit=false;}
				}
				else{maplit=false;}
				

			}
			else{maplit=false;}
		}
		else{printf("its false so it should not work");
		maplit=false;}
		return maplit;
}
boolean islit(){

		boolean ismlit=false;
		if(token==VIRG){
			printf("\nits a ,");
			token=_lire_token();
			if(term()){
				printf("\nits term 3");
				token=_lire_token();
				if(token==TWOP){
				printf("\nanother twop");
					token=_lire_token();
					if(term()){
					printf("\nalmost there ");
						token=_lire_token();
						if(token==ACOLF){
							printf("\nshit");
							follow_token=true;
							ismlit=true;
						}
						else{
							token=_lire_token();

							ismlit=islit();}
					}
					else{ismlit=false;}

				}
				else{ismlit=false;}

			}
			else{ismlit=false;}
		}
		else if(token==ACOLF){follow_token=true;ismlit=true;}
		else{printf("\nsomething went wrong");
		ismlit=false;}
		return ismlit;
}
boolean set_literal(){
	printf("\n-->set test");
	boolean setlit;

	if(token==ACOLO){
		printf("\n-->its acolo");
		token=_lire_token();
		if(term()){
			token=_lire_token();
			if(isset()){
				token=_lire_token();
				if(token==ACOLF){
					printf("\n--->set valid");
				setlit=true;}
				else{setlit=false;}

			}
			else{setlit=true;}
		}
		else{setlit=false;}

	}
	else{printf("\nit's not a set");
	setlit=false;}
	return setlit;

}
boolean isset(){
	boolean iset;
	if(token==VIRG){
		token=_lire_token();
		if(term()){
			token=_lire_token();
			
			
			//token=_lire_token();

				iset=isset();}

		
		
	}
	else if(token==ACOLF){
		follow_token=true;
		iset=true;
	}
	else{iset=false;}
	return iset;

}

boolean list_literal(){
	printf("\nlist");
	boolean listlit;
	if(token==CROPEN){
		token=_lire_token();
		if(term()){
			if(islist()){
				token=_lire_token();
				if(token==CRCLOSE){
				listlit=true;}
				else{listlit=false;}

			}
			else{listlit=true;}
		}
		else{listlit=false;}

	}
	else{listlit=false;}
	return listlit;


}
boolean islist(){
	boolean islis;
	if(token==VIRG){
		token=_lire_token();
		if(term()){
			token=_lire_token();
			if(token==CRCLOSE){
				follow_token=true;
				islis=true;
			}
			
			else{ 
			token=_lire_token();

				islist();}

		}
		else{islis=false;}
	}
	else if(token==CRCLOSE){
		follow_token=true;
		islis=true;
	}
	else{islis=false;}
	return islis;

}
boolean user_defined_type(){
	if(udt_name()){
		return true;
	}
	else{return false;}
}
boolean udt_name(){

	boolean udtname;
	if(keyspace_name()){
		token=_lire_token();
		if(token==POINT){
			token=_lire_token();
			if(IDF()){

					udtname=true;
			}
			else{udtname=false;}

		}
		else{udtname=false;}
	}
	else{udtname=false;}
	return udtname;

}

boolean field_definition(){
	boolean field;
	if(IDF()){
		token=_lire_token();
		if(cql_type()){
			field=true;
		}
		else{field=false;}
	}
	else{field=false;}
}
int lit(){
	int li=0;
	if(token==ACOLO){
		printf("\n--> acolo");
		token=_lire_token();
		if(term()){
			printf("\n-->its in");
			token=_lire_token();
			if(token==TWOP){
				printf("\n-->:");
				token=_lire_token();
				if(term()){
					token=_lire_token();
					if(islit()){
						token=_lire_token();
						if(token==ACOLF){
							li=1;
						}
					}
				}
			}
			else if(isset()){
				token=_lire_token();
				if(token==ACOLF){
					li=2;
				}
			}
		}
		else if(IDF()){
			printf("\n-->tuple test");
			token=_lire_token();
			if(token==TWOP){

				token=_lire_token();
				if(term()){
					token=_lire_token();
					if(is_udt()){
						token=_lire_token();
						if(token==ACOLF){
							li=3;
						}
					}
				}
			}
			

		}


	}
	else if(token==CROPEN){
		token=_lire_token();
		if(term()){
			token=_lire_token();
			if(islist()){
				token=_lire_token();
				if(token==CRCLOSE){
					li=4;
				}
			}
		}

	}
	else if(token==POPEN){
		token=_lire_token();
		if(term()){
			token=_lire_token();
			if(istuplit()){
				token=_lire_token();
				if(token==PCLOSE){
					li=5;
				}
			}
		}
	}
	return li;

}
boolean create_type_statement(){
	boolean create;
	if(token==CREATE){
		token=_lire_token();
		if(token==TYPE){
			if(udt_name()){
				token=_lire_token();
				if(token==POPEN){
					token=_lire_token();
					if(field_definition()){
						token=_lire_token();
						if(is_field()){
							token=_lire_token();
							if(token==PCLOSE){
								create=true;
							}
							else{create=false;}
						}
						else{create=false;}

					}
					else{create=false;}
				}
				else{create=false;}

			}
			else{create=false;}
		}
		else{create=false;}
	}
	else{create=false;}
	return create;
}
boolean is_field(){
	boolean isf;
	if(token==VIRG){
		token=_lire_token();
		if(field_definition()){
			token=_lire_token();

			isf=is_field();
		}
	}
	else if(token==PCLOSE){
		follow_token=true;
		isf=true;
	}
	else{isf=false;}
	return isf;
}
boolean udt_literal(){
	boolean udtl;
	if(token==ACOLO){
		token=_lire_token();
		if(IDF()){
			token=_lire_token();
			if(token=TWOP){
				token=_lire_token();
				if(term()){
					token=_lire_token();
					if(is_udt()){
						token=_lire_token();
						if(token==ACOLF){
							udtl=true;
						}

						else{udtl=false;}
					}
					else{udtl=false;}
				}
				else{udtl=false;}
			}
			else{udtl=false;}
		}
		else{udtl=false;}
	}
	else{udtl=false;}
	return udtl;


}
boolean is_udt(){
	boolean isu;
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
				else{isu=false;}

			}
			else{isu=false;}
		}
		else{isu=false;}
	}
	else if(token==ACOLF){
		follow_token=true;
		isu=true;
	}
	else{isu=false;}
	return isu;
}
boolean alter_type_statement(){
	boolean altert;
	if(token==ALTER){
		token=_lire_token();
		if(token==TYPE){
			token=_lire_token();
			if(udt_name()){
				token=_lire_token();
				if(alter_type_modification()){
					altert=true;
				}
				else{altert=false;}
			}
			else{altert=false;}

		}
		else{altert=false;}

	}
	else{altert=false;}
	return altert;
}
boolean alter_type_modification(){
	boolean altm;
	if(token==ALTER){
				token=_lire_token();
				if(IDF()){
					token=_lire_token();
					if(token==TYPE){
						token=_lire_token();
						if(cql_type()){
							altm=true;
						}
						else{altm=false;}
					}
					else{altm=false;}
				}
				else{altm=false;}
	}
	else if(token==ADD){
		token=_lire_token();
		if(field_definition()){
			altm=true;

		}
		else{altm=false;}

	}
	else if(token==RENAME){
		token=_lire_token();
		if(is_alt()){
			altm=true;
		}
		else{altm=false;}

	}
	else{altm=false;}
	return altm;

}
boolean is_alt(){
	boolean isa,dt=false;
	if(IDF()){
		token=_lire_token();
		if(token==TO_TOKEN){
			token=_lire_token();
			if(IDF()){
				dt=true;
				token=_lire_token();

				isa=is_alt();
			}
			else{isa=false;}
		}
		else{isa=false;}

	}
	else {
		if(dt==true){
			isa=true;
		}
		else{isa=false;}
	}
	return isa;
}
boolean drop_type_statement(){
	boolean drop;
	if(token==DROP){
		token=_lire_token();
		if(token==TYPE){
			token=_lire_token();
			if(udt_name()){
				drop=true;
			}
			else{drop=false;}
		}
		else{drop=false;}
	}
	else{drop=false;}
	return drop;

}
boolean tuple_type(){
	boolean tuple;
	printf("  1\n");
	if(token==TUPLE){
		token=_lire_token();
			printf("  2\n");
		if(token==LESSER){
			token=_lire_token();
			printf("  3\n");
			if(cql_type()){
				printf("  4\n");
				token=_lire_token();
				if(istuple()){
					printf("  5\n");
					token=_lire_token();
					if(token==BIGGER){
						tuple=true;
					}
					else{tuple=false;}
				}
				else{tuple=false;}

			}
			else{tuple=false;}
		}
		else{tuple=false;}
	}
	else{tuple=false;}
	return tuple;
}
boolean istuple(){
	boolean ist;
	printf("  4.1\n");
	if(token==VIRG){
		printf("  4.2\n");
		token=_lire_token();
		if(cql_type()){
			printf("  4.3\n");
			token=_lire_token();
			ist=istuple();
		}
		else{ist=false;}
	}
	else if(token==BIGGER){
		follow_token=true;
		ist=true;
	}
	else{ist=false;}
	return ist;
}
boolean keyspace_name(){
	boolean key=false;
	if(name()){
		key=true;
	}
	return key;
}
boolean tuple_literal(){
	boolean tup;
	if(token==POPEN){
		token=_lire_token();
		if(term()){
			token=_lire_token();
			if(istuplit()){
				token=_lire_token();
				if(token==PCLOSE){
					tup=true;
				}
				else{tup=false;}
			}
			else{tup=false;}
		}
		else{tup=false;}
	}
	else{tup=false;}
	return tup;
}
boolean istuplit(){
	boolean isti;
	if(token==VIRG){
		token=_lire_token();
		if(term()){
			token=_lire_token();

			isti=istuplit();
		}
		else{isti=false;}
	}
	else if(token==PCLOSE){
		follow_token=true;
		isti=true;
	}
	else{isti=false;}
	return isti;
}

boolean custom_type(){
	boolean custom;
	if(token==STRING_TOKEN){
			custom=true;
	}
	else{custom=false;}
	return custom;
}
boolean name(){
	boolean na;
	if(token==QUOTED_IDF){na=true;}
	else if(token=UNQUOTED_IDF){na=true;}
	else{na=false;}
}
boolean column_name(){
	boolean col;
	if(IDF()){
		col=true;

	}
	else{col=false;}
	return col;
}
boolean table_name(){
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
}
boolean select_statement(){
	printf("select_test");

	boolean select=false;
	if(token==SELECT){
		printf("\nSELECT");
		token=_lire_token();
		if(type_imput()){

			token=_lire_token();
			if((select_clause())||(token==MULT)){
				printf("\n-->select clause works");
			token=_lire_token();
			if(token==FROM){
				token=_lire_token();
				if(table_name()){
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
			else{select=false;}


		}
		}
		else if((select_clause())||(token==MULT)){
				printf("\n-->select clause works");
			token=_lire_token();
			if(token==FROM){
				token=_lire_token();
				if(table_name()){
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
boolean operator(){
	boolean ope=false;
	printf("\n--->operator test");
	if(token==IN){ope=true;}
	else if(token==EQ){ope=true;}
	else if(token==LESSEQ){ope=true;}
	else if(token==BIGEQ){ope=true;}
	else if(token==CONTAINS){ope=true;}
	else if(token==CONT_KEY){ope=true;}
	else if(token==LESSER){ope=true;}
	else if(token==BIGGER){ope=true;}
	else if(token==NOEQ){ope=true;}
	return ope;
}
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
boolean ascdesc(){
	boolean asc;
	if((token==ASC)||(token==DESC)){
		asc=true;

	}
	else{asc=false;}
	return asc;

}

boolean  function_name(){
	boolean fun;
	if(IDF()){
		fun=true;
	}
	else{fun=false;}
}
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
boolean update_parameter(){
	boolean updp=false;
	if((token==TIMESTAMP)||(token==TTL)){
		token==_lire_token();
		if((token==INUMBER)||(bind_marker())){updp=true;}
	}
	return updp;

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
boolean assigement(){
	boolean ass=false;
	
	if(simple_selection()==3){
		printf("\n-->simple");
		token=_lire_token();
		if(token==EQ){
			printf("\n-->eq");
			token=_lire_token();
			if(term()){
				printf("\n-->term");
				ass=true;
			}
		}
	}
	else if((simple_selection()!=0)){
		printf("---> thing");
		token=_lire_token();
		if(token==EQ){
			token=_lire_token();
			if(lit()==4){
				token=_lire_token();
				if(token==PLUS){
					token=_lire_token();
					if(column_name()){
						ass=true;
					}
				}
			}
			else if(column_name()){
				token=_lire_token();
				if((token==MINUS)||(token==PLUS)){
					token=_lire_token();
					if(term()){
						ass=true;
					}
				}
			}
		}

	}
		return ass;
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
int main(){
	boolean d;
	token=_lire_token();
	if(update_statement()){printf("\nupdate works");}
	else if(select_statement()){printf("\nits select ");}
	else{printf("\nits no good");}
	return 0;
}













