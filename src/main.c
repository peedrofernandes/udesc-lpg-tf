#include <stdio.h>
#include <stdlib.h>
#include "./types/constants.h"
#include "./functions/mainFunctions.h"
#include "./functions/handlers.h"
#include "./functions/utils.h"

int main()
{
  int op, qtdEventos = 0;
  FILE *lista;
  Evento *eventos = malloc(qtdEventos * sizeof(Evento));

  lista = fopen("lista.bin", "ab+");

  if (lista == NULL) {
    printf("Ocorreu um erro ao abrir o arquivo!\n");
    return 0;
  }

  while (1)
  {
    qtdEventos++;
    eventos = realloc(eventos, qtdEventos * sizeof(Evento));
    fread(&eventos[qtdEventos - 1], sizeof(Evento), 1, lista);
    if (feof(lista))
    {
      qtdEventos--;
      eventos = realloc(eventos, qtdEventos * sizeof(Evento));
      break;
    }
  } // Preencher o vetor local, utilizando o arquivo!

  fclose(lista);

  sortEvents(eventos, qtdEventos);

  do {
    clearScreen();

    op = showMenu();

    switch (op) {
    case 0: // Encerrar o programa
      clearScreen();
      printf("Saindo. Ate mais!\n");
      break;
    case 1: // Criar um evento novo
      createEvent(&qtdEventos, &eventos);
      break;
    case 2: // Mostrar todos os eventos
      printAllEvents(qtdEventos, eventos);
      break;
    case 3: // Mostrar todos os eventos de uma data especifica
      printEventsByDate(qtdEventos, eventos);
      break;
    case 4: // Mostrar os cinco proximos eventos a partir da data atual
      printFiveNextEvents(qtdEventos, eventos);
      break;
    case 5: // Remover algum evento de acordo com a data e hora inicial
      removeEvent(&qtdEventos, &eventos);
      break;
    default:
      clearScreen();
      clearBuffer();
      printf("Opcao invalida, tente novamente!\n");
      systemPause();
      break;
    }

  } while (op != 0);

  lista = fopen("lista.bin", "wb");

  writeEvents(eventos, qtdEventos, lista);

  free(eventos);
  fclose(lista);

  return 0;
}