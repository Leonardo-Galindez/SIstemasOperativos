#include <xinu.h>
#include <stdio.h>
void esBisiesto();
void esPrimo();
int pid1;
int ejercicio2()
{
    int i;
    int pid2;
    int pid3;
    int mensaje;

    char buf[10];
    printf("INGRESE UN NRO: ");
    read(CONSOLE, buf, 10);
    i = atoi(buf);
    printf("el numero ingresado entero es %d\n", i);

    pid1 = getpid();

    pid2 = create(esPrimo, 1024, 20, "proceso 2", 0);
    resume(pid2);
    send(pid2, i);
    mensaje = receive();
    if (mensaje == 1)
    {
        printf("es Primo\n");
    }
    else
    {
        if (mensaje == 0)
        {
            printf("No es Primo\n");
        }
    }
    pid3 = create(esBisiesto, 1024, 20, "proceso 3", 0);
    resume(pid3);
    send(pid3, i);
    mensaje = receive();

    if (mensaje == 3)
    {
        printf("es Bisiesto\n");
    }
    else
    {
        if (mensaje == 2)
        {
            printf("No es Bisiesto\n");
        }
    }
    return 0;
}

void esBisiesto()
{

    int anio = receive();

    int rta = 0;
    if (anio % 4 == 0)
    {
        if (anio % 100 != 0)
        {
            rta = 1;
        }
        else
        {
            if (anio % 400 == 0)
            {
                rta = 1;
            }
        }
    }
    if (rta)
    {
        send(pid1, 3);
        sleepms(1000);
    }
    else
    {
        send(pid1, 2);
        sleepms(1000);
    }
}

void esPrimo()
{
    int j = receive();
    int n, primo;
    primo = 1;
    n = 2;

    while (n <= j / 2 && primo)
    {
        if (j % n == 0)
            primo = 0;

        n++;
    }
    if (primo)
    {
        send(pid1, 1);
        sleepms(1000);
    }
    else
    {
        send(pid1, 0);
        sleepms(1000);
    }
}