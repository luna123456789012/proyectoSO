// Incluye librerï¿­as necesarias
#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <errno.h>

#define MAX_USUARIOS 100

// Estructura para usuarios conectados
typedef struct {
	int socket;
	char usuario[50];
} UsuarioConectado;

UsuarioConectado usuarios[MAX_USUARIOS];
int num_usuarios = 0;
pthread_mutex_t usuarios_mutex;

// Estructura para invitaciones
typedef struct {
	char invitador[50];
	char invitados[10][50];
	int aceptados[10];
	int total_invitados;
	int respuestas_recibidas;
} Invitacion;

Invitacion invitaciones[MAX_USUARIOS];
int num_invitaciones = 0;

// Conexiï¿³n a MySQL
MYSQL* ConectarSQL() {
	MYSQL *conn = mysql_init(NULL);
	if (!conn || !mysql_real_connect(conn, "localhost", "root", "mysql", "juego", 0, NULL, 0)) {
		fprintf(stderr, "Error en la conexion MySQL: %s\n", mysql_error(conn));
		mysql_close(conn);
		return NULL;
	}
	return conn;
}

// Agregar usuario conectado
void AgregarUsuario(int socket, const char* usuario) {
	pthread_mutex_lock(&usuarios_mutex);
	if (num_usuarios < MAX_USUARIOS) {
		strncpy(usuarios[num_usuarios].usuario, usuario, sizeof(usuarios[num_usuarios].usuario) - 1);
		usuarios[num_usuarios].socket = socket;
		num_usuarios++;
	} else {
		printf("Se alcanzo el maximo de usuarios.\n");
	}
	pthread_mutex_unlock(&usuarios_mutex);
}

void EliminarUsuario(int socket) {
	pthread_mutex_lock(&usuarios_mutex);
	for (int i = 0; i < num_usuarios; i++) {
		if (usuarios[i].socket == socket) {
			for (int j = i; j < num_usuarios - 1; j++)
				usuarios[j] = usuarios[j + 1];
			num_usuarios--;
			break;
		}
	}
	pthread_mutex_unlock(&usuarios_mutex);
}

// Procesar invitaciï¿³n (cï¿³digo 11)
void ProcesarInvitacion(int sock_conn, char* invitador, char* lista) {
	if (!invitador || !lista || strlen(lista) == 0) {
		char msg[] = "11/Error en datos de invitaciï¿³n";
		write(sock_conn, msg, strlen(msg));
		return;
	}
	
	char lista_original[256];
	strncpy(lista_original, lista, sizeof(lista_original) - 1);
	lista_original[sizeof(lista_original) - 1] = '\0';
	
	char* lista_para_envio = strdup(lista_original);
	char* lista_para_guardar = strdup(lista_original);
	if (!lista_para_envio || !lista_para_guardar) {
		perror("Error al duplicar memoria para lista");
		free(lista_para_envio);
		free(lista_para_guardar);
		return;
	}
	
	// Enviar invitaciones
	char* token_envio = strtok(lista_para_envio, ",");
	while (token_envio != NULL) {
		pthread_mutex_lock(&usuarios_mutex);
		for (int i = 0; i < num_usuarios; i++) {
			if (strcmp(usuarios[i].usuario, token_envio) == 0) {
				char mensaje[512];
				snprintf(mensaje, sizeof(mensaje), "INVITE/%s/%s", invitador, lista_original);
				write(usuarios[i].socket, mensaje, strlen(mensaje));
				break;
			}
		}
		pthread_mutex_unlock(&usuarios_mutex);
		token_envio = strtok(NULL, ",");
	}
	
	// Guardar invitaciï¿³n
	Invitacion* inv = &invitaciones[num_invitaciones];
	strncpy(inv->invitador, invitador, sizeof(inv->invitador) - 1);
	inv->respuestas_recibidas = 0;
	inv->total_invitados = 0;
	
	char* token_guardar = strtok(lista_para_guardar, ",");
	while (token_guardar != NULL && inv->total_invitados < 10) {
		strncpy(inv->invitados[inv->total_invitados], token_guardar, 49);
		inv->aceptados[inv->total_invitados] = 0;
		inv->total_invitados++;
		token_guardar = strtok(NULL, ",");
	}
	num_invitaciones++;
	
	free(lista_para_envio);
	free(lista_para_guardar);
}


// Funcion auxiliar para eliminar un usuario sin bloquear el mutex (se asume bloqueo ya adquirido)
void EliminarUsuarioSinLock(int socket) 
{
	for (int i = 0; i < num_usuarios; i++) {
		if (usuarios[i].socket == socket) {
			for (int j = i; j < num_usuarios - 1; j++) {
				usuarios[j] = usuarios[j + 1];
			}
			num_usuarios--;
			break;
		}
	}
}

void RegistrarUsuario(int sock_conn, MYSQL *conn, char *usuario, char *password) 
{
	char consulta[256], respuesta[512];
	snprintf(consulta, sizeof(consulta), "SELECT COUNT(*) FROM jugadores WHERE usuario = '%s'", usuario);
	if (mysql_query(conn, consulta)) {
		snprintf(respuesta, sizeof(respuesta), "Error en consulta");
	} else {
		MYSQL_RES *res = mysql_store_result(conn);
		if (res == NULL) {
			snprintf(respuesta, sizeof(respuesta), "Error en resultado");
		} else {
			MYSQL_ROW row = mysql_fetch_row(res);
			if (row != NULL && atoi(row[0]) > 0) {
				snprintf(respuesta, sizeof(respuesta), "EXISTE");
			} else {
				snprintf(consulta, sizeof(consulta), "INSERT INTO jugadores (usuario, contraseña) VALUES ('%s', '%s')", usuario, password);
				if (mysql_query(conn, consulta)) {
					snprintf(respuesta, sizeof(respuesta), "Error en insercion");
				} else {
					snprintf(respuesta, sizeof(respuesta), "OK");
				}
			}
			mysql_free_result(res);
		}
	}
	write(sock_conn, respuesta, strlen(respuesta));
}

void LoginUsuario(int sock_conn, MYSQL *conn, char *usuario, char *password) 
{
	char consulta[256], respuesta[512];
	snprintf(consulta, sizeof(consulta), "SELECT COUNT(*) FROM jugadores WHERE usuario = '%s' AND contraseña = '%s'", usuario, password);
	if (mysql_query(conn, consulta)) 
	{
		snprintf(respuesta, sizeof(respuesta), "Error en consulta de login");
	} 
	else 
	{
		MYSQL_RES *res = mysql_store_result(conn);
		if (res == NULL) {
			snprintf(respuesta, sizeof(respuesta), "Error en resultado de login");
		} 
		else 
		{
			MYSQL_ROW row = mysql_fetch_row(res);
			if (row != NULL && atoi(row[0]) > 0) {
				AgregarUsuario(sock_conn, usuario);
				snprintf(respuesta, sizeof(respuesta), "Login exitoso");
			} else {
				snprintf(respuesta, sizeof(respuesta), "ERROR");
			}
			mysql_free_result(res);
		}
	}
	write(sock_conn, respuesta, strlen(respuesta));
	
	if (strcmp(respuesta, "Login exitoso")==0)
	{
		char notificacion[512];
		sprintf(notificacion, "NOT: El jugador %s se ha unido la partida, actualmente hay %d jugadores conectados\n",usuario,num_usuarios);
		int j;
		for (j=0;j<num_usuarios;j++)
		{
			write(usuarios[j].socket, notificacion, strlen(notificacion));
		}
	}
}

void ObtenerCartas(int sock_conn, MYSQL *conn) 
{
	char consulta[256];
	snprintf(consulta, sizeof(consulta), "SELECT ID_c, color, numero FROM cartas WHERE sobran > 0 ORDER BY RAND() LIMIT 7;");
	printf("Ejecutando consulta: %s\n", consulta);
	if (mysql_query(conn, consulta) == 0) {
		MYSQL_RES *resultado = mysql_store_result(conn);
		MYSQL_ROW fila;
		char respuesta[512] = "6/";
		while ((fila = mysql_fetch_row(resultado))) {
			char carta[50];
			snprintf(carta, sizeof(carta), "%s,%s,%s;", fila[0], fila[1], fila[2]);  
			strcat(respuesta, carta);
		}
		mysql_free_result(resultado);
		write(sock_conn, respuesta, strlen(respuesta));
	} else {
		char error_msg[] = "6/Error en la consulta SQL";
		write(sock_conn, error_msg, strlen(error_msg));
	}
}

void Reglas (int sock_conn, char *color1, int *numero1, char *color2, int *numero2){
	int respuesta;
	if (*numero1 == *numero2){
		printf("Movimiento válido\n");
		respuesta = 0;
	}
	else if (strcmp(color1,color2)==0){
		printf("Movimiento válido\n");
		respuesta = 0;
	}
	else{
		printf("Movimiento inválido\n");
		respuesta = 1;
	}
	char respuesta_str[10];
	if (respuesta == 0)
		sprintf(respuesta_str, "9/y\n");
	else if (respuesta == 1)
		sprintf(respuesta_str, "9/n\n");
	printf("Enviando al cliente: %s", respuesta_str);
	//write(sock_conn, respuesta_str, strlen(respuesta_str));
	ssize_t bytes_sent = write(sock_conn, respuesta_str, strlen(respuesta_str));
	
	if (bytes_sent < 0) {
		perror("Error al enviar datos");
	} else {
		printf("Se enviaron %zd bytes al cliente: %s", bytes_sent, respuesta_str);
	}
}

void DarCarta (int sock_conn, MYSQL *conn) 
{
	char consulta[256];
	snprintf(consulta, sizeof(consulta), "SELECT ID_c, color, numero FROM cartas WHERE sobran > 0 ORDER BY RAND() LIMIT 1;");
	printf("Ejecutando consulta: %s\n", consulta);
	if (mysql_query(conn, consulta) == 0) {
		MYSQL_RES *resultado = mysql_store_result(conn);
		MYSQL_ROW fila;
		if ((fila = mysql_fetch_row(resultado)) != NULL) {
			char respuesta[512] = "7/";
			char carta[50];
			snprintf(carta, sizeof(carta), "%s,%s,%s;", fila[0], fila[1], fila[2]);
			strcat(respuesta, carta);
			if (respuesta[strlen(respuesta) - 1] == ';') {
				respuesta[strlen(respuesta) - 1] = '\0';
			}
			write(sock_conn, respuesta, strlen(respuesta));
			mysql_free_result(resultado);
		} 
	} else {
		char error_msg[] = "7/Error en la consulta SQL";
		write(sock_conn, error_msg, strlen(error_msg));
	}
}

void DarCartaPartida (int sock_conn, MYSQL *conn) 
{
	char consulta[256];
	snprintf(consulta, sizeof(consulta), "SELECT ID_c, color, numero FROM cartas ORDER BY RAND() LIMIT 1;");
	printf("Ejecutando consulta: %s\n", consulta);
	if (mysql_query(conn, consulta) == 0) {
		MYSQL_RES *resultado = mysql_store_result(conn);
		MYSQL_ROW fila;
		if ((fila = mysql_fetch_row(resultado)) != NULL) {
			char respuesta[512] = "8/";
			char carta[50];
			snprintf(carta, sizeof(carta), "%s,%s,%s;", fila[0], fila[1], fila[2]);
			strcat(respuesta, carta);
			if (respuesta[strlen(respuesta) - 1] == ';') {
				respuesta[strlen(respuesta) - 1] = '\0';
			}
			write(sock_conn, respuesta, strlen(respuesta));
			mysql_free_result(resultado);
		} 
	} else {
		char error_msg[] = "8/Error en la consulta SQL";
		write(sock_conn, error_msg, strlen(error_msg));
	}
}

// Logica del hilo por cliente
void* AtenderCliente(void* arg) {
	int sock_conn = *((int*)arg);
	free(arg);
	char buffer[512],  respuesta[512];
	int ret;
	
	while ((ret = read(sock_conn, buffer, sizeof(buffer) - 1)) > 0) {
		buffer[ret] = '\0';
		printf("Peticion recibida: %s\n", buffer);
		char* codigo_str = strtok(buffer, "/");
		if (codigo_str == NULL) 
		{
			snprintf(respuesta, sizeof(respuesta), "Formato de peticion incorrecto");
			write(sock_conn, respuesta, strlen(respuesta));
			continue;
		}
		int codigo = atoi(codigo_str);
		MYSQL* conn = ConectarSQL();
		
		if (!conn) {
			char msg[] = "Error de conexion BD";
			write(sock_conn, msg, strlen(msg));
			continue;
		}
		
		if (codigo == 2) 
		{
			char *usuario = strtok(NULL, "/");
			char *password = strtok(NULL, "/");
			if (usuario && password) RegistrarUsuario(sock_conn, conn, usuario, password);
			else 
			{
				snprintf(respuesta, sizeof(respuesta), "Datos insuficientes para registro");
				write(sock_conn, respuesta, strlen(respuesta));
			}
		} 
		else if (codigo == 3) 
		{
			char *usuario = strtok(NULL, "/");
			char *password = strtok(NULL, "/");
			
			if (usuario && password) 
			{
				LoginUsuario(sock_conn, conn, usuario, password);
			}
			else 
			{
				snprintf(respuesta, sizeof(respuesta), "Datos insuficientes para login");
				write(sock_conn, respuesta, strlen(respuesta));				
			}
		}
		else if (codigo == 4) 
		{ // Obtener usuarios conectados
			strcpy(respuesta, "4/Usuarios conectados:\n");
			for (int i = 0; i < num_usuarios; i++) 
			{
				strcat(respuesta, usuarios[i].usuario);
				strcat(respuesta, "\n");
			}
			write(sock_conn, respuesta, strlen(respuesta));
		}
		else if (codigo == 5) 
		{ 
			// Cerrar sesion
			char usuario[50];
			codigo_str = strtok(NULL, "/");
			if (codigo_str == NULL) break;
			strncpy(usuario, codigo_str, sizeof(usuario) - 1);
			usuario[sizeof(usuario) - 1] = '\0';
			
			// Eliminar el usuario de la lista de conectados
			for (int i = 0; i < num_usuarios; i++) 
			{
				if (strcmp(usuarios[i].usuario, usuario) == 0)
				{
					EliminarUsuario(usuarios[i].socket);
					sprintf(respuesta, "5/Logout exitoso");
					if (strcmp(respuesta, "5/Logout exitoso")==0)
					{
						char notificacion[512];
						sprintf(notificacion, "NOT: El jugador %s ha abandonado la partida, actualmente hay %d jugadores conectados\n",usuario,num_usuarios);
						int j;
						for (j=0;j<num_usuarios;j++)
						{
							write(usuarios[j].socket, notificacion, strlen(notificacion));
						}
					}
				}
				
			}
			write(sock_conn, respuesta, strlen(respuesta));
			
		}
		else if (codigo == 6) 
		{
			ObtenerCartas(sock_conn, conn);
		}
		else if (codigo == 7) 
		{
			DarCarta(sock_conn, conn);
		}
		else if (codigo == 8) 
		{
			printf("He recibido la peticion");
			DarCartaPartida(sock_conn, conn);
		}
		else if (codigo == 9){
			char *color1 = strtok(NULL, "/");
			char *numero1_str = strtok(NULL, "/");
			char *color2 = strtok(NULL, "/");
			char *numero2_str = strtok(NULL, "/");
			
			if (color1 && numero1_str && color2 && numero2_str) {
				int numero1 = atoi(numero1_str);
				int numero2 = atoi(numero2_str);
				Reglas(sock_conn, color1, &numero1, color2, &numero2);
				printf("color1: %s, numero1: %d, color2: %s, numero2: %d\n", color1, numero1, color2, numero2);
			} else {
				printf("Error al parsear la peticion\n");
			}
		}
		
		if (codigo == 11) {
			char* invitador = strtok(NULL, "/");
			char* lista = strtok(NULL, "/");
			ProcesarInvitacion(sock_conn, invitador, lista);
		}		
		mysql_close(conn);
	}
	close(sock_conn);
	EliminarUsuario(sock_conn);
	return NULL;
}

int ConexionC(int puerto) {
	int sock_listen = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in serv_adr = {0};
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(puerto);
	
	setsockopt(sock_listen, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
	bind(sock_listen, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
	listen(sock_listen, 3);
	printf("Servidor escuchando en puerto %d...\n", puerto);
	
	while (1) {
		int* sock_conn = malloc(sizeof(int));
		*sock_conn = accept(sock_listen, NULL, NULL);
		pthread_t tid;
		pthread_create(&tid, NULL, AtenderCliente, sock_conn);
		pthread_detach(tid);
	}
	return 0;
}

int main() {
	pthread_mutex_init(&usuarios_mutex, NULL);
	ConexionC(9050);
	pthread_mutex_destroy(&usuarios_mutex);
	return 0;
}



