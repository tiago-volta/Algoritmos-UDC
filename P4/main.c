#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#define TAM 256000


struct monticulo {
    int ultimo;     //Ultima celda ocupada del vector asociado al montículo
    int vector[TAM];
};
typedef struct monticulo * pmonticulo;

// Definición de la estructura para las cotas
typedef struct {
    double cotaSubestimada;
    double cotaAjustada;
    double cotaSobreestimada;
    const char *strCotaSubestimada;
    const char *strCotaAjustada;
    const char *strCotaSobreestimada;
} Cotas;

void iniMonticulo(pmonticulo *m);
void insertarMonticulo(pmonticulo m, int x);
void quitarMenor(pmonticulo m);
int consultarMenor(const pmonticulo m);
void crearMonticulo(pmonticulo m, int v [], int n);
void testOperaciones();
void ordenarPorMonticulos(int v[], int n);
void testOrdenarPorMonticulos();
int comprobarOrdenacion(const int v[], int n);

void inicializar_semilla();
void aleatorio(int v[], int n);
void ascendente(int v[], int n);
void descendente(int v[], int n);
void imprimirArray(const int v[], int n);

void imprimirTablas();


int main(void) {
    inicializar_semilla();
    testOperaciones();
    testOrdenarPorMonticulos();
    imprimirTablas();
}

static void intercambiar (pmonticulo m, int pos1, int pos2) {
    int tmp;
    tmp = m->vector[pos1];
    m->vector[pos1] = m->vector[pos2];
    m->vector[pos2] = tmp;
}

static void flotar(pmonticulo m, int i) {
    while (i > 0 && m->vector[(i - 1) / 2] > m->vector[i]) {
        intercambiar(m, (i - 1) / 2, i);
        i = (i - 1) / 2;
    }
}

static void hundir(pmonticulo m, int i) {
    int hijoIzq, hijoDer, j;

    do {
        hijoIzq = 2 * i + 1;  // Índice del hijo izquierdo
        hijoDer = 2 * i + 2;  // Índice del hijo derecho
        j = i;                // Índice original antes de buscar el mayor

        // Comparamos con el hijo derecho
        if (hijoDer <= m->ultimo && m->vector[hijoDer] < m->vector[i]) {
            i = hijoDer;
        }

        // Comparamos con el hijo izquierdo
        if (hijoIzq <= m->ultimo && m->vector[hijoIzq] < m->vector[i]) {
            i = hijoIzq;
        }

        // Intercambiamos los elementos si es necesario
        if (j != i) {
            intercambiar(m, j, i);
        }

    } while (j != i);  // Terminamos cuando el nodo alcanza su posición final
}

void crearMonticulo(pmonticulo m, int v[], int n) {
    for (int i = 0; i < n; i++) {
        m->vector[i] = v[i];
    }
    m->ultimo = n-1;

    //Construímos montículo desde el nodo hoja
    for (int i = m->ultimo/2; i >= 0; i--) {
        hundir(m,i);
    }

}

void iniMonticulo(pmonticulo *m) {
    (*m)->ultimo = -1;  // Inicializa el montículo vacío
}

// Inserta un elemento `x` en el montículo `m`, manteniendo la propiedad de mínimo.
void insertarMonticulo(pmonticulo m, int x) {

    // Verificamos si el montículo está lleno
    if (m->ultimo == TAM - 1) {
        printf("Montículo lleno\n");
        return;
    }

    // Incrementamos `ultimo` y colocamos el nuevo elemento al final
    m->ultimo++;
    m->vector[m->ultimo] = x;

    // Ajuste hacia arriba (flotar) para restaurar la propiedad del montículo
    flotar(m, m->ultimo);
}

// Elimina el elemento mínimo del montículo `m` (la raíz) y mantiene la propiedad de mínimo.
void quitarMenor(pmonticulo m) {
    int ultimo;

    // Verificamos si el montículo está vacío
    if (m->ultimo == -1) {
        printf("Montículo vacío\n");
        return;
    }

    // Guardamos el último elemento
    ultimo = m->vector[m->ultimo];

    // Reducimos el tamaño del montículo
    m->ultimo--;

    // Si el montículo tiene más de un elemento, realizamos el ajuste hacia abajo (hundir)
    if (m->ultimo >= 0) {
        m->vector[0] = ultimo;  // Colocamos el último elemento en la raíz

        hundir(m, 0);  // Llamamos a la función hundir para restaurar el montículo
    }

}

int consultarMenor(const pmonticulo m) {
    if (m->ultimo == -1) {
        printf("El montículo está vacío.\n");
        return -1;
    }
    // El menor elemento está siempre en la raíz (posición 0)
    return m->vector[0];
}

bool comprobarInicializacion(pmonticulo m) {
    return m->ultimo == -1;
}

//Verificar la inicialización del montículo
void test_iniMonticulo() {
    printf("\nTest de IniMonticulo\n\n");
    pmonticulo m = malloc(sizeof(struct monticulo));
    if (m == NULL) {
        fprintf(stderr, "Error al asignar memoria para el montículo\n");
        exit(EXIT_FAILURE);
    }
    printf("Última posición: %d\n", m->ultimo);
    printf("Inicializado? %d\n", comprobarInicializacion(m));
    printf("Inicializando...\n");
    iniMonticulo(&m);
    printf("Última posición: %d\n", m->ultimo);
    printf("Inicializado? %d\n", comprobarInicializacion(m));

    free(m);
}

void imprimirMonticulo(pmonticulo m) {
    if (m->ultimo == -1) {
        printf("El montículo está vacío\n");
        return;
    }

    printf("Contenido del montículo:\n");
    for (int i = 0; i <= m->ultimo; i++) {
        printf("%d ", m->vector[i]);
    }

    printf("\n");
}

bool esMonticuloMinimos(pmonticulo m) {
    for (int i = 0; i <= m->ultimo / 2; i++) {
        int hijoIzq = 2 * i + 1;
        int hijoDer = 2 * i + 2;

        // Verifica si el nodo actual es menor o igual que su hijo izquierdo (si existe)
        if (hijoIzq <= m->ultimo && m->vector[i] > m->vector[hijoIzq]) {
            return false;
        }

        // Verifica si el nodo actual es menor o igual que su hijo derecho (si existe)
        if (hijoDer <= m->ultimo && m->vector[i] > m->vector[hijoDer]) {
            return false;
        }
    }
    return true;
}

bool comprobarCreacion(pmonticulo m, int v[], int n) {
    int i, j;
    bool encontrado;
    if (m->ultimo < 0) {
        return false;
    }
    if (!esMonticuloMinimos(m)) {
        return false;
    }
    // Verifica que todos los elementos del array original están en el montículo
    for (i = 0; i < n; i++) {
        encontrado = false;

        // Busca el elemento `v[i]` en el montículo
        for (j = 0; j <= m->ultimo; j++) {
            if (m->vector[j] == v[i]) {
                encontrado = true;
                break; // Elemento encontrado, salir del bucle
            }
        }
        // Si un elemento del array original no está en el montículo, retorna false inmediatamente
        if (!encontrado) {
            return false;
        }
    }
    return true;
}

//Verificar la creación del montículo a partir de un array
void test_crearMonticulo() {
    printf("\nTest de crearMonticulo\n\n");
    pmonticulo m = malloc(sizeof(struct monticulo));
    int v[] = {20, 5, 15, 30, 10}, n = sizeof(v) / sizeof(v[0]);
    if (m == NULL) {
        fprintf(stderr, "Error al asignar memoria para el montículo\n");
        exit(EXIT_FAILURE);
    }
    crearMonticulo(m, v, n);

    printf("Crear monticulo con: \n");
    for (int i = 0; i < n; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
    printf("Creando...\n");
    imprimirMonticulo(m);
    printf("Creado correctamente? %d\n", comprobarCreacion(m, v, n));

    free(m);
}

bool comprobarInsercion(pmonticulo m, int insertado) {
    int i;
    if (m->ultimo < 0) {
        return false;
    }
    if (!esMonticuloMinimos(m)) {
        return false;
    }
    // Comprueba si el valor insertado está presente en el montículo
    for (i = 0; i <= m->ultimo; i++) {
        if (m->vector[i] == insertado) {
            return true; // Encontrado
        }
    }
    return false;
}


// Verificar la inserción de un elemento
void test_insertarMonticulo() {
    printf("\nTest de InsertarMonticulo\n\n");
    pmonticulo m = malloc(sizeof(struct monticulo));
    if (m == NULL) {
        fprintf(stderr, "Error al asignar memoria para el montículo\n");
        exit(EXIT_FAILURE);
    }
    iniMonticulo(&m);

    insertarMonticulo(m, 10);
    printf("Insertar 10 \n");
    printf("Insertando...\n");
    imprimirMonticulo(m);
    printf("Insertado correctamente? %d\n", comprobarInsercion(m, 10));

    insertarMonticulo(m, 5);
    printf("\nInsertar 5 \n");
    printf("Insertando...\n");
    imprimirMonticulo(m);
    printf("Insertado correctamente? %d\n", comprobarInsercion(m, 5));

    insertarMonticulo(m, 15);
    printf("\nInsertar 15 \n");
    printf("Insertando...\n");
    imprimirMonticulo(m);
    printf("Insertado correctamente? %d\n", comprobarInsercion(m, 15));

    free(m);
}

bool comprobarConsultarMenor(pmonticulo m, int menor) {

    // Verificar que el montículo no está vacío
    if (m->ultimo < 0) {
        return false;
    }

    // Verificar estado de montículo de mínimos para asegurarnos que el menor estará en la primera posición del vector
    if (!esMonticuloMinimos(m)) {
        return false;
    }

    // Verificar que el menor elemento corresponde al valor consultado
    if (m->vector[0] != menor) {
        return false;
    }

    return true;
}

//Verificar la consulta del menor
void test_consultarMenor() {
    printf("\nTest de ConsultarMenor\n\n");
    pmonticulo m = malloc(sizeof(struct monticulo));
    int v[] = {20, 5, 15, 30, 10}, n = sizeof(v) / sizeof(v[0]), menor;

    if (m == NULL) {
        fprintf(stderr, "Error al asignar memoria para el montículo\n");
        exit(EXIT_FAILURE);
    }
    iniMonticulo(&m);

    crearMonticulo(m, v, n);

    imprimirMonticulo(m);
    menor = consultarMenor(m);
    printf("Menor elemento: %d\n", menor);
    printf("Menor consultado correctamente? %d\n", comprobarConsultarMenor(m, menor));

    free(m);
}

bool comprobarQuitarMenor(pmonticulo m, int menorAnterior) {
    int menorActual = consultarMenor(m);

    // Verificar estado de montículo de mínimos para asegurarnos que el menor estará en la primera posición del vector
    if (!esMonticuloMinimos(m)) {
        return false;
    }

    // Verificar que el nuevo menor es mayor o igual al anterior
    if (menorActual < menorAnterior) { // Se espera que ahora el menor sea 10
        return false;
    }


    return true;
}


//Verificar la eliminación del menor
void test_quitarMenor() {
    printf("\nTest de QuitarMenor\n\n");
    pmonticulo m = malloc(sizeof(struct monticulo));
    int v[] = {20, 5, 15, 30, 10}, n = sizeof(v) / sizeof(v[0]), menorQuitado;

    if (m == NULL) {
        fprintf(stderr, "Error al asignar memoria para el montículo\n");
        exit(EXIT_FAILURE);
    }
    iniMonticulo(&m);

    crearMonticulo(m, v, n);

    imprimirMonticulo(m);
    menorQuitado = consultarMenor(m);
    printf("Quitando menor...\n");
    quitarMenor(m);
    imprimirMonticulo(m);
    printf("Menor quitado correctamente? %d\n\n", comprobarQuitarMenor(m, menorQuitado));

    imprimirMonticulo(m);
    menorQuitado = consultarMenor(m);
    printf("Quitando menor...\n");
    quitarMenor(m);
    imprimirMonticulo(m);
    printf("Menor quitado correctamente? %d\n", comprobarQuitarMenor(m, menorQuitado));

    free(m);
}


void testOperaciones() {
    test_iniMonticulo();
    test_crearMonticulo();
    test_insertarMonticulo();
    test_consultarMenor();
    test_quitarMenor();
}

// Ordena un array usando el algoritmo de montículos
void ordenarPorMonticulos(int v[], int n) {
    pmonticulo m = malloc(sizeof(struct monticulo));
    iniMonticulo(&m);           // Inicializar el montículo
    crearMonticulo(m, v, n);    // Construir el montículo con los elementos del array

    for (int i = 0; i < n; i++) { // Iterar sobre todos los elementos del array
        v[i] = consultarMenor(m); // Asignar el menor elemento del montículo al array
        quitarMenor(m);           // Eliminar el menor elemento del montículo
    }

    free(m);
}

void inicializar_semilla() {
    srand(time(NULL));
}

void aleatorio(int v[], int n) {/* se generan números pseudoaleatorio entre -n y +n */
    int i, m=2*n+1;
    for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
}
void ascendente(int v [], int n) {
    int i;
    for (i=0; i < n; i++)
        v[i] = i;
}

void descendente(int v [], int n) {
    int i;
    for (i=0; i < n; i++)
        v[i] = n-i;
}

// Imprime un array
void imprimirArray(const int v[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
}

// Verifica si un array está ordenado
int comprobarOrdenacion(const int v[], int n) {
    int i;
    for (i = 0; i < n - 1; i++) {
        if (v[i] > v[i + 1]) {
            return 0;
        }
    }
    return 1;
}

// Función para realizar la ordenación y mostrar resultados
static void ordenacion(void (*inicializacion)(int[], int), int n, int v[n]) {
    inicializacion(v, n);
    imprimirArray(v, n);
    printf("Ordenado? %d\n", comprobarOrdenacion(v, n));
    printf("Ordenando...\n");
    ordenarPorMonticulos(v, n);
    imprimirArray(v, n);
    printf("Ordenado? %d\n", comprobarOrdenacion(v, n));
}

static void imprimir_test(int n, int v[n], void (*inicializacion)(int[], int), char nombre_inicializacion[]) {
    printf("\nOrdenacion por monticulos con inicializacion %s\n\n", nombre_inicializacion);
    ordenacion(inicializacion, n, v);
}

void testOrdenarPorMonticulos() {
    int n = 10;
    int v[n];
    imprimir_test(n, v, aleatorio,"aleatoria");
    imprimir_test(n, v, descendente ,"descendente");
    imprimir_test(n, v, ascendente ,"ascendente");

}


// Mide el tiempo en microsegundos
double microsegundos() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0)
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

//Mide el tiempo de ejecución
double medirTiempoInsertar(void (*inicialización)(int*,int), const int n, bool *promedio) {
    double t1, t2, t, tTotal,tInicializacion;
    int i, j, k = 1000, v[n];
    pmonticulo m = malloc(sizeof(struct monticulo));
    if (m == NULL) {
        fprintf(stderr, "Error al asignar memoria para el montículo\n");
        exit(EXIT_FAILURE);
    }

    iniMonticulo(&m);
    inicialización(v,n);
    t1 = microsegundos();
    for (i = 0; i < n; i++) {
        insertarMonticulo(m,v[i]);
    }
    t2 = microsegundos();
    t = t2 - t1;

    //Si el tiempo es menor que 500 microsegundos
    if (t < 500) {
        t1 = microsegundos();           // Mide el tiempo total (inicialización + ejecución)
        for (i = 0; i < k; i++) {
            iniMonticulo(&m);
            inicialización(v,n);
            for (j = 0; j < n; j++) {
                insertarMonticulo(m,v[j]);
            }
        }
        t2 = microsegundos();
        tTotal = t2 - t1;

        t1 = microsegundos();           // Mide solo el tiempo de la inicialización
        for (i = 0; i < k; i++) {
            iniMonticulo(&m);
            inicialización(v,n);
        }
        t2 = microsegundos();
        tInicializacion = t2 - t1;
        t = (tTotal - tInicializacion) / k;
        *promedio = true; //Indica que se realizó un promedio
    }
    free(m);
    return t;
}


//Mide el tiempo de ejecución
double medirTiempoCrear(void (*inicialización)(int*,int) , const int n, bool *promedio) {
    double t1, t2, t, tTotal,tInicializacion;
    int i, k = 1000, v[n];
    pmonticulo m = malloc(sizeof(struct monticulo));
    if (m == NULL) {
        fprintf(stderr, "Error al asignar memoria para el montículo\n");
        exit(EXIT_FAILURE);
    }
    iniMonticulo(&m);

    inicialización(v,n);
    t1 = microsegundos();
    crearMonticulo(m,v, n);
    t2 = microsegundos();
    t = t2 - t1;

    //Si el tiempo es menor que 500 microsegundos
    if (t < 500) {
        t1 = microsegundos();           // Mide el tiempo total (inicialización + ejecución)
        for (i = 0; i < k; i++) {
            inicialización(v,n);
            crearMonticulo(m,v, n);
        }
        t2 = microsegundos();
        tTotal = t2 - t1;

        t1 = microsegundos();           // Mide solo el tiempo de la inicialización
        for (i = 0; i < k; i++) {
            inicialización(v,n);
        }
        t2 = microsegundos();
        tInicializacion = t2 - t1;
        t = (tTotal - tInicializacion) / k;
        *promedio = true; //Indica que se realizó un promedio
    }
    free(m);
    return t;
}

//Mide el tiempo de ejecución
double medirTiempoOrdenacionPorMonticulas(void (*inicializacion)(int*, int), int n, bool *promedio) {
    double t1, t2, t, tTotal, tInicializacion;
    int j, k = 1000, v[n];

    // Inicializar con la función de inicialización
    inicializacion(v, n);

    // Medir tiempo de ejecución del algoritmo
    t1 = microsegundos();
    ordenarPorMonticulos(v, n);
    t2 = microsegundos();
    t = t2 - t1;

    // Si el tiempo es menor que 500 microsegundos
    if (t < 500) {
        t1 = microsegundos();               // Mide el tiempo total (inicialización + ejecución)
        for (j = 0; j < k; j++) {
            inicializacion(v, n);
            ordenarPorMonticulos(v, n);
        }
        t2 = microsegundos();
        tTotal = t2 - t1;

        t1 = microsegundos();               // Mide solo el tiempo de la inicialización
        for (j = 0; j < k; j++) {
            inicializacion(v,n);
        }
        t2 = microsegundos();

        tInicializacion = t2 - t1;
        t = (tTotal - tInicializacion) / k;
        *promedio = true; //Indica que se realizó un promedio
    }
    return t;
}

// Función para imprimir el encabezado de las tablas
void imprimirEncabezado(const char *nombreTabla, Cotas *cotas,int tipo) {
    printf("\n%s\n", nombreTabla);
    if (tipo == 1) {
        // Imprimir los encabezados de la tabla directamente
        printf("\n%13s%16s%17s%s%18s%s%19s%s\n",
               "n",
               "t(n)",
               "t(n)/" ,cotas->strCotaSubestimada,
               "t(n)/" ,cotas->strCotaAjustada,
               "t(n)/" ,cotas->strCotaSobreestimada);
    }else {
        // Imprimir los encabezados de la tabla directamente
        printf("\n%13s%16s%13s%s%11s%s%12s%s\n",
               "n",
               "t(n)",
               "t(n)/" ,cotas->strCotaSubestimada,
               "t(n)/" ,cotas->strCotaAjustada,
               "t(n)/" ,cotas->strCotaSobreestimada);
    }

}


// Función para imprimir una fila de la tabla
void imprimirFila(int n, double t, double x, double y, double z, bool promedio) {
    if (promedio) {
        printf("*");    //Marca si se hizo un promedio
    } else {
        printf(" ");    //Marca si no se hizo un promedio, para mantener la alineación
    }
    printf("%12d%17.4f%20.6f%25.7f%25.7f\n", n, t, x, y, z);
}

//Devuelve la cota subestimada para el algoritmo correspondiente
double cotaSubestimada(int n, int tipo, Cotas *cotas) {
    switch (tipo) {
        case 1:
            return n * pow(log2(n), cotas->cotaSubestimada);
        case 2:
            return pow(n,cotas->cotaSubestimada);
        default:
            return 1.0; //Valor por defecto
    }
}

//Devuelve la cota ajustada para el algoritmo correspondiente
double cotaAjustada(int n, int tipo, Cotas *cotas) {
    switch (tipo) {
        case 1:
            return n * pow(log2(n), cotas->cotaAjustada);
        case 2:
            return pow(n,cotas->cotaAjustada);
        default:
            return 1.0; //Valor por defecto
    }
}

double cotaSobreestimada(int n, int tipo, Cotas *cotas) {
    switch (tipo) {
        case 1:
            return n * pow(log2(n), cotas->cotaSobreestimada);
        case 2:
            return pow(n,cotas->cotaSobreestimada);
        default:
            return 1.0; //Valor por defecto
    }
}

//Imprime los tiempos de ejecución de la función dada
void imprimirTiempos(double(*funcTiempos)(void (*inicialización)(int*,int), int, bool*),
                    void (*inicialización)(int*,int), Cotas *cotas,int tipo) {
    int i;
    double t, x, y, z;
    bool promedio;

    for (i = 1000; i <= 64000; i*= 2) {
        promedio = false;           //Indica si se calcula el promedio
        t = funcTiempos(inicialización, i, &promedio);
        x = t / cotaSubestimada(i, tipo, cotas);
        y = t / cotaAjustada(i, tipo, cotas);
        z = t / cotaSobreestimada(i, tipo, cotas);

        imprimirFila(i,t,x,y,z,promedio);
    }
}

// Imprime las tablas de tiempos para los diferentes algoritmos de ordenación
void imprimirTablas() {
    Cotas cotas;

    //Inserción de n elementos
    cotas = (Cotas){0.687, 0.987, 1.387, "n*log(n)^0.687",
        "n*log(n)^0.987", "n*log(n)^1.387"};
    imprimirEncabezado("Inserción de n elementos", &cotas,2);
    imprimirTiempos(medirTiempoInsertar,descendente, &cotas,1);

    //Crear montículo
    cotas = (Cotas){0.78, 0.99, 1.18, "n^0.787",
        "n^0.987", "n^1.187"};
    imprimirEncabezado("Creación de montículo", &cotas,1);
    imprimirTiempos(medirTiempoCrear, descendente, &cotas,2);

    //Ordenación por montículos
    //Caso ascendente
    cotas = (Cotas){0.587, 0.987, 1.487, "n*log(n)^0.687",
        "n*log(n)^0.987", "n*log(n)^1.387"};
    imprimirEncabezado("Ordenación por montículos con inicialización ascendente", &cotas,2);
    imprimirTiempos(medirTiempoOrdenacionPorMonticulas, ascendente, &cotas,1);

    //Caso descendente
    cotas = (Cotas){0.587, 0.987, 1.487, "n*log(n)^0.687",
        "n*log(n)^0.987", "n*log(n)^1.387"};
    imprimirEncabezado("Ordenación por montículos con inicialización descendente", &cotas,2);
    imprimirTiempos(medirTiempoOrdenacionPorMonticulas, descendente, &cotas,1);

    //Caso aleatorio
    cotas = (Cotas){0.587, 0.987, 1.487, "n*log(n)^0.687",
        "n*log(n)^0.987", "n*log(n)^1.387"};
    imprimirEncabezado("Ordenación por montículos con inicialización aleatoria", &cotas,2);
    imprimirTiempos(medirTiempoOrdenacionPorMonticulas, aleatorio, &cotas,1);
}

