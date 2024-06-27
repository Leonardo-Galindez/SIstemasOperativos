#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define BMP_FILE "desordenado.txt"
//*direcciones de memoria que apunta a una direccion donde hay un entero
int buffer[1000];
int nuevo_buffer[1000];

void mostrarArray()
{
    for (int i = 0; i < 1000; i++)
    {
        int n = nuevo_buffer[i];
        printf("%i nro:%i\n", i, n);
    }
}

/*void ordenar(int inicio, int fin)
{
    int ordenado = 0;
    int binario;
    int j = 0;
    for (int i = inicio; i < fin; i++)
    {
        binario = buffer[i];
        while (!ordenado && j < fin)
        {
            if (binario > buffer[j++])
            {
                int aux = binario;
                buffer[j] = buffer[j++];
                buffer[j++] = aux;
                j++;
            }
            else
            {
                j = i;
                ordenado = 1;
            }
        }
    }
}*/

void ordenar(int inicio, int fin)
{
    for (int i = inicio; i < fin - 1; i++)
    {
        for (int j = i + 1; j < fin; j++)
        {
            if (buffer[i] > buffer[j])
            {
                int aux = buffer[i];
                buffer[i] = buffer[j];
                buffer[j] = aux;
            }
        }
    }
}

/*void ordenarFinal(int inicio, int fin, int inicio2, int fin2)
{
    int ordenado = 0;
    int binario;
    int j = inicio2 - 1;
    for (int i = inicio; i < fin; i++)
    {
        if (buffer[i] < buffer[inicio2])
        {
            nuevo_buffer[i] = buffer[i];
            i++;
        }
        else
        {
            nuevo_buffer[i] = buffer[inicio2];
            inicio2++;
        }
    }
}*/

void ordenarFinal(int inicio1, int fin1, int inicio2, int fin2)
{
    int i = inicio1;
    int j = inicio2;
    int k = 0; // Índice para el nuevo buffer

    // Número total de elementos a combinar
    int total = (fin1 - inicio1) + (fin2 - inicio2);

    for (k = 0; k < total; k++)
    {
        if (i < fin1 && (j >= fin2 || buffer[i] < buffer[j]))
        {
            nuevo_buffer[k] = buffer[i++];
        }
        else
        {
            nuevo_buffer[k] = buffer[j++];
        }
    }
}

void *funcion(void *arg)
{
    int thread_id = *((int *)arg);
    int inicio;
    int fin;
    if (thread_id == 1)
    {
        inicio = 0;
        fin = 500;
    }
    else
    {
        inicio = 500;
        fin = 1000;
    }
    ordenar(inicio, fin);
}

int main()
{
    int n;
    int in_fd = open(BMP_FILE, O_RDONLY);
    if (in_fd < 0)
    {
        printf("error al abrir");
    }
    read(in_fd, buffer, 1000 * sizeof(n)); // cada posicion tiene el tamaño de un entero consulta como funciona esto
//buffer grande,
//sizeof(buffer), cargo n
    pthread_t threads[2];
    int thread_ids[2] = {0, 1};

    // Crear hilos
    for (int i = 0; i < 2; i++)
    {
        pthread_create(&threads[i], NULL, funcion, &thread_ids[i]);
    }

    // Esperar a que los hilos terminen
    for (int i = 0; i < 2; i++)
    {
        pthread_join(threads[i], NULL);
    }

    ordenarFinal(0, 500, 500, 1000);
    mostrarArray();
}