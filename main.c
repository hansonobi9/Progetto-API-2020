#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define true 1
#define false 0

typedef int bool;

typedef struct stack_s {
    int i1;
    int i2;
    int old_dim; //old(t_dim)
    char **data; //array of strings
    char opr; // operation type
    struct stack_s *next;
}stack_t;


char **text;
int t_dim;
char a[1024];
int q,w,e;
stack_t *u = NULL;
stack_t *r = NULL;
bool reset = false;


stack_t* createU (int i1, int i2, int old_dim, char opr) { //create a new element
    stack_t *n;
    n = malloc(sizeof(stack_t));
    n->i1 = i1;
    n->i2 = i2;
    n->old_dim = old_dim;
    n->opr = opr;
    n->data = NULL;
    n->next = NULL;
    return n;
}

stack_t* pushU (stack_t *h,stack_t *new) { //insert an element on the top
    new->next = h;
    h = new;
    return h;
}

stack_t* popU (stack_t *h) { //remove the first element
    stack_t *tbd;
    if (h != NULL) {
        tbd = h;
        h = h->next;
        free(tbd->data);
        free(tbd);
    }
    return h;
}

stack_t* resetU(stack_t *h) { //delete all elements of the stack
    stack_t  *tmp, *tbd;

    tmp = h;
    while (tmp != NULL) {
        tbd = tmp;
        tmp = tmp->next;
        //free(tbd->data);
        free(tbd);
    }

    return tmp;
}

int min (int x, int y) {
    if (x < y)
        return x;
    return y;
}

void change (int ind1, int ind2) {
    if (reset == true) {
        r = resetU(r);
        reset = false;
        q = 0;
    }
    int m = min(ind2,t_dim);
    stack_t *el = createU(ind1,ind2,t_dim,'c');

    if (!(t_dim == 0 || ind1 == t_dim+1)) {
        el->data = malloc(sizeof(char*)*(m-ind1+1));
        for (int i = ind1, c = 0; i <=m; i++,c++)
            el->data[c] = text[i];
    }
    u = pushU(u,el);

    if(t_dim == 0) {
        text = realloc(text,(ind2+1)*sizeof(char *));
        t_dim = ind2;
    }

    if (ind1 == t_dim+1) {
        text = realloc(text, (t_dim + 2) * sizeof(char *));
        t_dim++;
    }

    if (ind2 > t_dim) {
        text = realloc(text, (t_dim+(ind2-t_dim)+1) * sizeof(char *));
        t_dim = t_dim+(ind2-t_dim);
    }

    for (int i = ind1; i <= ind2; i++) {
        fgets(a,1024,stdin);
        text[i] = malloc((strlen(a)+1)*sizeof(char));
        strcpy(text[i], a);
    }

    fgets(a,1024,stdin);
}

void print (int ind1, int ind2) {
    for (int i = ind1; i<= ind2; i++)
        if (i == 0 || i > t_dim)
            printf(".\n");
        else
            printf("%s",text[i]);
}

void delete (int ind1, int ind2) {
    if (reset == true) {
        r = resetU(r);
        reset = false;
        q = 0;
    }
    int shift = 0;
    stack_t *el = createU(ind1,ind2,t_dim,'d');
    el->data = malloc(sizeof(char*)*t_dim);

    for (int i = 1,c = 0; i <= t_dim; i++,c++)
        el->data[c] = text[i];

    for (int i = ind1; i <= ind2; i++) {
        if (i > 0 && i <= t_dim) {
            shift++;
        }
    }

    if (t_dim > ind2) {
        for (int i = ind2+1; i <= t_dim; i++) {
            text[i-shift] = text[i];
        }
    }
    u = pushU(u,el);
    text = realloc(text,(t_dim-shift+1)*sizeof(char*));
    t_dim = t_dim-shift;
}

void uchange (stack_t *el) {
    reset = true;
    int m = min(el->old_dim,el->i2);
    stack_t *rel = createU(el->i1,el->i2,t_dim,'c');
    rel->data = malloc(sizeof(char*)*(rel->i2-rel->i1+1));

    for (int i = rel->i1,c = 0; i <= rel->i2; i++,c++)
        rel->data[c] = text[i];

    r = pushU(r,rel);

    if (el->i1 > el->old_dim) {
        text = realloc(text,sizeof(char*)*(1+el->old_dim));
    } else {
        text = realloc(text,sizeof(char*)*(1+el->old_dim));
        for (int i = el->i1,c = 0; i <= m; i++, c++) {
            text[i] = el->data[c];
        }
    }

    t_dim = el->old_dim;
}

void udelete(stack_t *el) {
    reset = true;
    stack_t *rel = createU(el->i1,el->i2,t_dim,'d');
    r = pushU(r,rel);
    if (t_dim < el->old_dim)
        text = realloc(text,sizeof(char*)*(1+el->old_dim));
    for (int i = 1,c = 0; i <= el->old_dim; i++, c++) {
        text[i] = el->data[c];
    }

    t_dim = el->old_dim;
}

void rdelete(stack_t *rel) {
    int shift = 0;
    stack_t *el = createU(rel->i1,rel->i2,t_dim,'d');
    el->data = malloc(sizeof(char*)*t_dim);

    for (int i = 1,c = 0; i <= t_dim; i++,c++)
        el->data[c] = text[i];

    for (int i = rel->i1; i <= rel->i2; i++) {
        if (i > 0 && i <= t_dim) {
            shift++;
        }
    }

    if (t_dim > rel->i2) {
        for (int i = rel->i2+1; i <= t_dim; i++) {
            text[i-shift] = text[i];
        }
    }
    u = pushU(u,el);
    text = realloc(text,(t_dim-shift+1)*sizeof(char*));
    t_dim = t_dim-shift;
}

void rchange(stack_t *rel) {
    int m = min(rel->i2,t_dim);
    stack_t *el = createU(rel->i1,rel->i2,t_dim,'c');
    if (!(t_dim == 0 || rel->i1 == t_dim+1)) {
        el->data = malloc(sizeof(char*)*(m-rel->i1+1));
        for (int i = rel->i1, c = 0; i <=m; i++,c++)
            el->data[c] = text[i];
    }
    u = pushU(u,el);

    if(t_dim == 0) {
        text = realloc(text,(rel->i2+1)*sizeof(char *));
        t_dim = rel->i2;
    }

    if (rel->i1 == t_dim+1) {
        text = realloc(text, (t_dim + 2) * sizeof(char *));
        t_dim++;
    }

    if (rel->i2 > t_dim) {
        text = realloc(text, (t_dim+(rel->i2-t_dim)+1) * sizeof(char *));
        t_dim = t_dim+(rel->i2-t_dim);
    }

    for (int i = rel->i1, c = 0; i <= rel->i2; i++,c++) {
        text[i] = rel->data[c];
    }
}

void undo (int n) {
    for (int i = 0; i < n; i++) {
        if (u == NULL)
            break;
        if (u->opr == 'c')
            uchange(u);
        else
            udelete(u);
        u = popU(u);
    }
}

void redo (int n) {
    for (int i = 0; i < n; i++) {
        if (r == NULL)
            break;
        if (r->opr == 'c')
            rchange(r);
        else
            rdelete(r);
        r = popU(r);
    }
}

int main(int argc, char *argv[]) {
    int ind1,ind2;
    char opr;
    q = 0, w = 0, e = 0;

    text = malloc(1*sizeof(char*));
    gets(a);
    while(strcmp(a,"q") != 0) {
        opr = a[strlen(a)-1];
        switch (opr) {
            case 'c':
                sscanf(a, "%d,%d", &ind1, &ind2);
                if (w > 0) {
                    undo(w);
                    e = e - w;
                    q = q - w;
                }
                if (w < 0) {
                    redo (- w);
                    e = e - w;
                    q = q - w;
                }
                w = 0;
                change(ind1, ind2);
                e++;
                break;
            case 'p':
                sscanf(a, "%d,%d", &ind1, &ind2);
                if (w > 0) {
                    undo(w);
                    e = e - w;
                    q = q - w;
                }
                if (w < 0) {
                    redo (- w);
                    e = e - w;
                    q = q - w;
                }
                w = 0;
                print(ind1, ind2);
                break;
            case 'd':
                sscanf(a, "%d,%d", &ind1, &ind2);
                if (w > 0) {
                    undo(w);
                    e = e - w;
                    q = q - w;
                }
                if (w < 0) {
                    redo (-w);
                    e = e - w;
                    q = q - w;
                }
                w = 0;
                delete(ind1, ind2);
                e++;
                break;
            case 'u':
                sscanf(a, "%d", &ind1);
                if (w+ind1 > e)
                    w = e;
                else
                    w = w + ind1;
                break;
            case 'r':
                sscanf(a, "%d", &ind1);
                if (w-ind1 < q)
                    w = q;
                else
                    w = w - ind1;
                break;
        }
        gets(a);
    }
    return 0;
}
