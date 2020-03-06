// Ciobanu Alin-Matei 315 CB
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NMAX_CARD 17
#define NMAX 100
#define NMAX_PIN 10
#define NMAX_CVV 4
#define NMAX_EXPIRY_DATE 10

typedef struct hist {
	char infoH[NMAX];
	struct hist *urmH;
}TH, *THistory, **AHistory;

typedef struct LSC {
	char CARD_NUMBER[NMAX_CARD];
	char PIN[NMAX_PIN];
	char EXPIRY_DATE[NMAX_EXPIRY_DATE];
	char CVV[NMAX_CVV];
	int BALANCE;
	char STATUS[15];
	int incercari_pin;

	THistory HISTORY;
	struct LSC *urmC;
}TLCard, *TListaC;

typedef struct LC {
	TListaC info;
	struct LC *urm;
}TLC, *TLista,  **ALista;

THistory AlocHistory(char infoH[NMAX]);
TListaC AlocCard(char x[NMAX_CARD], char y[NMAX_PIN], char s[NMAX_EXPIRY_DATE], char a[NMAX_CVV]);
TLista AlocCelula(char x[NMAX_CARD], char y[NMAX_PIN], char s[NMAX_EXPIRY_DATE], char a[NMAX_CVV]);

int check(char s[NMAX_PIN]);
int suma(char NUMBER[NMAX_CARD]);
int exista(ALista aL, char CARD_NUMBER[NMAX_CARD]);
void inserareIn(ALista aL, int poz, char CARD_NUMBER[NMAX_CARD], char PIN[NMAX_PIN], char EXPIRY_DATE[NMAX_EXPIRY_DATE], char CVV[NMAX_CVV]);
void add_card(ALista aL, int poz, char CARD_NUMBER[NMAX_CARD], char PIN[NMAX_PIN], char EXPIRY_DATE[NMAX_EXPIRY_DATE], char CVV[NMAX_CVV]);
void InsSf(ALista aL, TLista aux);
void insInc(THistory *H, char x[NMAX]);
void elimina(AHistory aH, char x[NMAX]);
void distruge(ALista aL);

void adauga(ALista aL, int poz3, int poz, char CARD_NUMBER[NMAX_CARD], char PIN[NMAX_PIN], char EXPIRY_DATE[NMAX_EXPIRY_DATE], char CVV[NMAX_CVV]);
void realoc(ALista aL, int nr_max_carduri, int *max);
void show_all(TLista L, FILE *g);
void show_card(TLista L, char CARD_NUMBER[NMAX_CARD], FILE *g);
void delete_card(ALista aL, char CARD_NUMBER[NMAX_CARD]);
void unblock_card(ALista aL, char CARD_NUMBER[NMAX_CARD]);
void reverse_transaction(ALista aL, char CARD_NUMBER[NMAX_CARD], char CARD_NUMBER2[NMAX_CARD], int SUM, FILE *g);

void insert_card(ALista aL, char CARD_NUMBER[NMAX_CARD], char PIN[NMAX_PIN], FILE *g);
void cancel_card(ALista aL, char CARD_NUMBER[NMAX_CARD]);
void pin_change(ALista aL, char CARD_NUMBER[NMAX_CARD], char PIN[NMAX_PIN], FILE *g);
void balance_inquiry(ALista aL, char CARD_NUMBER[NMAX_CARD], FILE *g);
void recharge(ALista aL, char CARD_NUMBER[NMAX_CARD], int SUM, FILE *g);
void cash_withdrawal(ALista aL, char CARD_NUMBER[NMAX_CARD], int SUM, FILE *g);
void transfer_funds(ALista aL, char CARD_NUMBER[NMAX_CARD], char CARD_NUMBER2[NMAX_CARD], int SUM, FILE *g);