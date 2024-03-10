#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

typedef struct lf{
    char *dato;
    struct lf *left;
    struct lf *right;
} leaf;
typedef leaf *bst;

typedef struct nd{
    char *dato;
    struct nd *next;
}node;
typedef node *accepted;

typedef struct info{
    char simbolo;
    int *dove_sta;
    int *dove_non_sta;
    int m_ripetizioni[2];
    int e_ripetizioni;
    struct info *prox;
} info_simbolo;
typedef info_simbolo *archivio;

typedef struct node1{
    bst p;
    struct node1 *next;
}Dstack;
typedef Dstack *Dstack_p;


char NG[]="+nuova_partita";
char SF[]="+stampa_filtrate";
char II[]="+inserisci_inizio";
char IF[]="+inserisci_fine";
char OK[]="ok";
char KO[]="ko";

int search(bst root, char *value);    //molto usata
bst store(bst ptr, char *str, int len);            //molto usata
void print_tree(bst root);
void deleteTree(bst node);
char *new_game(bst root, int len, int adj_len);
bst add_words0(bst p, int len);
bst add_words1(bst p, accepted *lis, archivio ptr, int len);
archivio aggiornamento_archivio(archivio lis, char c, int pos, int interruttore);
void free_archivio(archivio head);
void correction(archivio head);
int stampa_filtrate1(bst root, accepted *lis, archivio head, int len);
int stampa_filtrate2(accepted *lis, archivio head, int len);
int check_archivio(char *str, archivio cur_a, int len);
accepted newNode(char *str, int len);
void push(Dstack_p *top_ref, bst p);
bst pop(Dstack_p *top_ref);
int empty_check(Dstack_p top);
void print_list(accepted head);
void delete_list(accepted head);
//da modificare, ma funzionano
void MergeSort(accepted *headRef);
accepted SortedMerge(accepted a, accepted b);
void FrontBackSplit(accepted source, accepted *frontRef, accepted *backRef);

int main() {
    bst pointer=NULL;
    int k, len;
    char *input;
    if(scanf("%d", &k)==0)
        exit(1);
    len=k;
    if(k<17)
        len=17;
    input=(char *)malloc(sizeof(char)*(len+1));
    while (!feof(stdin)){
        if(scanf("%s", input)==0)
            exit(1);
        //printf("%s\n", input);
        if(strcmp(input, NG)==0){
            printf("%s\n", new_game(pointer, k, len));
        }
        if(strcmp(input, II)==0){
            pointer=add_words0(pointer, k);
        }
        if((strcmp(input, NG)!=0)&&(strcmp(input, II)!=0))
            pointer = store(pointer, input, k);
        strcpy(input, "");
    }
    free(input);
    deleteTree(pointer);
    return 0;
}

bst store(bst ptr, char *str, int len) {
    bst node = (bst) malloc(sizeof(leaf));
    node->dato = (char *) malloc(len+1);
    strcpy(node->dato, str);
    node->right = NULL;
    node->left = NULL;
    if(!ptr)
        return node;
    bst cur = ptr, pre = NULL;
    while (cur) {
        pre = cur;
        if (strcmp(str, cur->dato) < 0)
            cur = cur->left;
        else
            cur = cur->right;
    }
    if (strcmp(str, pre->dato) < 0)
        pre->left = node;
    else
        pre->right = node;
    return ptr;
}

void print_tree(bst root){
    if(root){
        print_tree(root->left);
        printf("%s\n", root->dato);
        print_tree(root->right);
    }
}

void deleteTree(bst node){
    if (node) {
        deleteTree(node->left);
        deleteTree(node->right);
        free(node->dato);
        free(node);
    }
}

char *new_game(bst root, int len, int adj_len){
    int count=0, n=0, jump, reps;
    char *victory_word=NULL, *buffer=NULL, *str=NULL;
    archivio key=NULL;
    accepted head = NULL, support = NULL;
    victory_word=(char *) malloc((len+1)*sizeof(char));
    buffer=(char *) malloc((len+1)*sizeof(char));
    str=(char *) malloc((adj_len+1)*sizeof(char));
    if(scanf("%s", victory_word)==0)
        exit(1);
    if(scanf("%d", &n)==0)
        exit(1);
    while(count<n){
        if(scanf("%s", str)==0)
            exit(1);
        strcpy(buffer, victory_word);
        if(strcmp(str, II)==0){
            if (!head)
                root = add_words0(root, len);
            else {
                root = add_words1(root, &support, key, len);
                MergeSort(&support);
                head = SortedMerge(head, support);
                support = NULL;
            }
        }
        if(strcmp(str, SF)==0){
            if (!head){
                print_tree(root);
            }
            else
                print_list(head);
        }
        if((strcmp(str, II)!=0)&&(strcmp(str, SF)!=0)){
            if(strcmp(str, victory_word)==0){
                free(victory_word);
                free(buffer);
                free(str);
                free_archivio(key);
                delete_list(head);
                return OK;
            }
            else{
                if(search(root, str)==0)
                    printf("not_exists\n");
                else{
                    for (int a = 0; a < len; a++) {
                        jump=0;
                        for (int b = 0; b < len; b++) {
                            if (jump==1)
                                break;
                            if (str[a] == buffer[b]) {
                                if(a==b){
                                    printf("+");
                                    buffer[b]='@';
                                    key=aggiornamento_archivio(key, str[a], a, 0);
                                    jump=1;
                                }
                                else {
                                    if ((str[b]!=buffer[b])&&(str[a]!=buffer[a])) {
                                        printf("|");
                                        buffer[b]='@';
                                        key=aggiornamento_archivio(key, str[a], a, 1);
                                        jump=1;
                                    }
                                }
                            }
                        }
                        if (jump==0){
                            printf("/");
                            key=aggiornamento_archivio(key, str[a], a, 2);
                        }
                    }
                    correction(key);
                    count++;
                    if (!head)
                        reps = stampa_filtrate1(root, &head, key, len);
                    else
                        reps = stampa_filtrate2(&head, key, len);
                    printf("\n%d\n", reps);
                }
            }
        }
    }
    free(victory_word);
    free(buffer);
    free(str);
    free_archivio(key);
    delete_list(head);
    return KO;
}

int search(bst root, char *value){              //controlla l'esistenza di una parola all'interno dell'albero
    while (root){
        if (strcmp(value, root->dato)>0)
            root = root->right;
        else if (strcmp(value, root->dato)<0)
            root = root->left;
        else
            return 1;                           // se le parola esiste nell'albero ritorna 1
    }
    return 0;
}

bst add_words0(bst p, int len){                  //aggiunge parole all'albero dopo il comando II
    char *str;
    if(len<15)
        str=(char *)malloc(15+1);
    else
        str=(char *)malloc(len+1);
    while (1){
        if(scanf("%s", str)==0)
            exit(1);
        if(strcmp(str, IF)==0)
            break;
        p=store(p, str, len);
    }
    free(str);
    return p;
}

bst add_words1(bst p, accepted *lis, archivio ptr, int len){                  //aggiunge parole all'albero dopo il comando II
    char *str;
    if(len<15)
        str=(char *)malloc(15+1);
    else
        str=(char *)malloc(len+1);
    while (1){
        if(scanf("%s", str)==0)
            exit(1);
        if(strcmp(str, IF)==0)
            break;
        p=store(p, str, len);
        if(check_archivio(str, ptr, len) == 1) {
            accepted new_node = newNode(str, len);
            new_node->next = (*lis);
            (*lis) = new_node;
        }
    }
    free(str);
    return p;
}

archivio aggiornamento_archivio(archivio lis, char c, int pos, int interruttore){
    archivio cur = lis, pre = NULL;
    int new_len, already_exists=0;
    if(!lis){
        archivio punt = (archivio)malloc(sizeof(info_simbolo));
        punt->simbolo = c;
        punt->m_ripetizioni[0]=0;
        punt->e_ripetizioni=0;
        switch (interruttore) {
            case 0:
                punt->m_ripetizioni[1]=1;
                punt->dove_sta=(int *)malloc(sizeof(int)*2);
                punt->dove_sta[0]=2;
                punt->dove_sta[1]=pos;
                punt->dove_non_sta=(int *)malloc(sizeof(int));
                punt->dove_non_sta[0]=1;
                break;
            case 1:
                punt->m_ripetizioni[1]=1;
                punt->dove_non_sta=(int *)malloc(sizeof(int)*2);
                punt->dove_non_sta[0]=2;
                punt->dove_non_sta[1]=pos;
                punt->dove_sta=(int *)malloc(sizeof(int)*2);
                punt->dove_sta[0]=1;
                break;
            case 2:
                punt->m_ripetizioni[1]=0;
                punt->e_ripetizioni=1;
                punt->dove_sta=(int *)malloc(sizeof(int));
                punt->dove_sta[0]=1;
                punt->dove_non_sta=(int *)malloc(sizeof(int)*2);
                punt->dove_non_sta[0]=2;
                punt->dove_non_sta[1]=pos;
                break;
        }
        punt->prox = NULL;
        return punt;
    }
    while (cur) {
        pre = cur;
        if (cur->simbolo == c) {
            switch (interruttore) {
                case 0:
                    cur->m_ripetizioni[1]++;
                    for (int i = 1; i < cur->dove_sta[0]; i++) {
                        if (pos == cur->dove_sta[i]) {
                            already_exists = 1;
                            break;
                        }
                    }
                    if (already_exists == 1)
                        break;
                    cur->dove_sta[0]++;
                    new_len = cur->dove_sta[0];
                    cur->dove_sta = (int *) realloc(cur->dove_sta, sizeof(int) * new_len);
                    cur->dove_sta[new_len - 1] = pos;
                    break;
                case 1:
                    cur->m_ripetizioni[1]++;
                    for (int i = 1; i < cur->dove_non_sta[0]; i++) {
                        if (pos == cur->dove_non_sta[i]) {
                            already_exists = 1;
                            break;
                        }
                    }
                    if (already_exists == 1)
                        break;
                    cur->dove_non_sta[0]++;
                    new_len = cur->dove_non_sta[0];
                    cur->dove_non_sta = (int *) realloc(cur->dove_non_sta, sizeof(int) * new_len);
                    cur->dove_non_sta[new_len - 1] = pos;
                    break;
                case 2:
                    cur->e_ripetizioni = cur->m_ripetizioni[1] + 1;
                    for (int i = 1; i < cur->dove_non_sta[0]; i++) {
                        if (pos == cur->dove_non_sta[i]) {
                            already_exists = 1;
                            break;
                        }
                    }
                    if (already_exists == 1)
                        break;
                    cur->dove_non_sta[0]++;
                    new_len = cur->dove_non_sta[0];
                    cur->dove_non_sta = (int *) realloc(cur->dove_non_sta, sizeof(int) * new_len);
                    cur->dove_non_sta[new_len - 1] = pos;
                    break;
            }
            return lis;
        }
        cur = cur->prox;
    }
    archivio punt = (archivio)malloc(sizeof(info_simbolo));
    punt->simbolo = c;
    punt->m_ripetizioni[0]=0;
    punt->e_ripetizioni=0;
    switch (interruttore) {
        case 0:
            punt->m_ripetizioni[1]=1;
            punt->dove_sta=(int *)malloc(sizeof(int)*2);
            punt->dove_sta[0]=2;
            punt->dove_sta[1]=pos;
            punt->dove_non_sta=(int *)malloc(sizeof(int));
            punt->dove_non_sta[0]=1;
            break;
        case 1:
            punt->m_ripetizioni[1]=1;
            punt->dove_non_sta=(int *)malloc(sizeof(int)*2);
            punt->dove_non_sta[0]=2;
            punt->dove_non_sta[1]=pos;
            punt->dove_sta=(int *)malloc(sizeof(int)*2);
            punt->dove_sta[0]=1;
            break;
        case 2:
            punt->m_ripetizioni[1]=0;
            punt->e_ripetizioni=1;
            punt->dove_sta=(int *)malloc(sizeof(int));
            punt->dove_sta[0]=1;
            punt->dove_non_sta=(int *)malloc(sizeof(int)*2);
            punt->dove_non_sta[0]=2;
            punt->dove_non_sta[1]=pos;
            break;
    }
    punt->prox = NULL;
    pre->prox = punt;
    return lis;
}

void free_archivio(archivio head){
    if(head!=NULL) {
        free_archivio(head->prox);
        free(head->dove_sta);
        free(head->dove_non_sta);
        free(head);
    }
}

void correction(archivio head){
    if (head!=NULL) {
        if (head->m_ripetizioni[1]>head->m_ripetizioni[0])
            head->m_ripetizioni[0]=head->m_ripetizioni[1];
        head->m_ripetizioni[1]=0;
        if ((head->e_ripetizioni-1!=head->m_ripetizioni[0])&&(head->e_ripetizioni!=0))
            head->e_ripetizioni=head->m_ripetizioni[0]+1;
        correction(head->prox);
    }
}

accepted newNode(char *str, int len){
    accepted newnode = (accepted) malloc(sizeof(node));
    newnode->dato = (char *) malloc(len+1);
    strcpy(newnode->dato, str);
    newnode->next = NULL;
    return newnode;
}

void push(Dstack_p *top_ref, bst p){
    Dstack_p newnode = (Dstack_p) malloc(sizeof(Dstack));
    if(!newnode)
        exit(0);
    newnode->p = p;
    newnode->next = (*top_ref);
    (*top_ref) = newnode;
}

int empty_check(Dstack_p ptr) {
    if(!ptr)
        return 1;
    else
        return 0;
}

bst pop(Dstack_p *top_ref){
    bst res;
    Dstack_p top;
    if(empty_check(*top_ref)==1)
        exit(0);
    else{
        top = *top_ref;
        res = top->p;
        *top_ref = top->next;
        free(top);
        return res;
    }
}

void print_list(accepted head){
    if(head){
        print_list(head->next);
        printf("%s\n", head->dato);
    }
}

void delete_list(accepted head){
    if(head){
        delete_list(head->next);
        free(head->dato);
        free(head);
    }
}

int stampa_filtrate1(bst root, accepted *lis, archivio head, int len){
    bst cur_d = root;
    Dstack_p s = NULL;
    accepted ptr = NULL;
    archivio cur_a = NULL;
    int reps = 0, count, check = 1, a, i, max, rip_esatte;
    while ((cur_d)||(!empty_check(s))) {
        if (cur_d) {
            push(&s, cur_d);
            cur_d = cur_d->left;
        } else {
            if (empty_check(s) == 0) {
                cur_d = pop(&s);
                cur_a = head;
                while (cur_a) {
                    max = cur_a->m_ripetizioni[0];
                    rip_esatte = cur_a->e_ripetizioni - 1;
                    count = 0;
                    check = 1;
                    if ((rip_esatte == 0) && (rip_esatte == max)) {
                        for (a = 0; a < len; a++) {
                            if (cur_a->simbolo == cur_d->dato[a]) {
                                check = 0;
                                break;
                            }
                        }
                    } else {
                        for (a = 0; a < len; a++) {
                            if (cur_a->simbolo == cur_d->dato[a]) {
                                count++;
                            }
                            if ((rip_esatte == max) && (count > rip_esatte)) {
                                check = 0;
                                break;
                            }
                        }
                        if (count < max) {
                            check = 0;
                        }
                        if (check == 1) {
                            for (i = 1; i < cur_a->dove_sta[0]; i++) {
                                if (cur_d->dato[cur_a->dove_sta[i]] != cur_a->simbolo) {
                                    check = 0;
                                    break;
                                }
                            }
                        }
                        if (check == 1) {
                            for (i = 1; i < cur_a->dove_non_sta[0]; i++) {
                                if (cur_d->dato[cur_a->dove_non_sta[i]] == cur_a->simbolo) {
                                    check = 0;
                                    break;
                                }
                            }
                        }
                    }
                    if (check == 0) {
                        break;
                    }
                    cur_a = cur_a->prox;
                }
                if (check == 1) {
                    reps++;
                    ptr = newNode(cur_d->dato, len);
                    ptr->next = (*lis);
                    (*lis) = ptr;
                }
                cur_d = cur_d->right;
            }
        }
    }
    return reps;
}

int stampa_filtrate2(accepted *lis, archivio head, int len){
    accepted cur_l = (*lis), pre = NULL;
    archivio cur_a = NULL;
    int reps = 0, count, check = 1, a, i, max, rip_esatte;
    while (cur_l) {
        cur_a = head;
        while (cur_a) {
            max = cur_a->m_ripetizioni[0];
            rip_esatte = cur_a->e_ripetizioni - 1;
            count = 0;
            check = 1;
            if ((rip_esatte == 0) && (rip_esatte == max)) {
                for (a = 0; a < len; a++) {
                    if (cur_a->simbolo == cur_l->dato[a]) {
                        check = 0;
                        break;
                    }
                }
            } else {
                for (a = 0; a < len; a++) {
                    if (cur_a->simbolo == cur_l->dato[a]) {
                        count++;
                    }
                    if ((rip_esatte == max) && (count > rip_esatte)) {
                        check = 0;
                        break;
                    }
                }
                if (count < max) {
                    check = 0;
                }
                if (check == 1) {
                    for (i = 1; i < cur_a->dove_sta[0]; i++) {
                        if (cur_l->dato[cur_a->dove_sta[i]] != cur_a->simbolo) {
                            check = 0;
                            break;
                        }
                    }
                }
                if (check == 1) {
                    for (i = 1; i < cur_a->dove_non_sta[0]; i++) {
                        if (cur_l->dato[cur_a->dove_non_sta[i]] == cur_a->simbolo) {
                            check = 0;
                            break;
                        }
                    }
                }
            }
            if (check == 0) {
                break;
            }
            cur_a = cur_a->prox;
        }
        if (check == 1) {
            reps++;
            pre = cur_l;
            cur_l = cur_l->next;
        }
        else {
            if(!pre) {
                (*lis) = (*lis)->next;
                free(cur_l->dato);
                free(cur_l);
                cur_l = (*lis);
            } else {
                pre->next = cur_l->next;
                free(cur_l->dato);
                free(cur_l);
                cur_l = pre->next;
            }
        }
    }
    return reps;
}

int check_archivio(char *str, archivio cur_a, int len){
    int a, count, check;
    while (cur_a) {
        count = 0;
        check = 1;
        if ((cur_a->e_ripetizioni - 1 == 0) && (cur_a->e_ripetizioni - 1 == cur_a->m_ripetizioni[0])) {
            for (a = 0; a < len; a++) {
                if (cur_a->simbolo == str[a]) {
                    check = 0;
                    break;
                }
            }
        } else {
            for (a = 0; a < len; a++) {
                if (cur_a->simbolo == str[a]) {
                    count++;
                }
                if ((cur_a->e_ripetizioni - 1 == cur_a->m_ripetizioni[0]) && (count > cur_a->e_ripetizioni - 1)) {
                    check = 0;
                    break;
                }
            }
            if (count < cur_a->m_ripetizioni[0])
                check = 0;
            if (check == 1) {
                for (a = 1; a < cur_a->dove_sta[0]; a++) {
                    if (str[cur_a->dove_sta[a]] != cur_a->simbolo) {
                        check = 0;
                        break;
                    }
                }
            }
            if (check == 1) {
                for (a = 1; a < cur_a->dove_non_sta[0]; a++) {
                    if (str[cur_a->dove_non_sta[a]] == cur_a->simbolo) {
                        check = 0;
                        break;
                    }
                }
            }
        }
        if (check == 0) {
            break;
        }
        cur_a = cur_a->prox;
    }
    return check;
}

void MergeSort(accepted *headRef){
    accepted head = *headRef;
    accepted a;
    accepted b;
    if ((head == NULL) || (head->next == NULL)) {
        return;
    }
    FrontBackSplit(head, &a, &b);
    MergeSort(&a);
    MergeSort(&b);
    *headRef = SortedMerge(a, b);
}

accepted SortedMerge(accepted a, accepted b){
    accepted result = NULL;
    if (a == NULL)
        return (b);
    else if (b == NULL)
        return (a);
    if (strcmp(a->dato, b->dato)>0) {
        result = a;
        result->next = SortedMerge(a->next, b);
    }
    else {
        result = b;
        result->next = SortedMerge(a, b->next);
    }
    return (result);
}

void FrontBackSplit(accepted source, accepted *frontRef, accepted *backRef){
    accepted fast;
    accepted slow;
    slow = source;
    fast = source->next;
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
}