#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//costante usata per allocare memoria dinamicamente
#define DIM 50
#define SIZE 200
#define ESIZE 10039
#define PRIME 43



//enumerazione usata per alberi rb
enum ncolor{BLACK, RED};
//tipo usato per l'albero delle entità
typedef struct eeTree{
    char *name;
    struct eeTree*next,*prev;
}eTree;

//tipo usato per ogni albero delle relazioni
typedef struct rrTree{
    eTree*receiver;
    eTree*sender;
    struct rrTree*l,*r,*p;
    enum ncolor color;
}rTree;

//tipo usato per l'albero del tipo di relazioni
typedef struct rrTable{
    char*rel;
    rTree**r;
    rTree*root;
    struct rrTable*p,*l,*ri;
    int nmax;
    unsigned short i;
    eTree**w;
    rTree*nil_node;
}rTable;




//funzione di hash
unsigned int eHash(char*key);
//funzione per cercare nell'albero delle entità/relazioni
eTree*eSearch(eTree**e,char*key);
rTree*rSearch(rTable*t,rTree*r,char*s,char*d);
rTree*rSearch1(rTable*t,rTree*r,char*key);
rTable*tSearch(rTable*tt,char*key);
rTable*tSearchNull(rTable*tt);
//funzioni di ricerca del minimo e del prossimo elemento
rTree *rMin(rTable*t,rTree*r);
rTree *rNext(rTable*t,rTree*r);
rTable*tMin(rTable*r);
rTable*tNext(rTable*r);
//funzioni per inserire/eliminare nell'albero delle entità/relazioni
void eInsert(eTree**r,char*key);
void rInsert(rTable*t,rTree**r,eTree*s,eTree*d);
void tInsert(rTable**r,rTable*key);
void eDelete(eTree**r,eTree*node);
void rNewDelete(rTable*t,rTree**r,rTree*node);
void tDelete(rTable**r,rTable*node);

//funzione per creare/scambiare nodi dell'albero di entità
eTree*eCreate(char*key,eTree*p);
rTree*rCreate(rTable*r,eTree*s,eTree*d,rTree*p);
rTable*tCreate(char*key,rTable*p);

//funzioni di bilanciamento
void rNewInsertBalance(rTable*t,rTree**r,rTree*x);
void rNewDeleteBalance(rTable*t,rTree**r,rTree*node);
//funzioni di rotazione albero/i delle relazioni
void rRotateL (rTable*t,rTree**r, rTree* node);
void rRotateR (rTable*t,rTree**r, rTree* node);

//funzione di gestione max entità riceventi
void tUpdate(rTable*a);






int main() {
    //comandi in input
    const char com1[] = "addent";
    const char com2[] = "delent";
    const char com3[] = "addrel";
    const char com4[] = "delrel";
    const char com5[] = "report";
    const char com6[] = "end";

    //stringa di controllo strtok
    const char pSpace[] =" ";
    //inizializza albero delle entità
    eTree*eTable[ESIZE];
    for(short df=0;df<ESIZE;df++)
        eTable[df]=NULL;
    rTable**K;
    rTable*kroot=NULL;
    K=&kroot;

    while(1){
        //predispone la memoria per la stringa di input
        char *pString = (char*)malloc(DIM);
        char *pController = NULL;
        if (pString == NULL) 
            return -1;
        int i = 0; //indice per scorrere la stringa d'ingresso
        short sz = 1;//indice per gestire la dimensione della stringa d'ingresso
        char ch;//carattere in input

        //gestore sequenza fornita in input
        while ((ch = getchar()) != '\n') {
            pString[i] = ch;
            i++;
            if (i % DIM == 0) {
                sz++;
                pString = (char*)realloc(pString, sz * DIM* sizeof(char));
            }
        }
        pString[i] = '\0';


        //analisi comando
        pController = strtok(pString, pSpace);


        //comando aggiungi entità
        if(strcmp(pController,com1)==0) {
            //legge il nome dell'entità e verifica che non sia presente
            pController = strtok(NULL, pSpace);
            if(eSearch(eTable,pController)==NULL)
                eInsert(eTable, pController);
            free(pString);


        //comando elimina entità
        }else if(strcmp(pController,com2)==0){
            pController=strtok(NULL,pSpace);
            eTree*toDel=eSearch(eTable,pController);
            if(toDel==NULL){
                free(pString);
                continue;
            }
            rTable*yy=tMin(*K);
            while(yy!=NULL){
                rTree*xxx=rSearch1(yy,yy->root,pController);
                while(xxx!=NULL && xxx!=yy->nil_node){
                    rNewDelete(yy,yy->r,xxx);
                    xxx=rSearch1(yy,yy->root,pController);
                }
                tUpdate(yy);
                yy=tNext(yy);
            }
            rTable*zzz=tSearchNull(*K);
            while(zzz!=NULL){
                tDelete(K,zzz);
                zzz=tSearchNull(*K);
            }
            eDelete(eTable,toDel);
            free(pString);


        //comando aggiungi relazione
        }else if(strcmp(pController,com3)==0) {
            char *e1 = strtok(NULL, pSpace);
            char *e2 = strtok(NULL, pSpace);
            eTree *n1 = eSearch(eTable, e1);
            eTree *n2 = eSearch(eTable, e2);
            if (n1 == NULL || n2 == NULL) {
                free(pString);
                continue;
            }
            char *r0 = strtok(NULL, pSpace);
            rTable *temp = tSearch(kroot, r0);
            if (temp == NULL) {
                temp = tCreate(r0, NULL);
                tInsert(K, temp);
                temp->w = malloc(SIZE * sizeof(eTree *));
                rInsert(temp,temp->r, n1, n2);
                tUpdate(temp);

            }else{
            if (rSearch(temp,*temp->r, e1, e2) != NULL) {
                free(pString);
                continue;
            }
            rInsert(temp,temp->r, n1, n2);
            tUpdate(temp);

        }
            free(pString);

        //comando elimina relazione
        }else if(strcmp(pController,com4)==0){
            char*e1=strtok(NULL, pSpace);
            char*e2=strtok(NULL, pSpace);
            eTree*n1=eSearch(eTable,e1);
            eTree*n2=eSearch(eTable,e2);
            if(n1==NULL || n2==NULL){
                free(pString);
                continue;
            }
            char*r0=strtok(NULL,pSpace);
            rTable*temp=tSearch(*K,r0);
            if(temp==NULL){
                free(pString);
                continue;
            }
            rTree*toDel=rSearch(temp,*temp->r,e1,e2);
            if(toDel!=NULL){
                rNewDelete(temp,temp->r,toDel);
                if(temp->root==NULL || temp->root==temp->nil_node) {
                    tDelete(K, temp);
                }
                else{
                    for(short kkkk=0;kkkk<SIZE*temp->i && temp->w[kkkk]!=NULL;kkkk++){
                        if(strcmp(temp->w[kkkk]->name,n2->name)==0)
                            tUpdate(temp);
                    }
                }

            }
            free(pString);



        //comando report
        }else if(strcmp(pController,com5)==0){
            if(*K==NULL)
                fputs("none\n",stdout);
            else{
                rTable*ytemp=tMin(*K);
                while(ytemp!=NULL){
                    fputs(ytemp->rel,stdout);
                    fputs(" ",stdout);
                    for (int j = 0; j < ytemp->i*SIZE; j++) {
                        if(ytemp->w[j]!=NULL) {
                            fputs(ytemp->w[j]->name,stdout);
                            fputs(" ",stdout);
                        }else
                            break;
                    }
                    printf("%d",ytemp->nmax);
                    fputs(";",stdout);
                    ytemp=tNext(ytemp);
                    if(ytemp!=NULL)
                        fputs(" ",stdout);
                }
                fputs("\n",stdout);
            }

            free(pString);
        }else if(strcmp(pController,com6)==0){
            fputs("\n",stdout);
            return 0;
        }else
        free(pString);
        }
}









//funzione di ricerca nell'albero delle entità'
eTree* eSearch(eTree**e, char*key){
    eTree*ee=e[eHash(key)];
    while(ee!=NULL) {
        if (strcmp(ee->name, key) == 0)
            return ee;
        ee=ee->next;
    }
    return ee;
}

//funzione di inserimento/cancellazione di un nodo nell'albero
void eInsert(eTree**r,char*key){
    if(r[eHash(key)]==NULL)
        r[eHash(key)]=eCreate(key,NULL);
    else{
        eTree*etemp=r[eHash(key)];
        while(etemp->next!=NULL)
            etemp=etemp->next;
        etemp->next=eCreate(key,etemp);
    }

}
void eDelete(eTree**r,eTree*node){
    if(node->prev==NULL) {
        r[eHash(node->name)] = node->next;
        if(node->next!=NULL)
            node->next->prev=NULL;
    }else{
        node->prev->next=node->next;
        if(node->next!=NULL)
            node->next->prev=node->prev;
        }
    free(node->name);
    free(node);

}

//funzione di creazione/sostituzione nuovo nodo
eTree*eCreate(char*key,eTree*p){
    eTree*n = (eTree*)malloc(sizeof(eTree));
    n->name=(char*)malloc(strlen(key)+1);
    strcpy(n->name,key);
    n->next=NULL;
    n->prev=p;
    return n;
}
rTree*rCreate(rTable*r,eTree*s,eTree*d,rTree*p){
    rTree*n = (rTree*)malloc(sizeof(rTree));
    n->l=r->nil_node;
    n->r=r->nil_node;
    n->sender=s;
    n->receiver=d;
    n->p=p;
    n->color=RED;
    return n;
    
}


//funzione di ricerca per relazioni
rTree*rSearch(rTable*t,rTree*r,char*s,char*d){
    if(r==t->nil_node)
        return NULL;
    if(strcmp(s,r->sender->name)==0 && strcmp(d,r->receiver->name)==0)
        return r;
    if(strcmp(d,r->receiver->name)<0 || (strcmp(d,r->receiver->name)==0 && strcmp(s,r->sender->name)<0))
        return rSearch(t,r->l,s,d);
    else
        return rSearch(t,r->r,s,d);
}


//funzione di aggiunta relazioni
void rInsert(rTable*t,rTree**r,eTree*s,eTree*d){
    if(t->root==NULL || t->root==t->nil_node){
        t->root=rCreate(t,s,d,t->nil_node);
        t->root->color=BLACK;
        return;
    }
    rTree*xtemp=*r;
    while(xtemp!=NULL && xtemp!=t->nil_node){
        if(strcmp(d->name,xtemp->receiver->name)<0 ||(strcmp(d->name,xtemp->receiver->name)==0 && strcmp(s->name,xtemp->sender->name)<0)) {
            if(xtemp->l!=t->nil_node)
                xtemp = xtemp->l;
            else{
                xtemp->l=rCreate(t,s,d,xtemp);
                rNewInsertBalance(t,r,xtemp);
                return;
            }
        }
        else{
            if(xtemp->r!=t->nil_node)
                xtemp = xtemp->r;
            else{
                xtemp->r=rCreate(t,s,d,xtemp);
                rNewInsertBalance(t,r,xtemp);
                return;
            }
        }
    }

}

void rRotateL (rTable*t,rTree**r, rTree* node){
    rTree*B = node->r;
    rTree*BLeft = B->l;
    rTree*parent = node->p;

    B->p = parent;
    B->l = node;
    node->r = BLeft;
    node->p = B;

    if (BLeft!=t->nil_node && BLeft!=NULL)
        BLeft->p = node;

    if (node == *r) {
        *r = B;
    } else if (parent->r == node) {
        parent->r = B;
    } else {
        parent->l = B;
    }


    return;
}
void rRotateR (rTable*t,rTree**r, rTree* node){
    rTree* B = node->l;
    rTree* BRight = B->r;
    rTree*parent = node->p;

    B->p = parent;
    B->r = node;
    node->l = BRight;
    node->p = B;

    if (BRight!=NULL && BRight!=t->nil_node) {
        BRight->p = node;
    }

    if (node == *r) {
        *r = B;
    } else if (parent->l == node) {
        parent->l = B;
    } else {
        parent->r = B;
    }

    return;

}

rTree *rSearch1(rTable*t,rTree *r, char *key) {
    if(r==NULL || r==t->nil_node)
        return NULL;
    rTree*toFind=rMin(t,r);
    while(toFind!=NULL && toFind!=t->nil_node) {
        if (strcmp(toFind->receiver->name, key) == 0 || strcmp(toFind->sender->name,key) == 0)
            return toFind;
        toFind=rNext(t,toFind);
    }
    return NULL;
}

rTree *rMin(rTable*t,rTree*r){
    while (r->l!=NULL && r->l!=t->nil_node)
        r=r->l;
    return r;

}

rTree *rNext(rTable*t,rTree *r) {

    if (r->r !=t->nil_node) {
        r = r->r;
        while (r->l!=t->nil_node) {
            r = r->l;
        }
        return r;
    }
    rTree*nTemp = r->p;
        while (nTemp!= NULL && nTemp!=t->nil_node && r==nTemp->r) {
            r=nTemp;
            nTemp=nTemp->p;
        }
    return nTemp;
}

rTable *tSearch(rTable *tt, char *key) {
    if(tt==NULL)
        return tt;
    if(strcmp(tt->rel,key)==0)
        return tt;
    if(strcmp(key,tt->rel)<0)
        return tSearch(tt->l,key);
    else
        return tSearch(tt->ri,key);
}

rTable *tCreate(char *key, rTable *p) {
    rTable*o=(rTable*)malloc(sizeof(rTable));
    o->l=NULL;
    o->ri=NULL;
    o->p=p;
    o->rel=(char*)malloc(strlen(key)+1);
    strcpy(o->rel,key);
    o->nmax=0;
    o->i=1;
    o->w=NULL;
    o->r=&o->root;
    o->nil_node=(rTree*)malloc(sizeof(rTree));
    o->nil_node->r=o->nil_node;
    o->nil_node->l=o->nil_node;
    o->nil_node->p=o->nil_node;
    o->nil_node->sender=NULL;
    o->nil_node->receiver=NULL;
    o->nil_node->color=BLACK;
    o->root=o->nil_node;
    return o;
}

void tInsert(rTable **r, rTable *key) {
    rTable*y=NULL;
    rTable*z=*r;
    while(z!=NULL){
        y=z;
        if(strcmp(key->rel,z->rel)<0)
            z=z->l;
        else
            z=z->ri;
    }
    key->p=y;
    if(y==NULL)
        *r=key;
    else if(strcmp(key->rel,y->rel)<0)
        y->l=key;
    else
        y->ri=key;

}

void tUpdate(rTable *a) {
    if(a==NULL)
        return;
    if(a->root!=NULL && a->root!=a->nil_node) {
        int v = 0;
        short j = 0;
        a->nmax=0;
        rTree *bb = rMin(a,a->root);
        eTree *eAnalyzed = bb->receiver;
        while (bb != NULL && bb!=a->nil_node) {
            eTree*actual=bb->receiver;
            if(strcmp(eAnalyzed->name,actual->name)==0){
                v++;
            }else {
                v = 1;
                eAnalyzed=actual;
            }
            if(v>a->nmax){
                for (short hh = 0; hh < SIZE * a->i; hh++)
                    a->w[hh] = NULL;
                a->nmax=v;
                a->w[0]=actual;
                j=0;
            }else if(v==a->nmax){
                j++;
                if (j > (SIZE * a->i) - 1) {
                    a->i++;
                    a->w=realloc(a->w,SIZE* sizeof(eTree*)*a->i);
                    for (short kk = SIZE * (a->i - 1); kk < SIZE * a->i; kk++)
                        a->w[kk] = NULL;
                }
                a->w[j]=actual;
            }

            bb=rNext(a,bb);
        }

    }
}

rTable *tMin(rTable *r) {
    if(r==NULL)
        return r;
    while(r->l!=NULL)
        r=r->l;
    return r;
}

rTable *tNext(rTable *r) {
    if (r->ri != NULL) {
        r = r->ri;
        while (r->l != NULL) {
            r = r->l;
        }
        return r;
    } else {
        rTable*nTemp = r->p;
        while (nTemp != NULL && r == nTemp->ri) {
            r = nTemp;
            nTemp = nTemp->p;
        }
        return nTemp;
    }


}

void tDelete(rTable **r, rTable *node) {
    if(node->w!=NULL){
        free(node->w);
        node->w=NULL;
    }
    if(node->rel!=NULL){
        free(node->rel);
        node->rel=NULL;
    }
    if(node->nil_node!=NULL) {
        free(node->nil_node);
        node->nil_node = NULL;
    }
    rTable*atemp=NULL;
    if(node->l==NULL || node->ri==NULL)
        atemp=node;
    else
        atemp=tNext(node);
    rTable*x=NULL;
    if(atemp->l!=NULL)
        x=atemp->l;
    else
        x=atemp->ri;
    if(x!=NULL)
        x->p=atemp->p;
    if(atemp->p==NULL)
        *r=x;
    else if(atemp==atemp->p->l)
        atemp->p->l=x;
    else
        atemp->p->ri=x;
    if(atemp!=node){
        node->root=atemp->root;
        node->nil_node=atemp->nil_node;
        node->i=atemp->i;
        node->rel=atemp->rel;
        node->w=atemp->w;
        node->nmax=atemp->nmax;
    }
     free(atemp);

}

rTable *tSearchNull(rTable *tt) {
    if(tt==NULL)
        return tt;
    else{
        rTable*fTemp=tMin(tt);
        while(fTemp!=NULL) {
            if(fTemp->root==NULL || fTemp->root==fTemp->nil_node)
                return fTemp;
            fTemp = tNext(fTemp);
        }
        return fTemp;
    }
}

unsigned int eHash(char *key) {
    unsigned int hash = 0;
    for(int l=0;l<strlen(key);l++){
        if(key[l]!='\"')
            hash = PRIME*hash+(unsigned int)key[l];
    }
    hash = hash%ESIZE;
    return hash;
}

void rNewInsertBalance(rTable*t,rTree**r,rTree *x) {
if(x->p==NULL || x->p==t->nil_node) {
    x->color = BLACK;
}else if(x->p!=NULL && x->p->color==RED){
    rTree*uncle=NULL;
    if(x->p==x->p->p->l)
        uncle=x->p->p->r;
    else
        uncle=x->p->p->l;
    if(uncle==NULL || uncle->color==BLACK){
        if(x==x->p->l && x->p==x->p->p->l){
            bool ctemp=x->p->color;
            x->p->color=x->p->p->color;
            x->p->p->color=ctemp;
            rRotateR(t,r,x->p->p);
        }else if(x==x->p->r && x->p==x->p->p->l){
            rRotateL(t,r,x->p);
            bool ctemp=x->p->color;
            x->p->color=x->color;
            x->color=ctemp;
            rRotateR(t,r,x->p);
        }else if(x==x->p->r && x->p==x->p->p->r){
            bool ctemp=x->p->color;
            x->p->color=x->p->p->color;
            x->p->p->color=ctemp;
            rRotateL(t,r,x->p->p);
        }else if(x==x->p->l && x->p==x->p->p->r){
            rRotateR(t,r,x->p);
            bool ctemp=x->p->color;
            x->p->color=x->color;
            x->color=ctemp;
            rRotateR(t,r,x->p);
        }
    }else{
        x->p->color=BLACK;
        uncle->color=BLACK;
        x->p->p->color=RED;
        rNewInsertBalance(t,r,x->p);
    }
}
return;
}

void rNewDelete(rTable*t,rTree **r, rTree *node) {
    rTree*y=NULL;
  if(node->l==t->nil_node || node->r==t->nil_node)
      y=node;
  else
      y=rNext(t,node);
  rTree*x=NULL;
  if(y->l!=t->nil_node)
      x=y->l;
  else
      x=y->r;
  x->p=y->p;
  if(y->p==t->nil_node)
      *r=x;
  else if(y==y->p->l)
      y->p->l=x;
  else
      y->p->r=x;
  if(y!=node){
      node->receiver=y->receiver;
      node->sender=y->sender;
  }
  if(y->color==BLACK)
      rNewDeleteBalance(t,r,x);
  t->nil_node->color=BLACK;
  t->nil_node->p=t->nil_node;
  t->nil_node->l=t->nil_node;
  t->nil_node->r=t->nil_node;
  free(y);
  return;


}

void rNewDeleteBalance(rTable *t, rTree **r, rTree *node) {
    rTree*w=NULL;
    if(node->color==RED || node->p==t->nil_node)
        node->color=BLACK;
    else if(node==node->p->l){
        w=node->p->r;
        if(w->color==RED){
            w->color=BLACK;
            node->p->color=RED;
            rRotateL(t,r,node->p);
            w=node->p->r;
        }
        if(w->l->color==BLACK && w->r->color==BLACK){
            w->color=RED;
            rNewDeleteBalance(t,r,node->p);
        }else if(w->r->color==BLACK){
            w->l->color=BLACK;
            w->color=RED;
            rRotateR(t,r,w);
            w=node->p->r;
        }
        w->color=node->p->color;
        node->p->color=BLACK;
        w->r->color=BLACK;
        rRotateL(t,r,node->p);
    }else{
        w=node->p->l;
        if(w->color==RED){
            w->color=BLACK;
            node->p->color=RED;
            rRotateR(t,r,node->p);
            w=node->p->l;
        }
        if(w->r->color==BLACK && w->l->color==BLACK){
            w->color=RED;
            rNewDeleteBalance(t,r,node->p);
        }else if(w->l->color==BLACK){
            w->r->color=BLACK;
            w->color=RED;
            rRotateL(t,r,w);
            w=node->p->l;
        }
        w->color=node->p->color;
        node->p->color=BLACK;
        w->l->color=BLACK;
        rRotateR(t,r,node->p);
    }
}
