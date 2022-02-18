#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int hora;
  int minuto;
} Horario;

typedef struct {
  int dia;
  int mes;
  int ano;
} Data;

typedef struct {
  Data data;
  Horario horaInicio;
  Horario horaFim;
  char local[50];
  char descricao[100];
} Evento;

#endif