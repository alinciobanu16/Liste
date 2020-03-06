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

void insert_card(ALista aL, char CARD_NUMBER[NMAX_CARD], char PIN[NMAX_PIN], FILE *g) {
	TLista L = *aL;
	TListaC p, u;
	char infoH[NMAX] ={0};

	for (L = *aL; L; L = L->urm) {
		for (p = L->info; p; p = p->urmC) {
			if (strcmp(p->CARD_NUMBER, CARD_NUMBER) == 0) {
				u = p;
				break;
			}
		}
	}

	if (strcmp(u->STATUS, "LOCKED") == 0) {
		fprintf(g, "The card is blocked. Please contact the administrator.\n");
		sprintf(infoH, "FAIL, insert_card %s %s", CARD_NUMBER, PIN);
		insInc(&(u->HISTORY), infoH);
		return;
	}

	if (strlen(PIN) != 4 || strcmp(u->PIN, PIN) != 0) {
		fprintf(g, "Invalid PIN\n");
		sprintf(infoH, "FAIL, insert_card %s %s", CARD_NUMBER, PIN);
		insInc(&(u->HISTORY), infoH);
		u->incercari_pin++;
		if (u->incercari_pin == 3) {
			fprintf(g, "The card is blocked. Please contact the administrator.\n");
			strcpy(u->STATUS, "LOCKED");
		}

	} else {
		if (strcmp(u->PIN, PIN) == 0) {
			if (strcmp(u->STATUS, "ACTIVE") != 0) {
				fprintf(g, "You must change your PIN.\n");
			}
			sprintf(infoH, "SUCCESS, insert_card %s %s", CARD_NUMBER, PIN);
			u->incercari_pin = 0;
			insInc(&(u->HISTORY), infoH);
		}
	}
}

void cancel_card(ALista aL, char CARD_NUMBER[NMAX_CARD]) {
	TLista L;
	TListaC u, p;
	char infoH[NMAX] = "SUCCESS, cancel ";
	for (L = *aL; L; L = L->urm) {
		for (p = L->info; p; p = p->urmC) {
			if (strcmp(p->CARD_NUMBER, CARD_NUMBER) == 0) {
				u = p;
				break;
			}
		}
	}
	strcat(infoH, CARD_NUMBER);
	insInc(&(u->HISTORY), infoH);
}

void pin_change(ALista aL, char CARD_NUMBER[NMAX_CARD], char PIN[NMAX_PIN], FILE *g) {
	TLista L;
	TListaC u, p;
	char infoH[NMAX] = {0};
	for (L = *aL; L; L = L->urm) {
		for (p = L->info; p; p = p->urmC) {
			if (strcmp(p->CARD_NUMBER, CARD_NUMBER) == 0) {
				u = p;
				break;
			}
		}
	}

	if (strlen(PIN) != 4 || check(PIN) == 0) {
		fprintf(g, "Invalid PIN\n");
		sprintf(infoH, "FAIL, pin_change %s %s", CARD_NUMBER, PIN);
		insInc(&(u->HISTORY), infoH);
	} else {
		strcpy(u->PIN, PIN);
		strcpy(u->STATUS, "ACTIVE");
		sprintf(infoH, "SUCCESS, pin_change %s %s", CARD_NUMBER, PIN);
		insInc(&(u->HISTORY), infoH);
	}
}

void balance_inquiry(ALista aL, char CARD_NUMBER[NMAX_CARD], FILE *g) {
	TLista L;
	TListaC p, u;
	char infoH[NMAX] = {0};
	for (L = *aL; L; L = L->urm) {
		for (p = L->info; p; p = p->urmC) {
			if (strcmp(p->CARD_NUMBER, CARD_NUMBER) == 0) {
				u = p;
				break;
			}
		}
	}
	fprintf(g, "%d\n", u->BALANCE);
	strcpy(infoH, "SUCCESS, balance_inquiry ");
	strcat(infoH, CARD_NUMBER);
	insInc(&(u->HISTORY), infoH);
} 

void recharge(ALista aL, char CARD_NUMBER[NMAX_CARD], int SUM, FILE *g) {
	TLista L;
	TListaC p, u;
	char infoH[NMAX] = {0};
	for (L = *aL; L; L = L->urm) {
		for (p = L->info; p; p = p->urmC) {
			if (strcmp(p->CARD_NUMBER, CARD_NUMBER) == 0) {
				u = p;
				break;
			}
		}
	}

	if (SUM % 10 != 0) {
		fprintf(g, "The added amount must be multiple of 10\n");
		sprintf(infoH, "FAIL, recharge %s %d", CARD_NUMBER, SUM);
		insInc(&(u->HISTORY), infoH);
	} else {
		u->BALANCE += SUM;
		fprintf(g, "%d\n", u->BALANCE);
		sprintf(infoH, "SUCCESS, recharge %s %d", CARD_NUMBER, SUM);
		insInc(&(u->HISTORY), infoH);
	}
}

void cash_withdrawal(ALista aL, char CARD_NUMBER[NMAX_CARD], int SUM, FILE *g) {
	TLista L;
	TListaC p, u;
	char infoH[NMAX] = {0};
	for (L = *aL; L; L = L->urm) {
		for (p = L->info; p; p = p->urmC) {
			if (strcmp(p->CARD_NUMBER, CARD_NUMBER) == 0) {
				u = p;
				break;
			}
		}
	}
	if (SUM % 10 != 0 || u->BALANCE < SUM) {
		if (SUM % 10 != 0) {
			fprintf(g, "The requested amount must be multiple of 10\n");
		} else if (u->BALANCE < SUM) {
			fprintf(g, "Insufficient funds\n");
		} 
		sprintf(infoH, "FAIL, cash_withdrawal %s %d", CARD_NUMBER, SUM);
		insInc(&(u->HISTORY), infoH);
	} else {
		u->BALANCE -= SUM;
		fprintf(g, "%d\n", u->BALANCE);
		sprintf(infoH, "SUCCESS, cash_withdrawal %s %d", CARD_NUMBER, SUM);
		insInc(&(u->HISTORY), infoH);
	}
}

void transfer_funds(ALista aL, char CARD_NUMBER[NMAX_CARD], char CARD_NUMBER2[NMAX_CARD], int SUM, FILE *g) {
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

	if (SUM % 10 != 0 || u->BALANCE < SUM) {
		if (SUM % 10 != 0) {
			fprintf(g, "The transferred amount must be multiple of 10\n");
		} else if (u->BALANCE < SUM) {
			fprintf(g, "Insufficient funds\n");
		}
		sprintf(infoH, "FAIL, transfer_funds %s %s %d", CARD_NUMBER, CARD_NUMBER2, SUM);
		insInc(&(u->HISTORY), infoH);
	} else {
		u->BALANCE -= SUM;
		fprintf(g, "%d\n", u->BALANCE);
		v->BALANCE += SUM;
		sprintf(infoH, "SUCCESS, transfer_funds %s %s %d", CARD_NUMBER, CARD_NUMBER2, SUM);
		insInc(&(u->HISTORY), infoH);
		insInc(&(v->HISTORY), infoH);
	}
}


