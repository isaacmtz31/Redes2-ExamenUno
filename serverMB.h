#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

//ESTA FUNCION ES UTIL PARA ASIGAR DE MANERA DINAMICA MEMORIA A LAS MATRICES
int ** asignaMemoria(int filas, int columnas)
{
	int i = 0;
  int j = 0;
	int **aux = (int**)malloc(filas * sizeof(int*));
	if(aux != NULL)
	{
		int k = 0;
		for(k; k< filas; k++)
		{
			aux[k] = (int*)malloc(columnas * sizeof(int));
		}

	}

  //Antes de enviar llenamos con 1's
  for(i = 0; i < filas; i++)
  {
    for(j = 0; j < columnas; j++)
    {
      aux[i][j] =  1;
    }
  }
  printf("Se asigno memorio y se lleno de 1's\n");
	return aux;
}

//ESTA FUNCION ES UTIL PARA IMPRIMIR MATRICES
int imprimeMat(int** matriz, int fil, int col)
{
	int i = 0;
	int j = 0;
	for(i; i< fil; i++)
	{
		printf("\n");
		for(j; j < col; j++)
		{
			printf("| %d |", matriz[i][j]);
		}
		printf("\n");
		j=0;
	}
	printf("\n");
	printf("\n");
	return 0;
}

//ESTA FUNCION ES UTIL PARA LLENAR DE MANERA RANDOM MATRICES
int llenaMatrizRan(int ** matriz, int fil, int col, int numBom)
{
	int i = 0;
	int j = 0;
	int n = 0;
  int m = 0;
  int contB = 0;
  int ** aux  = matriz;
	srand(time(NULL));
  while(contB < numBom)
  {
    for(i = 0; i < fil ; i++)
    {
      for(j = 0; j < col; j++)
      {
        n = rand() % (1000+1);
        m = rand() % (1000+1);
        n = ((n*2) + (m*3));
        if(matriz[i][j] == 1 && contB<numBom) //El 1 significa que no tiene cambios en la casilla
        {
          if( n%9 == 0 )
          {
            matriz[i][j] = -1;
            contB=contB+1;
          }
          else
          {
            continue;
          }
        }
        else
        {
          if(matriz[i][j] == -1) //0 La casilla no contiene bomba
          {
            continue;
          }
        }
      }
      n=0;
    }
  }

  //La matriz ya contiene todas las bombas
  for(i = 0; i < fil; i++)
  {
    for(j = 0; j < col; j++)
    {
        if(matriz[i][j] != -1)
        {
          matriz[i][j] = 0;
        }
    }
  }
	return 0;
}




//ESTA FUNCION ES UTIL PARA LIBERAR DE MANERA DINAMICA MEMORIA A LAS MATRICES
void limpiar(int** matriz, int colu)
{
	int i = 0;
	for(i; i < colu; i++)
		free(matriz[i]);
	free(matriz);
}


//GENERAMOS LA MATRIZ CON EL NUMERO DE BOMBAS YESPECIFICADO
int ** generarMatriz(int dif)
{
  int ** matriz = NULL;
  if ( dif == 1 ) //Principiante
  {
    matriz = asignaMemoria(9,9);
    //imprimeMat(matriz, 9,9);
    llenaMatrizRan(&(*matriz), 9, 9, 10); //10 bombas

  }
  else
  {
    if( dif == 2 ) //Intermedio
    {
      matriz = asignaMemoria(16,16);
      llenaMatrizRan(matriz, 16, 16, 40); // 40 bombas
    }
    else //Experto
    {
      matriz = asignaMemoria(16,30);
      llenaMatrizRan(&(*matriz), 16, 30, 99); // 99 bombas
    }
  }

  return matriz;
}



int escribirPuntuacion(char ** score)
{
	//char * arch1 = "/home/imanol/Documentos/scores.txt";
	FILE * f;
	char * arch1 = "/home/isaacmtz/Documentos/scores.txt";
	if (f = fopen(arch1, "r+"))
	{
		fseek(f, 0, SEEK_END );
		if(fputs((*score), f) != -1)
		{
			fputc(10, f);
			printf("OK\n");
			fclose(f);
			return 1;
		}
	}else{
		printf("Error al abrir el archivo\n");
		fclose(f);
		return 0;
	}
}

int ganarPerderPuntaje(char * usuario)
{
/*
	if( escribirPuntuacion(usuario) == 1 )
	{
		printf("El puntaje fue escrito correctamente\n");
		return 1;
	}
	else
	{
		printf("Imposible escribir el puntaje\n");
	}*/
	/*
	int n;

	switch (statusx)
	{
		case 0: // GANO //RECIBIMOS EL NOMBRE Y TIEMP

			if( n < 0 )
			{
				printf("Error al recibir los datos del ganador\n");
				return -1;
			}
			else
			{
				printf("TIEMPOUSR %s\n",usuario );
				if( escribirPuntuacion(usuario) == 1 )
				{
					printf("El puntaje fue escrito correctamente\n");
					return 1;
				}
				else
				{
					printf("Imposible escribir el puntaje\n");
				}
			}
		break;

		case 9: //PERDIO
			printf("El usuario ha perdido la partida. El puntaje no se escribirá\n");
			break;
		break;
	}*/
}


//ENVIAMOS LA MATRIZ AL CLIENTE
int enviarMatriz(int filas, int columnas, int cd, int ** matriz, char * usuario)
{
	int prueba;
	int aux;
	int enviados;
	int n;
	//int m;
	for(int i = 0; i < filas ; i++)
	{
		for(int j = 0; j < columnas; j++)
		{
			 //aux = htonl(matriz[i][j]);
			aux = matriz[i][j];
			 enviados = write(cd,&aux,sizeof(aux));
			 if(enviados < 0)
			 {
				 printf("No se pudo enviar la matriz\n");
				 return -1;
			 }
		}
	}

	printf("Esperando el termino de la partida PRUEBA\n");
	n = read(cd,&prueba,sizeof(prueba));
	if( n < 0)
	{
		printf("No se pudo leer el estatus del jugador\n");
		return -1;
	}

	printf("EL ESTATUS FINAL ES %d\n", prueba);

	if(prueba==0)
 	{
		printf("NO PUES SI ENTRA\n" );
		if((read(cd, usuario, sizeof(usuario))) == 0)
		printf("EL socket anda cerrado\n");
		else if((read(cd, usuario, sizeof(usuario))) < 0 )
			printf("No se leyo nada\n");
			else
				{
					read(cd, usuario, sizeof(usuario));
					printf("USR %s\n", usuario);
					escribirPuntuacion(&usuario);}
	}
	else
	{
		printf("NO GANO, ADIOS!\n");
	}

}
