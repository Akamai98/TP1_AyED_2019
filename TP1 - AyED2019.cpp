#include<string.h> //Para objetos de tipo 'string'.
#include<iostream> //Para instrucción 'cout'.
#include<fstream> //I/O de archivos *.txt.
#include "OPERADOR.h" //Para analizar expresiones.

using namespace std;

//Nodos
class NodoInstruccion
{
	private:
		string instruccion, numeroInstruccion;
		NodoInstruccion* next;
	public:
		NodoInstruccion() { this->next = NULL; }
		NodoInstruccion(string,string);
		~NodoInstruccion() {};
		NodoInstruccion* getNext() { return this->next; }
		string getInstruccion() { return this->instruccion; }
		string getNumeroInstruccion() { return this->numeroInstruccion; }
		bool estaVacio() { return this->getNext()==NULL; }
		void setNext(NodoInstruccion* next) { this->next = next; }
		void setInstruccion(string instruccion) { this->instruccion = instruccion; }
		void setNumeroInstruccion(string numeroInstruccion) { this->numeroInstruccion = numeroInstruccion; }
};

class NodoVariable
{
	private:
		char nombreVariable;
		int valorVariable;
		NodoVariable* next;
	public:
		NodoVariable() { this->next = NULL; }
		NodoVariable(char);
		~NodoVariable() {};
		NodoVariable* getNext() { return this->next; }
		char getNombreVariable() { return this->nombreVariable; }
		int getValorVariable() { return this->valorVariable; }
		bool estaVacio() { return this->getNext()==NULL; }
		void setNext(NodoVariable* next) { this->next = next; }
		void setValorVariable(int valorVariable) { this->valorVariable = valorVariable; }
};

//Métodos de los nodos de instrucciones
NodoInstruccion::NodoInstruccion(string instruccion, string numeroInstruccion)
{
	this->instruccion = instruccion;
	this->numeroInstruccion = numeroInstruccion;
	
	next = NULL;
}

//Métodos de los nodos de variables
NodoVariable::NodoVariable(char nombreVariable)
{
	this->nombreVariable = nombreVariable;

	next = NULL;
}

//Listas
class ListaInstrucciones
{
	private:
		NodoInstruccion* comienzo;
	public:
		ListaInstrucciones() {comienzo = new NodoInstruccion();}
		ListaInstrucciones(NodoInstruccion* n) {comienzo = n;}
		~ListaInstrucciones() {};
		ListaInstrucciones* resto();
		NodoInstruccion* getComienzo() { return this->comienzo; }
		NodoInstruccion* buscadorInst(string);
		string getCabeza();
		int cantidadInstrucciones();
		bool estaVacia() { return comienzo->estaVacio(); }
		void addInstruccion(string,string);
		void readTXTFile();
		void rellenarLista(ListaInstrucciones*);
		void mostrarInstrucciones();
		void free();
};

class ListaVariables
{
	private:
		NodoVariable* comienzo;
	public:
		ListaVariables() { this->comienzo = new NodoVariable(); }
		ListaVariables(NodoVariable* n) { this->comienzo = n; }
		~ListaVariables() {};
		ListaVariables* resto();
		NodoVariable* getComienzo() { return this->comienzo; }
		NodoVariable* buscarVariable(char);
		int getCabeza();
		int getValorVariable(char);
		bool estaVacia() { return this->comienzo->estaVacio(); }
		void addVariable(char);
		void free();
};

void ListaInstrucciones::free()
{
	if(!this->comienzo->estaVacio())
	{
		NodoInstruccion *nodoAux,*pivote;
	
		pivote = comienzo;
		nodoAux = comienzo;
		
		pivote = pivote->getNext();
		comienzo = pivote;
		
		delete nodoAux;
		
		this->free();
	}
}

NodoInstruccion* ListaInstrucciones::buscadorInst(string line)
{
	if(this->comienzo->getNumeroInstruccion().compare(line)!=0) this->resto()->buscadorInst(line);

	return this->comienzo;
}

int ListaInstrucciones::cantidadInstrucciones()
{
	int contador = 0;
	
	NodoInstruccion* pivote = comienzo;
	
	while(pivote->getNext()!=NULL)
	{
		contador++;
		
		pivote = pivote->getNext();
	}
	
	return contador;
}

void ListaInstrucciones::addInstruccion(string i, string n)
{
	NodoInstruccion* nuevoNodo = new NodoInstruccion(i,n);
	
	nuevoNodo->setNext(comienzo);
	
	comienzo = nuevoNodo;
}

ListaInstrucciones* ListaInstrucciones::resto()
{
	ListaInstrucciones* lista = new ListaInstrucciones(comienzo->getNext());
	
	return lista;
}

string ListaInstrucciones::getCabeza()
{
	if(estaVacia()) return "ERROR";
	
	return comienzo->getInstruccion();
}

void ListaInstrucciones::rellenarLista(ListaInstrucciones* listaAux)
{
	if(!listaAux->estaVacia())
	{
		this->addInstruccion(listaAux->getCabeza(),listaAux->comienzo->getNumeroInstruccion());

		rellenarLista(listaAux->resto());
	}
}

void ListaInstrucciones::mostrarInstrucciones()
{
	cout << "----INSTRUCCIONES A REALIZAR----" << endl << endl;
	
	NodoInstruccion* pivote = new NodoInstruccion();
	
	pivote = comienzo;
	
	while(pivote->getNext()!=NULL)
	{
		cout << pivote->getInstruccion() << endl;

		pivote = pivote->getNext();
	}
	
	cout << endl
		 << "----SALIDA DEL PROGRAMA----"
		 << endl << endl;
}

void ListaInstrucciones::readTXTFile()
{
	ifstream txtFile;

	string line,cantidadInstrucciones;
	
	ListaInstrucciones* listaAux = new ListaInstrucciones();

	NodoInstruccion* pivote = listaAux->comienzo;
	
	int cantidad = 0;
	
	txtFile.open("code.txt",ios::in); //Abro el archivo en modo lectura.
	
	if(txtFile.fail())
	{
		cout << "ERROR AL LEER ARCHIVO .txt" << endl;
		
		system("PAUSE");
		
		exit(1);
	}
	
	while(!txtFile.eof()) //Analizo el archivo *.txt siempre suponiendo que está escrito correctamente.
	{
		getline(txtFile,line,'\n'); //Recorro el archivo 'txtFile' línea por línea. Cada línea la almaceno en el string 'line'.
		
		int i = 0;
		
		while(line.at(i)!='.')
		{
			cantidad++;

			i++;
		}
		
		cantidadInstrucciones = line.substr(0,cantidad);

		listaAux->addInstruccion(line,cantidadInstrucciones);
		
		line.clear();

		cantidad = 0;
	}
	
	rellenarLista(listaAux);
	
	cout << "----INICIO DEL PROGRAMA----" << endl << endl
		 << "SE CARGARON LAS INSTRUCCIONES DEL ARCHIVO .txt" << endl << endl;
}

//Métodos de la lista de variables
int ListaVariables::getValorVariable(char nombre)
{
	if(this->comienzo->getNombreVariable()==nombre) return this->getCabeza();
	else this->resto()->getValorVariable(nombre);
}

NodoVariable* ListaVariables::buscarVariable(char nombre)
{
	if(this->comienzo->getNombreVariable()==nombre) return this->comienzo;
	else this->resto()->buscarVariable(nombre);
}

void ListaVariables::free()
{
	if(!this->comienzo->estaVacio())
	{
		NodoVariable *nodoAux,*pivote;
	
		pivote = comienzo;
		nodoAux = comienzo;
		
		pivote = pivote->getNext();
		comienzo = pivote;

		delete nodoAux;
		
		this->free();
	}
}

void ListaVariables::addVariable(char n)
{
	NodoVariable* nuevoNodo = new NodoVariable(n);
	
	nuevoNodo->setNext(comienzo);
	
	comienzo = nuevoNodo;
}

ListaVariables* ListaVariables::resto()
{
	ListaVariables* lista = new ListaVariables(comienzo->getNext());
	
	return lista;
}

int ListaVariables::getCabeza()
{
	if(estaVacia()) return -1;
	
	return comienzo->getValorVariable();
}

//Funciones aparte
string reemplaza_var(ListaVariables* variables, string expresion)
{
	for(int i=0; i<expresion.length(); i++) //Recorro el string  para encontrar las variables...
	{
		if(expresion.at(i)>=97 && expresion.at(i)<=122) //Cuando encuentro una variable declarada...
		{
			int valorAux = variables->getValorVariable(expresion.at(i)); //Almaceno su valor en una variable entera auxiliar...

			string valorAuxString = to_string(valorAux); //Convierto su valor de int a string...
			
			expresion.replace(i,1,valorAuxString); //Y reemplazo el valor en forma de string en la posición correspondiente del string auxiliar.
		}
	}
	
	return expresion;
}

bool tiene_variables(string expresion)
{
	bool tiene = false;
	
	for(int i=0; i<expresion.length(); i++)
	{
		if(expresion.at(i)>=97 && expresion.at(i)<=122)
		{
			tiene = true;

			break;
		}
	}
	
	return tiene;
}

bool busca_logico(string expresion)
{
	bool op = false;
	
	for(int i=0; i<expresion.length(); i++)
	{
		if(expresion.at(i)=='<' || expresion.at(i)=='>' || expresion.at(i)=='!' || expresion.at(i)=='=')
		{
			op = true;

			break;
		}
	}
	
	return op;
}

string saca_iguales(string lineaLogica)
{
	for(int i=0; i<lineaLogica.length(); i++)
	{
		if(lineaLogica.at(i)=='!' && lineaLogica.at(i+1)=='=')
		{
			lineaLogica.erase(i+1,1);

			break;
		}
		else if(lineaLogica.at(i)=='=' && lineaLogica.at(i+1)=='=')
		{
			lineaLogica.erase(i,1);

			break;
		}
	}
	
	return lineaLogica;
}

void ejecutar(ListaInstrucciones* instrucciones, ListaVariables* variables)
{
	bool tieneVariables = false;

	char nombreVariable;
	
	int cantidad = 0;

	string line = instrucciones->getCabeza();
	string expresion, naux;

	NodoInstruccion *pivote = instrucciones->getComienzo();
	NodoInstruccion *aux = new NodoInstruccion();
	
	while(pivote->getNext()!=NULL)
	{
		line = pivote->getInstruccion();
		
		cout << "VOY A REALIZAR LA INSTRUCCION " << line << endl;
		
		for(int i=0; i<line.length(); i++)
		{
			if(line.at(i)=='I')
			{
				if(line.at(i+1)=='N' && line.at(i+2)=='T') //INT
				{
					char nombreVariable = line.at(i+4);
						
					variables->addVariable(nombreVariable);
					
					cout << "SE CREO UNA VARIABLE DE NOMBRE: " << nombreVariable << " EN LA LISTA DE VARIABLES" << endl << endl;
					
					continue;
				}
				else if(line.at(i+1)=='F') //IF
				{
					cantidad = 0;
					
					int desde;
					
					for(int j=i+4; j<line.length(); j++)
					{
						if(line.at(j)==')')
						{
							desde = j;
							break;
						}

						cantidad++;
					}
					
					string lineaLogica = line.substr(i+4,cantidad);
					
					cout << "VOY A EVALUAR LA EXPRESION: " << lineaLogica << endl;
					
					if(tiene_variables(lineaLogica))
					{
						lineaLogica = reemplaza_var(variables,lineaLogica);
						
						cout << "REEMPLAZADA ES: " << lineaLogica << endl;
					}
						 
					lineaLogica = saca_iguales(lineaLogica);
					
					if(operarLogico(lineaLogica)==1)
					{
						cout << "HE DETERMINADO QUE LA EXPRESION LOGICA ES VERDADERA." << endl
							 << "PROCEDO A REALIZAR LA INSTRUCCION: ";
							 
						line = line.substr(desde+7,(line.length()-(desde+7)));
						
						cout << line << endl << endl;
						
						i = -1;
						
						continue;
					}
					else
					{
						cout << "HE DETERMINADO QUE LA EXPRESION LOGICA ES FALSA." << endl
						 	 << "PROCEDO CON LA SIGUIENTE INSTRUCCION." << endl << endl;
						
						if(pivote->getNext()->getNext()==NULL) break;
						else
						{
							pivote = pivote->getNext();
							
							i = -1;
							
							line = pivote->getInstruccion();
							
							cout << "VOY A REALIZAR LA INSTRUCCION: " << line << endl;
							
							continue;
						}
					}

					continue;
				}
			}
			else if(line.at(i)=='J' && line.at(i+1)=='U' && line.at(i+2)=='M' && line.at(i+3)=='P') //JUMP
			{
				aux = instrucciones->getComienzo();
				naux = line.substr(i+5,(line.length()-(i+5)));
				
				while(aux->getNext()!=NULL)
				{
					if(aux->getNumeroInstruccion().compare(naux)==0) break;

					aux = aux->getNext();
				}
				
				line = aux->getInstruccion();
				
				cout << "VOY A SALTAR A LA INSTRUCCION NUMERO: " << naux << endl
					 << "LA INSTRUCCION A EJECUTAR ES: " << line << endl;
				
				i = -1;
				
				pivote = aux;
				
				continue;
			}
			else if(line.at(i)=='S' && line.at(i+1)=='H' && line.at(i+2)=='O' && line.at(i+3)=='W') //SHOW
			{
				char caux;
				bool op = false;
				int largo = line.length() - (i+5);

				expresion = line.substr(i+5,largo);
				
				if(expresion.length()==1)
				{
					caux = line.at(i+5);
					
					cout << "VOY A MOSTRAR EN PANTALLA EL VALOR DE LA VARIABLE " << caux << ": " << variables->buscarVariable(caux)->getValorVariable() << endl << endl;
					
					continue;
				}
				else
				{
					cout << "VOY A MOSTRAR EN PANTALLA EL VALOR DE LA EXPRESION: " << expresion << endl;
					
					bool es_bool = false;
					
					es_bool = busca_logico(expresion);
					
					if(es_bool)
					{
						cout << "DETERMINE QUE LA EXPRESION ES LOGICA." << endl;
						
						if(tiene_variables(expresion))
						{
							expresion = reemplaza_var(variables,expresion);
							
							cout << "REEMPLAZADA ES: " << expresion << endl;
						}
						
						cout << "EL RESULTADO ES: ";
							
						expresion = saca_iguales(expresion);
						
						if(operarLogico(expresion)==1)
						{
							cout << "TRUE" << endl << endl;

							continue;
						}
						else
						{
							cout << "FALSE" << endl << endl;

							continue;
						}
						
						continue;
					}
					else
					{
						cout << "DETERMINE QUE LA EXPRESION ES ARITMETICA." << endl;
						
						if(tiene_variables(expresion))
						{
							expresion = reemplaza_var(variables,expresion);
							
							cout << "REEMPLAZADA ES: " << expresion << endl;
						}
						
						cout << "EL RESULTADO ES: " << operar(expresion) << endl << endl;
						
						continue;
					}
				}
			}
			else if(line.at(i)=='=' && line.at(i-1)!='!' && line.at(i-1)!='=' && line.at(i+1)!='=') //ASIGNACIÓN
			{	
				int largo = line.length() - (i+1);
				
				nombreVariable = line.at(i-1);
				expresion = line.substr(i+1,largo); //Tomo el string después de '='. Tomo 'a + 1'.
				
				tieneVariables = tiene_variables(expresion);
				
				if(tieneVariables) //Si la expresión tiene variables declaradas...
				{
					expresion = reemplaza_var(variables,expresion);
					
					cout << "REEMPLAZADA ES: " << expresion << endl;
					
					//Cuando reemplacé todo, mando la expresión final string auxiliar al evaluador para obtener su valor numérico para asignárselo a la variable.	
					variables->buscarVariable(nombreVariable)->setValorVariable(operar(expresion));
				}
				else
				{
					//Si no tiene variables declaradas, mando la expresión string auxiliar al evaluador para obtener su valor numérico para asignárselo a la variable.
					variables->buscarVariable(nombreVariable)->setValorVariable(operar(expresion));
				}
				
				cout << "SE ASIGNO EL VALOR " << variables->buscarVariable(nombreVariable)->getValorVariable() << " EN LA VARIABLE DE NOMBRE " << nombreVariable << endl << endl;
				
				continue;
			}
		}
		
		if(pivote->getNext()->getNext()==NULL) break;
		else pivote = pivote->getNext();
	}
	
	cout << endl << "----FIN DEL PROGRAMA----" << endl << endl;
}

//Función main
int main()
{
	ListaInstrucciones* instrucciones = new ListaInstrucciones();

	ListaVariables* variables = new ListaVariables();
	
	instrucciones->readTXTFile();
	instrucciones->mostrarInstrucciones();
	
	ejecutar(instrucciones,variables);
	
	cout << "----FINALIZADO EL PROGRAMA, PROCEDO A LIBERAR MEMORIA----" << endl << endl;
	
	instrucciones->free();

	variables->free();
	
	system("PAUSE");

	return EXIT_SUCCESS;
}
