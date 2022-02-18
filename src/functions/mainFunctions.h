#ifndef MAIN_FUNCTIONS_H
#define MAIN_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "./utils.h"
#include "./handlers.h"
#include "../types/constants.h"

int showMenu() {
  int op;

  clearScreen();

  printf("Bem-vindo ao registro de eventos!\n\n");
  printf("[1] Cadastrar um evento;\n");
  printf("[2] Mostrar todos os eventos registrados;\n");
  printf("[3] Mostrar todos os eventos de uma data especifica;\n");
  printf("[4] Mostrar os 5 proximos eventos a partir de hoje;\n");
  printf("[5] Remover um evento;\n");
  printf("[0] Sair do programa.\n\n");
  printf("Digite sua opcao: ");
  scanf("%d", &op);

  return op;
}

void createEvent(int *qtdEventos, Evento **evs, FILE *lista) {
    clearScreen();
    clearBuffer();

    int dia, mes, ano;
    int horaInicio, minutoInicio;
    int horaFim, minutoFim;

    printf("DATA\n\n");

    printf("Dia: ");
    scanf("%d", &dia);
    printf("Mes: ");
    scanf("%d", &mes);
    printf("Ano: ");
    scanf("%d", &ano);

    if (!validateDate(dia, mes, ano)) {
      clearScreen();
      clearBuffer();
      printf("A data fornecida e invalida!\n");
      systemPause();
      return;
    }

    printf("\nHORA DE INICIO\n\n");

    printf("Hora: ");
    scanf("%d", &horaInicio);
    printf("Minutos: ");
    scanf("%d", &minutoInicio);

    if (eventExists(evs, (*qtdEventos), dia, mes, ano, horaInicio)) {
      clearScreen();
      clearBuffer();
      printf("Ja existe outro evento cadastrado na mesma data e hora!\n");
      systemPause();
      return;
    }

    if (!validateTime(horaInicio, minutoInicio)) {
      clearScreen();
      clearBuffer();
      printf("O horario fornecido e invalido!\n");
      systemPause();
      return;
    }

    printf("\nHORA FINAL\n\n");
    
    printf("Hora: ");
    scanf("%d", &horaFim);
    printf("Minutos: ");
    scanf("%d", &minutoFim);
    clearBuffer();

    if (!validateTime(horaFim, minutoFim)) {
      clearScreen();
      printf("O horario fornecido e invalido!\n");
      systemPause();
      return;
    }

    *qtdEventos = *qtdEventos + 1;
    *evs = realloc(*evs, (*qtdEventos) * sizeof(Evento));
    Evento e;

    e.data.dia = dia;
    e.data.mes = mes;
    e.data.ano = ano;
    e.horaInicio.hora = horaInicio;
    e.horaInicio.minuto = minutoInicio;
    e.horaFim.hora = horaFim;
    e.horaFim.minuto = minutoFim;
   
    printf("\nLOCAL\n\n");

    printf("Digite o local do evento: ");
    inputString(e.local, 50);

    printf("\nDESCRICAO\n\n");

    printf("Digite a descricao do evento: ");
    inputString(e.descricao, 100);

    (*evs)[(*qtdEventos) - 1] = e;
    
    sortEvents(*evs, (*qtdEventos));
    
    fwrite(&e, sizeof(Evento), 1, lista);

    clearScreen();
    printf("Evento registrado com sucesso!\n");

    systemPause();
}

void printEvents(int qtdEventos, Evento *eventos) {
  int j;

  if (qtdEventos == 0) {
    printf("Nao ha eventos para mostrar!\n");
    clearBuffer();
    systemPause();
    return;
  }

  for (j = 0; j < qtdEventos; j++) {
    printSingleEvent(eventos[j], j);
  }

  clearBuffer();
  systemPause();
}

void printAllEvents(int qtd, Evento *eventos) {
  clearScreen();
  printf("~~ Lista de todos os eventos ~~\n\n");
  printEvents(qtd, eventos);
}

void printEventsByDate(int qtdEventos, Evento *eventos) {
  int i, dia, mes, ano, qtd = 0;
  Evento *dataLista = malloc(qtd * sizeof(Evento));

  clearScreen();
  printf("Encontre eventos de acordo com a data!\n");

  printf("Dia: ");
  scanf("%d", &dia);
  printf("Mes: ");
  scanf("%d", &mes);
  printf("Ano: ");
  scanf("%d", &ano);

  if (!validateDate(dia, mes, ano)) {
    clearScreen();
    clearBuffer();
    printf("A data digitada e invalida!\n");
    systemPause();
    free(dataLista);
    return;
  }

  for (i = 0; i < qtdEventos; i++) {
    if ((eventos[i].data.dia == dia) && (eventos[i].data.mes == mes) && (eventos[i].data.ano == ano)) {
      qtd++;
      dataLista = realloc(dataLista, qtd * sizeof(Evento));
      dataLista[qtd - 1] = eventos[i];
    }
  }

  if (qtd == 0) {
    clearScreen();
    clearBuffer();
    printf("Nao ha eventos em %02d/%02d/%d.\n", dia, mes, ano);
    systemPause();
    free(dataLista);
    return;
  }

  clearScreen();
  printf("~~ Lista de eventos da data %02d/%02d/%d ~~\n\n", dia, mes, ano);
  printEvents(qtd, dataLista);

  free(dataLista);
}

void printFiveNextEvents(int qtd, Evento *eventos) {
  struct tm *data;
  time_t s;
  time(&s);

  data = localtime(&s);
  
  int seg = data->tm_sec;
  int min = data->tm_min;
  int hora = data->tm_hour;
  int dia = data->tm_mday;
  int mes = data->tm_mon + 1;
  int ano = data->tm_year + 1900;

  clearScreen();
  printf("~~ Eventos a partir da data de hoje: %02d/%02d/%d - %dh ~~\n\n", dia, mes, ano, hora);

  int i, j, rest = 0;
  Evento *fiveEvents = malloc(0);

  for (i = 0; i < qtd; i++) {
    int d1 = dateSize(eventos[i].data.dia, eventos[i].data.mes, eventos[i].data.ano, eventos[i].horaInicio.hora) - eventos[i].horaInicio.hora;
    int dToday = dateSize(dia, mes, ano, hora) - hora; // Hoje: 17726004!
    if (d1 >= dToday) {

      rest = qtd - 1 - i;

      if (rest > 5)
        rest = 5;

      fiveEvents = realloc(fiveEvents, rest * sizeof(Evento));

      for (j = 0; j < rest; j++)
        fiveEvents[j] = eventos[i + j];

      break;
    }
  }

  if (rest == 0) {
    printf("Nao ha eventos depois da data de hoje (%02d/%02d/%d).\n", dia, mes, ano);
    free(fiveEvents);
    clearBuffer();
    systemPause();
    return;
  }

  printEvents(rest, fiveEvents);

  free(fiveEvents);
}

void removeEvent(int *qtd, Evento **eventos) {
  int i = 0, j, dia, mes, ano, hora, op;
  Evento e;

  clearScreen();
  clearBuffer();

  printf("Remova um evento de acordo com a data!\n\n");

  printf("Dia: ");
  scanf("%d", &dia);
  printf("Mes: ");
  scanf("%d", &mes);
  printf("Ano: ");
  scanf("%d", &ano);
  printf("Horario de inicio: ");
  scanf("%d", &hora);

  if (!eventExists(eventos, (*qtd), dia, mes, ano, hora)) {
    clearScreen();
    clearBuffer();
    printf("Nao ha nenhum evento correspondente aos dados inseridos!\n");
    systemPause();
    return;
  }

  while (i < (*qtd)) {
    int d = (*eventos)[i].data.dia;
    int m = (*eventos)[i].data.mes;
    int a = (*eventos)[i].data.ano;
    int h = (*eventos)[i].horaInicio.hora;

    if (d == dia && m == mes && a == ano && h == hora) {
      e = (*eventos)[i];
      break;
    }
    i++;
  }

  do {
    clearScreen();
    printf("Evento encontrado:\n\n");
    printSingleEvent(e, 1);
    printf("\nDeseja remove-lo?\n");
    printf("[0] Nao\n");
    printf("[1] Sim\n");
    printf("Sua opcao: ");
    scanf("%d", &op);

    if (op != 0 && op != 1) {
      clearScreen();
      clearBuffer();
      printf("Opcao invalida, tente novamente!\n\n");
      systemPause();
    }

  } while (op != 0 && op != 1);

  if (op == 0)
    return;

  for (j = i; j < (*qtd) - 1; j++) {
    (*eventos)[j] = (*eventos)[j + 1];
  }

  *qtd = *qtd - 1;
  (*eventos) = realloc(*eventos, (*qtd) * sizeof(Evento));

  clearScreen();
  clearBuffer();
  printf("Evento excluido com sucesso!\n\n");
  systemPause();
}

#endif