#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger ; //= log_create("tp0.log",logear,1,LOG_LEVEL_INFO);
	

	t_config* config ;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();
	
	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger,"Soy un Log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();
	if (config == NULL){
		log_info(logger,"No se pudo crear el config!");
		abort();
	}
	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	ip = config_get_string_value(config ,"IP");
	puerto= config_get_string_value(config,"PUERTO");
	valor = config_get_string_value(config, "CLAVE");

	// Loggeamos el valor de config
	log_info(logger , valor);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor,conexion);
	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("tp0.log","logger",1,LOG_LEVEL_INFO);

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config= config_create("cliente.config");
	
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido= string_new();

	// La primera te la dejo de yapa
	//leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while (1)
	{
		leido = readline(">");
		 if (string_is_empty(leido)) {
            break;
        }
		else{
			log_info(logger,string_from_format("Leido:%s",leido));
		}
		
		free(leido);
	}
	
	
	// ¡No te olvides de liberar las lineas antes de regresar!
	//free(leido);
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete=crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	while (1)
	{
		leido = readline(">");
		 if (string_is_empty(leido)) {
            break;
        }
		else{
			
		agregar_a_paquete(paquete,leido,strlen(leido)+1);
		}
		
		free(leido);
	}
	enviar_paquete(paquete,conexion);
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
}
