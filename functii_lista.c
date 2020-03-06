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

THistory AlocHistory(char infoH[NMAX]) {
	THistory aux = (THistory)calloc(1, sizeof(TH));
	if(!aux) return NULL;
	strcpy(aux->infoH, infoH);
	aux->urmH = NULL;
	return aux;
}

TListaC AlocCard(char x[NMAX_CARD], char y[NMAX_PIN], char s[NMAX_EXPIRY_DATE], char a[NMAX_CVV]) {
	TListaC aux = (TListaC)calloc(1, sizeof(TLCard));
	if (!aux) {
		//free(aux->HISTORY);
		return NULL;
	}
	strcpy(aux->CARD_NUMBER, x);
	strcpy(aux->PIN, y);
	strcpy(aux->EXPIRY_DATE, s);
	strcpy(aux->CVV, a);
	aux->BALANCE = 0;
	aux->incercari_pin = 0;
	strcpy(aux->STATUS, "NEW");
	aux->urmC = NULL;

	return aux;
}

TLista AlocCelula(char x[NMAX_CARD], char y[NMAX_PIN], char s[NMAX_EXPIRY_DATE], char a[NMAX_CVV]) {
	TLista aux = (TLista)calloc(1, sizeof(TLC));
	if (!aux) {
		//free(aux->info->HISTORY);
		//free(aux->info);
		return NULL;
	}
	aux->info = AlocCard(x, y, s, a);
	if (!aux->info) {
		free(aux);
		return NULL;
	}
	aux->urm = NULL;

	return aux;
}

int check(char s[NMAX_PIN]) { // verifica daca un sir e format doar din cifre
	int i;
	for (i = 0; s[i]; i++) {
		if (s[i] < '0' || s[i] > '9') return 0;
	}
	return 1;
}

int suma(char NUMBER[NMAX_CARD]) { //suma cifrelor lui card number
	int s = 0, i;
	for (i = 0; i < strlen(NUMBER); i++) {
		s += NUMBER[i] - '0';
	}
	return s;
}

int exista(ALista aL, char CARD_NUMBER[NMAX_CARD]) { //daca exista deja un card cu acelasi numar
	TLista L = *aL;
	TListaC p;
	for (; L; L = L->urm) {
		for (p = L->info; p; p = p->urmC) {
			if (strcmp(p->CARD_NUMBER, CARD_NUMBER) == 0) {
				return 1;
			}
		}
	}

	return 0;
}

void inserareIn(ALista aL, int poz, char CARD_NUMBER[NMAX_CARD], char PIN[NMAX_PIN], char EXPIRY_DATE[NMAX_EXPIRY_DATE], char CVV[NMAX_CVV]) {
	//functie care imi inlocuieste o celula
	TLista aux, p, ant, dup;
	int k = 0;
  	for (p = *aL, ant = NULL, dup = p->urm; p && dup; ant = p, p = p->urm, dup = dup->urm) {
		if (k == poz) break;
		k++;
	}

	aux = AlocCelula(CARD_NUMBER, PIN, EXPIRY_DATE, CVV);
	if (!aux) return;
	if (ant == NULL) {
		aux->urm = dup;
		*aL = aux;
		//free(p->info);
		free(p);
	} else {
		aux->urm = dup;
		ant->urm = aux;
		//free(p->info);
		free(p);
	}
}

void add_card(ALista aL, int poz, char CARD_NUMBER[NMAX_CARD], char PIN[NMAX_PIN], char EXPIRY_DATE[NMAX_EXPIRY_DATE], char CVV[NMAX_CVV]) {
	TLista p, auxc, ant;
	TListaC aux;
	int k = 0;
	for (p = *aL, ant = NULL; p; ant = p, p = p->urm) {
		if (k == poz) break; // gasesc pozitia de inserat
		k++;
	}


	if (p == NULL && ant == NULL) { // trebuie sa inserez pe prima pozitie care este prima inceputul listei
		auxc = AlocCelula(CARD_NUMBER, PIN, EXPIRY_DATE, CVV);
		if (!auxc) {
			//free(auxc->info);
			return;
		}
		*aL = auxc;
		return;
	}

	if (ant != NULL && p == NULL) { //sfarsitul listei
		auxc = AlocCelula(CARD_NUMBER, PIN, EXPIRY_DATE, CVV);
		if (!auxc) {
			//free(auxc->info);
			return;
		}
		ant->urm = auxc;
		return;
	}

	if (p != NULL) { //inserez in interiorul listei
		if (strcmp(p->info->CARD_NUMBER, "1") == 0) {
			auxc = AlocCelula(CARD_NUMBER, PIN, EXPIRY_DATE, CVV);
			if (!auxc) {
				//free(auxc->info);
				return;
			}
			// inlocuiesc celula alocata cu 1 1 1 1
			inserareIn(aL, poz, CARD_NUMBER, PIN, EXPIRY_DATE, CVV);
		} else { // sau inserez la inceput in lista de carduri
			aux = AlocCard(CARD_NUMBER, PIN, EXPIRY_DATE, CVV);
			if (!aux) return;
			aux->urmC = p->info;
			p->info = aux;
		}
	}
}

void InsSf(ALista aL, TLista aux) {
	TLista u = NULL;
	if (*aL != NULL) {
		u = *aL;
		while (u->urm != NULL) u = u->urm;
	}
	if (u == NULL) *aL = aux;
		else u->urm = aux;
}

void insInc(THistory *H, char x[NMAX]) {
	THistory aux;
	aux = AlocHistory(x);
	if (!aux) return;
	aux->urmH = *H;
	*H = aux;
}

void elimina(AHistory aH, char x[NMAX]) {
	THistory ant, p;
	for (p = *aH, ant = NULL; p != NULL; ant = p, p = p->urmH) {
		if (strcmp(p->infoH, x) == 0)
			break;
	}

	if (p == NULL) return;
	if (ant == NULL) {
		*aH = p->urmH;
		free(p);
	} else {
		ant->urmH = p->urmH;
		free(p);
	}
}

void distruge(ALista aL) {
	TLista aux;
	TListaC c;
	THistory h;

	while (*aL) {
		aux = *aL;
		*aL = aux->urm;
		while (aux->info) {
			c = aux->info;
			aux->info = c->urmC;
			while (c->HISTORY) {
				h = c->HISTORY;
				c->HISTORY = h->urmH;
				free(h);
			}
			free(c);
		}
		free(aux);
	}
}
