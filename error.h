
//TYPE D ERREUR SYMANTIQUE
typedef enum{KDE,KAE,UK,RE,FDE,PAD,CIR,CIC,CIV,TAE,TDE,TCI,DPK}Semantic_error_type;

/*
if(cu->error==KDE){
					printf("\nKeyspace doesn't exist%d\n",cu->cursor);
		}
		else if(cu->error==KAE){
			printf("\nKeyspace already exists%d\n",cu->cursor);
		}
		else if(cu->error==CIC){
			printf("Class invalid%d\n",cu->cursor);
		}
		else if(cu->error==CIV){
			printf("\nClass incompatible with elements%d\n",cu->cursor);
		}
		else if(cu->error==FDE){
			printf("\nPrimary key element  is not a member %d\n",cu->cursor);
		}
*/
typedef struct datae{
	Semantic_error_type error;
	int cursor;
	struct datae* next;

}data_error;


data_error* creer_semantic_error(Semantic_error_type,int ,data_error*);
void afficher_semantic_error(data_error* );
int nbr_semantic_errors(data_error*);
