// Ciobanu Alin-Matei 315CB
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblioteca.h"
#define NMAX_CARD 17
#define NMAX 100
#define NMAX_PIN 10
#define NMAX_CVV 4
#define NMAX_EXPIRY_DATE 10


void adauga(ALista aL, int poz3, int poz, char CARD_NUMBER[NMAX_CARD], char PIN[NMAX_PIN], char EXPIRY_DATE[NMAX_EXPIRY_DATE], char CVV[NMAX_CVV]) {
	int j;
	TLista aux;
	for (j = 0; j < poz3; j++) {
		aux = AlocCelula("1", "1", "gol", "1"); // aloc celule ca sa ma pot deplasa spre poz
		if (!aux) {
			return;
		}
		InsSf(aL, aux); // le adaug mereu la sfarsitul listei mari
	}

	add_card(aL, poz, CARD_NUMBER, PIN, EXPIRY_DATE, CVV);

}

void realoc(ALista aL, int nr_max_carduri, int *max) {
	int j, poz = 0, poz3;
	*max = 0;
	TLista copie = NULL, aux, L = *aL;
	TListaC p;
	for (; L != NULL; L = L->urm) {
		for (p = L->info; p; p = p->urmC) { // parcurg toate cardurile din lista
			poz = suma(p->CARD_NUMBER) % nr_max_carduri; // calculez noua pozitia in care trebuie inserat cardul
			if (*max == 0) {
				poz3 = poz - *max;
				*max = poz;
			} else if (*max != 0) {
				if (poz < *max) {
					poz3 = -1;
				} else {
					poz3 = poz - *max - 1;
					*max = poz;
				}
			}

			for (j = 0; j < poz3; j++) {
				aux = AlocCelula("1", "1", "gol", "1"); // aloc celule sa ma pot deplasa
				if (!aux) {
					return;
				}
				InsSf(&copie, aux); // le inserez la final in noua baza de data(lista mare)
			}
			
			add_card(&copie, poz, p->CARD_NUMBER, p->PIN, p->EXPIRY_DATE, p->CVV); //adaug cardul in noua lista
			copie->info->BALANCE = p->BALANCE;
			strcpy(copie->info->STATUS, p->STATUS);
			copie->info->incercari_pin = p->incercari_pin;
			copie->info->HISTORY = p->HISTORY; //copiez istoricul
		
		}
	}
	distruge(aL);
	*aL = copie;
}

void show_all(TLista L, FILE *g) {
	int k = -1;
	TListaC q;
	THistory a;
	for (; L != NULL; L = L->urm) {
		k++;
		if (strcmp(L->info->CARD_NUMBER, "1") == 0) {
			fprintf(g, "pos%d: []\n", k);
		} else {
			fprintf(g, "pos%d: [\n", k);
			for(q = L->info; q; q = q->urmC) {
				fprintf(g, "(card number: %s, PIN: %s, expiry date: %s, CVV: %s, balance: %d, status: %s, ",
					q->CARD_NUMBER, q->PIN, q->EXPIRY_DATE, q->CVV, q->BALANCE, q->STATUS);
					
				fprintf(g, "history: [");
				for (a = q->HISTORY; a; a = a->urmH) {
					if (a->urmH != NULL) {
						fprintf(g, "(%s), ", a->infoH);
					} else {
						fprintf(g, "(%s)", a->infoH);
					}
				}
				
				fprintf(g, "])\n");
			
			}
			fprintf(g, "]\n");
		}
	}
}

void show_card(TLista L, char CARD_NUMBER[NMAX_CARD], FILE *g) {
	TListaC q;
	THistory a;
	for (; L!= NULL; L = L->urm) {
		for (q = L->info; q; q = q->urmC) {
			if (strcmp(CARD_NUMBER, q->CARD_NUMBER) == 0) {
				fprintf(g, "(card number: %s, PIN: %s, expiry date: %s, CVV: %s, balance: %d, status: %s, ",
					q->CARD_NUMBER, q->PIN, q->EXPIRY_DATE, q->CVV, q->BALANCE, q->STATUS);
				fprintf(g, "history: [");
				for (a = q->HISTORY; a; a = a->urmH) {
					if (a->urmH != NULL) {
						fprintf(g, "(%s), ", a->infoH);
					} else {
						fprintf(g, "(%s)", a->infoH);
					}
				}
				fprintf(g, "])\n");
				break;
			}
		}
	}
}

void delete_card(ALista aL, char CARD_NUMBER[NMAX_CARD]) {
	TLista L;
	TListaC ant, aux, p = NULL;
	int ok = 0, lg = 0;
	for (L = *aL; L; L = L->urm) {
		lg = 0;
		p = L->info;
		for (ant = NULL; p; ant = p, p = p->urmC) {
			lg++;
			if (strcmp(CARD_NUMBER, p->CARD_NUMBER) == 0) {
				aux = p;
				if (lg == 1 && aux->urmC == NULL) { // daca este primul card de pe pozitie
					free(aux->HISTORY);
					strcpy(aux->CARD_NUMBER, "1"); // il transform in 1

				} else {
					if (ant == NULL) {
						(*aL)->info = aux->urmC; ok = 1;
					} else {
						ant->urmC = aux->urmC;
						ok = 1;
				}
					if(ok == 1) {
						free(aux->HISTORY);
						free(aux);
						break;
					}
				}
			}
		}		
	}
}


void unblock_card(ALista aL, char CARD_NUMBER[NMAX_CARD]) {
	TLista L;
	TListaC u, p;
	for (L = *aL; L; L = L->urm) {
		for (p = L->info; p; p = p->urmC) {
			if (strcmp(p->CARD_NUMBER, CARD_NUMBER) == 0) {
				u = p;
				break;
			}
		}
	}
	u->incercari_pin = 0;
	strcpy(u->STATUS, "ACTIVE");
}

void reverse_transaction(ALista aL, char CARD_NUMBER[NMAX_CARD], char CARD_NUMBER2[NMAX_CARD], int SUM, FILE *g) {
	int ok = 0, ok2 = 0;
	TLista L;
	TListaC p, u, v;
	char infoH[NMAX] = {0};
	for (L = *aL; L; L = L->urm) {
		for (p = L->info; p; p = p->urmC) {
			if (strcmp(p->CARD_NUMBER, CARD_NUMBER) == 0) {
				u = p;
				ok = 1;
			}
			if (strcmp(p->CARD_NUMBER, CARD_NUMBER2) == 0) {
				v = p;
				ok2 = 1;
			}
			if (ok == 1 && ok2 == 1) break;
		}
	}

	if (v->BALANCE < SUM) {
		fprintf(g, "The transaction cannot be reversed\n");
	} else {
		u->BALANCE += SUM;
		v->BALANCE -= SUM;
		sprintf(infoH, "SUCCESS, reverse_transaction %s %s %d", CARD_NUMBER, CARD_NUMBER2, SUM);
		insInc(&(u->HISTORY), infoH);
		sprintf(infoH, "SUCCESS, transfer_funds %s %s %d", CARD_NUMBER, CARD_NUMBER2, SUM);
		elimina(&(v->HISTORY), infoH);
	}
}
