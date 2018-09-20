// se hacen todos las funciones con la memria: reservarla, leerla, limpiarla, imprimir la y sus partes,buscar sus partes, almacenamiento y swich entre memorias
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int paging[32];
int *posIntMain;

int paggnigVirtual[64];
int *posIntVirt;

//PARA QUE ES ESTO
/*
int tamDescriptor[2][32];

int descriptor=0;
int moverVirtual(int desc);
*/
//..........RESERVAR............
int fReservar(int n,int desc)
{   
	int pagDispo=fPaginasDisponibles();
	if(n>128)
	{
		return -1;
	}
	else
	{
    //Páginas necesarias 
		int pages=n/2+(n%2);
		
		int descaux;
    //Reviso si hay páginas disponibles
		if(pagDispo>=pages)
		{
        //Aumento el descriptor para ser el proceso actual
			if(desc<0)
			{
				descriptor++;
				descaux = descriptor;
			}else{
				descaux=desc;   
			}

			int faltan=pages;
			int pos=0;

            //Recorro la memoria y donde este vacia lo guardo
			while(pos<32&&faltan>0)
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
			}

			//tamDescriptor[1][descriptor-1]=pages;
			int posDes=0;
			while(posDes<32&&tamDescriptor[0][posDes]!=descriptor){
				posDes++;
			}
			tamDescriptor[1][posDes]=pages;

			

			ordenar();
			return descriptor;
		}
		else
		{
            //printf("No cupo en la memoria fisica");
			//se tiene que mover a memoria virtual

			int necesito=pages-pagDispo;
			int posMat=0;
			//cantidad del mayor
			int mayor;

			while(posMat<32){

				mayor=tamDescriptor[1][posMat];

                //printf("necesito= %i, mayor= %i\n",necesito,mayor);

				if(mayor>=necesito){
					printf("mover descriptor %i a virtual ",tamDescriptor[0][posMat]);
					moverVirtual(tamDescriptor[0][posMat]);
					posMat=32;

				}
				else{
					printf("mover descriptor %i a virtual ",tamDescriptor[0][posMat]);
					necesito-=mayor;
					moverVirtual(tamDescriptor[0][posMat]);

				}
				posMat++;


			}
			if(posMat!=32){
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

                        //Recorro la memoria y donde este vacia lo guardo
				while(pos<32&&faltan>0)
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
				}

                        //tamDescriptor[1][descriptor-1]=pages;
				int posDes=0;
				while(posDes<32&&tamDescriptor[0][posDes]!=descriptor){
					posDes++;
				}
				tamDescriptor[1][posDes]=pages;

				ordenar();
				return descriptor;       


			}else{



				return -1;

			//se tiene que mover a memoria virtual

            //miro cuantas paginas hay disponibles 
			}
		}
	}
}

int vReservar(int n, int desc)
{
	if(n>128)
	{
		return -1;
	}
	else
	{
    //Páginas necesarias 
		int pages=n/2+(n%2);
		
    //Reviso si hay páginas disponibles
		if(vPaginasDisponibles()>=pages)
		{
        //Aumento el descriptor para ser el proceso actual
			if(desc<0)
			{
				descriptor++;
			}

			int faltan=pages;
			int pos=0;

        //Recorro la memoria y donde este vacia lo guardo
			while(pos<128 && faltan>0)
			{
				if(paginacionVirtual[pos]==0)
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
