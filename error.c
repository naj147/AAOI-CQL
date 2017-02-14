#include <stdio.h>
#include <stdlib.h>
#include "error.h"

data_error* creer_semantic_error(Semantic_error_type err,int cur , data_error* dte){
	data_error* p=(data_error*)malloc(sizeof(data_error));
	data_error* q=dte;
	p->next=NULL;
	p->cursor=cur;
	p->error=err;

	if(dte==NULL){
		dte=p;
	}
	else
	{
		while(q->next!=NULL)
			q=q->next;
		q->next=p;
	}
	return dte;
}
void afficher_semantic_error(data_error* errors){
	data_error* cu=errors;
	while(cu!=NULL){
		switch(cu->error){
			case KDE:
				printf("\nKeyspace doesn't exist%d\n",cu->cursor);
				break;
			case KAE:
				printf("\nKeyspace already exists%d\n",cu->cursor);
				break;
			case UK:
				printf("\nKeyspace is not defined %d\n",cu->cursor);
				break;
			case CIC:
				printf("Class invalid%d\n",cu->cursor);
				break;
			case CIV:
				printf("\nClass incompatible with elements%d\n",cu->cursor);
				break;
			case FDE:
				printf("\nPrimary key element is not a member %d\n",cu->cursor);
				break;
			case DPK:
				printf("\nDouble definition of primary key%d\n",cu->cursor);
				break;
			case TAE:
				printf("\nTable Already Exists%d\n",cu->cursor);
				break;
			case TDE:
				printf("\nTable Doesnt Exist%d\n",cu->cursor);
				break;
		}
		cu=cu->next;
	}

}
int nbr_semantic_errors(data_error* errs){
	data_error* cu=errs;
	
	int counter=0;
	while(cu!=NULL){
		//printf("\nerror");
		counter++;
		cu=cu->next;

	}
	if(counter==0)
		printf("THERE IS NO ERROR");
	return counter;
}