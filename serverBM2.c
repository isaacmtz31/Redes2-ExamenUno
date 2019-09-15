#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>//exit
#include <netdb.h>
#include "serverMB.h"
#define PUERTO "1234"

int main(int argc, char* argv[])
{
  int sd,n,v=1,rv,op=0;
  socklen_t ctam;
  char s[INET6_ADDRSTRLEN], hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

  FILE *f,*f1;
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_storage their_addr; // connector's address
  struct stat st;
  ctam= sizeof(their_addr);
  memset(&hints, 0, sizeof (hints));  //indicio
  hints.ai_family = AF_INET6;    /* Allow IPv4 or IPv6  familia de dir*/
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; // use my IP
  hints.ai_protocol = 0;          /* Any protocol */
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;

 if ((rv = getaddrinfo(NULL, PUERTO, &hints, &servinfo)) != 0)
 {
     fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
     return 1;
 }//if

for(p = servinfo; p != NULL; p = p->ai_next)
{
  if ((sd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1)
  {
    perror("server: socket");
    continue;
  }
  if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &v,sizeof(int)) == -1)
  {
    perror("setsockopt");
    exit(1);
  }

	if (setsockopt(sd, IPPROTO_IPV6, IPV6_V6ONLY, (void *)&op, sizeof(op)) == -1)
  {
    perror("setsockopt   no soporta IPv6");
    exit(1);
  }

  if (bind(sd, p->ai_addr, p->ai_addrlen) == -1)
  {
    close(sd);
    perror("server: bind");
    continue;
  }

  printf("Servicio iniciado... esperando cliente\n");
  break;
}
  freeaddrinfo(servinfo); // all done with this structure

if (p == NULL)
{
  fprintf(stderr, "servidor: error en bind\n");
  exit(1);
}
  listen(sd, 5);
  socklen_t clientADDRESSLENGTH;
  struct sockaddr_in clientADDRESS;

  for( ;; )
  {
    int cd, enviados = 0;
    clientADDRESSLENGTH = sizeof(clientADDRESS);
    cd = accept(sd, (struct sockaddr *)&their_addr, &ctam);
    if (cd == -1)
    {
      perror("accept");
      exit(1);
    }//if
    if (getnameinfo((struct sockaddr *)&their_addr, sizeof(their_addr), hbuf, sizeof(hbuf), sbuf,sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV) == 0)
        printf("cliente conectado desde %s:%s\n", hbuf,sbuf);
        f1 = fdopen(cd,"w+");
        struct linger linger;
            linger.l_onoff = 1;
            linger.l_linger = 30;
        if(setsockopt(cd,SOL_SOCKET, SO_LINGER,(const char *) &linger,sizeof(linger))==-1)
        {
          perror("setsockopt(...,SO_LINGER,...)");
        }//if

        int dificultad;
        int dif;
        n = read(cd,&dificultad,sizeof(dificultad));
        printf("\nSe recibieron %d bytes\n",n);
        dif = htons(dificultad);
        if(n < 0)
        {
          perror("Error en funcion read()\n");
          close(cd);
          exit(1);
        } else if( n==0 )
        {
          perror("Socket cerrado\n");
          close(cd);
          exit(1);
        }
        else
        {
          printf("La dificultad es: %d\n", dif );

          int ** matriz = NULL;
          switch (dif) {
            case 4: //Ver puntuaciones

            break;

            case 1: //Principiante
              matriz = asignaMemoria(9,9);
              llenaMatrizRan(&(*matriz), 9, 9, 10);
              imprimeMat(matriz, 9,9);
              for(int i = 0; i < 9 ; i++)
              {
                for(int j = 0; j < 9; j++)
                {
                  int aux = ntohl(matriz[i][j]);
                  enviados = write(cd,&aux,sizeof(aux));
                }
              }
            break;
            case 2: //Intermedio
              matriz = asignaMemoria(16,16);
              imprimeMat(matriz, 16,16);
              llenaMatrizRan(&(*matriz), 16, 16, 40);
              for(int i = 0; i < 16 ; i++)
              {
                for(int j = 0; j < 16; j++)
                {
                  int aux = htons(matriz[i][j]);
                  enviados = write(cd,&aux,sizeof(aux));
                }
              }
            break;

            case 3: //Experto
              matriz = asignaMemoria(16,30);
              imprimeMat(matriz, 16,30);
              llenaMatrizRan(&(*matriz), 16, 30, 99);
              for(int i = 0; i < 16; i++)
              {
                for(int j = 0; j < 30; j++)
                {
                  int aux = htons(matriz[i][j]);
                  enviados = write(cd,&aux,sizeof(aux));
                }
              }
            break;

            default:
              printf("ERROR EN LEER LA OPCIÓN\n");
              exit(-1);
            break;
          }
        }//else
	}//for;;

        close(sd);

return 0;
}//main
