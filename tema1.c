// Ciobanu Alin-Matei 315 CB
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

int main() {
	int nr_max_carduri, poz = 0;
	TLista L = NULL;
	char EXPIRY_DATE[NMAX_EXPIRY_DATE];
	int SUM;

	FILE *f = fopen("input.in", "r");
	FILE *g = fopen("output.out", "w");
	char operatie[1000];
	fscanf(f, "%d", &nr_max_carduri);
	int poz3 = 0, max_poz = 0, nr_carduri = 0;

	char CARD_NUMBER[NMAX_CARD], CARD_NUMBER2[NMAX_CARD], PIN[NMAX_PIN], CVV[NMAX_CVV];
	unsigned long long c1;

	while (fscanf(f, "%s", operatie) == 1) {
		c1 = 0;
		if (strcmp(operatie, "show") == 0) {
			if(fscanf(f, "%llu", &c1) == 1) {
				sprintf(CARD_NUMBER, "%llu", c1);
				show_card(L, CARD_NUMBER, g);
			} else if (c1 == 0) {
				show_all(L, g);
			}
		}
		
		if (strcmp(operatie, "add_card") == 0) {
			fscanf(f, "%s %s %s %s", CARD_NUMBER, PIN, EXPIRY_DATE, CVV);
			poz = suma(CARD_NUMBER) % nr_max_carduri;
			if (max_poz == 0) {
				poz3 = poz;
				max_poz = poz;
			} else {
				if (poz < max_poz) {
					poz3 = - 1;
				} else {
					poz3 = poz - max_poz - 1;
					max_poz = poz;
				}
			}

			if (exista(&L, CARD_NUMBER) == 1) {
				fprintf(g, "The card already exists\n");
			} else {
				nr_carduri++;
				if (nr_carduri <= nr_max_carduri) {
					adauga(&L, poz3, poz, CARD_NUMBER, PIN, EXPIRY_DATE, CVV);
				} else {
					nr_max_carduri = 2 * nr_max_carduri;
					realoc(&L, nr_max_carduri, &max_poz);
					poz = suma(CARD_NUMBER) % nr_max_carduri;
					if (poz < max_poz) {
						poz3 = -1;
					} else {
						poz3 = poz - max_poz - 1;
						max_poz = poz;
					}
					adauga(&L, poz3, poz, CARD_NUMBER, PIN, EXPIRY_DATE, CVV);
				}
			}

			}

			if (strcmp(operatie, "delete_card") == 0) {
				nr_carduri--;
				fscanf(f, "%s", CARD_NUMBER);
				delete_card(&L, CARD_NUMBER);
			}

			if (strcmp(operatie, "insert_card") == 0) {
				fscanf(f, "%s %s", CARD_NUMBER, PIN);
				insert_card(&L, CARD_NUMBER, PIN, g);
			}

			if (strcmp(operatie, "cancel") == 0) {
				fscanf(f, "%s", CARD_NUMBER);
				cancel_card(&L, CARD_NUMBER);
			}

			if (strcmp(operatie, "unblock_card") == 0) {
				fscanf(f, "%s", CARD_NUMBER);
				unblock_card(&L, CARD_NUMBER);
			}

			if (strcmp(operatie, "pin_change") == 0) {
				fscanf(f, "%s %s", CARD_NUMBER, PIN);
				pin_change(&L, CARD_NUMBER, PIN, g);
			}

			if (strcmp(operatie, "balance_inquiry") == 0) {
				fscanf(f, "%s", CARD_NUMBER);
				balance_inquiry(&L, CARD_NUMBER, g);
			}

			if (strcmp(operatie, "recharge") == 0) {
				fscanf(f, "%s %d", CARD_NUMBER, &SUM);
				recharge(&L, CARD_NUMBER, SUM, g);
			}

			if (strcmp(operatie, "cash_withdrawal") == 0) {
				fscanf(f, "%s %d", CARD_NUMBER, &SUM);
				cash_withdrawal(&L, CARD_NUMBER, SUM, g);
			}

			if (strcmp(operatie, "transfer_funds") == 0) {
				fscanf(f, "%s %s %d", CARD_NUMBER, CARD_NUMBER2, &SUM);
				transfer_funds(&L, CARD_NUMBER, CARD_NUMBER2, SUM, g);
			}

			if (strcmp(operatie, "reverse_transaction") == 0) {
				fscanf(f, "%s %s %d", CARD_NUMBER, CARD_NUMBER2, &SUM);
				reverse_transaction(&L, CARD_NUMBER, CARD_NUMBER2, SUM, g);
			}
			
	}

	distruge(&L);
	fclose(f);
	fclose(g);


	//afisare(L);

	return 0;

}