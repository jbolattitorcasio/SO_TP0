#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* clave;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	log_info(logger, "Soy un Log");

	log_destroy(logger);


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	logger = iniciar_logger();

	config = iniciar_config();

	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	clave = config_get_string_value(config, "CLAVE");

	log_info(logger, "Los valores del config son:\n\tIp: %s\n\tPuerto: %s\n\tClave: %s", ip, puerto, clave);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(clave, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	// Crear Logger: (ruta, nombre, si muestra por consola, nivel)
	t_log* nuevo_logger = log_create("tp0.log", "Cliente", true, LOG_LEVEL_INFO);

	// Error si el logger devuelve Null
	if(nuevo_logger == NULL) {
		printf("Error al crear logger\n");
		exit(EXIT_FAILURE);
	}
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("cliente.config");
	t_log* logger = iniciar_logger();

	// Error si el config devuelve Null
	if(nuevo_config == NULL) {
		log_info(logger, "Error al crear config\n");
		exit(EXIT_FAILURE);
	}

	log_destroy(logger);
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	while((leido = readline("Log: > ")) != NULL && strcmp(leido, "") != 0) {
		log_info(logger, leido);
		free(leido);
	}
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	while((leido = readline("Paquete: > ")) != NULL && strcmp(leido, "") != 0) {
		agregar_a_paquete(paquete, leido, strlen(leido) + 1);
		free(leido);
	}

	enviar_paquete(paquete, conexion);

	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	liberar_conexion(conexion);
	config_destroy(config);
	log_destroy(logger);
}
