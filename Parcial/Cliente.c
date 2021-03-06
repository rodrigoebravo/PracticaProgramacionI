#include "Cliente.h"
#include "Venta.h"
#include "utn.h"

/**
    funcion pide datos para hacer alta en clientes
    parametro pEntidad: lista de clientes
    parametro len: longitud de lista de clientes
    parametro index: posicion a dar de alta
    return -1 si ocurre algun error o >=0 si esta todo OK (id)
*/
static int cli_getAltaCliente(Cliente* pEntidad, int len, int index);
/**
    funcion pide datos para hacer baja en clientes
    parametro cli: lista de clientes
    parametro lenCli: longitud de lista de clientes
    parametro ven: lista ventas
    parametro lenVen: longitud de lista de ventas
    parametro index: posicion a dar de baja
    return -1 si ocurre algun error o >=0 si esta todo OK
*/
static int cli_BajaPorPosicion(Cliente* cli, int lenCli, int index, Venta* ven, int lenVen);
/**
    funcion encontrar posicion vacia
    parametro pEntidad: lista de clientes
    parametro len: longitud de lista de clientes
    parametro ven: lista ventas
    parametro lenVen: longitud de lista de ventas
    parametro *indexVacio: posicion retorno
    return -1 si ocurre algun error o >=0 si esta todo OK
*/
static int cli_obtenerPosicionVacia(Cliente* pEntidad, int len, int* indexVacio);
/**
    funcion para generarID para Clientes (Array)
*/
static int generarID(void);
/**
    funcion pide datos para hacer modificacion en clientes
    parametro pEntidad: lista de clientes
    parametro len: longitud de lista de clientes
    parametro index: posicion a modificar
    return -1 si ocurre algun error o >=0 si esta todo OK
*/
static int cli_modificar(Cliente* pEntidad, int len, int index);

/**
    funcion da de alta un cliente a la vez por posicion
    parametro pEntidad: lista de clientes
    parametro nombre: nombre del cliente
    parametro apellido: apellido del cliente
    parametro cuit: cuit del cliente
    parametro i: posicion a dar de alta

*/
static void altaCliente(Cliente* pEntidad, char* nombre, char* apellido, char* cuit, int i);
/**
    funcion ingresa clientes forzados
    parametro pEntidad: lista de clientes
    parametro len: longitud de Clientes (array)
    parametro i: posicion a dar de alta

    return -1 si ocurre algun error o >=0 si esta todo OK
*/
static void forzarAltasClientes(Cliente* pEntidad, int len);

int cli_inicializarListaClientes(Cliente* pEntidad, int len)
{
    int i;
    int retorno=ERROR;
    if(pEntidad !=NULL && len>0)
    {
        for(i=0; i<len; i++)
        {
            pEntidad[i].isEmpty=TRUE;
        }
        forzarAltasClientes(pEntidad, len);
        retorno=TODOOK;
    }
    return retorno;
}

int cli_darAltaCliente(Cliente* pEntidad, int len)
{
    int index;
    int retorno=ERROR;

    if(cli_validarParametros(pEntidad, len)==TODOOK && cli_obtenerPosicionVacia(pEntidad, len, &index)==TODOOK)
    {
        retorno=cli_getAltaCliente(pEntidad, len, index);
    }
    return retorno;
}

int cli_modificarClientePorID(Cliente* pEntidad, int len)
{
    int retorno=ERROR;
    int idModificar;
    int indexModificar;
    int idMaximo=cli_getMaxIDCliente(pEntidad, len);
    if(clientesTieneDatos(pEntidad, len))
    {
        cli_printClientes(pEntidad, len);

        if(idMaximo!=ERROR && utn_getEntero(&idModificar, 3, idMaximo+1, -1, "Ingrese ID a modificar:\n", "ID erroneo\n")==TODOOK &&
                cli_obtenerPosicionPorID(pEntidad, len, idModificar, &indexModificar)==TODOOK &&
                cli_posicionEstaVacia(pEntidad, len, indexModificar)==FALSE &&
                cli_modificar(pEntidad, len, indexModificar)==TODOOK)
        {
                retorno=TODOOK;
        }
        else
        {
                limpiarScreen();
                printf("No es posible modificar cliente.\n");
        }
    }
    else
    {
        limpiarScreen();
        printf("No es posible modificar clientes, no existen datos.\n");
    }
    return retorno;
}

int cli_bajaClientePorID(Cliente* pEntidad, int lenCli, Venta* ven, int lenVen)
{
    int retorno=ERROR;
    int idBaja;
    int indexBaja;
    int idMaxCliente=cli_getMaxIDCliente(pEntidad, lenCli);
    if(clientesTieneDatos(pEntidad, lenCli))
    {
        cli_printClientes(pEntidad, lenCli);
        if(cli_validarParametros(pEntidad, lenCli)==TODOOK && ven!=NULL && lenVen>0 && idMaxCliente!=ERROR
            && utn_getEntero(&idBaja, 3, idMaxCliente+1, -1, "Ingrese ID del cliente a dar de baja\n", "Error al dar de baja el ID\n")==TODOOK
            && cli_obtenerPosicionPorID(pEntidad, lenCli, idBaja, &indexBaja)==TODOOK
            && cli_posicionEstaVacia(pEntidad, lenCli, indexBaja)==FALSE)
        {
            retorno=cli_BajaPorPosicion(pEntidad, lenCli, indexBaja, ven, lenVen);
        }
        else
        {
            limpiarScreen();
            printf("No es posible eliminar cliente.\n");
        }
    }
    else
    {
        limpiarScreen();
        printf("No es posible eliminar clientes, no existen datos.\n");
    }

    return retorno;
}

static int cli_getAltaCliente(Cliente* pEntidad, int len, int index)
{
    int retorno=ERROR;
    Cliente auxCli;

    limpiarScreen();
    if(cli_validarParametros(pEntidad, len)==TODOOK &&
            index>=0 &&
            index<len &&
            utn_getCadena(auxCli.nombre, 20, 20, 3, 3, "Ingrese nombre\n", "Nombre incorrecto\n")==TODOOK &&
            utn_getCadena(auxCli.apellido, 20, 20, 3, 3, "Ingrese apellido\n", "Apellido incorrecto\n")==TODOOK &&
            utn_getCadena(auxCli.cuit, 20, 20, 11, 3, "Ingrese cuit\n", "Cuit incorrecto\n")==TODOOK)
    {
        auxCli.id=generarID();
        auxCli.isEmpty=FALSE;
        pEntidad[index]=auxCli;
        retorno=pEntidad[index].id;
    }
    return retorno;
}

static int cli_modificar(Cliente* pEntidad, int len, int index)
{
    int retorno=ERROR;
    int opcionModificar;
    Cliente auxCli;

    if(cli_validarParametros(pEntidad, len)==TODOOK && index>=0 && index<len)
    {
        auxCli=pEntidad[index];

        limpiarScreen();
        if(utn_getEntero(&opcionModificar, 3, 5, 0, "1-Nombre\n2-Apellido\n3-CUIT\nIngrese Opcion a modificar:", "Opcion incorrecta")==TODOOK)
        {
            switch(opcionModificar)
            {
                case 1:
                    utn_getCadena(auxCli.nombre, 20, 20, 3, 3, "Ingrese nombre\n", "Nombre incorrecto\n");
                    break;
                case 2:
                    utn_getCadena(auxCli.apellido, 20, 20, 3, 3, "Ingrese apellido\n", "Apellido incorrecto\n");
                    break;
                case 3:
                    utn_getCadena(auxCli.cuit, 20, 20, 11, 3, "Ingrese cuit\n", "Cuit incorrecto\n");
                    break;
                case 4:
                    break;
            }
            pEntidad[index]=auxCli;
        }
        retorno=TODOOK;
    }
    return retorno;
}

static int cli_BajaPorPosicion(Cliente* cli, int lenCli, int index, Venta* ven, int lenVen)
{
    int retorno=ERROR;
    int i;
    if(cli_validarParametros(cli, lenCli)==TODOOK && ven!=NULL && lenVen>0 && index>=0 && index<lenCli)
    {
        cli[index].isEmpty=TRUE;

        for(i=0; i<lenVen; i++)
        {
            if(ven[i].idCliente==cli[index].id && ven[i].isEmpty==FALSE)
            {
                ven[i].isEmpty=TRUE;
            }
        }
        retorno=TODOOK;
    }
    return retorno;
}

int cli_posicionEstaVacia(Cliente* pEntidad, int len, int index)
{
    int retorno=TRUE;

    if(pEntidad!=NULL && index>=0 && len>0 && index<len)
    {
        retorno=pEntidad[index].isEmpty;
    }
    return retorno;
}

static int cli_obtenerPosicionVacia(Cliente* pEntidad, int len, int* indexVacio)
{
    int i;
    int retorno=ERROR;

    if(cli_validarParametros(pEntidad, len)==TODOOK)
    {
        for(i=0; i<len; i++)
        {
            if(pEntidad[i].isEmpty)
            {
                *indexVacio=i;
                retorno=TODOOK;
                break;
            }
        }
    }
    return retorno;
}

static int generarID(void)
{
    static int id=-1;
    id++;
    return id;
}

int cli_obtenerPosicionPorID(Cliente* pEntidad, int len, int id, int* indexRetorno)
{
    int i;
    int retorno=ERROR;
    for(i=0; i<len; i++)
    {
        if(pEntidad[i].id==id && !pEntidad[i].isEmpty)
        {
            *indexRetorno=i;
            retorno=TODOOK;
            break;
        }
    }
    return retorno;
}

int cli_printClientes(Cliente* pEntidad, int lenCli)
{
    int retorno=ERROR;
    int i;
    if(cli_validarParametros(pEntidad, lenCli)==TODOOK)
    {
        //limpiarScreen();
        retorno=TODOOK;
        for(i=0; i<lenCli; i++)
        {
            if(!pEntidad[i].isEmpty)
            {
                cli_printCliente(pEntidad, i);
            }
        }
    }
    return retorno;
}

void cli_printCliente(Cliente* pEntidad, int index)
{
    printf("ID: %d Nombre: %s Apellido: %s Cuit: %s \n", pEntidad[index].id, pEntidad[index].nombre, pEntidad[index].apellido, pEntidad[index].cuit);
}

int cli_validarParametros(Cliente* pEntidad, int lenCli)
{
    int retorno=ERROR;
    if(pEntidad!=NULL && lenCli>0)
    {
        retorno=TODOOK;
    }
    return retorno;
}

int clientesTieneDatos(Cliente* pEntidad, int len)
{
    int retorno=FALSE;
    int i;
    if(cli_validarParametros(pEntidad, len)==TODOOK)
    {
        for(i=0; i<len; i++)
        {
            if(!pEntidad[i].isEmpty)
            {
                retorno=TRUE;
                break;
            }
        }
    }
    return retorno;
}

static void forzarAltasClientes(Cliente* pEntidad, int len)
{
    altaCliente(pEntidad, "Rodrigo", "Bravo", "20453164166", 0);
    altaCliente(pEntidad, "Sebastian", "Martinez", "23645168354", 1);
    altaCliente(pEntidad, "Maria", "Arange", "23845186798", 2);
    altaCliente(pEntidad, "Jose", "Morin", "23135416874", 3);
    altaCliente(pEntidad, "Brian", "Miria", "20835421687", 4);
    altaCliente(pEntidad, "Romina", "Zapiola", "20531687461", 5);
    altaCliente(pEntidad, "Ana", "Echeverria", "21168749684", 6);
    altaCliente(pEntidad, "Ana", "Echeverria", "21168749100", 30);
    altaCliente(pEntidad, "Ayelen", "Ituzaingo", "21653196874", 7);
    altaCliente(pEntidad, "Daniel", "Mirin", "21845146879", 8);
    altaCliente(pEntidad, "Juan", "Isasia", "20459896874", 9);
    altaCliente(pEntidad, "Ramiro", "Birus", "23584168514", 10);
    altaCliente(pEntidad, "Lorenzo", "Abarin", "20851465841", 11);
    altaCliente(pEntidad, "Lorenzo", "Mulli", "22163446465", 12);
    altaCliente(pEntidad, "Lauriel", "Paoli", "22879774131", 13);
    altaCliente(pEntidad, "Gael", "Mique", "22465784698", 14);
    altaCliente(pEntidad, "David", "Lamel", "22623251648", 15);
    altaCliente(pEntidad, "Paula", "Ramirez", "20849469864", 16);
    altaCliente(pEntidad, "Alejandra", "Coria", "20684613351", 17);
    altaCliente(pEntidad, "Miguel", "Lev", "20746532168", 18);
    altaCliente(pEntidad, "Angel", "Yuri", "20873696387", 19);
    altaCliente(pEntidad, "Elio", "Pobli", "21258963214", 20);
    altaCliente(pEntidad, "XXXXXXX", "Bari", "21621478963", 21);
    altaCliente(pEntidad, "Maximiliano", "Vu", "21874541235", 22);
    altaCliente(pEntidad, "Ornela", "Wall", "21646584891", 23);
    altaCliente(pEntidad, "Wally", "Kor", "24654168468", 24);
    altaCliente(pEntidad, "Wol", "apl", "25346846384", 25);
    altaCliente(pEntidad, "Celeste", "Xur", "26385468768", 26);
    altaCliente(pEntidad, "Rosa", "Florencio", "23684634638", 27);
    altaCliente(pEntidad, "Blanca", "Troll", "23684124638", 28);
    altaCliente(pEntidad, "Gabriel", "Gabri", "23114634638", 28);
    altaCliente(pEntidad, "Julieta", "venegas", "23114631000", 29);
}

static void altaCliente(Cliente* pEntidad, char* nombre, char* apellido, char* cuit, int i)
{
    pEntidad[i].id=generarID();
    strcpy(pEntidad[i].nombre, nombre);
    strcpy(pEntidad[i].apellido, apellido);
    strcpy(pEntidad[i].cuit, cuit);
    pEntidad[i].isEmpty=FALSE;
}

int cli_obtenerMaximoID(Cliente* pEntidad, int len, int* idRetorno)
{
    int maxID;
    int retorno=ERROR;
    int i;

    if(cli_validarParametros(pEntidad, len)==TODOOK && clientesTieneDatos(pEntidad, len))
    {
        for(i=0; i<len; i++)
        {
            if(!pEntidad[i].isEmpty)
            {
                if(i==0)
                {
                    maxID=pEntidad[i].id;
                }
                else if(pEntidad[i].id>maxID)
                {
                    maxID=pEntidad[i].id;
                }
            }
        }
        *idRetorno=maxID;
        retorno=TODOOK;
    }
    return retorno;
}


int cli_getMaxIDCliente(Cliente* pEntidad, int len)
{
    int id=ERROR;
    cli_obtenerMaximoID(pEntidad, len, &id);
    return id;
}
