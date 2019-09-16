#include <netdb.h>  //getaddrinfo() getnameinfo() freeaddrinfo()
#include <netinet/in.h> //htons
#include <string.h>  //bzero
#include <stdio.h>   //printf perror
#include <stdlib.h> //atoi() exit()
#include <sys/types.h>
#include <unistd.h>  //exit
#include "serverMB.h"



  int main(int argc, char* argv[])
  {
  int cd,n,rv,op=0;
  char *PUERTO="1234";
  long tam;
  struct sockaddr_in serverADDRESS;
  struct hostent *hostINFO;
  char *srv="::1";
  FILE *f;

int status;
struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6  familia de dir*/
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;

    if ((rv = getaddrinfo(srv, PUERTO, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((cd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

	/*if (setsockopt(cd, IPPROTO_IPV6, IPV6_V6ONLY, (void *)&op, sizeof(op)) == -1) {
            perror("setsockopt   no soporta IPv6");
            exit(1);
        }*/

        if (connect(cd, p->ai_addr, p->ai_addrlen) == -1) {
            close(cd);
            perror("client: connect");
            continue;
        }

        break;
    }//for

    if (p == NULL) {
        fprintf(stderr, "client: error al conectar con el servidor\n");
        return 2;
    }

    freeaddrinfo(servinfo); // all done with this structure

    printf("\nConexion establecida con el servidor...\n");
    //Hacer scanf
    int dificultad = 1;
    int aux = ntohs(dificultad);
    int m = write(cd, &aux, sizeof(aux));
    int ** matriz = NULL;
    int ij = 0;
    int ijaux = 0;
    switch (dificultad)
     {
       case 1:
         matriz = asignaMemoria(9,9);
         for(int i = 0; i < 9 ; i++)
         {
           for(int j = 0; j < 9; j++)
           {
             read(cd,&ij,sizeof(ij));
             ijaux = htonl(ij);
             matriz[i][j] = ijaux;
           }
         }
         printf("El usuario ha recibido la matriz: \n");
         imprimeMat(matriz, 9,9);
       break;
       case 2:
       break;
       case 3:
       break;
       case 4:
        printf("Esperando scores\n");
       break;
       default:
        printf("ERROR AL LEER OPCIÓN\n");
       break;
    }


return 0;
}//main
