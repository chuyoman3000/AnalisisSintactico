#include "tabladesimbolos.h"

TablaDeSimbolos::TablaDeSimbolos(int tam)
{
    if(tam <= 0) {
        size = 0;
        table = nullptr; //0 o NULL
    } else if ((table = new Symbol *[size = tam]) != NULL)
        for(int i =0; i < size; i++)
            table[i] = nullptr;
     else
        size = 0;

}


//MUESTRE GRAFICAMENTE EL FUNCIONAMIENTO DE ESTE METODO

//   "hola", "HOLA", "holA", "tec", "laguna"
//COMO FINALIZA EL METODO HASH PARA HACER EL RETURN
int TablaDeSimbolos::Hash(char *nombre) //"casa"
{
    //slot, celda, indice
    int slot = 0;
    while(*nombre)
        slot += *nombre++;
    return slot % size;
}

/*
El metodo AddSym crea una nueva entrada para un simbolo,
siempre que se le pueda asignar un espacio. Se utiliza Hash
para decidir el espacio donde se insertara la entrada al inicio
de la lista enlazada.
Este metodo regresa un apuntador 0  (NULL) cuando no se puede
asignar espacio para un simbolo. Esto determina si la solicitud
tuvo exito.

*/


Symbol *TablaDeSimbolos::AddSym(char *name, char *symbol)
{
    Symbol *sym;
    sym = new Symbol;
    sym->name = new char[strlen(name)+1];
    sym->symbol = new char[strlen(symbol)+1];
       //indice
    int slot = Hash(name); //NULL
    if(sym !=  NULL && sym->name != NULL && sym->symbol != NULL)
    {
        cout<<"VALOR :  "<<name<<endl;
        //cout<<"NOMBRE: "<<name<<endl;
        strcpy(sym->name,name);
        strcpy(sym->symbol,symbol);
        //sym->next = table[slot];
        table[slot]=sym;
        return sym;
    }
    else       //si falla new
    {
        delete sym;
        sym = 0;
    }
    return sym;

}

Symbol *TablaDeSimbolos::FindSym(char *name)
{
    int slot = Hash(name);
    Symbol *sym= table[slot];
    for(; sym ; sym = sym->next )
        if(strcmp(sym->name,name) == 0 )
            break;
    return sym;
}
void TablaDeSimbolos::PrintTable()
{
    for(int i = 0; i < size; i++)
        for(Symbol *sym = table[i];  sym   ; sym = sym->next)
//EN SU ANALISIS MOSTRAR LOS CONTENIDOS
            cout<<"LEXEMA"<< setw(10) <<sym->symbol<<setw(20)<<"VALOR"<<setw(15)<<sym->name<<endl;//"Next : "<<hex<<sym->next<<dec<<endl;

}
             //BORRAR UN SIMBOLO
bool TablaDeSimbolos::RmvSym(char * name)
{
      //indice
    int slot = Hash(name);
    Symbol *sym = table[slot];

    string aux = sym->name;

    if(sym && strcmp(sym->name,name)== 0) //inicio de la lista
    {
      table[slot] = sym->next;
      delete sym;
      return true;
    }


    for(sym = table[slot]; sym->next ; sym = sym->next) //resto de la lista
        if(strcmp(sym->next->name,name)== 0)
        {
            Symbol *tmp = sym->next;
            sym->next = sym->next->next;
            delete tmp;
            return true;
        }
    return false;
}
bool TablaDeSimbolos::SetValue(char *name, SymValue val)
{
    Symbol *sym = FindSym(name);
    //if(sym == nullptr)
       // return AddSym(name) != nullptr;
   // else
        //sym->value = val;
    return true;
}

bool TablaDeSimbolos::GetValue(char *name, SymValue *val)
{
    Symbol *sym = FindSym(name);
    if(sym != NULL)  //0  o nullptr
       // *val = sym->value;
    return sym != 0;
}


