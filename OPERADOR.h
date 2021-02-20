#include <iostream>
#include <sstream>

using namespace std;

typedef long int tipolista;

class Nodo
{
    protected: 
        tipolista dato;
        Nodo *next;
    public:
        Nodo() {next=NULL;};
        Nodo(tipolista a) {dato=a; next=NULL;}
        Nodo *get_next() {return next;}
        tipolista get_dato() {return dato;}
        bool es_vacio() {return next==NULL;}
        void set_dato(tipolista a) {dato=a;}
        void set_next(Nodo *n) {next=n;}
};

class Lista
{
    protected:
		Nodo *czo;
    public:
            Lista() {czo=new Nodo();}
            Lista(Nodo *n) {czo=n;}
            Lista *resto();
            tipolista cabeza();
            void del();
            void add(tipolista d);
            bool esvacia();
};

class Pila : public Lista
{
      public:
            Pila(){Lista();};
            tipolista tope(){return cabeza();}
            void apilar(tipolista x){add(x);}
            void desapilar(){del();}
            bool pilavacia(){return esvacia();}
};                  

void Lista::del()
{
	Nodo* aux;
    aux = czo;
    czo = czo->get_next();
    delete aux;
}

void Lista::add(tipolista d)
{  
    Nodo* nuevo = new Nodo(d);
    nuevo->set_next(czo);
    czo = nuevo;
}

bool Lista::esvacia()
{   
    return czo->es_vacio();
}

tipolista Lista::cabeza()
{ 
  if(esvacia())
  {
  	cout << " Error, Cabeza de lista vacia";
    return ' '; 
  }
  
  return czo->get_dato();
}

Lista *Lista::resto()
{ 
	Lista* l = new Lista(czo->get_next());
	
	return l;
}

// funcion precedencia 
// prcd(o1,'(') = 0 para todo o1 != de ')'
// prcd(o1,')') = 1 para todo o1 != de '('
// prcd('(',o2) = 0 para todo 02
// prcd(o1,o2)  = 1 para todo o1 >= precedencia que o2
int prcd(int o1,int o2)
{
	if(o2=='(')
	 if(o1==')') return 1;
        else return 0;
        
  	if(o2==')')
	 if(o1=='(') return 0;
		else return 1;
		
	if(o1=='(') return 0;

  	if((o1=='*')||(o1=='/')) return 1;
  	if(((o1=='+')||(o1=='-'))&&((o2=='+')||(o2=='-'))) return 1;
		else return 0;
}

int operar(string cadena)
{
	string pf;
    Pila *p = new Pila();

 	//Chequeo de la correspondencia de los ()
  	int error = 0;
  	
	for(int i=0;(i<cadena.length())&&(!error);i++)
    {
		if(cadena.at(i)=='{') p->apilar('{');       
		if(cadena.at(i)=='[') p->apilar('[');
	    if(cadena.at(i)=='(') p->apilar('(');
		      
	    if(cadena.at(i)=='}')
		{
	        if(p->pilavacia()) error=1;
	        else
	            if(p->tope()!='{') error=1;
	            else p->desapilar();
		}                               
		if(cadena.at(i)==']')
		{
		    if(p->pilavacia()) error=1;
		    else
	           if(p->tope()!='[') error=1;
	           else p->desapilar();                     
		}                               
		if(cadena.at(i)==')')
		{
		    if(p->pilavacia()) error=1;
	        else
		        if(p->tope()!='(') error=1;
		        else p->desapilar();                     
		}
	}
	
	//Conversion de entrefijo a posfijo
	char d,p1;
	     
	for(int j=0;j<cadena.length();j++)
	{
		d = cadena.at(j);
		    
		if ((d>='0')&&(d<='9'))
		{
			pf.push_back(d);
	    }
		else
		{
			while((!p->pilavacia())&&(prcd(p->tope(),d)))
		    {
				p1 = p->tope();
				p->desapilar();
				pf.push_back(p1);
			}
		           
			if((p->pilavacia())||(d!=')')) p->apilar(d);
		    else p->desapilar();
		}
	}
	      
	while(!p->pilavacia())
	{
		p1 = p->tope();
		p->desapilar();
		pf.push_back(p1);
	}
	      
	// Evaluacion de la expresion en posfijo
	int o1,o2;
	  
    for(int i=0;(i<pf.length())&&(!error);i++)
	{ 
	    d = pf.at(i);
	     
		if(d>='0' && d<='9') p->apilar(d-'0');
		
	    if(d=='+')
		{
	        o2=p->tope(); p->desapilar();
	        o1=p->tope(); p->desapilar(); 
	        p->apilar(o1+o2);
	    }
	    
	    if(d=='-')
		{
	        o2 = p->tope(); p->desapilar();
	        o1 = p->tope(); p->desapilar(); 
	        p->apilar(o1-o2);
	    }
		   
	    if(d=='*')
		{
	        o2 = p->tope(); p->desapilar();
	        o1 = p->tope(); p->desapilar();
	        p->apilar(o1*o2);
	    }         
		
		if(d=='/')
		{
	        o2 = p->tope(); p->desapilar();
	        o1 = p->tope(); p->desapilar();
	        p->apilar(o1/o2);
	    }      
	}
	
	return p->tope();
}

int operarLogico(string cadena)
{
	string pf;
    Pila *p = new Pila();

 	//Chequeo de la correspondencia de los ()
  	int error = 0;
  	
	for(int i=0;(i<cadena.length())&&(!error);i++)
    {
		if(cadena.at(i)=='{') p->apilar('{');       
		if(cadena.at(i)=='[') p->apilar('[');
	    if(cadena.at(i)=='(') p->apilar('(');
		      
	    if(cadena.at(i)=='}')
		{
	        if(p->pilavacia()) error=1;
	        else
	            if(p->tope()!='{') error=1;
	            else p->desapilar();
		}                               
		if(cadena.at(i)==']')
		{
		    if(p->pilavacia()) error=1;
		    else
	           if(p->tope()!='[') error=1;
	           else p->desapilar();                     
		}                               
		if(cadena.at(i)==')')
		{
		    if(p->pilavacia()) error=1;
	        else
		        if(p->tope()!='(') error=1;
		        else p->desapilar();                     
		}                               
	}  
	
	//Conversion de entrefijo a posfijo    
	char d,p1;
	     
	for(int j=0;j<cadena.length();j++)
	{
		d = cadena.at(j);
		    
		if ((d>='0')&&(d<='9'))
		{
			pf.push_back(d);
	    }
		else
		{
			while((!p->pilavacia())&&(prcd(p->tope(),d)))
		    {
				p1 = p->tope();
				p->desapilar();
				pf.push_back(p1);
			}
		           
			if((p->pilavacia())||(d!=')')) p->apilar(d);
		    else p->desapilar();
		}
	}
	      
	while(!p->pilavacia())
	{
		p1 = p->tope();
		p->desapilar();
		pf.push_back(p1);
	}
	      
	// Evaluacion de la expresion en posfijo      
	int o1,o2;
	  
    for(int i=0;(i<pf.length())&&(!error);i++)
	{ 
	    d = pf.at(i);
	     
		if(d>='0' && d<='9') p->apilar(d-'0');
		
	    if(d=='<')
		{
	        o2=p->tope(); p->desapilar();
	        o1=p->tope(); p->desapilar();
	        
	        if(o1<o2) p->apilar(1);
	        else p->apilar(0);
	    }
	    
	    if(d=='>')
		{
	        o2=p->tope(); p->desapilar();
	        o1=p->tope(); p->desapilar();
	        
	        if(o1>o2) p->apilar(1);
	        else p->apilar(0);
	    }
		   
	    if(d=='!')
		{
	        o2=p->tope(); p->desapilar();
	        o1=p->tope(); p->desapilar();
	        
	        if(o1!=o2) p->apilar(1);
	        else p->apilar(0);
	    }     
		
		if(d=='=')
		{
	        o2=p->tope(); p->desapilar();
	        o1=p->tope(); p->desapilar();
	        
	        if(o1==o2) p->apilar(1);
	        else p->apilar(0);
	    }       
	}
	
	return p->tope();
}
