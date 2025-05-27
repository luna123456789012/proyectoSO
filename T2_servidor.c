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
#define MAX_PARTIDAS 10

#define MAX_INVITADOS 10
#define MAX_INVITACIONES 100

typedef struct {
    int socket;
    char usuario[50];
} UsuarioConectado;

typedef struct {
	int num;
	int id;
	int estado;
	int turno;
	UsuarioConectado listajugadores[4];
	char cartaInicial_ID[10];
	char cartaInicial_color[10];
	char cartaInicial_numero[10];
	int carta_inicial_asignada;
} Partida;

UsuarioConectado usuarios[MAX_USUARIOS];
int num_usuarios = 0;

Partida partidas[MAX_PARTIDAS];
int num_partidas=0;

pthread_mutex_t usuarios_mutex;
pthread_mutex_t partidas_mutex;

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


MYSQL* ConectarSQL() {
    MYSQL *conn = mysql_init(NULL);
    if (conn == NULL) {
        printf("Error al inicializar la conexion.\n");
        return NULL;
    }
    if (mysql_real_connect(conn, "localhost", "root", "mysql", "juego", 0, NULL, 0) == NULL) {
        printf("Error al conectar a MySQL: %s\n", mysql_error(conn));
        mysql_close(conn);
        return NULL;
    }
    return conn;
}

void AgregarUsuario(int socket, const char* usuario) 
{
    pthread_mutex_lock(&usuarios_mutex);
    if (num_usuarios < MAX_USUARIOS) 
	{
        strncpy(usuarios[num_usuarios].usuario, usuario, sizeof(usuarios[num_usuarios].usuario) - 1);
        usuarios[num_usuarios].usuario[sizeof(usuarios[num_usuarios].usuario) - 1] = '\0';
        usuarios[num_usuarios].socket = socket;
        num_usuarios++;
    } 
	else 
	{
        printf("Se alcanzo el numero maximo de usuarios.\n");
    }
    pthread_mutex_unlock(&usuarios_mutex);
}

// Funcion para eliminar un usuario de forma segura (con mutex).
void EliminarUsuario(int socket) {
    pthread_mutex_lock(&usuarios_mutex);
    for (int i = 0; i < num_usuarios; i++) {
        if (usuarios[i].socket == socket) {
            for (int j = i; j < num_usuarios - 1; j++) {
                usuarios[j] = usuarios[j + 1];
            }
            num_usuarios--;
            break;
        }
    }
    pthread_mutex_unlock(&usuarios_mutex);
}

// Procesar invitacion (codigo 11)
void ProcesarInvitacion(int sock_conn, char* invitador, char* lista, int partida) {
	if (!invitador || !lista || strlen(lista) == 0) {
		const char msg[] = "11/Error en datos de invitacion";
		write(sock_conn, msg, strlen(msg));
		return;
	}
	
	if (num_invitaciones >= MAX_USUARIOS) {
		const char msg[] = "11/Li mite de invitaciones alcanzado";
		write(sock_conn, msg, strlen(msg));
		return;
	}
	
	// Validar tama os
	if (strlen(invitador) >= sizeof(invitaciones[0].invitador)) {
		const char msg[] = "11/Nombre de invitador demasiado largo";
		write(sock_conn, msg, strlen(msg));
		return;
	}
	if (strlen(lista) >= 256) {
		const char msg[] = "11/Lista de invitados demasiado larga";
		write(sock_conn, msg, strlen(msg));
		return;
	}
	
	// Copia segura de lista
	char lista_original[256] = {0};
	strcpy(lista_original, lista);
	
	// Duplicacion segura de la lista
	char* lista_para_envio = malloc(strlen(lista_original) + 1);
	char* lista_para_guardar = malloc(strlen(lista_original) + 1);
	if (!lista_para_envio || !lista_para_guardar) {
		perror("malloc fallido");
		free(lista_para_envio);
		free(lista_para_guardar);
		return;
	}
	strcpy(lista_para_envio, lista_original);
	strcpy(lista_para_guardar, lista_original);
	
	printf("-> Entrando a ProcesarInvitacion\n");
	printf("-> invitador: %s, lista: %s\n", invitador, lista);
	printf("-> lista_original copiada: %s\n", lista_original);
	
	// Enviar invitaciones
	char* token_envio = strtok(lista_para_envio, ",");
	while (token_envio != NULL) {
		printf("-> Enviando a: '%s'\n", token_envio);
		pthread_mutex_lock(&usuarios_mutex);
		for (int i = 0; i < num_usuarios; i++) {
			if (strcmp(usuarios[i].usuario, token_envio) == 0) {
				char mensaje[512];
				snprintf(mensaje, sizeof(mensaje), "INVITE/%s/%d", invitador, partida);
				write(usuarios[i].socket, mensaje, strlen(mensaje));
				break;
			}
		}
		pthread_mutex_unlock(&usuarios_mutex);
		token_envio = strtok(NULL, ",");
	}
	
	// Guardar invitacion
	Invitacion* inv = &invitaciones[num_invitaciones];
	memset(inv, 0, sizeof(Invitacion));
	strncpy(inv->invitador, invitador, sizeof(inv->invitador) - 1);
	
	char* token_guardar = strtok(lista_para_guardar, ",");
	while (token_guardar != NULL && inv->total_invitados < 10) {
		strncpy(inv->invitados[inv->total_invitados], token_guardar, sizeof(inv->invitados[0]) - 1);
		inv->aceptados[inv->total_invitados] = 0;
		inv->total_invitados++;
		token_guardar = strtok(NULL, ",");
	}
	
	inv->respuestas_recibidas = 0;
	num_invitaciones++;
	
	free(lista_para_envio);
	free(lista_para_guardar);
}



void NotificarAPartida(int socket_remitente, const char* mensaje) 
{
	pthread_mutex_lock(&partidas_mutex);
	
	for (int i = 0; i < num_partidas; i++) 
	{
		for (int j = 0; j < partidas[i].num; j++) 
		{
			if (partidas[i].listajugadores[j].socket == socket_remitente) 
			{
				// Este jugador esta en esta partida, notificar a todos los demas jugadores de la misma
				for (int k = 0; k < partidas[i].num; k++) 
				{
					if (partidas[i].listajugadores[k].socket != socket_remitente) 
					{
						write(partidas[i].listajugadores[k].socket, mensaje, strlen(mensaje));
					}
				}
				pthread_mutex_unlock(&partidas_mutex);
				return;
			}
		}
	}	
	pthread_mutex_unlock(&partidas_mutex);
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
}

void ObtenerCartas(int sock_conn, MYSQL *conn) 
{
    char consulta[256];
    snprintf(consulta, sizeof(consulta), "SELECT ID_c, color, numero FROM cartas ORDER BY RAND() LIMIT 7;");
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
		printf("Movimiento v lido\n");
		respuesta = 0;
	}
	else if (strcmp(color1,color2)==0){
		printf("Movimiento v lido\n");
		respuesta = 0;
	}
	else{
		printf("Movimiento inv lido\n");
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
	snprintf(consulta, sizeof(consulta), "SELECT color, numero FROM cartas ORDER BY RAND() LIMIT 1;");
	printf("Ejecutando consulta: %s\n", consulta);
	if (mysql_query(conn, consulta) == 0) {
		MYSQL_RES *resultado = mysql_store_result(conn);
		MYSQL_ROW fila;
		if ((fila = mysql_fetch_row(resultado)) != NULL) {
			char respuesta[512] = "7/";
			char carta[50];
			snprintf(carta, sizeof(carta), "%s,%s;", fila[0], fila[1]);
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

void GenerarCartaInicialPartida(int idPartida, MYSQL *conn, Partida partidas[]) {
	if (partidas[idPartida].carta_inicial_asignada == 0) {
		char consulta[256];
		snprintf(consulta, sizeof(consulta), "SELECT ID_c, color, numero FROM cartas ORDER BY RAND() LIMIT 1;");
		
		if (mysql_query(conn, consulta) == 0) {
			MYSQL_RES *resultado = mysql_store_result(conn);
			MYSQL_ROW fila;
			if ((fila = mysql_fetch_row(resultado)) != NULL) {
				strncpy(partidas[idPartida].cartaInicial_ID, fila[0], sizeof(partidas[idPartida].cartaInicial_ID));
				strncpy(partidas[idPartida].cartaInicial_color, fila[1], sizeof(partidas[idPartida].cartaInicial_color));
				strncpy(partidas[idPartida].cartaInicial_numero, fila[2], sizeof(partidas[idPartida].cartaInicial_numero));
				partidas[idPartida].carta_inicial_asignada = 1;
			}
			mysql_free_result(resultado);
		}
	}
}

void EnviarCartaInicial(int sock_conn, int idPartida, Partida partidas[]) {
	if (partidas[idPartida].carta_inicial_asignada == 1) {
		char respuesta[512] = "8/";
		char carta[50];
		snprintf(carta, sizeof(carta), "%s,%s,%s;", 
				 partidas[idPartida].cartaInicial_ID, 
				 partidas[idPartida].cartaInicial_color, 
				 partidas[idPartida].cartaInicial_numero);
		strcat(respuesta, carta);
		printf(respuesta);
		if (respuesta[strlen(respuesta) - 1] == ';') {
			respuesta[strlen(respuesta) - 1] = '\0';
		}
		
		write(sock_conn, respuesta, strlen(respuesta));
	}
}


int AsignarPartida(char* usuario, int socket,char* notificacion) {
	pthread_mutex_lock(&partidas_mutex);

	if (num_partidas==0)
	{
		int nuevaID=num_partidas;
		partidas[nuevaID].id=nuevaID;
		partidas[nuevaID].estado=0;				
		strcpy(partidas[nuevaID].listajugadores[partidas[0].num].usuario,usuario);
		partidas[nuevaID].listajugadores[partidas[0].num].socket=socket;
		partidas[nuevaID].num=1;
		
		num_partidas++;
		printf("Sala inicial creada");
		sprintf(notificacion, "NOT: El jugador %s ha iniciado y se ha unido la partida en la sala %d.\n", usuario, nuevaID);
		pthread_mutex_unlock(&partidas_mutex);
		return nuevaID;
	}
	else
	{
		int encontrado=0;
		int index=0;
		
		while (index<num_partidas && !encontrado)
		{
			if (partidas[index].estado == 0 && partidas[index].num < 4) {
				// Verificar si el usuario ya esta en esta partida
				int yaEsta = 0;
				for (int j = 0; j < partidas[index].num; j++) {
					if (strcmp(partidas[index].listajugadores[j].usuario, usuario) == 0) {
						yaEsta = 1;
						break;
					}
				}
				if (!yaEsta)
				{
					int num_jugadores=partidas[index].num;
					partidas[index].listajugadores[num_jugadores].socket = socket;
					strncpy(partidas[index].listajugadores[num_jugadores].usuario, usuario, sizeof(partidas[index].listajugadores[num_jugadores].usuario) - 1);
					partidas[index].num++;
					
					if (partidas[index].num==4)
					{
						partidas[index].estado=1;
					}
					pthread_mutex_unlock(&partidas_mutex);
					
					char notificacion[512];
					sprintf(notificacion, "NOT: El jugador %s se ha unido a la partida.\n", usuario);
					NotificarAPartida(socket, notificacion);
					return partidas[index].id;
				}
			}
			index++;
		}
		if (!encontrado)
		{
			if (num_partidas<MAX_PARTIDAS)
			{
				int nuevaID=num_partidas+1;
				partidas[nuevaID].id=nuevaID;
				partidas[nuevaID].estado=0;				
				strcpy(partidas[nuevaID].listajugadores[0].usuario,usuario);
				partidas[nuevaID].listajugadores[0].socket=socket;
				partidas[nuevaID].num=1;
				
				num_partidas++;
				pthread_mutex_unlock(&partidas_mutex);
				
				char notificacion[512];
				sprintf(notificacion, "NOT: El jugador %s ha iniciado la partida.\n", usuario);
				NotificarAPartida(socket, notificacion);
				
				return nuevaID;
			}
			else 
			{
				pthread_mutex_unlock(&partidas_mutex);
				return -1;
			}
		}
	}
}

void EliminarJugadorDePartida(int socket_jugador) 
{
	pthread_mutex_lock(&partidas_mutex);
	
	for (int i = 0; i < num_partidas; i++) 
	{
		for (int j = 0; j < partidas[i].num; j++) 
		{
			if (partidas[i].listajugadores[j].socket == socket_jugador) 
			{
				// Eliminar al jugador desplazando los siguientes
				for (int k = j; k < partidas[i].num - 1; k++) 
				{
					partidas[i].listajugadores[k] = partidas[i].listajugadores[k + 1];
				}
				partidas[i].num--;
				
				// Si no quedan jugadores, se podri a resetear la partida (opcional)
				if (partidas[i].num == 0) 
				{
					partidas[i].estado = 0;
				}
				
				pthread_mutex_unlock(&partidas_mutex);
				return;
			}
		}
	}
	pthread_mutex_unlock(&partidas_mutex);
}

void IniciarPartida(Partida* partidas,char* usuario, int num_partida)
{
	pthread_mutex_lock(&partidas_mutex);
	
}

void *AtenderCliente(void *arg) 
{
    int sock_conn = *((int *)arg);
    free(arg);
    char peticion[512], respuesta[512]; char notificacion[512];
    int ret;
    while (1) {
        memset(peticion, 0, sizeof(peticion));
        ret = read(sock_conn, peticion, sizeof(peticion) - 1);
        if (ret <= 0) {
            if (ret < 0) perror("Error en read");
            else printf("Cliente desconectado\n");
            EliminarUsuario(sock_conn);
            break;
        }
        peticion[ret] = '\0';
		printf("Peticion recibida: %s\n", peticion);
        char *p = strtok(peticion, "/");
        if (p == NULL) 
		{
            snprintf(respuesta, sizeof(respuesta), "Formato de peticion incorrecto");
            write(sock_conn, respuesta, strlen(respuesta));
            continue;
        }
        int codigo = atoi(p);
        MYSQL *conn = ConectarSQL();
        if (conn == NULL) 
		{
            snprintf(respuesta, sizeof(respuesta), "Error de conexion a la base de datos");
            write(sock_conn, respuesta, strlen(respuesta));
            continue;
        }
		else if (codigo == 1)
		{
			char *usuario = strtok(NULL, "/");
			char notificacion[512];
			
			if (usuario)
			{
				int ID_partida = AsignarPartida(usuario,sock_conn,notificacion);
				
				if (ID_partida==-1)
				{
					snprintf(respuesta, sizeof(respuesta), "1/No hay partidas disponibles");
					printf("1/No hay partidas disponibles\n");
					write(sock_conn, respuesta, strlen(respuesta));
				}
				else{
					snprintf(respuesta, sizeof(respuesta), "1/%d/Has sido asignado a la sala %d\n",ID_partida,ID_partida);
					printf("1/%s ha sido asignado a la sala %d\n",usuario,ID_partida);
					printf("%s",respuesta);
					write(sock_conn, respuesta, strlen(respuesta));
				}
			}
			else {
				sprintf(respuesta, "1/Formato incorrecto");
				printf("1/Formato incorrecto\n");
				write(sock_conn, respuesta, strlen(respuesta));
			}
						
			NotificarAPartida(sock_conn, notificacion);
		}
        else if (codigo == 2) 
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
			char *sala = strtok(NULL, "/");
			int ID_sala= atoi(sala);
			printf("%d\n",ID_sala);
			
			strcpy(respuesta, "4/Usuarios conectados:\n");
			for (int i = 0; i < num_usuarios; i++) 
			{
				strcat(respuesta, partidas[ID_sala].listajugadores[i].usuario);
				strcat(respuesta, "\n");
			}
			printf("%s",respuesta);
			write(sock_conn, respuesta, strlen(respuesta));			
		}
		else if (codigo == 5) 
		{ 
			// Cerrar sesion
			char usuario[50];
			p = strtok(NULL, "/");
			if (p == NULL) break;
			strncpy(usuario, p, sizeof(usuario) - 1);
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
						sprintf(notificacion, "NOT: El jugador %s ha abandonado la partida.\n", usuario);
						NotificarAPartida(sock_conn, notificacion);
						
						// Se elimina al jugador de su partida
						EliminarJugadorDePartida(sock_conn);
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
			char *sala = strtok(NULL, "/");
			int ID_sala= atoi(sala);
			printf(sala);
			GenerarCartaInicialPartida(ID_sala, conn, partidas);
			EnviarCartaInicial(sock_conn, ID_sala, partidas);
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
		
		else if (codigo == 10) {
			char *remitente = strtok(NULL, "/");
			char *mensaje = strtok(NULL, "/");
			printf("%s\n",remitente);
			printf("%s\n",mensaje);
			
			if (remitente && mensaje) {
				char mensaje_chat[512];
				snprintf(mensaje_chat, sizeof(mensaje_chat), "CHAT/%s: %s", remitente, mensaje);
				printf("CHAT/%s: %s\n", remitente, mensaje);
				
				pthread_mutex_lock(&partidas_mutex);
				for (int i = 0; i < num_partidas; i++) 
				{
					for (int j = 0; j < partidas[i].num; j++) 
					{
						if (partidas[i].listajugadores[j].socket == sock_conn) 
						{
							// Este jugador esta en esta partida, notificar a todos los jugadores de la misma, incluido l mismo
							for (int k = 0; k < partidas[i].num; k++) 
							{
								write(partidas[i].listajugadores[k].socket, mensaje_chat, strlen(mensaje_chat));
							}
							
						}
					}
				}
				pthread_mutex_unlock(&partidas_mutex);				
				
				/*				snprintf(respuesta, sizeof(respuesta), "Mensaje enviado");*/
				/*				write(sock_conn, respuesta, strlen(respuesta));*/
			} else {
				snprintf(respuesta, sizeof(respuesta), "Error en el formato del mensaje de chat");
				write(sock_conn, respuesta, strlen(respuesta));
			}
		}	
		else if (codigo == 12) {
			char *invitado = strtok(NULL, "/");
			char *invitador = strtok(NULL, "/");
			char *decision = strtok(NULL, "/");
			char* partida=strtok(NULL,"/");
			int ID_partida=atoi(partida);

			printf("%s",decision);
			if (invitado && decision) 
			{
				if (strcmp(decision,"ACEPTAR")==0)
				{
					for (int i = 0; i < num_invitaciones; i++) {
						for (int j = 0; j < invitaciones[i].total_invitados; j++) {
							if (strcmp(invitaciones[i].invitados[j], invitado) == 0) {
								// Notificar al remitente
								for (int k = 0; k < num_usuarios; k++) {
									if (strcmp(usuarios[k].usuario, invitaciones[i].invitador) == 0) {
										char mensaje[100];
										sprintf(mensaje, "12/%s/ACEPTADA", invitado);
										write(usuarios[k].socket, mensaje, strlen(mensaje));
										break;
									}
								}
							
								// Eliminar este invitado de la lista
								for (int m = j; m < invitaciones[i].total_invitados - 1; m++) {
									strcpy(invitaciones[i].invitados[m], invitaciones[i].invitados[m+1]);
								}
								invitaciones[i].total_invitados--;
								// Si no quedan invitados, eliminar la invitacion entera
								if (invitaciones[i].total_invitados == 0) {
									for (int n = i; n < num_invitaciones - 1; n++) {
										invitaciones[n] = invitaciones[n + 1];
									}
									num_invitaciones--;
								}
							
								break;
							}
						}
					}
					
					if (ID_partida!=NULL)
					{						
						EliminarJugadorDePartida(sock_conn);

						pthread_mutex_lock(&partidas_mutex);
						for (int j=0;j<num_partidas;j++)
						{
							if (partidas[j].id==ID_partida)
							{
								strcpy(partidas[j].listajugadores[partidas[j].num].usuario,invitado);
								break;
							}
						}
						pthread_mutex_unlock(&partidas_mutex);

						char confirmacion[100];
						sprintf(confirmacion, "13/");
						write(sock_conn, confirmacion, strlen(confirmacion));
					}
				}
				else
				{
					for (int i = 0; i < num_invitaciones; i++) 
					{
						for (int j = 0; j < invitaciones[i].total_invitados; j++) 
						{
							if (strcmp(invitaciones[i].invitados[j], invitado) == 0) 
							{
								// Notificar al remitente
								for (int k = 0; k < num_usuarios; k++) {
									if (strcmp(usuarios[k].usuario, invitaciones[i].invitador) == 0) 
									{
										char mensaje[100];
										sprintf(mensaje, "12/%s/ACEPTADA", invitado);
										write(usuarios[k].socket, mensaje, strlen(mensaje));
										break;
									}
								}
							}
						}
					}
				}
			} else {
				const char msg[] = "12/Formato invalido de respuesta a invitacion";
				write(sock_conn, msg, strlen(msg));
			}
		}
		else if (codigo == 13){
			char *ganador = strtok(NULL, "/");
			char *sala = strtok(NULL, "/");
			int ID_sala= atoi(sala);
			char p = strtok(NULL, "/");
			printf(ganador);
			snprintf(respuesta, sizeof(respuesta), "17/%s", ganador);
			for (int i = 0; i < partidas[ID_sala].num; i++) {
				int sock_jugador = partidas[ID_sala].listajugadores[i].socket;
				printf("Enviando a: %s\n",partidas[ID_sala].listajugadores[i].usuario);
				if (sock_jugador > 0) {
					ssize_t bytes_sent = write(sock_jugador, respuesta, strlen(respuesta));
					if (bytes_sent < 0) {
						perror("Error al enviar datos");
					} else {
						printf("Se enviaron %zd bytes al cliente: %s", bytes_sent, respuesta);
					}
				}
			}
		}
		else if (codigo == 14){
			char* color = strtok(NULL, "/");
			char* numero = strtok(NULL, "/");
			char* sala_str = strtok(NULL, "/");
			int num = atoi(numero);
			int sala = atoi(sala_str);
			printf("Recibida carta a enviar: color=%s, numero=%d, sala = %d\n", color, num, sala);
			for (int i = 0; i < partidas[sala].num; i++) {
				int sock_jugador = partidas[sala].listajugadores[i].socket;
				printf("Enviando carta a: %s",partidas[sala].listajugadores[i].usuario);
				if (sock_jugador > 0) {
					char respuesta[512];
					snprintf(respuesta, sizeof(respuesta), "14/%s,%d", color, num);
					ssize_t bytes_sent = write(sock_jugador, respuesta, strlen(respuesta));
					if (bytes_sent < 0) {
						perror("Error al enviar datos");
					} else {
						printf("Se enviaron %zd bytes al cliente: %s", bytes_sent, respuesta);
					//write(sock_jugador, respuesta, strlen(respuesta) + 1);
					}
				}
				else {
					printf("Socket de jugador no valido: %d\n", sock_jugador);
				}
			}
		}
		else if (codigo == 15)
		{
			EliminarJugadorDePartida(sock_conn);
			snprintf(respuesta, sizeof(respuesta), "Has abandonado con exito la partida");
			write(sock_conn, respuesta, strlen(respuesta));
		}
		else if (codigo == 16) 
		{ // Obtener jugadores en linia
			strcpy(respuesta, "16/Jugadores en linea:\n");
			for (int i = 0; i < num_usuarios; i++) 
			{
				strcat(respuesta, usuarios[i].usuario);
				strcat(respuesta, "\n");
			}
			write(sock_conn, respuesta, strlen(respuesta));
		}
		else if (codigo == 18){
			char *usuarioUNO = strtok(NULL, "/");
			char *sala = strtok(NULL, "/");
			int ID_sala= atoi(sala);
			char p = strtok(NULL, "/");
			printf(usuarioUNO);
			snprintf(respuesta, sizeof(respuesta), "17/%s", usuarioUNO);
			for (int i = 0; i < partidas[ID_sala].num; i++) {
				int sock_jugador = partidas[ID_sala].listajugadores[i].socket;
				printf("Enviando a: %s",partidas[ID_sala].listajugadores[i].usuario);
				if (sock_jugador > 0) {
					ssize_t bytes_sent = write(sock_jugador, respuesta, strlen(respuesta));
					if (bytes_sent < 0) {
						perror("Error al enviar datos");
					} else {
						printf("Se enviaron %zd bytes al cliente: %s", bytes_sent, respuesta);
					}
				}
			}
		}
		else 
		{
			snprintf(respuesta, sizeof(respuesta), "Codigo desconocido");
			write(sock_conn, respuesta, strlen(respuesta));
		}

        mysql_close(conn);
    }
    close(sock_conn);
    return NULL;
}

int ConexionC(int puerto) 
{
	int sock_listen, sock_conn;
	struct sockaddr_in serv_adr;
	
	
	sock_listen = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_listen < 0) {
		perror("Error al crear el socket");
		return -1;
	}
	
	int optval = 1;
	if (setsockopt(sock_listen, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
		perror("Error al configurar SO_REUSEADDR");
		close(sock_listen);
		return -1;
	}
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(puerto);
	
	if (bind(sock_listen, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) < 0) {
		perror("Error en bind");
		close(sock_listen);
		return -1;
	}
	if (listen(sock_listen, 3) < 0) {
		perror("Error en listen");
		close(sock_listen);
		return -1;
	}
	printf("Servidor escuchando en el puerto %d...\n", puerto);
	
	while (1) {
		sock_conn = accept(sock_listen, NULL, NULL);
		if (sock_conn < 0) {
			perror("Error en accept");
			continue;
		}
		printf("Cliente conectado\n");
		
		pthread_t tid;
		int *pclient = malloc(sizeof(int));
		if (pclient == NULL) {
			perror("Error al asignar memoria para el socket del cliente");
			close(sock_conn);
			continue;
		}
		*pclient = sock_conn;
		if (pthread_create(&tid, NULL, AtenderCliente, pclient) != 0) {
			perror("Error al crear el hilo");
			free(pclient);
			close(sock_conn);
			continue;
		}
		pthread_detach(tid);
	}
	close(sock_listen);
	return 0;
}

int main() 
{
	
    if (pthread_mutex_init(&usuarios_mutex, NULL) != 0) {
        perror("Error al inicializar el mutex");
        return -1;
    }
	if (pthread_mutex_init(&partidas_mutex, NULL) != 0) {
		perror("Error al inicializar el mutex de partidas");
		return -1;
	}
	
    int puerto = 9100;
    int result = ConexionC(puerto);
    pthread_mutex_destroy(&usuarios_mutex);
	pthread_mutex_destroy(&partidas_mutex);
	
    return result;
}
