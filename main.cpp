//Valerie Yace (801-17-9737)
//Stephanie Ramos (802-12-5866)
//Carlos Diaz (801-17-6405)
//Spell Checker with Hash Table: Final Project

#include <iostream>
#include<vector>
#include<map>
#include<set>
#include<queue>
#include<math.h>
#include<fstream>

using namespace std;

class HashTable
{
    
private:
    int hashFunction(string st);
    int standSize = 19443;
    vector< vector <string> > bucket;
    int mySize = 0;
    
public:
    void add(HashTable &h, string st);
    void remove(HashTable &h, string st);
    bool contains(HashTable &h, const string st)const;
    int size(const HashTable &h);
    HashTable(int size);
};

//constructor con parametro de tama~o
HashTable::HashTable(int size)
{
    bucket.resize(size);
}

//funcion hash
int HashTable::hashFunction(string st)
{
    int count = 0, length = st.length();
    
    //le asigna a cada caracter un valor especifico
    //segun su posicion en el string
    for (int i = 0, e = length-1; i < length; i++, e--)
    {
        count += st[i] * pow( 2 , e );
    }
    
    count = count % standSize;
    
    return count;
}

//devuelve true si encuentra el string en la tabla,
//de lo contrario devuelve false
bool HashTable::contains(HashTable &h, const string st)const
{
    int position = h.hashFunction(st);
    
    //recorre el vector interior en el bucket que
    //deberia encontrarse
    for (int i = 0; i < bucket[position].size(); i++)
    {
        if (bucket[position][i] == st)
            return true;
    }
    
    return false;
}

//inserta en el hash table
void HashTable::add(HashTable &h, string st)
{
    int position = h.hashFunction(st);
    
    //para no insertar strings iguales
    if (!h.contains(h, st))
    {
        bucket[position].push_back(st);
        mySize++;
        
    }
    else
        cout << "Repeated word " << endl;
}

//remueve strings del hash table
void HashTable::remove(HashTable &h, string st)
{
    int position = h.hashFunction(st);
    
    //si el string aparece en el vector lo elimina
    if (h.contains(h, st))
    {
        //cuando llega aquí, recorre el vector interior para eliminar el string deseado
        for (int i = 0; i < bucket[position].size(); i++)
        {
            if (bucket[position][i] == st)
            {
                bucket[position][i].erase();
                mySize--;
            }
        }
    }
}

int HashTable::size(const HashTable &h)
{
    return mySize;
}


int main()
{
    //variable para detener ciclo while
    //para el cin del texto a corregir
    const string STOP = "-1";
    
    HashTable diccionario(19433);
    string words;
    int timesMisspelled = 0, totalMisspells = 0, ordermispelled = 0,
    totalWords = 0;
    set <string> docSpellCheck;
    map <string, int> misspelled;
    
    //para almacenar los near misses
    //en orden alfabetico
    std::priority_queue<string, std::vector<string>, std::greater<string> > nearMisses;
    
    
    ifstream fp;
    string st;
    fp.open("/Users/stephaniem.ramos/Documents/Proyecto_Final-Spelling_Checker/Proyecto_Final-Spelling_Checker/demoenglish3.txt");
    
    // ******************************************* \\
    
    //para calcular tiempo de ejecucion
    //de hacer el diccionario
    double elapsed_secs;
    clock_t begin, end;
    
    begin = clock();
    
    //pone el diccionario en el hash table
    while (fp >> st)
    {
        cout << st << endl;
        diccionario.add(diccionario, st);
    }
    
    //tiempo de ejecucion del diccionario
    end = clock();
    
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    
    cout << "Time elapsed for building dictionary: "
    << elapsed_secs << "secs " << endl;
    // ******************************************* \\
    
    //para calcular tiempo de ejecucion
    //para procesar el texto a analizar
    double elapsed_secs2;
    clock_t begin2, end2;
    
    begin2 = clock();
    
    //pone el texto que se hara spell check
    //dentro del set
    while (cin >> words)
    {
        //cuando el usuario entra -1,
        //se detiene el ciclo
        if (words == STOP)
            break;
        else
        {
            docSpellCheck.insert(words);
            
            if (!diccionario.contains(diccionario, words))
            {
                //si es la primera vez que aparece el misspell
                if (misspelled.find(words)==misspelled.end())
                {
                    timesMisspelled = 1;
                    misspelled[words] = timesMisspelled;
                }
                //cuando aparece por segunda vez en adelante
                else
                {
                    misspelled[words]++;
                }
                
                totalWords++;
                totalMisspells++;
            }
            else
            {
                totalWords++;
                
            }
        }
    }
    //tiempo de ejecucion del analisis del texto
    end2 = clock();
    
    elapsed_secs2 = double(end2 - begin2) / CLOCKS_PER_SEC;
    
    // ******************************************* \\
    
    //Near Misses Strategies
    for (auto e: misspelled)
    {
        ordermispelled++;
        
        string palabra = e.first;
        
        //variable que permite que la palabra
        //vuelva a su estado original luego del ciclo
        string copiaPalabra = palabra;
        
        
        for (int i = 0 ; i < palabra.length(); i++)
        {
            //se utiliza los valores de ascii codes
            //en las letras minusculas para insertarlas
            //en cada posicion posible
            for(int ascii = 97; ascii <= 122; ascii++)
            {
                //Replace Strategy
                palabra[i] = (char)ascii;
                
                //si es una palabra en el diccionario,
                //la pone en el min-priority queue
                if (diccionario.contains(diccionario, palabra))
                {
                    nearMisses.push(palabra);
                }
                //la palabra vuelve a su estado original
                palabra = copiaPalabra;
                
                //Insert Strategy
                palabra.insert(palabra.begin() + i,(char)ascii);
                
                //si es una palabra en el diccionario,
                //la pone en el min-priority queue
                if (diccionario.contains(diccionario, palabra))
                {
                    nearMisses.push(palabra);
                }
            }
            //la palabra vuelve a su estado original
            palabra= copiaPalabra;
            
            //Delete Strategy
            palabra.erase(palabra.begin() + i);
            
            //si es una palabra en el diccionario,
            //la pone en el min-priority queue
            if (diccionario.contains(diccionario, palabra))
            {
                nearMisses.push(palabra);
            }
            
            //la palabra vuelve a su estado original
            palabra = copiaPalabra;
            
            if(i != palabra.length())
            {
                //la palabra vuelve a su estado original
                palabra = copiaPalabra;
                
                //Swap Strategy
                swap(palabra[i], palabra[i+1]);
                
                //si es una palabra en el diccionario,
                //la pone en el min-priority queue
                if (diccionario.contains(diccionario, palabra))
                {
                    nearMisses.push(palabra);
                }
                
                //la palabra vuelve a su estado original
                palabra = copiaPalabra;
            }
        }
        
        cout << ordermispelled << ". " << palabra;
        if(e.second > 1)
        {
            cout << " (found " << e.second << " times)";
        }
        cout << ": ";
        if (nearMisses.empty())
        {
            cout << "No options." << endl;
        }
        else
        {
            while(!nearMisses.empty())
            {
                cout << nearMisses.top() << " ";
                nearMisses.pop();
            }
        }
        
        cout << endl;
    }
    
    cout << endl << "words considered: " << totalWords << endl;
    cout << "misspelled: " << totalMisspells << endl;
    
    cout << endl << "exec time for spell check:"
    << elapsed_secs2 << " secs " << endl;
    
    return 0;
}

