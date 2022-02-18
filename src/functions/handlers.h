#ifndef HANDLERS_H
#define HANDLERS_H

#include <stdio.h>
#include <stdlib.h>
#include "../types/constants.h"
#include "./utils.h"


int dateSize(int dia, int mes, int ano, int horas) {
  int i;
  int s;
  s = (ano * 365 * 24) + (6 * ano);

  for (i = 2; i <= mes; i++) {
    if (i == 2 || i == 4 || i == 6 || i == 8 || i == 9 || i == 11) {
      s += (31 * 24);
    } else if (i == 5 || i == 7 || i == 10 || i == 12) {
      s += (30 * 24);
    } else if (i == 3) {
      if (ano % 4 == 0)
        s += (29 * 24);
      else
        s += (28 * 24);
    }
  }

  s += ((dia - 1) * 24) + horas;
  return s;
} // Calcula um valor numerico correspondente a uma data. Valor maior = mais recente!

void swapEvent(Evento *e1, Evento *e2) {
  Evento aux;
  aux = *e1;
  *e1 = *e2;
  *e2 = aux;
}

void sortEvents(Evento *eventos, int qtdEventos) {
  int i, j;
  for (i = 0; i < qtdEventos; i++) {
    for (j = i + 1; j < qtdEventos; j++) {
      int di = eventos[i].data.dia;
      int mi = eventos[i].data.mes;
      int ai = eventos[i].data.ano;
      int hi = eventos[i].horaInicio.hora;
      int dj = eventos[j].data.dia;
      int mj = eventos[j].data.mes;
      int aj = eventos[j].data.ano;
      int hj = eventos[j].horaInicio.hora;

      if (dateSize(dj, mj, aj, hj) < dateSize(di, mi, ai, hi)) {
        swapEvent(&eventos[i], &eventos[j]);
      }

    }
  }
}

void printSingleEvent(Evento e, int i) {
  printf("EVENTO %d\n\n", i + 1);
  printf("Data: %02d/%02d/%02d\n", e.data.dia, e.data.mes, e.data.ano);
  printf("Hora do inicio: %02d:%02d\n", e.horaInicio.hora, e.horaInicio.minuto);
  printf("Hora do fim: %02d:%02d\n", e.horaFim.hora, e.horaFim.minuto);
  printf("Local: %s\n", e.local);
  printf("Descricao: %s\n\n", e.descricao);
}

int validateDate(int day, int month, int year) {
  int maxDay;

  if (month < 1 || month > 12 || year < 0 || day < 1)
    return 0;

  if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) {
    maxDay = 31;
  } else if (month == 4 || month == 6 || month == 9 || month == 11) {
    maxDay = 30;
  } else if (month == 2) {
    if (year % 4 == 0) {
      maxDay = 29;
    } else {
      maxDay = 28;
    }
  }

  if (day > maxDay)
    return 0;

  return 1;
}

int validateTime(int hora, int minuto) {
  if (hora < 0 || hora > 23 || minuto < 0 || minuto > 59)
    return 0;
  return 1;
}

int eventExists(Evento **evs, int qtd, int dia, int mes, int ano, int hora) {
  int i;

  for (i = 0; i < qtd; i++) {
    int d = (*evs)[i].data.dia;
    int m = (*evs)[i].data.mes;
    int a = (*evs)[i].data.ano;
    int h = (*evs)[i].horaInicio.hora;

    if (d == dia && m == mes && a == ano && h == hora) {
      return 1;
    }
  }

  return 0;
}


#endif