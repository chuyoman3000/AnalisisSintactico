//
//  main.cpp
//  appASintactico_XCode
//
//  Created by Martin Valdes on 6/4/19.
//  Copyright © 2019 Martin Valdes. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <iostream>
using namespace std;

//#define TRUE 1
//#define FALSE 0

#define MAXTOKEN 20
#define NUMPALRES 4
#define MAX 50

//char *PalRes[]= {"char", "float","int","puts"}; //printf
char PalRes[5][10]= {"char", "float","int","puts"}; //printf

//si se usa objeto string, cambiar el uso de strcmp
//string PalRes[10]= {"char", "float","int","puts"}; //printf

char sLexema[127];
char sLinea[127];
//char *non="13579", *par="24680";
char non[6]="13579", par[6]="24680";
char asTokens [MAXTOKEN][10];
int indice=0, edoAct, edoIni, iniToken=0, k, numBytesArch;

int cima = -1;
char pilac[MAX][10];

void generararch();
void vabrirarch();
int  bytesarch();

void vanalisislexico();
void vanalisis_sintactico();
void viniedos();
char nextchar();
void vretract();
int edoActesacept();
void vmuestra();
void falla();
void recuperaerror();
int esId();

void insertapila(string elem);//(char *elem);  //(char elem[]);
int buscaTabla(char a[], char x[]);
void eliminapila();
int estoken(char x[]);

bool finarch = false;
FILE *Fd;

char token[17][8] = {"x", ";", ",", "*", "Id", "[", "]", "Num", "char", "int", "float",
    "puts", "(", ")", "Cte.Lit", "{", "}"};//, "="};

char varsint[15][3]={"x", "D", "L", "L'", "I", "I'", "A", "A'", "K",
    "T", "F", "E", "P"};
                                                   // e -> cadena vacia
int tablaM[29][8]= {{1, 8, 1, 9, 2, -1, 999, 999}, //char D->TL:
                    {1, 9, 1, 9, 2, -1, 999, 999}, //int D->TL:
                    {1, 10, 1, 9, 2, -1, 999, 999}, //float D->TL:
                    {2, 3, 2, 4, 3, 999, 999, 999}, // * L->IL'
                    {2, 4, 2, 4, 3, 999, 999, 999}, //Id L->IL'
         /* 5 */    {3, 1, 3, 5, 999, 999, 999, 999}, //; L'->I'
                    {3, 1, 3, 999, 999, 999, 999, 999},//; L'->e
                    {3, 2, 3, -2, 4, 3, 999, 999}, //, L'->,IL'
                    {3, 14, 3, -14, 5, 999, 999, 999},// cte. Lit. L'->cte Lit. I'
                    {4, 3, 4, -3, -4, 5, 999, 999}, // * I->*Id I'
        /* 10 */    {4, 4, 4, -4, 5, 999, 999, 999},// Id I->Id I'
                    {5, 1, 5, 999, 999, 999, 999, 999},// ; I'->e
                    {5, 2, 5, 999, 999, 999, 999, 999},// , I'->e
                    {5, 5, 5, 6, 999, 999, 999, 999}, // [ I' -> A
                    {5, 7, 5, -7, 999, 999, 999, 999}, //Num I' -> Num
        /* 15 */    {5, 14, 5, -14, 999, 999, 999, 999}, //cte lit I'->cte lit
                    {6, 5, 6, -5, 8, -6, 7, 999}, //[ A->[k]A'
                    {7, 1, 7, 999, 999, 999, 999, 999},//; A'->e
                    {7, 2, 7, 999, 999, 999, 999, 999},//, A'->e
        /* -> */    {7, 5, 7, -5, 8, -6, 7, 999}, //[ A' -> [K]A'
        /* 20 */    {8, 4, 8, -4, 999, 999, 999, 999}, //Id K -> Id
                    {8, 7, 8, -7, 999, 999, 999, 999}, //Num K -> Num
                    {9, 8, 9, -8, 999, 999, 999, 999}, //char T->Char
                    {9, 9, 9, -9, 999, 999, 999, 999}, //int T->int
                    {9, 10, 9, -10, 999, 999, 999, 999},//float T->float
                    /*segunda gramatica */
        /* 25 */    {10, 11, 10, -11, 11, -1, 999, 999},//puts F-> puts E;
                    {11, 12, 11, -12, 12, -13, 999, 999},//( E -> (P)
                    {12, 14, 12, -14, 999, 999, 999, 999},//cte lit P->cte lit
                    {12, 4, 12, -4, 999, 999, 999, 999}//Id P ->Id
                   };


int main(void)
{
    char resp;
    do
    {
        puts("Desea Generar el Archivo (s/n) : ");

        //cout<<"Desea Generar el Archivo (s/n) : ";
        //resp = getchar();
        //cin>>resp;

        resp = cin.get();
        //cin.get();  //USAR CUANDO SE REQUIERA ELIMINAR EL ENTER


        // if(resp==0)  //NULL
        //   cin.get();// getch();

        if(strchr("Ss",resp))
            generararch();
        vabrirarch();
        numBytesArch= bytesarch();
        if(numBytesArch==0)
        {
            printf("El archivo NO Tiene Datos. Pulse una tecla");
            if(cin.get() == 0) //  getch()==0)
            { cin.get();//getch();
                exit(-1);
            }
        }
        //fclose(Fd);

        else
            vanalisislexico();
        fclose(Fd);
        printf("Salida del Analizador Lexico (asTokens)");
        vmuestra();
        printf("Pulse una tecla para continuar");
        if(cin.get() == 0) //getch()==0)
            cin.get();     //getch();
        vanalisis_sintactico();
        cin.get();
        printf("Presiona (sS) para continuar ? : " );
        cin>>resp;
    }while (strchr("Ss",resp));

    return 0;
}
void generararch()
{
    char car;
    char nomArch[100];

    // char *nombre = new char[100];
    //cin.getline(nombre,100,'\n');
    string nombre;
    printf("Nombre del Archivo a GENERAR (sin extension): ");
    //gets(Nombre);  //<--- ERROR CON ESTE METODO
    cin>>nombre;
    cin.get();    //ELIMINAR ENTER

    //PATH APP. QT
    // /Users/martinos/Documents/MATERIAS/AUTOMATAS\ I/MATERIAL/UNIDAD\ IV\ SINTACTICO/ANALIZADOR\ SINTACTICO\ REVISADO/appAnalizadorSintactico_

    //PATH APP. XCODE
    //sprintf(nomArch,"/Users/martinos/Desktop/appASintactico_XCode/%s.dat",nombre.c_str());  //,'\0');
    sprintf(nomArch,"%s.dat",nombre.c_str());
    Fd = fopen(nomArch,"w+b");
    if(Fd == nullptr)
    {
        cout<<"NO SE PUEDE ABRIR EL ARCHIVO : "<<nomArch;
        exit(-1);
    }
    //puts("teclea el archivo (<ESC> para terminar el archivo): ");
    printf("teclea el archivo (<ESC> para terminar el archivo): \n");
    //cin.get();  //eliminar el enter
    //printf("Tecela <ESC> para terminar el archivo \n");
    //LA INFO ESTA EN EL BUFFER
    do
    {          //CHECAR
        // car = getchar();    //getche();
        car = cin.get();
        //cin>>car;
        if( car == '\n')  //13)
        {
            car = '\n';
            printf("\n");
        }
        //if(car == 0)  //NULL
        //  car = cin.get();    //getch();
        if(car != 64 && car != 8)
            fwrite(&car,sizeof(car),1,Fd);

    }while(car != 64);

    fclose(Fd);

}

void vabrirarch()
{
    char nomArch[100];
    //char nombre[100];
    char *nombre = new char[100];

    printf("\n\n Nombre del Archivo a ABRIR(sin extension): ");
    //gets(nombre);
    //cin>>nombre;
    cin.get();  //ELIMINAR EL ENTER
    cin.getline(nombre,100,'\n');

    //sprintf(nomArch,"/Users/martinos/Desktop/appASintactico_XCode/%s.dat",nombre);  //,'\0');
    sprintf(nomArch,"%s.dat",nombre);


    Fd = fopen(nomArch,"r+b");

    if(Fd==NULL)
    {
        printf("NO SE PUEDE ABRIR EL ARCHIVO");
        exit(-1);    //cin.get();

    }
    else
    {
        printf("EL ARCHIVO SE ENCUENTRA ABIERTO \n");

    }

}

int bytesarch()
{
    int aux;
    fseek(Fd,0L,SEEK_END);
    aux = (int) ftell(Fd);
    fseek(Fd,0L,SEEK_SET);
    return aux;
}

void vanalisislexico()
{
    char cCarent;

    indice = iniToken = k = 0;
    viniedos();

    while(indice < numBytesArch || edoActesacept())
    {
        switch(edoAct)
        {
            case 0: cCarent=nextchar();
                if (strchr(non,cCarent))
                    edoAct = 1;
                else
                    if(strchr(par,cCarent))
                        edoAct = 2;
                    else
                        falla();
                break;

            case 1: cCarent=nextchar();
                if(strchr(non,cCarent))
                    edoAct=1;
                else
                    if(strchr(par,cCarent))
                        edoAct = 2;
                    else
                        falla();
                break;

            case 2: cCarent=nextchar();
                if(strchr(non,cCarent))
                    edoAct=1;
                else
                    if (strchr(par,cCarent))
                        edoAct=2;
                    else
                        edoAct=3;
                break;

            case 3: vretract();
                strcpy(asTokens[k++],"Num");
                if (indice >= numBytesArch)
                    return;
                iniToken=indice;
                viniedos();
                break;

            case 4: cCarent=nextchar();
                switch(cCarent)
                {
                    case '+': edoAct=5; break;
                    case '-': edoAct=6; break;
                    case '*': edoAct=7; break;
                    case '/': edoAct=8; break;
                    default: falla();
                }
                break;

            case 5: strcpy(asTokens[k++], "+");
                if (indice >= numBytesArch)
                    return;
                iniToken=indice;
                viniedos();
                break;

            case 6: strcpy(asTokens[k++], "-");
                if (indice>=numBytesArch)
                    return;
                iniToken=indice;
                viniedos();
                break;

            case 7: strcpy(asTokens[k++], "*");
                if (indice>=numBytesArch)
                    return;
                iniToken=indice;
                viniedos();
                break;

            case 8: strcpy(asTokens[k++], "/");
                if (indice>=numBytesArch)
                    return;
                iniToken=indice;
                viniedos();
                break;


            case 9: cCarent=nextchar();
                if (strchr(par,cCarent))
                    edoAct=10;
                else
                    if(strchr(non,cCarent))
                        edoAct=11;
                    else
                        falla();
                break;

            case 10: cCarent=nextchar();
                if (strchr(par,cCarent))
                    edoAct=10;
                else
                    if(strchr(non,cCarent))
                        edoAct=11;
                    else
                        falla();
                break;

            case 11 : cCarent=nextchar();
                if (strchr(non,cCarent))
                    edoAct=11;
                else
                    if(strchr(par,cCarent))
                        edoAct=10;
                    else
                        edoAct=12;
                break;

                // NON - PAR
            case 12: vretract();
                strcpy(asTokens[k++],"Num");
                if (indice>=numBytesArch)
                    return;
                iniToken=indice;
                viniedos();
                break;

            case 13: cCarent=nextchar();
                if ((isalpha(cCarent)|| cCarent=='_'))
                    edoAct=14;
                else
                    falla();
                break;

            case 14: cCarent=nextchar();
                if ((isalpha(cCarent)|| cCarent=='_') || isdigit(cCarent))
                    edoAct=14;
                else
                    edoAct= 15;
                break;

            case 15: vretract();
                if(esId())
                    strcpy(asTokens[k++], "Id");
                else
                    strcpy(asTokens[k++],sLexema);

                if(indice>=numBytesArch)
                    return;
                iniToken=indice;
                viniedos();
                break;


            case 16: cCarent=nextchar();
                if(cCarent==';')
                    edoAct=17;
                else
                    falla();
                break;

                /* Al encontrar el caracter ';' , que indica fin de linea,
                 Se copia el caracter '$' al fin del arreglo astokens
                 */
            case 17: strcpy(asTokens[k++], ";");
                strcpy(asTokens[k], "$");
                if (indice >= numBytesArch)
                    return;
                iniToken=indice;
                viniedos();
                break;

            case 18: cCarent=nextchar();
                if(cCarent=='[')
                    edoAct=19;
                else
                    falla();
                break;

            case 19: strcpy(asTokens[k++], "[");
                if (indice >= numBytesArch)
                    return;
                iniToken=indice;
                viniedos();
                break;

            case 20: cCarent=nextchar();
                if(cCarent==']')
                    edoAct=21;
                else
                    falla();
                break;

            case 21: strcpy(asTokens[k++], "]");
                if (indice >= numBytesArch)
                    return;
                iniToken = indice;
                viniedos();
                break;

            case 22: cCarent = nextchar();
                if(cCarent == ',')
                    edoAct=23;
                else
                    falla();
                break;

            case 23: strcpy(asTokens[k++], ",");
                if (indice >= numBytesArch)
                    return;
                iniToken=indice;
                viniedos();
                break;

            case 24: cCarent = nextchar();
                if (('"' == cCarent) && cCarent)
                    edoAct=25;
                else
                    falla();
                break;

            case 25: cCarent=nextchar();
                if (('"' != cCarent) && cCarent)
                    edoAct=25;
                else
                    if(cCarent)
                        edoAct=26;
                break;

            case 26: strcpy(asTokens[k++],"Cte.Lit");
                if (indice >= numBytesArch)
                    return;
                iniToken=indice;
                viniedos();
                break;

            case 27: cCarent=nextchar();
                if(cCarent == '(')
                    edoAct=28;
                else
                    falla();
                break;

            case 28: strcpy(asTokens[k++], "(");
                if (indice>=numBytesArch)
                    return;
                iniToken = indice;
                viniedos();
                break;

            case 29:cCarent=nextchar();
                if(cCarent == ')')
                    edoAct=30;
                else
                    falla();
                break;

            case 30: strcpy(asTokens[k++], ")");
                if (indice>=numBytesArch)
                    return;
                iniToken=indice;
                viniedos();
                break;

            /*case 31: cCarent = nextchar();
                if(cCarent == '{')
                    edoAct = 32;
                else
                    falla();
                break;

            case 32: strcpy(asTokens[k++], "{");
                if (indice >= numBytesArch)
                    return;
                iniToken = indice;
                viniedos();
                break;

            case 33: cCarent = nextchar();
                if(cCarent == '}')
                    edoAct = 34;
                else
                    falla();
                break;

            case 34: strcpy(asTokens[k++], "}");
                if (indice >= numBytesArch)
                    return;
                iniToken = indice;
                viniedos();
                break;

            case 35: cCarent = nextchar();
                if(cCarent == '=')
                    edoAct = 36;
                else
                    falla();
                break;

            case 36: strcpy(asTokens[k++], "=");
                if (indice >= numBytesArch)
                    return;
                iniToken = indice;
                viniedos();
                break;*/
        }/*switch*/
    } /*while*/
}

void viniedos()
{
    edoAct=0;
    edoIni=0;
}
char nextchar()
{
    char cAux;
    fread(&cAux,sizeof(cAux),1,Fd);
    indice++;
    return cAux;
}
void vretract()
{
    indice--;
    fseek(Fd,(long)indice,SEEK_SET);
}

int edoActesacept()
{
    switch (edoAct){
    case 8: case 5: case 7: case 6: case 3:
    case 12: case 15: case 17: case 19: case 21:
    case 23 : case 26: case 28: case 30: //case 32:
    //case 34: case 36:
        return true;
        default : return false;
    }
}

void falla()
{
    switch (edoIni)
    {
        case 0 : edoIni=4;
            indice=iniToken;
            fseek(Fd, (long)iniToken, SEEK_SET);
            break;

        case 4 : edoIni=9;
            indice=iniToken;
            fseek(Fd, (long)iniToken, SEEK_SET);
            break;

        case 9 : edoIni=13;
            indice=iniToken;
            fseek(Fd, (long)iniToken, SEEK_SET);
            break;

        case 13 : edoIni=16;
            indice = iniToken;
            fseek(Fd, (long)iniToken, SEEK_SET);
            break;

        case 16 : edoIni=18;
            indice = iniToken;
            fseek(Fd, (long)iniToken, SEEK_SET);
            break;

        case 18 : edoIni=20;
            indice=iniToken;
            fseek(Fd, (long)iniToken, SEEK_SET);
            break;

        case 20 : edoIni=22;
            indice = iniToken;
            fseek(Fd, (long)iniToken, SEEK_SET);
            break;

        case 22 : edoIni=24;
            indice = iniToken;
            fseek(Fd, (long)iniToken, SEEK_SET);
            break;

        case 24 : edoIni=27;
            indice = iniToken;
            fseek(Fd, (long)iniToken, SEEK_SET);
            break;

        case 27 : edoIni=29;
            indice = iniToken;
            fseek(Fd, (long)iniToken, SEEK_SET);
            break;

        case 29: recuperaerror(); /*edoIni = 31;
            indice = iniToken;
            fseek(Fd, (long)iniToken, SEEK_SET);
            break;

        case 31: edoIni = 33;
            indice = iniToken;
            fseek(Fd, (long)iniToken, SEEK_SET);
            break;

        case 33: edoIni = 35;
            indice = iniToken;
            fseek(Fd, (long)iniToken, SEEK_SET);
            break;

        case 35: recuperaerror();*/
    }

    edoAct=edoIni;
}

void recuperaerror()
{
    iniToken = indice;
    viniedos();
}

int esId()
{
    int n,m,found = false;
    fseek(Fd,(long)iniToken,SEEK_SET);
    for (m=iniToken, n=0; m<indice ; m++, n++)
        fread(&sLexema[n],sizeof(char),1, Fd);
    sLexema[n]= '\0';
    for (m=0 ; m < NUMPALRES && !found ;)
        if (strcmp(PalRes[m], sLexema)==0)
            found = true;
        else
            m++;
    return (found ? 0 : 1);
}


void vmuestra()
{
    int iJ;

    puts("\nTOKENS RECONOCIDOS : \n");
    for(iJ=0; iJ < k; iJ++)  //,Ren++)
    {
        puts(asTokens[iJ]);

    }
}


void vanalisis_sintactico()
{
    int ip=0, i, j;
    int renglon, iast;
    char x[10], a[10];

    insertapila("$");

    if(strcmp(asTokens[ip], "puts") == 0)
        insertapila("F");
    else
        insertapila("D");

    printf("\nSalida del Analizador Sintactico (asTokens): \n");
    printf("Arreglo de Tokens: \n");

    for(i=0; strcmp(asTokens[i], "$") != 0; i++)
        printf("%s ", asTokens[i]);

    printf("\n\n Producciones: \n");

    do
    {
        strcpy(x, pilac[cima]);
        strcpy(a, asTokens[ip]);

        if(estoken(x) || (strcmp(x, "$") == 0))
        {
            if(strcmp(x, a) == 0)
            {
                eliminapila();
                ip++;
            }
            else
            {
                if(strcmp(asTokens[ip], "puts") == 0)
                    insertapila("F");
                else //Sino
                    insertapila("D");

                strcpy(x, pilac[cima]);

            }
        }
        else
        {
            renglon = buscaTabla(a, x);

            if(renglon != 999)
            {
                eliminapila();
                iast = 0;
                printf("%-3s -> ", varsint[tablaM[renglon][0]]);

                for(j= 3; iast!=999; j++)
                {
                    iast = tablaM[renglon][j];  //999
                    if(iast < 0)
                    {
                        iast *= -1;
                        printf("%s ",token[iast]);

                    }
                    else
                    {
                        if(iast != 999)
                            printf("%s", varsint[iast]);
                    }
                }

                printf("\n");

                for(i=j-2; i>2; i--)
                {
                    iast = tablaM[renglon][i];
                    if(iast < 0)
                    {
                        iast *= -1;
                        insertapila(token[iast]);
                    }
                    else
                        insertapila(varsint[iast]);
                }
            }
            else
            {
                printf("\n\n Error de Sintaxis");
                return;
            }
        }
     }while(strcmp(x, "$") != 0);
    printf("\n");
}

void insertapila(string elem)//(char *elem) //(char elem[])
{
    if(cima == -1){
        cima = 0;
        strcpy(pilac[cima],elem.c_str());
    }
    else{
        if(cima == MAX - 1)
            puts("Pila LLena");
        else{
            cima++;
            strcpy(pilac[cima],elem.c_str());
        }
    }
}

void eliminapila()
{
    if(cima == -1)
        puts("Pila Vacia");
    else{
        strcpy(pilac[cima],"");
        cima--;
    }
}

int estoken(char x[])
{
    int i;

    for(i=0; i<15; i++)
    {
        if(strcmp(x, token[i]) == 0)
            return 1;
    }

    return 0;
}

int buscaTabla(char a[], char x[])
{
    int indx=0, inda=0, i;

    for(i=0; i<18; i++)
        if(strcmp(a, token[i]) == 0)
            inda = i;

    for(i=0; i<15; i++)
        if(strcmp(x, varsint[i]) == 0)
            indx=i;

    for(i=0; i<29; i++)
    {
        if(indx == tablaM[i][0])
            if(inda == tablaM[i][1])
                return i;
    }

    return 999;
}

