#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **text;
int t_dim;
char a[1024];

void initialize (int ind1,int ind2) {
    for (int i = ind1; i <= ind2; i++)
        text[i] = NULL;
}

void change (int ind1, int ind2) {

    if(t_dim == 0) {
        text = calloc((ind2+1),sizeof(char *));
        t_dim = ind2;
    }

    if (ind1 == t_dim+1) {
        text = realloc(text, (t_dim + 2) * sizeof(char *));
        initialize(ind1,ind1);
        t_dim++;
    }

    if (ind2 > t_dim) {
        text = realloc(text, (t_dim+(ind2-t_dim)+1) * sizeof(char *));
        initialize(t_dim+1,ind2);
        t_dim = t_dim+(ind2-t_dim);
    }

    for (int i = ind1; i <= ind2; i++) {
        fgets(a,1024,stdin);
        if (text[i] == NULL)
            text[i] = malloc((strlen(a)+1)*sizeof(char));
        else
            text[i] = realloc(text[i],(strlen(a)+1)*sizeof(char));
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
    int shift = 0;

    for (int i = ind1; i <= ind2; i++) {
        if (i > 0 && i <= t_dim) {
            free(text[i]);
            shift++;
        }
    }

    if (t_dim > ind2) {
        for (int i = ind2+1; i <= t_dim; i++) {
            text[i-shift] = malloc((strlen(text[i])+1)*sizeof(char));
            strcpy(text[i-shift],text[i]);
            free(text[i]);
        }
    }
    text = realloc(text,(t_dim-shift+1)*sizeof(char*));
    t_dim = t_dim-shift;
}

int main(int argc, char *argv[]) {
    int ind1,ind2;

    t_dim = 0;
    text = malloc(1*sizeof(char*));
    gets(a);


    while(strcmp(a,"q") != 0) {
        sscanf(a, "%d,%dx", &ind1, &ind2);
        //ind1 = atoi(&a[0]);
        //ind2 = atoi(&a[2]);
        if (strcmp(&a[strlen(a)-1],"c") == 0)
            change(ind1,ind2);
        else if(strcmp(&a[strlen(a)-1],"d") == 0)
            delete(ind1,ind2);
        else print(ind1,ind2);
        gets(a);
    }

    return 0;
}
