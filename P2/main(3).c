#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

// Definición de la estructura para las cotas
typedef struct {
    double cotaSubestimada;
    double cotaAjustada;
    double cotaSobreestimada;
    const char *strCotaSubestimada;
    const char *strCotaAjustada;
    const char *strCotaSobreestimada;
} Cotas;

//Cabeceras de las funciones
void test();
double microsegundos();
void imprimirTiempos(void (*algoritmo)(int[],int), void (*inicialización)(int*,int) ,Cotas *cotas,int tipo);
void imprimirTablas();
double medirTiempo(void (*func)(int*,int),void (*inicialización)(int*,int) , const int n, bool *promedio);
void inicializar_semilla();

void aleatorio(int v[], int n);
void ascendente(int v [], int n);
void descendente(int v [], int n);
void ord_ins(int v[], int n);
void ord_rap (int v [], int n);
int esta_ordenado(int v[], int n);
void imprimir_array(int v[], int n);


//Realiza las pruebas de los algoritmos Fibonacci y muestra las tablas de tiempos
int main(void) {
    inicializar_semilla();
    test();
    imprimirTablas();
    return 0;
}


void ord_ins(int v[], int n) {
    int x, j, i;
    for (i = 1; i < n; i++) {
        x = v[i];
        j = i - 1;
        while (j >= 0 && v[j] > x) {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = x;
    }
}


void intercambiar(int *a, int *b) {
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

void ord_rap_aux (int v [], int iz, int dr) {
    int i,j,x, pivote;
    if (iz < dr) {
        x = (rand() % (dr - iz + 1)) + iz;;
        pivote = v[x];
        intercambiar(&v[iz], &v[x]);
        i = iz + 1;
        j = dr;

        while (i <= j) {
            while (i <= dr && v[i] < pivote) i++;
            while (v[j] > pivote) j--;

            if (i <= j) {
                intercambiar(&v[i], &v[j]);
                i++;
                j--;
            }
        }
        intercambiar(&v[iz], &v[j]);
        ord_rap_aux (v, iz, j-1);
        ord_rap_aux (v, j+1, dr);
    }
}

void ord_rap (int v [], int n) {
    ord_rap_aux(v, 0, n-1);
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

// Verifica si el array esta ordenado
int esta_ordenado(int v[], int n) {
    int i;
    for (i = 0; i < n - 1; i++) {
        if (v[i] > v[i + 1]) {
            return 0;
        }
    }
    return 1;
}

void imprimir_array(int v[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i == n - 1) {
            printf("%d", v[i]);  // Último elemento sin coma
        } else {
            printf("%d, ", v[i]);  // Elementos anteriores con coma
        }
    }
    printf("\n");
}


void ordenacion(void (*inicialización)(int*,int),void (*algoritmo) (int*,int),int n, int v[n]) {
    inicialización(v, n);
    imprimir_array(v, n);
    printf("ordenado? %d\n", esta_ordenado(v, n));
    printf("ordenando...\n");
    algoritmo(v, n);
    imprimir_array(v, n);
    printf("ordenado? %d\n\n", esta_ordenado(v, n));
}


void imprimir_test(int n, int v[n],void (*algoritmo) (int*,int), char nombre_algoritmo[]) {
    printf("Ordenacion %s con inicializacion aleatoria\n",nombre_algoritmo);
    ordenacion(aleatorio,algoritmo,n, v);
    printf("Ordenacion %s con inicializacion descendente\n",nombre_algoritmo);
    ordenacion(descendente,algoritmo,n,v);
    printf("Ordenacion %s con inicializacion ascendente\n",nombre_algoritmo);
    ordenacion(ascendente,algoritmo,n,v);
}

// Función de prueba que realiza las ordenaciones
void test() {
    inicializar_semilla();
    int n = 17;
    int v[n];
    imprimir_test(n, v, ord_ins, "por inserción");
    imprimir_test(n, v, ord_rap ,"rapida auxiliar");
}

//Obtiene la hora actual en microsegundos
double microsegundos() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0)
        return 0.0; //Maneja el error en la obtención del tiempo
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

//Mide el tiempo de ejecución
double medirTiempo(void (*func)(int*,int),void (*inicialización)(int*,int) , const int n, bool *promedio) {
    double t1, t2, t, tTotal,tInicializacion;
    int j, k = 1000, v[32000];

    inicialización(v,n);
    t1 = microsegundos();
    func(v,n);
    t2 = microsegundos();
    t = t2 - t1;

    //Si el tiempo es menor que 500 microsegundos
    if (t < 500) {
        t1 = microsegundos();           // Mide el tiempo total (inicialización + ejecución)
        for (j = 0; j < k; j++) {
            inicialización(v,n);
            func(v,n);
        }
        t2 = microsegundos();
        tTotal = t2 - t1;

        t1 = microsegundos();           // Mide solo el tiempo de la inicialización
        for (j = 0; j < k; j++) {
            inicialización(v,n);
        }
        t2 = microsegundos();
        tInicializacion = t2 - t1;
        t = (tTotal - tInicializacion) / k;
        *promedio = true; //Indica que se realizó un promedio
    }
    return t;
}


// Función para imprimir el encabezado de las tablas
void imprimirEncabezado(const char *nombreAlgoritmo, const char *initType, Cotas *cotas,int tipo) {
    printf("\nOrdenación %s con inicialización %s\n", nombreAlgoritmo, initType);
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
        case 1: //Para fib1
            return pow(n,cotas->cotaSubestimada);
        case 2: //Para fib2
            return pow(n,cotas->cotaSubestimada) * log2(n);
        default:
            return 1.0; //Valor por defecto
    }
}

//Devuelve la cota ajustada para el algoritmo correspondiente
double cotaAjustada(int n, int tipo, Cotas *cotas) {
    switch (tipo) {
        case 1: // Para fib1
            return pow(n,cotas->cotaAjustada);
        case 2: // Para fib2
            return pow(n,cotas->cotaAjustada) * log2(n);
        default:
            return 1.0; //Valor por defecto
    }
}

double cotaSobreestimada(int n, int tipo, Cotas *cotas) {
    switch (tipo) {
        case 1:
            return pow(n,cotas->cotaSobreestimada);
        case 2:
            return pow(n,cotas->cotaSobreestimada) * log2(n);
        default:
            return 1.0; //Valor por defecto
    }
}

//Imprime los tiempos de ejecución de la función dada
void imprimirTiempos(void (*algoritmo)(int[],int), void (*inicialización)(int*,int) ,Cotas *cotas,int tipo) {
    int i;
    double t, x, y, z;
    bool promedio;

    for (i = 500; i <= 32000; i*= 2) {
        promedio = false;   //Indica si se calcula el promedio
        t = medirTiempo(algoritmo,inicialización, i, &promedio);
        x = t / cotaSubestimada(i, tipo, cotas);
        y = t / cotaAjustada(i, tipo, cotas);
        z = t / cotaSobreestimada(i, tipo, cotas);

        imprimirFila(i,t,x,y,z,promedio);
    }
}

// Imprime las tablas de tiempos para los diferentes algoritmos de ordenación
void imprimirTablas() {
    Cotas cotas;

    // Ordenación por Inserción con inicialización ascendente
    cotas = (Cotas){0.78, 0.98, 1.18, "n^0.78",
        "n^0.98", "n^1.18"};
    imprimirEncabezado("por inserción", "ascendente", &cotas,1);
    imprimirTiempos(ord_ins, ascendente, &cotas,1);

    // Ordenación por Inserción con inicialización descendente
    cotas = (Cotas){1.8, 2.0, 2.2, "n^1.8 ",
        "n^2.0 ", "n^2.2"};
    imprimirEncabezado("por inserción", "descendente", &cotas,1);
    imprimirTiempos(ord_ins, descendente, &cotas,1);

    // Ordenación por Inserción con inicialización aleatoria
    cotas = (Cotas){1.79, 1.99, 2.19, "n^1.79",
        "n^1.99", "n^2.19"};
    imprimirEncabezado("por inserción", "aleatoria", &cotas,1);
    imprimirTiempos(ord_ins, aleatorio, &cotas,1);

    // Ordenación rápida con inicialización ascendente
    cotas = (Cotas){0.759, 0.959, 1.259, "n^0.759*log(n)",
        "n^0.959*log(n)", "n^1.259*log(n)"};
    imprimirEncabezado("rápida", "ascendente", &cotas,2);
    imprimirTiempos(ord_rap, ascendente, &cotas,2);

    // Ordenación rápida con inicialización descendente
    cotas = (Cotas){0.756, 0.956, 1.256, "n^0.756*log(n)",
        "n^0.956*log(n)", "n^1.256*log(n)"};
    imprimirEncabezado("rápida", "descendente", &cotas,2);
    imprimirTiempos(ord_rap, descendente, &cotas,2);

    // Ordenación rápida con inicialización aleatoria
    cotas = (Cotas){0.78, 0.98, 1.18, "n^0.78*log(n) ",
        "n^0.98*log(n) ", "n^1.18*log(n)"};
    imprimirEncabezado("rápida", "aleatoria", &cotas,2);
    imprimirTiempos(ord_rap, aleatorio, &cotas,2);
}

