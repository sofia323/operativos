#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int paginacion[32];
int *posIniRam;

int paginacionVirtual[64];
int *posIniVirtual;

int tamDescriptor[2][16];// puntero en memoria

int descriptor=0;
int moverVirtual(int desc );

//...limpiar...

void limpiar(){
	int *pun;
	pun=posIniRam;
	for (int i = 0; i < 16; ++i)//recoje toda la memoria main
	{
		if(paginacion[i]==0){ //ciclo para eliminar la posicion
			*pun=0; 
			*(pun+1)=0;
		}
		pun+=2;
	}
	pun=posIniVirtual;
	for (int i = 0; i < 32; ++i)//recoje toda la memoria virtual
	{
		if(paginacionVirtual[i]==0){ //ciclo par eliminar posicion
			*pun=0;
			*(pun+1)=0;
		}
		pun+=2;
	}
}

//...ordenar...

void ordenar(){
	int n=16;
	int i, key, j,value;
	for (i = 1; i < n; i++) //recoje la memoria main
	{
		key = tamDescriptor[1][i];
		value=tamDescriptor[0][i];
		j = i-1;

		while (j >= 0 && tamDescriptor[1][j] < key)
		{
			tamDescriptor[1][j+1] = tamDescriptor[1][j];
			tamDescriptor[0][j+1] = tamDescriptor[0][j];
			j = j-1;
		}
		tamDescriptor[1][j+1] = key;
		tamDescriptor[0][j+1] = value;
	}
}

//...imprimir paginas...

void printTamDes(){
	for (int j = 0; j < 2; ++j){
		for (int i = 0; i < 16; ++i){
			printf("%i ",tamDescriptor[j][i]);
		}
		printf("\n");
	}
	printf("\n");
}
void printPagiR()
{
	for (int i = 0; i < 16; ++i) 
	{
		printf("%i ",paginacion[i]);
	}
	printf("\n\n");
}
void printPagiV()
{
	for (int i = 0; i < 32; ++i)
	{
		printf("%i ",paginacionVirtual[i]);
	}
	printf("\n\n");
}

//...imprimir memoria...

void printMemoriaR()
{
	int *pun;
	pun=posIniRam;
	for (int i = 0; i < 32; ++i)
	{
		printf("%i ",*pun);
		pun++;
		if(i%2==1)
		{
			printf("|");
		}
	}
	printf("\n\n");
}

void printMemoriaV()
{
	int *pun;
	pun=posIniVirtual;
	for (int i = 0; i < 64; ++i)
	{
		printf("%i ",*pun);
		pun++;
		if(i%2==1)
		{
			printf("|");
		}
	}
	printf("\n\n");
}

//...N° pag disponibles...

int PaginasDisponiblesR()
{
	int cont=0;
	for (int i = 0; i < 16; ++i)
	{
		if(paginacion[i]==0)
		{
			cont++;
		}
	}
	return cont;
}

int PaginasDisponiblesV()
{
	int cont=0;
	for (int i = 0; i < 32; ++i)
	{
		if(paginacionVirtual[i]==0)
		{
			cont++;
		}
	}
	return cont;
}

//...reservar...

int fReservar(int n,int desc) //desc es un descriptor de la ram
{   
	int pagDispo=fPaginasDisponibles();
	if(n>32)
	{
		return -1; //se va para memoria virtual porque necesita mas delo que tiene la memoria
	}
	else
	{
		int pages=n/2+(n%2); //numero páginas necesarias para el proceso
		int descaux;
		if(pagDispo>=pages)  //Reviso si hay páginas disponibles
		{
			if(desc<0)//cuando termina el programa,no hay mas memoria
			{
				descriptor++;  //Aumento el descriptor para ser el proceso actual
				descaux = descriptor; //descriptor como si fuero un puntero de la memoria
			}else{
				descaux=desc;   
			}
			int faltan=pages;
			int pos=0;
			while(pos<16 && faltan>0) //si no he recojido todas las paginas y todavia me faltan por meter
			{
				if(paginacion[pos]==0) //Recorro la memoria y donde este vacia y puede guardo lo
				{
					if(desc<0)
					{
						paginacion[pos]=descriptor;
					}
					else
					{
						paginacion[pos]=desc;
					}
					faltan--;
				}
				pos++;
			}
			//tamDescriptor[1][descriptor-1]=pages;
			int posDes=0;
			while(posDes<16 && tamDescriptor[0][posDes]!=descriptor){
				posDes++;
			}
			tamDescriptor[1][posDes]=pages;
			ordenar();
			return descriptor;
		}
		else //se tiene que mover a memoria virtual
		{
			int necesito=pages-pagDispo;
			int posMat=0;
			int mayor;
			while(posMat<16){
				mayor=tamDescriptor[1][posMat];
                //printf("necesito= %i, mayor= %i\n",necesito,mayor);
				if(mayor>=necesito){
					printf("mover descriptor %i a virtual ",tamDescriptor[0][posMat]);
					moverVirtual(tamDescriptor[0][posMat]);
					posMat=16;
				}
				else{
					printf("mover descriptor %i a virtual ",tamDescriptor[0][posMat]);
					necesito-=mayor;
					moverVirtual(tamDescriptor[0][posMat]);
				}
				posMat++;
			}
			if(posMat!=16){
                //fReservar(n,-1);
				if(desc<0)
				{
					descriptor++;
					descaux = descriptor;
				}else{
					descaux=desc;   
				}
				int faltan=pages;
				int pos=0;
				while(pos<16 && faltan>0)  //Recorro la memoria y donde este vacia lo guardo
				{
					if(paginacion[pos]==0)
					{
						if(desc<0)
						{
							paginacion[pos]=descriptor;
						}
						else
						{
							paginacion[pos]=desc;
						}
						faltan--;
					}
					pos++;
				}             //tamDescriptor[1][descriptor-1]=pages;
				int posDes=0;
				while(posDes<16 && tamDescriptor[0][posDes]!=descriptor){
					posDes++;
				}
				tamDescriptor[1][posDes]=pages;
				ordenar();
				return descriptor;       
			}else{ //se tiene que mover a memoria virtual
				return -1;
			}
		}
	}
}

int vReservar(int n, int desc)
{
	if(n>64)
	{
		return -1;
	}
	else
	{
		int pages=n/2+(n%2);//Páginas necesarias 
		if(vPaginasDisponibles()>=pages) //Reviso si hay páginas disponibles
		{
			if(desc<0)
			{
				descriptor++;//Aumento el descriptor para ser el proceso actual
			}
			int faltan=pages;
			int pos=0;
			while(pos<32 && faltan>0)
			{
				if(paginacionVirtual[pos]==0)//Recorro la memoria y donde este vacia lo guardo
				{
					if(desc<0)
					{
						paginacionVirtual[pos]=descriptor;
					}
					else
					{
						paginacionVirtual[pos]=desc;
					}
					faltan--;
				}
				pos++;
			}
			return descriptor;
		}
		else
		{
			printf("No cupo en la memoria virtual");
			return -1;
		}
	}
}

//...actualizar...

void actualizarTamanos(){
	for (int i = 0; i < 32; ++i)
	{
		tamDescriptor[1][i]=fTamano(tamDescriptor[0][i]);
	}
	ordenar();
}

//...tamaño procesos...

int fTamano(int proc)
{
	int tam=0;
	for (int i = 0; i < 16 ; ++i)
	{
		if (paginacion[i]==proc)
		{
			tam++;
		}
	}
	return tam;
}

int vTamano(int proc)
{
	int tam=0;
	for (int i = 0; i < 32 ; ++i)
	{
		if (paginacionVirtual[i]==proc)
		{
			tam++;
		}
	}
	return tam;
}

//...almacenar...

int fAlmacenar(int proc,char*a)
{
	int pos=0;
	int n = strlen(a); //tamaño cadena 
	int carActual=0;
	if(n<=fTamano(proc)*2)
	{
		int *pun;
		pun=posIniRam;
		int i =0,j=0;
		while(i<32 && j<n){ //recorre la memoria
			if(paginacion[i]==proc)
			{
				*pun=a[carActual];
				carActual++;
				pun++;
				*pun=a[carActual];
				carActual++;
				pun++;
				j++;
			}
			else
			{
				pun+=2;
			}
			i++;
			pos++;
		}
	}
	else
	{
		printf("No cupo en el espacio reservado\n");
		return -1;
	}
}

int vAlmacenar(int proc,char*a)
{
	int pos=0;
	int n = strlen(a); //tamaño cadena 
	int carActual=0;
	if(n<=vTamano(proc)*2)
	{
		int *pun;
		pun=posIniVirtual;
		int i =0,j=0;
		while(i<32 && j<n){
			if(paginacionVirtual[i]==proc)
			{
				*pun=a[carActual];
				printf("pos>%i carac>%c\n",carActual,a[carActual]);
				carActual++;
				printf("pos>%i carac>%c\n",carActual,a[carActual]);
				pun++;
				*pun=a[carActual];
				carActual++;
				pun++;
				j++;
			}
			else
			{
				pun+=2;
			}
			i++;
			pos++;
		}

	}
	else
	{
		printf("No cupo en el espacio reservado\n");
		return -1;
	}
}

//...leer procesos...
//PORQUE HAY DOS MANERAS DE LEER??
int fLeer(int proc)
{
	int pos=0,tam=0;
	int *pun;
	pun=posIniRam;
	while(pos<16) //lee memoria
	{
		if(paginacion[pos]==proc)
		{
			tam++;
			printf("%c",*pun);
			printf("%c",*(pun+1));
		}
		pos++;
		pun+=2;
	}  
	printf("\n");
	if(tam==0)
	{
		printf("descriptor no encontrado\n");
		return -1;
	}
	return 1;
}

int vLeer(int proc)
{
	int pos=0,tam=0;
	int *pun;
	pun=posIniVirtual;
	while(pos<32) //menor al numero de paginas, lee toda la memoria
	{
		if(paginacionVirtual[pos]==proc)
		{
			tam++;
			printf("%c",*pun);
			printf("%c",*(pun+1));
		}
		pos++;
		pun+=2;
	}  
	printf("\n");
	if(tam==0)
	{
		printf("descriptor no encontrado\n");
		return -1;
	}
	return 1;
}

char* fContenido(int proc)
{
	int pos=0,tam=0;
	int *pun;
	pun=posIniRam;
	char*lectura=(char*) calloc(1, sizeof(char));
	while(pos<16)
	{
		if(paginacion[pos]==proc)
		{
			tam++;
			strcat(lectura,pun); 
			strcat(lectura,pun+1); 
		}
		pos++;
		pun+=2;
	}  
	printf("\n");
	if(tam==0)
	{
		printf("descriptor no encontrado\n");
	}
	printf("La cadena es: %s y su tamano es:%i\n",lectura,(int )strlen(lectura));
	return lectura;
}

char* vContenido(int proc)
{
	int pos=0,tam=0;
	int *pun;
	pun=posIniVirtual;
	char*lectura=(char*) calloc(1, sizeof(char));
	while(pos<32)
	{
		if(paginacionVirtual[pos]==proc)
		{
			tam++;
			strcat(lectura,pun); 
			strcat(lectura,pun+1); 
		}
		pos++;
		pun+=2;
	}  
	if(tam==0)
	{
		printf("descriptor no encontrado\n");
	}
	return lectura;
}

//...liberar procesos...

int fLiberar(int proc)
{
	int borrados=fTamano(proc);
	int tam =borrados*2;
	char aux[tam];
	/*
	for (int i = 0; i < tam; ++i)
	{
		aux[i]=0;
	}*/
	int bus=0;
	printf("antes while\n");
	while(bus<16 && tamDescriptor[0][bus] != proc){
		bus++;
	}
	tamDescriptor[1][bus]=0;
	printf(" bus> %i\n",bus);
	//fAlmacenar(proc,aux);
	//printf("%i\n",proc);
	for (int i = 0; i < 16; ++i)
	{
		if(paginacion[i]==proc)
		{
			paginacion[i]=0;
		}
	}
	limpiar();
}

int vLiberar(int proc)
{
	int borrados=vTamano(proc);
	int tam =borrados*2;
	char aux[tam];
	//printf("%i\n",proc);
	for (int i = 0; i < 32; ++i)
	{
		if(paginacionVirtual[i]==proc)
		{
			paginacionVirtual[i]=0;
		}
	}
	limpiar();
}

//...mover a fisico...

int moverFisico(int desc)
{
	int pagVirtualDispo=vPaginasDisponibles();
	int pagRamDispo=fPaginasDisponibles();
	int tam =vTamano(desc);
	if(tam-pagRamDispo<=pagVirtualDispo){
		int a=fReservar(vTamano(desc)*2,desc);
		int b=fAlmacenar(desc,vContenido(desc));
		if(a>=0 && b>=0)
		{
			printf("entro\n");
			vLiberar(desc);
			actualizarTamanos();
		}
	}
}

//...mover a virtual...

int moverVirtual(int desc)
{
	int a=vReservar(fTamano(desc)*2,desc);
	int b=vAlmacenar(desc,fContenido(desc));
	if(a>=0 && b>=0)
	{
		fLiberar(desc);
	}
}

//...main...

int main(){
	for (int i = 0; i < 16 i++){
		paginacion[i]=0;
		tamDescriptor[0][i]=i+1;	
	}
    //definir memorias
	
	//HELP
	//HELP
	
	int *iniRam = (int *)calloc(32*2,2);
	int *iniVirtual = (int *)calloc(64*2,2);

    //posicion inicial memorias
	posIniVirtual=iniVirtual;
	posIniRam=iniRam;
	
