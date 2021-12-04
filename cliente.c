#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* sockets */
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#define IP_SERVIDOR "127.0.0.1"

int main() {
	int socket_cliente;
	int puerto;
	char ip_servidor[10];
	int tam_struct_cliente;
	int estadoConexion;

	char cadenaCliente[1024];
    char cadenaServidor[1024];

	socket_cliente = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in servidor_direccion;
    struct sockaddr_in cliente_direccion;

    printf("Ingresa el puerto del servidor:\n");
    scanf("%d", &puerto);

	cliente_direccion.sin_family = AF_INET;
	cliente_direccion.sin_addr.s_addr = inet_addr(IP_SERVIDOR);
	cliente_direccion.sin_port = puerto;
	tam_struct_cliente = sizeof(cliente_direccion);

    estadoConexion = connect(socket_cliente, (struct sockaddr *)&cliente_direccion, tam_struct_cliente );
    if(estadoConexion == -1 ){
         printf("Problemas al conectar con servidor... \n");
         exit(-1);
         close(socket_cliente);
    }
    printf("\t--SESION INICIADA CON EL SERVIDOR--\n");

	char nivel[2];

	printf("\t     SELECCIONE DIFICULTAD: \n");
	printf("\t     1- FACIL (ANIMALES) \n");
	printf("\t     2- MEDIA (VEHICULOS) \n");
	printf("\t     3- DIFICIL (PLATILLOS) \n");
	scanf("%c", &nivel[0]);
	send(socket_cliente, nivel, 1024, 0);

	while(strcmp(cadenaCliente, "BREAK") != 0) {

        recv(socket_cliente, cadenaServidor, 1024, 0);
        if(strcmp(cadenaServidor, "BREAK") == 0) {
        	break;
        } else {
        	printf("%s", cadenaServidor); // la palabra es
        }

		recv(socket_cliente, cadenaServidor, 1024, 0);
		printf("%s", cadenaServidor); // _ _ _ _ _ _ _

		recv(socket_cliente, cadenaServidor, 1024, 0);
		printf("%s", cadenaServidor); // ingrese una

		printf("Letra: ");
		scanf("%*c%[^\n]", cadenaCliente);
		send(socket_cliente, cadenaCliente, 1024, 0);

		recv(socket_cliente, cadenaServidor, 1024, 0);
		//printf("OPOR: %s\n", cadenaServidor);
        if(strcmp(cadenaServidor, "NO") == 0) {
        	recv(socket_cliente, cadenaServidor, 1024, 0);
			printf("%s", cadenaServidor); // ingrese una
        }
  	}
  	recv(socket_cliente, cadenaServidor, 1024, 0);
  	printf("%s", cadenaServidor);

    close(socket_cliente);
}
