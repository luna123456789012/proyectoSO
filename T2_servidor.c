<<<<<<< HEAD
=======
// Incluye librer￭as necesarias
>>>>>>> 61231ba33c673faff748e09115aa9c0c3180bbf9
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
<<<<<<< HEAD
#define MAX_PARTIDAS 10

#define MAX_INVITADOS 10
#define MAX_INVITACIONES 100

#define OP_INVITE_RESPONSE 12   /* 12/ACCEPT|DECLINE/<idSala>/<invitador>/<invitado> */

typedef struct {
    int socket;
	int estado;
    char usuario[50];
} UsuarioConectado;

typedef struct {
	int num;
	int id;
	int estado;
	int turno;
	int turnos_asignados;
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
=======

// Estructura para usuarios conectados
typedef struct {
	int socket;
	char usuario[50];
} UsuarioConectado;

UsuarioConectado usuarios[MAX_USUARIOS];
int num_usuarios = 0;
pthread_mutex_t usuarios_mutex;
>>>>>>> 61231ba33c673faff748e09115aa9c0c3180bbf9

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

<<<<<<< HEAD

MYSQL* ConectarSQL() {
    MYSQL *conn = mysql_init(NULL);
    if (conn == NULL) {
        printf("Error al inicializar la conexion.\n");
        return NULL;
    }
    if (mysql_real_connect(conn, "localhost", "root", "mysql", "T2_juego", 0, NULL, 0) == NULL) {
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


/**
* Inserta un jugador aceptado en la partida especificada
* Devuelve 0 si todo OK, \u22121 si la partida est￡ llena o no existe
*/
static int unirJugadorAPartida(Partida partidas[], int idPartida, const char *usuario, int sock)
{
	if (idPartida < 0 || idPartida >= num_partidas) return -1;
	
	pthread_mutex_lock(&partidas_mutex);
	Partida *p = &partidas[idPartida];
	
	if (p->num >= 4) {                      /* llena */
		pthread_mutex_unlock(&partidas_mutex);
		return -1;
	}
	/* evita duplicados */
	for (int i = 0; i < p->num; ++i){
		if (!strcmp(p->listajugadores[i].usuario, usuario)) {
			pthread_mutex_unlock(&partidas_mutex);
			return 0; /* ya estaba */
		}
	}
	int idx = p->num++;
	p->listajugadores[idx].socket  = sock;
	strncpy(p->listajugadores[idx].usuario, usuario, sizeof(p->listajugadores[idx].usuario) - 1);
	p->listajugadores[idx].estado  = 0;
	pthread_mutex_unlock(&partidas_mutex);
	return 0;
}

// Funcion para eliminar un usuario de forma segura (con mutex).
void EliminarUsuario(int socket) {
	char respuesta[512];
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
	strcpy(respuesta, "5/Logout exitoso\n");
	write(socket, respuesta, strlen(respuesta));
	printf("Jugador desconectado");
    pthread_mutex_unlock(&usuarios_mutex);
}

// Procesar invitacion (codigo 11)
void ProcesarInvitacion(int sock_conn, char* invitador, char* lista, int ID_partida) {
	if (!invitador || !lista || strlen(lista) == 0) {
		const char msg[] = "11/Error en datos de invitacion";
=======
// Conexi￳n a MySQL
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

// Procesar invitaci￳n (c￳digo 11)
void ProcesarInvitacion(int sock_conn, char* invitador, char* lista) {
	if (!invitador || !lista || strlen(lista) == 0) {
		char msg[] = "11/Error en datos de invitaci￳n";
>>>>>>> 61231ba33c673faff748e09115aa9c0c3180bbf9
		write(sock_conn, msg, strlen(msg));
		return;
	}
	
<<<<<<< HEAD
	if (num_invitaciones >= MAX_USUARIOS) {
		const char msg[] = "11/Limite de invitaciones alcanzado";
		write(sock_conn, msg, strlen(msg));
		return;
	}
	
	// Validar tama\ufff1os
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
=======
	char lista_original[256];
	strncpy(lista_original, lista, sizeof(lista_original) - 1);
	lista_original[sizeof(lista_original) - 1] = '\0';
	
	char* lista_para_envio = strdup(lista_original);
	char* lista_para_guardar = strdup(lista_original);
	if (!lista_para_envio || !lista_para_guardar) {
		perror("Error al duplicar memoria para lista");
>>>>>>> 61231ba33c673faff748e09115aa9c0c3180bbf9
		free(lista_para_envio);
		free(lista_para_guardar);
		return;
	}
<<<<<<< HEAD
	strcpy(lista_para_envio, lista_original);
	strcpy(lista_para_guardar, lista_original);
	
	printf("-> Entrando a ProcesarInvitacion\n");
	printf("-> invitador: %s, lista: %s\n", invitador, lista);
	printf("-> lista_original copiada: %s\n", lista_original);
=======
>>>>>>> 61231ba33c673faff748e09115aa9c0c3180bbf9
	
	// Enviar invitaciones
	char* token_envio = strtok(lista_para_envio, ",");
	while (token_envio != NULL) {
<<<<<<< HEAD
		printf("-> Enviando a: '%s'\n", token_envio);
=======
>>>>>>> 61231ba33c673faff748e09115aa9c0c3180bbf9
		pthread_mutex_lock(&usuarios_mutex);
		for (int i = 0; i < num_usuarios; i++) {
			if (strcmp(usuarios[i].usuario, token_envio) == 0) {
				char mensaje[512];
<<<<<<< HEAD
				snprintf(mensaje, sizeof(mensaje), "INVITE/%s/%d", invitador, ID_partida);
=======
				snprintf(mensaje, sizeof(mensaje), "INVITE/%s/%s", invitador, lista_original);
>>>>>>> 61231ba33c673faff748e09115aa9c0c3180bbf9
				write(usuarios[i].socket, mensaje, strlen(mensaje));
				break;
			}
		}
		pthread_mutex_unlock(&usuarios_mutex);
		token_envio = strtok(NULL, ",");
	}
	
<<<<<<< HEAD
	// Guardar invitacion
	Invitacion* inv = &invitaciones[num_invitaciones];
	memset(inv, 0, sizeof(Invitacion));
	strncpy(inv->invitador, invitador, sizeof(inv->invitador) - 1);
	
	char* token_guardar = strtok(lista_para_guardar, ",");
	while (token_guardar != NULL && inv->total_invitados < 10) {
		strncpy(inv->invitados[inv->total_invitados], token_guardar, sizeof(inv->invitados[0]) - 1);
=======
	// Guardar invitaci￳n
	Invitacion* inv = &invitaciones[num_invitaciones];
	strncpy(inv->invitador, invitador, sizeof(inv->invitador) - 1);
	inv->respuestas_recibidas = 0;
	inv->total_invitados = 0;
	
	char* token_guardar = strtok(lista_para_guardar, ",");
	while (token_guardar != NULL && inv->total_invitados < 10) {
		strncpy(inv->invitados[inv->total_invitados], token_guardar, 49);
>>>>>>> 61231ba33c673faff748e09115aa9c0c3180bbf9
		inv->aceptados[inv->total_invitados] = 0;
		inv->total_invitados++;
		token_guardar = strtok(NULL, ",");
	}
<<<<<<< HEAD
	
	inv->respuestas_recibidas = 0;
=======
>>>>>>> 61231ba33c673faff748e09115aa9c0c3180bbf9
	num_invitaciones++;
	
	free(lista_para_envio);
	free(lista_para_guardar);
}

<<<<<<< HEAD
//Consulta 1
void listarJugadoresRelacionados(int sock_conn, MYSQL *conn, char *nombre_usuario) 
{
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[2048], respuesta[512]; 
    
    respuesta[0] = '\0';  // Vaciar respuesta
    snprintf(respuesta, sizeof(respuesta), "20/");

    // Consulta jugadores con los que ha jugado
    snprintf(query, sizeof(query),"SELECT DISTINCT jugadores.usuario FROM jugadores, "
			 "jugadores_partida WHERE jugadores.ID_j = jugadores_partida.ID_j AND jugadores_"
			 "partida.ID_p IN (SELECT ID_p FROM jugadores_partida WHERE ID_j = (SELECT ID_j FROM "
			 "jugadores WHERE usuario = '%s')) AND jugadores.usuario != '%s'; ", nombre_usuario, nombre_usuario);
	if (mysql_query(conn, query)) {
        snprintf(respuesta, sizeof(respuesta), "Error en la consulta1");
    }
    else 
	{
		res = mysql_store_result(conn);
		printf("%s",res);
		if (res == NULL) {
    		   snprintf(respuesta, sizeof(respuesta), "Error en la consulta2");
   		}
		else {
   		    int primero=0;
			row = mysql_fetch_row(res);
			if (row == NULL) {
				snprintf(respuesta, sizeof(respuesta), "No se han obtenido datos");
			} else {
				strcpy(respuesta, "20/");
				int primero = 1;
				while (row != NULL) {
					if (!primero) {
						strcat(respuesta, ",");
					}
					strcat(respuesta, row[0]);
					primero = 0;
					row = mysql_fetch_row(res);
				}
			}
		}
		mysql_free_result(res);
    }
    write(sock_conn, respuesta, strlen(respuesta));
}

//Consulta 2: datos de partidas jugadas con otro jugador
void listarPartidasRelacionadas(int sock_conn, MYSQL *conn, const char *nombre_usuario, char *nombre_usuario2) 
{
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[2048], respuesta[512]; 
    
    respuesta[0] = '\0';  // Vaciar respuesta
    snprintf(respuesta, sizeof(respuesta), "21");

    // Consulta jugadores con los que ha jugado
    snprintf(query, sizeof(query),
        "SELECT DISTINCT ID_partida, jugadores.usuario, hora_fin FROM jugadores, jugadores_partida, "
		"partida WHERE ID_partida IN (SELECT ID_p FROM jugadores_partida WHERE ID_j IN (SELECT ID_j FROM jugadores"
		" WHERE jugadores.usuario = '%s')) AND "
		"ID_partida IN (SELECT ID_p FROM jugadores_partida WHERE ID_j IN (SELECT ID_j FROM jugadores"
		" WHERE jugadores.usuario = '%s')) AND (jugadores.ID_j = partida.ID_ganador);",
        nombre_usuario2, nombre_usuario
    );
    if (mysql_query(conn, query)) {
        snprintf(respuesta, sizeof(respuesta), "Error en la consulta");
    }
    else 
	{
		res = mysql_store_result(conn);
		if (res == NULL) {
    		   snprintf(respuesta, sizeof(respuesta), "Error en la consulta");
   		}
		else {

		    // Construir la cadena de respuesta
   		    int primero=0;			
			unsigned int num_filas = mysql_num_rows(res);
			for (int i = 0; i < num_filas; i++) {
				row = mysql_fetch_row(res);
				if (row && row[0] && row[1] && row[2]) {
					printf("Fila %d: %s, %s, %s\n", i, row[0], row[1], row[2]);
					strcat(respuesta, "/");
					strcat(respuesta, row[0]); // ID_partida
					strcat(respuesta, ",");
					strcat(respuesta, row[1]); // usuario
					strcat(respuesta, ",");
					strcat(respuesta, row[2]); // hora_fin
				}
			}
		}
		mysql_free_result(res);
    }
    write(sock_conn, respuesta, strlen(respuesta));
}

//Consulta 3
void listarPartidasHoras(int sock_conn, MYSQL *conn, const char *nombre_usuario, int dia, int mes, int ano) 
{
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[2048], respuesta[512]; 
    
    respuesta[0] = '\0';  // Vaciar respuesta
    snprintf(respuesta, sizeof(respuesta), "97/");

    // Consulta jugadores con los que ha jugado
	snprintf(query, sizeof(query),
		"SELECT DISTINCT partida.ID_partida FROM partida, jugadores_partida "
		"WHERE partida.hora_fin >= STR_TO_DATE('%04d-%02d-%02d 00:00:00', '%%Y-%%m-%%d %%H:%%i:%%s') AND partida.hora_fin<= NOW() "
		"AND partida.ID_partida IN (SELECT ID_p FROM jugadores_partida WHERE ID_j = "
		"(SELECT ID_j FROM jugadores WHERE usuario = '%s'))",
		ano, mes, dia, nombre_usuario);
	printf("%s",query);
    if (mysql_query(conn, query)) {
        snprintf(respuesta, sizeof(respuesta), "Error en la consulta");
    }
    else 
	{
		res = mysql_store_result(conn);
		if (res == NULL) {
    		   snprintf(respuesta, sizeof(respuesta), "Error en la consulta");
   		}
		else {

		    // Construir la cadena de respuesta
   		    int primero=0;
			row = mysql_fetch_row(res);
			if (row==NULL)
				printf("No se han obtenido datos de la consulta");
			else
			{
				strcpy(respuesta, "22/");
				MYSQL_ROW row;
				while ((row = mysql_fetch_row(res)) != NULL) {
					strcat(respuesta, row[0]); // ID_partida
					strcat(respuesta, ",");
				}
			}
		}
		mysql_free_result(res);
    }
    write(sock_conn, respuesta, strlen(respuesta));
}


void NotificarAPartida(int ID_partida,int socket_remitente, const char* mensaje) 
{
	pthread_mutex_lock(&partidas_mutex);	
	
	// Este jugador esta en esta partida, notificar a todos los demas jugadores de la misma
	for (int k = 0; k < partidas[ID_partida].num; k++) 
	{
		if (partidas[ID_partida].listajugadores[k].socket != socket_remitente) 
		{
			write(partidas[ID_partida].listajugadores[k].socket, mensaje, strlen(mensaje));
		}
	}
	pthread_mutex_unlock(&partidas_mutex);
	return;
		
	
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
                snprintf(consulta, sizeof(consulta), "INSERT INTO jugadores (usuario, password) VALUES ('%s', '%s')", usuario, password);
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
=======

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
				snprintf(consulta, sizeof(consulta), "INSERT INTO jugadores (usuario, contrase�a) VALUES ('%s', '%s')", usuario, password);
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
>>>>>>> 61231ba33c673faff748e09115aa9c0c3180bbf9
}

void LoginUsuario(int sock_conn, MYSQL *conn, char *usuario, char *password) 
{
<<<<<<< HEAD
    char consulta[256], respuesta[512];
    snprintf(consulta, sizeof(consulta), "SELECT COUNT(*) FROM jugadores WHERE usuario = '%s' AND password = '%s';", usuario, password);
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
=======
	char consulta[256], respuesta[512];
	snprintf(consulta, sizeof(consulta), "SELECT COUNT(*) FROM jugadores WHERE usuario = '%s' AND contrase�a = '%s'", usuario, password);
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
>>>>>>> 61231ba33c673faff748e09115aa9c0c3180bbf9
}

void ObtenerCartas(int sock_conn, MYSQL *conn) 
{
<<<<<<< HEAD
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


void Reglas (Partida* partidas, int sock_conn, char *color1, int *numero1, char *color2, int *numero2, int ID_partida, char* usuario)
{//Determina si es el turno del jugador que hace la petici n
	if (strcmp(partidas[ID_partida].listajugadores[partidas[ID_partida].turno].usuario,usuario)==0)
	{
		int respuesta;
		if (*numero1 == *numero2){
			printf("Movimiento valido\n");
			respuesta = 0;
		}
		else if (strcmp(color1,color2)==0){
			printf("Movimiento valido\n");
			respuesta = 0;
		}
		else{
			printf("Movimiento invalido\n");
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
	else
	{//Si no lo es, le envia este mensaje
		char respuesta[512];
		sprintf(respuesta,"NOT: Espere su turno");
		write(sock_conn, respuesta, strlen(respuesta));
	}
}


void DarCarta (Partida partidas[], int sock_conn, MYSQL *conn, int  ID_partida, char* usuario) 
{
	if (strcmp(partidas[ID_partida].listajugadores[partidas[ID_partida].turno].usuario,usuario)==0)
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
	else
	{//Si no lo es, le envia este mensaje
		char respuesta[512];
		sprintf(respuesta,"NOT: Espere su turno");
		write(sock_conn, respuesta, strlen(respuesta));
=======
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
		printf("Movimiento v�lido\n");
		respuesta = 0;
	}
	else if (strcmp(color1,color2)==0){
		printf("Movimiento v�lido\n");
		respuesta = 0;
	}
	else{
		printf("Movimiento inv�lido\n");
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
>>>>>>> 61231ba33c673faff748e09115aa9c0c3180bbf9
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

<<<<<<< HEAD
void GenerarCartaInicialPartida(int ID_partida, MYSQL *conn, Partida partidas[]) {	
	if (partidas[ID_partida].carta_inicial_asignada == 0) {
		char consulta[256];
		snprintf(consulta, sizeof(consulta), "SELECT ID_c, color, numero FROM cartas ORDER BY RAND() LIMIT 1;");
		
		if (mysql_query(conn, consulta) == 0) {
			MYSQL_RES *resultado = mysql_store_result(conn);
			MYSQL_ROW fila;
			if ((fila = mysql_fetch_row(resultado)) != NULL) {
				strncpy(partidas[ID_partida].cartaInicial_ID, fila[0], sizeof(partidas[ID_partida].cartaInicial_ID));
				strncpy(partidas[ID_partida].cartaInicial_color, fila[1], sizeof(partidas[ID_partida].cartaInicial_color));
				strncpy(partidas[ID_partida].cartaInicial_numero, fila[2], sizeof(partidas[ID_partida].cartaInicial_numero));
				partidas[ID_partida].carta_inicial_asignada = 1;
			}
			mysql_free_result(resultado);
		}
	}
}

void EnviarCartaInicial(int sock_conn, int ID_partida, Partida partidas[]) {
	if (partidas[ID_partida].carta_inicial_asignada == 1) {
		char respuesta[512] = "8/";
		char carta[50];
		snprintf(carta, sizeof(carta), "%s,%s,%s;", 
				 partidas[ID_partida].cartaInicial_ID, 
				 partidas[ID_partida].cartaInicial_color, 
				 partidas[ID_partida].cartaInicial_numero);
		strcat(respuesta, carta);
		printf("%s", respuesta);		
		if (respuesta[strlen(respuesta) - 1] == ';') {
			respuesta[strlen(respuesta) - 1] = '\0';
		}
		
		write(sock_conn, respuesta, strlen(respuesta));
		NotificarAPartida(ID_partida,sock_conn,respuesta);
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
		pthread_mutex_unlock(&partidas_mutex);
		return nuevaID;
	}
	else
	{
		int index=0;
		
		while (index<num_partidas)
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
					
					/*if (partidas[index].num==4)
					{
						partidas[index].estado=1;
					}*/
				
					pthread_mutex_unlock(&partidas_mutex);
					
					char notificacion[512];
					sprintf(notificacion, "NOT: El jugador %s se ha unido a la partida.\n", usuario);
					NotificarAPartida(partidas[index].id,socket, notificacion);
					return partidas[index].id;
				}
			}
			index++;
		}

		if (num_partidas<MAX_PARTIDAS)
		{
			int nuevaID=num_partidas;
			partidas[nuevaID].id=nuevaID;
			partidas[nuevaID].estado=0;				
			strcpy(partidas[nuevaID].listajugadores[0].usuario,usuario);
			partidas[nuevaID].listajugadores[0].socket=socket;
			partidas[nuevaID].num=1;
				
			num_partidas++;
			pthread_mutex_unlock(&partidas_mutex);
								
			return nuevaID;
		}
		else 
		{
			pthread_mutex_unlock(&partidas_mutex);
			return -1;
		}
	}
}

void EliminarJugadorDePartida(Partida partidas[], int ID_partida,int socket_jugador) 
{
	printf("Turno %d",partidas[ID_partida].turno);
	pthread_mutex_lock(&partidas_mutex);	
	printf("Procediendo a eliminar el jugador");
	for (int j = 0; j < partidas[ID_partida].num; j++) 
	{
		if (partidas[ID_partida].listajugadores[j].socket == socket_jugador) 
		{
			//Determinar si es el turno del jugador que abandona
			int esSuTurno=0;
			if ((partidas[ID_partida].estado == 1) && (partidas[ID_partida].turnos_asignados == 1) && (j==partidas[ID_partida].turno))
			{
				esSuTurno=1;
			}
			// Eliminar al jugador desplazando los siguientes
			for (int k = j; k < partidas[ID_partida].num - 1; k++) 
			{
				partidas[ID_partida].listajugadores[k] = partidas[ID_partida].listajugadores[k + 1];
			}
			partidas[ID_partida].num--;
				
			// Si no quedan jugadores, se  resetea la partida
			if (partidas[ID_partida].num == 0) 
			{
				partidas[ID_partida].estado = 0;
				partidas[ID_partida].carta_inicial_asignada=0;
				partidas[ID_partida].turnos_asignados=0;
				
				pthread_mutex_unlock(&partidas_mutex);
				return;
			}
			
			printf("%d", esSuTurno);
			if (esSuTurno==1 && partidas[ID_partida].num > 0)
			{
				if (j >= partidas[ID_partida].num)
					partidas[ID_partida].turno = j-1;
				else
					partidas[ID_partida].turno = j;

				printf("Se procede a notificar a los jugadores");
				//Notifica a los jugadores de la partida de quien tiene el turno actualmente
				/*char notificacion[512];
				sprintf(notificacion, "NOT: El jugador %s tiene el turno", partidas[ID_partida].listajugadores[j].usuario);
				NotificarAPartida(ID_partida,partidas[ID_partida].listajugadores[partidas[ID_partida].turno].socket,notificacion);*/

				//Da turno al jugador que ocupa la posicion del que ha abandonado la partida
				char respuesta[512] = "TURNO/";
				write(partidas[ID_partida].listajugadores[partidas[ID_partida].turno].socket, respuesta, strlen(respuesta));
			}
				
			pthread_mutex_unlock(&partidas_mutex);
			return;
		}
	}
	
	pthread_mutex_unlock(&partidas_mutex);
}

int cambiarEstadoJugador(Partida partidas[], int ID_partida, int sock_conn, int cambio)
{	
	//Si el parametro cambio es un 0, el jugador quiere indicar que esta listo para iniciar la partida
	if (cambio==0)
	{
		pthread_mutex_lock(&partidas_mutex);
		int i;
		for (i=0;i<partidas[ID_partida].num;i++)
		{
			if (partidas[ID_partida].listajugadores[i].socket==sock_conn)
			{
				partidas[ID_partida].listajugadores[i].estado=1;
			}
		}
		int contador=0;

		int j;
		for(j=0;j<partidas[ID_partida].num;j++)
		{
			if (partidas[ID_partida].listajugadores[j].estado==1)
			{
				contador++;
			}
		}
	
		pthread_mutex_unlock(&partidas_mutex);

		if(contador==partidas[ID_partida].num && partidas[ID_partida].num>1)
		{			
			return 0;
		}
	}
	else
	{//Si el parametro cambio es 1, el jugador quiere indicar que no est  listo para jugar
		pthread_mutex_lock(&partidas_mutex);
		int i;
		for (i=0;i<partidas[ID_partida].num;i++)
		{
			if (partidas[ID_partida].listajugadores[i].socket==sock_conn)
			{
				partidas[ID_partida].listajugadores[i].estado=1;
			}
		}
		pthread_mutex_unlock(&partidas_mutex);
	}
	return -1;	
}
void cambiarEstadoPartida(Partida partidas[], int ID_partida)
{
	if (partidas[ID_partida].estado==0)
	{
		partidas[ID_partida].estado=1;
	}
	else{
		partidas[ID_partida].estado=0;
	}
}

void AsignarJugadorInicial(Partida partidas[], int ID_partida)
{
	if (partidas[ID_partida].turnos_asignados==0)
	{
		//Copia la cantidad de jugadores en la partida en la variable numJugadores
		int numJugadores = partidas[ID_partida].num;	
	
		//Inicia el generador de numeros aleatorios
		srand(time(NULL));
	
		int posicionInicial= (rand() % numJugadores); //Selecciona un numero aleatorio entre 0 y el numero de jugadores en partida -1
		printf("%d", posicionInicial);
		
		//Cambia el valor de las varibles de la clase partida correspondientes
		partidas[ID_partida].turno=posicionInicial;
		partidas[ID_partida].turnos_asignados=1;

		int socket= partidas[ID_partida].listajugadores[posicionInicial].socket;
		//Notifica a los jugadores de la partida de quien tiene el turno inicial
		/*char notificacion[512];
		sprintf(notificacion, "NOT: El jugador %s tiene el turno inicial", partidas[ID_partida].listajugadores[posicionInicial].usuario);
		NotificarAPartida(ID_partida,socket,notificacion);*/

		//Da turno al jugador inicial
		char respuesta[512];
		sprintf(respuesta, "TURNO/");
		write(socket, respuesta, strlen(respuesta));					
	}
}

void pasarTurno(Partida partidas[], int ID_partida)
{
	//Si el turno es el del  ltimo jugador en la lista o mayor, se situa al inicio del vector listajugadores de la partida
	if (partidas[ID_partida].turno== partidas[ID_partida].num-1 || partidas[ID_partida].turno> partidas[ID_partida].num-1)
	{
		partidas[ID_partida].turno=0;
	}
	else
	{//De lo contrario, avanza una posici n inicial
		partidas[ID_partida].turno ++;
	}
	
	//Notifica el turno del siguiente jugador a los otros jugadores de la partida 
	int socket=partidas[ID_partida].listajugadores[partidas[ID_partida].turno].socket;
	
	char notificacion[512];
	sprintf(notificacion, "NOT: Es el turno del jugador %s\n", partidas[ID_partida].listajugadores[partidas[ID_partida].turno].usuario);
	NotificarAPartida(ID_partida,socket, notificacion);
	//Da turno a ese jugador
	char respuesta[512] = "TURNO/";
	write(partidas[ID_partida].listajugadores[partidas[ID_partida].turno].socket, respuesta, strlen(respuesta));
}

int insertar_partida_finalizada(MYSQL *conn, int ID_partida, const char *usuario_ganador) {
    char query[512];
    MYSQL_RES *res;
    MYSQL_ROW row;

    // Paso 1: Obtener el ID del usuario ganador
    snprintf(query, sizeof(query),
             "SELECT ID_j FROM jugadores WHERE usuario = '%s'", usuario_ganador);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error al buscar ID del usuario ganador '%s': %s\n", usuario_ganador, mysql_error(conn));
        return -1;
    }

    res = mysql_store_result(conn);
    if ((row = mysql_fetch_row(res)) == NULL) {
        fprintf(stderr, "Usuario ganador '%s' no encontrado.\n", usuario_ganador);
        mysql_free_result(res);
        return -1;
    }

    int id_ganador = atoi(row[0]);
    mysql_free_result(res);

    // Paso 2: Obtener num_jugadores desde estructura en memoria
    int num_jugadores = partidas[ID_partida].num;

    // Paso 3: Insertar en la tabla partida
    snprintf(query, sizeof(query),
             "INSERT INTO partida (tipo, num_jugadores, ID_ganador, hora_fin) "
             "VALUES ('UNO multijugador', %d, %d, NOW())",
             num_jugadores, id_ganador);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error al insertar partida: %s\n", mysql_error(conn));
	return -1;
    } else {
        printf("Partida finalizada insertada correctamente.\n");
    }

    // Paso 4: Obtener el ID_partida autogenerado
    int id_partida_generada = (int)mysql_insert_id(conn);
    printf("Partida insertada con ID = %d\n", id_partida_generada);
    return id_partida_generada;
}

void insertar_jugadores_en_partida(MYSQL *conn, Partida partidas[], int indice_partida, int id_partida_db)
{
    char query[256];
    MYSQL_RES *res;
    MYSQL_ROW row;

    Partida partida = partidas[indice_partida];

    for (int i = 0; i < partida.num; i++) {
        char *usuario = partida.listajugadores[i].usuario;

        snprintf(query, sizeof(query),
                 "SELECT ID_j FROM jugadores WHERE usuario = '%s'", usuario);

        if (mysql_query(conn, query)) {
            fprintf(stderr, "Error buscando jugador '%s': %s\n", usuario, mysql_error(conn));
            continue;
        }

        res = mysql_store_result(conn);
        if ((row = mysql_fetch_row(res)) != NULL) {
            int id_jugador = atoi(row[0]);

            snprintf(query, sizeof(query),
                     "INSERT INTO jugadores_partida (ID_j, ID_p) VALUES (%d, %d)",
                     id_jugador, id_partida_db);

            if (mysql_query(conn, query)) {
                fprintf(stderr, "Error insertando jugador '%s' en partida %d: %s\n",
                        usuario, id_partida_db, mysql_error(conn));
            } else {
                printf("Jugador '%s' insertado en partida %d (ID_j=%d)\n",
                       usuario, id_partida_db, id_jugador);
            }
        } else {
            fprintf(stderr, "Usuario '%s' no encontrado en la base de datos.\n", usuario);
        }
        mysql_free_result(res);
    }
}

void EliminarCuenta(MYSQL *conn, char * usuario){
	char consulta[256];
	char respuesta[128];
	snprintf(consulta, sizeof(consulta), "DELETE FROM jugadores WHERE usuario = '%s';", usuario);
	printf("Ejecutando consulta: %s\n", consulta);
	if (mysql_query(conn, consulta) == 0) {
		my_ulonglong filas_afectadas = mysql_affected_rows(conn);
		if (filas_afectadas > 0)
		{
			printf("Fila eliminada.\n");
			snprintf(respuesta, sizeof(respuesta), "23/OK");
		}
		else
			printf("No se encontro la fila.\n");
	} else {
		printf("Error en la consulta DELETE.\n");
	}
}


void *AtenderCliente(void *arg) 
{
    int sock_conn = *((int *)arg);
    free(arg);
    char peticion[512], respuesta[512];
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
			EliminarUsuario(sock_conn);
		}
		else if (codigo == 6) 
		{
            ObtenerCartas(sock_conn, conn);
        }
		else if (codigo == 7) 
		{
			char *sala = strtok(NULL, "/");
			int ID_sala= atoi(sala);
			printf("%d\n",ID_sala);
			
			char *usuario = strtok(NULL, "/");			
			
			DarCarta(partidas, sock_conn, conn, ID_sala, usuario);
		}
		
		else if (codigo == 8) 
		{
			char *sala = strtok(NULL, "/");
			int ID_sala= atoi(sala);
			printf("%s\n", sala);			

			cambiarEstadoPartida(partidas,ID_sala);

			if (partidas[ID_sala].num>1)
			{
				GenerarCartaInicialPartida(ID_sala, conn, partidas);
				EnviarCartaInicial(sock_conn, ID_sala, partidas);
				sleep(0.001);
				pthread_mutex_lock(&partidas_mutex);
				AsignarJugadorInicial(partidas, ID_sala);
				pthread_mutex_unlock(&partidas_mutex);
			}
			else
			{
				EliminarJugadorDePartida(partidas,ID_sala,sock_conn);			
				snprintf(respuesta, sizeof(respuesta), "15/Has abandonado con exito la partida");
				write(sock_conn, respuesta, strlen(respuesta));
			}
		}
		else if (codigo == 9){
			char *color1 = strtok(NULL, "/");
			char *numero1_str = strtok(NULL, "/");
			char *color2 = strtok(NULL, "/");
			char *numero2_str = strtok(NULL, "/");
			char *usuario = strtok(NULL, "/");
			char *sala = strtok(NULL, "/");
			int ID_sala= atoi(sala);
			
			if (color1 && numero1_str && color2 && numero2_str && sala && usuario){
				int numero1 = atoi(numero1_str);
				int numero2 = atoi(numero2_str);
				Reglas(partidas, sock_conn, color1, &numero1, color2, &numero2, ID_sala, usuario);
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
			} else {
				snprintf(respuesta, sizeof(respuesta), "Error en el formato del mensaje de chat");
				write(sock_conn, respuesta, strlen(respuesta));
			}
		}
		else if (codigo == 11) {
			char* invitador = strtok(NULL, "/");
			char* lista = strtok(NULL, "/");
			char* partida=strtok(NULL,"/");
			int ID_partida=atoi(partida);
			
			if (invitador && lista) {
				ProcesarInvitacion(sock_conn, invitador, lista,ID_partida);
			} else {
				const char msg[] = "11/Formato invalido de invitacion";
				write(sock_conn, msg, strlen(msg));
			}
		}		
/* -------------------------------------------------------------
		12/ACCEPT|DECLINE/<idSala>/<invitador>/<invitado>
		-------------------------------------------------------------*/
		
		else if (codigo == OP_INVITE_RESPONSE) {
			char tipo      = strtok(NULL, "/");      // ACCEPT o DECLINE */
			char sala_str  = strtok(NULL, "/");      // id sala          */
			char *invitador = strtok(NULL, "/");
			char invitado  = strtok(NULL, "/");      // quien responde   */
			int   idSala    = sala_str ? atoi(sala_str) : -1;
			
			if (!tipo || !invitador || !invitado || idSala < 0) {
				write(sock_conn, "12/Formato invalido", 18);
				continue;
			}
			
			if (!strcmp(tipo, "ACCEPT")) {
				/* ---- 1. A￱adir al jugador ------------------------ */
				if (unirJugadorAPartida(partidas, idSala, invitado, sock_conn) == 0) {
					/* ---- 2. Aviso a todos ------------------------ */
					char notif[128];
					snprintf(notif, sizeof(notif),
							 "NOT:%s ha aceptado la invitaci￳n", invitado);
					NotificarAPartida(idSala, sock_conn, notif);
					
					/* ---- 3. Orden de abrir la sala --------------- */
					char go[32];
					snprintf(go, sizeof(go), "13/%d", idSala);
					
					pthread_mutex_lock(&partidas_mutex);
					for (int i = 0; i < partidas[idSala].num; ++i)
						write(partidas[idSala].listajugadores[i].socket,
							  go, strlen(go));
					pthread_mutex_unlock(&partidas_mutex);
				}
			} else { /* DECLINE */
				char notif[128];
				snprintf(notif, sizeof(notif),
						 "NOT:%s ha rechazado la invitaci￳n", invitado);
				NotificarAPartida(idSala, sock_conn, notif);
			}
			
			/* acuse individual para cerrar el di￡logo */
			write(sock_conn, "12/OK", 5);
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
			pasarTurno(partidas, sala);
		}
		else if (codigo == 15)
		{
			char *sala = strtok(NULL, "/");
			int ID_sala= atoi(sala);
			char *usuario = strtok(NULL, "/");

			EliminarJugadorDePartida(partidas,ID_sala,sock_conn);			
			snprintf(respuesta, sizeof(respuesta), "15/Has abandonado con exito la partida");
			write(sock_conn, respuesta, strlen(respuesta));

			char notificacion[512];
			sprintf(notificacion, "NOT: El jugador %s ha abandonado la partida.\n", usuario);
			NotificarAPartida(ID_sala,sock_conn, notificacion);
		}
		else if (codigo == 16) 
		{ // Obtener jugadores en linia
			strcpy(respuesta, "16/Jugadores en linea:\n");
			for (int i = 0; i < num_usuarios; i++) 
			{
=======
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
>>>>>>> 61231ba33c673faff748e09115aa9c0c3180bbf9
				strcat(respuesta, usuarios[i].usuario);
				strcat(respuesta, "\n");
			}
			write(sock_conn, respuesta, strlen(respuesta));
		}
<<<<<<< HEAD
		else if (codigo== 17)
		{
			char* sala = strtok(NULL, "/");
			int id_partida=atoi(sala);

			char *state = strtok(NULL, "/");
			char estado[20];
			if (state == NULL) break;
			strcpy(estado,state);
			printf("%s",estado);

			int cambioEstado;
			if (strcmp(estado,"LISTO")==0)
			{
				cambioEstado=0;
			}
			else
			{
				cambioEstado=1;
			}

			int resultado=cambiarEstadoJugador(partidas,id_partida, sock_conn,cambioEstado);

			if (resultado==-1) //Resultado que indica que faltan jugadores por estar listos
			{
				snprintf(respuesta, sizeof(respuesta), "17/NO LISTOS");
				write(sock_conn, respuesta, strlen(respuesta));
			}
			else{ //Todos los jugadores preparados
				snprintf(respuesta, sizeof(respuesta), "17/TODOS LISTOS");
				write(sock_conn, respuesta, strlen(respuesta));
			}
		}
		else if (codigo == 18){
			char *usuarioUNO = strtok(NULL, "/");
			char *sala = strtok(NULL, "/");
			int ID_sala= atoi(sala);
			char p = strtok(NULL, "/");
			printf(usuarioUNO);

			snprintf(respuesta, sizeof(respuesta), "18/%s", usuarioUNO);
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
		else if (codigo== 19)
		{
			char* sala = strtok(NULL, "/");
			int id_partida= atoi(sala);

			//pthread_mutex_lock(&partidas_mutex);
			pasarTurno(partidas, id_partida);
			//pthread_mutex_unlock(&partidas_mutex);
		}
		else if (codigo == 20) 
		{ // Consulta 1: jugadores con los que he hecho alguna partida
			char *usuario = strtok(NULL, "/");
			listarJugadoresRelacionados(sock_conn, conn, usuario);
		}
		else if (codigo == 21) 
		{ // Consulta 2: datos de partidas jugadas con otro jugador
			char* usuario = strtok(NULL, "/");
			char* usuario2 = strtok(NULL, "/");
			listarPartidasRelacionadas(sock_conn, conn, usuario, usuario2);
		}
		else if (codigo == 22) 
		{ // Consulta 3: partidas jugadas después de fecha
			char* usuario = strtok(NULL, "/");
			printf("%s",usuario);
			char* dia_str = strtok(NULL, "/");
			int dia = atoi(dia_str);
			char* mes_str = strtok(NULL, "/");
			int mes = atoi(mes_str);
			char* ano_str = strtok(NULL, "/");
			int ano = atoi(ano_str);
			if (!usuario || !dia_str || !mes_str || !ano_str) {
				printf("Error en los parametros");
			}
			listarPartidasHoras(sock_conn, conn, usuario, dia, mes, ano);
		}
		else if (codigo == 23) 
		{ // Eliminar cuenta
			char* usuario = strtok(NULL, "/");
			EliminarCuenta(conn, usuario);
		}

		else if (codigo == 97)
		{ //Fin de la partida
			char* sala = strtok(NULL, "/");
			int id_partida=atoi(sala);
			char *usuario = strtok(NULL, "/");
			
			MYSQL_RES *res;
			MYSQL_ROW row;
			char query[2048];
			snprintf(query, sizeof(query),"SELECT ID_j FROM jugadores WHERE usuario = '%s'", usuario);

			if (mysql_query(conn, query)) {
				fprintf(stderr, "Error buscando jugador '%s': %s\n", usuario, mysql_error(conn));
				continue;
			}

			res = mysql_store_result(conn);
			if ((row = mysql_fetch_row(res)) != NULL) {
				int id_jugador = atoi(row[0]);
			}
		}
		else if (codigo == 100) 
		{ //Fin de la partida
			char* sala = strtok(NULL, "/");
			int id_partida=atoi(sala);

			char usuario_ganador[20];
			p = strtok(NULL, "/");
			if (p != NULL) {
				strncpy(usuario_ganador, p, sizeof(usuario_ganador) - 1);
        			usuario_ganador[sizeof(usuario_ganador) - 1] = '\0'; // Seguridad de terminación

				int id_partida_generada = insertar_partida_finalizada(conn, id_partida, usuario_ganador);

		 		if (id_partida_generada != -1) {
					insertar_jugadores_en_partida(conn, partidas, id_partida, id_partida_generada);
				}
			}
			else {
			     fprintf(stderr, "Error: No se recibió el nombre del usuario ganador.\n");
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
=======
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
>>>>>>> 61231ba33c673faff748e09115aa9c0c3180bbf9
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(puerto);
	
<<<<<<< HEAD
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
	
    int puerto = 9050;
    int result = ConexionC(puerto);
    pthread_mutex_destroy(&usuarios_mutex);
	pthread_mutex_destroy(&partidas_mutex);
	
    return result;
}
=======
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



>>>>>>> 61231ba33c673faff748e09115aa9c0c3180bbf9
