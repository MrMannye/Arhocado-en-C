#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* sockets */
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#define CHANCE 6
#define MAX 0x100

const char *animales[]={"ABEJA", "BALLENA", "CABALLO", "COCODRILO", 
"DELFIN", "ESCORPION", "FLAMENCO", "HAMSTER", "IGUANA", "LEON"};


const char *vehiculos[]={"TAXI", "AUTOBUS", "CARRO", "BICICLETA", 
"VELERO", "HELICOPTERO", "MOTOCICLETA", "AVION", "METRO", "TELEFERICO"};


const char *platillos[]={"PAELLA", "CROISSANT", "SOUFFLE", "TAMALES", 
"ENCHILADAS", "RAMEN", "SUSHI", "SASHIMI", "WASABI", "BURRITOS"};

char inttochar(int x) {
	return x+'0';
}

int main() {

	struct sockaddr_in servidor_direccion;
    struct sockaddr_in cliente_direccion;
    int tam_struct_servidor;
    int tam_struct_cliente;
    int puerto;

	int socket_servidor, socket_cliente;
	int socket_bind;
	int socket_listen;
	char cadenaCliente[1024];
    char cadenaServidor[1024];

	socket_servidor = socket(AF_INET, SOCK_STREAM, 0); // creacion del socket
	if(socket_servidor == -1) {
        printf("No se puede crear el socket \n");
        exit(-1);
    }
    printf("Ingrese el puerto de escucha:\n");
    scanf("%d", &puerto);

	servidor_direccion.sin_addr.s_addr = inet_addr("127.0.0.1");
	servidor_direccion.sin_family = AF_INET;
    servidor_direccion.sin_port = puerto;
    tam_struct_servidor = sizeof(servidor_direccion);

	socket_bind = bind(socket_servidor, (struct sockaddr *)&servidor_direccion, tam_struct_servidor);
	if(socket_bind == -1) {
		printf("No se puede crear el bind\n");
		exit(-1);
	}

	int backlog = 1;
	socket_listen = listen(socket_servidor, backlog);
	if(socket_listen == -1) {
		printf("No se puede crear el listen\n");
		exit(-1);
	}
	printf("Servidor en espera...\n");

	while(1) {
		socket_cliente = accept(socket_servidor, (struct sockaddr *)&cliente_direccion, &tam_struct_servidor);
		if(socket_cliente == -1) {
			printf("No se puede crear el accept \n");
			exit(-1);
		}
		printf("\t-- CLIENTE CONECTADO --\n");

		char nivel[2];
		recv(socket_cliente, nivel, 1024, 0);


		const char *elegida;
		srand(time(NULL));
		int aleatoria;
		aleatoria = rand()%15;
		int opcion = (int)nivel[0];

		switch (opcion)
		{
		case 1:
			elegida = animales[aleatoria];
			break;
		case 2:
			elegida = vehiculos[aleatoria];
			break;
		case 3:
			elegida = platillos[aleatoria];
			break;
		default:
			break;
		}
		
		printf("%s | %ld\n", elegida, strlen(elegida));
		int x = strlen(elegida);

		int oportunidades = CHANCE;

		int N = strlen(elegida);
		int mask[N];
		char aux[1024];
		for (int i=0; i < N; ++i) {
		    mask[i] = 0;
		}
		char copia[1024];
		strcpy(copia, elegida);
		int cont = 0;
		int gameover = 0;
		char opor[1024] = "";
		while(strcmp(cadenaServidor, "SALIR") != 0 && gameover == 0) {
			strcpy(opor, "NO");
			int flag = 0;
			send(socket_cliente, "\nLa palabra es: ", 1024, 0);
			for(int j=0; j < N; ++j) {
				if (mask[j] == 1) {
				    aux[j] = copia[j];
				}
				else {
				    aux[j] = '_';
				}
			}

			send(socket_cliente, aux, 1024, 0);

			char guess[2];
			send(socket_cliente, "\nIngrese una ", 1024, 0);
			recv(socket_cliente, guess, 1024, 0);

			for(int k=0; k < N; ++k) {
				if (copia[k] == guess[0]) {
				    mask[k] = 1;
				    flag = 1;
				    strcpy(opor, "SI");
				}
			}
			send(socket_cliente, opor, 1024, 0);
			if(flag == 0) {
				oportunidades--;
				char c = inttochar(oportunidades);
				char o[50] = "Te quedan ";
				int x = strlen(o);
				o[strlen(o)] = c;
				strcat(o, " oportunidades.\n");
				send(socket_cliente, o, 1024, 0);
				if(oportunidades == 0) {
				   	send(socket_cliente, "BREAK", 1024, 0);
				    break;
				}
			}

			for(int m = 0; m < N; ++m) {
				cont += mask[m];
				if (mask[m] == 0) {
				    cont = 0;
				    break;
				} else if(cont == N) {
				   	gameover = 1;
				    send(socket_cliente, "BREAK", 1024, 0);
				}
			}
		}
		if(oportunidades == 0) {
			char p[50] = "\nPerdiste! La palabra es ";
			copia[N] = '\n';
			copia[N+1] = '\0';
			strcat(p, copia);
			send(socket_cliente, p, 1024, 0);
		} else {
			char g[50] = "\nGanaste! La palabra es ";
			copia[N] = '\n';
			copia[N+1] = '\0';
			strcat(g, copia);
			send(socket_cliente, g, 1024, 0);
		}
		close(socket_cliente);
	}
	close(socket_servidor);

}
