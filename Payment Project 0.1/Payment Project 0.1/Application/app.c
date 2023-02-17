#include "app.h"
#define MAX_AMOUNT 50000.0
char cardErrorMsg[4][50] = { {"CARD_OK"},{"WRONG_NAME"},{"WRONG_EXP_DATE"}, {"WRONG_PAN"} };
char terminalErrorMsg[7][50] = { {"TERMINAL_OK"},{"WRONG_DATE"},{"EXPIRED_CARD"}, {"INVALID_CARD"},{"INVALID_AMOUNT"},{"EXCEED_MAX_AMOUNT"}, {"INVALID_MAX_AMOUNT"} };






void appStart(void) {
	ST_transaction_t trans1;
	int err = 0;
	err = getCardData(&trans1.cardHolderData);
	if (err != 0) {
		printf("CARD_ERROR: %s\n", cardErrorMsg[err]);
		printf("Card DECLINED");
		return;
	}
	err = getTerminalData(&trans1.terminalData);
	if (err != 0) {
		printf("Terminal_ERROR: %s\n", terminalErrorMsg[err]);
		printf("Transaction DECLINED");
		return;
	}
	if (isCardExpired(&trans1.cardHolderData, &trans1.terminalData) == 2) {
		printf("CARD_ERROR: %s\n", cardErrorMsg[EXPIRED_CARD]);
		printf("Card DECLINED");
		return;
	}
	switch (recieveTransactionData(&trans1)) {
	case 0:
		printf( "Transaction APPROVED");
		break;
	case 1:
		printf("DECLINED_INSUFFECIENT_FUND");
		break;
	case 2:
		printf("DECLINED_STOLEN_CARD");
		break;
	case 3:
		printf("FRAUD_CARD");
		break;
	case 4:
		printf("INTERNAL_SERVER_ERROR");
		break;
	default:
		printf("undefined Error");
		break;
	}

	
}

EN_cardError_t getCardData(ST_cardData_t* cardData) {
	if (getCardHolderName(cardData) == 1) return WRONG_NAME;
	if (getCardExpiryDate(cardData) == 2) return WRONG_EXP_DATE;
	if (getCardPAN(cardData) == 3) return WRONG_PAN;
	return CARD_OK;
}


EN_terminalError_t getTerminalData(ST_terminalData_t* termData) {
	getTransactionDate(termData);
	if (getTransactionAmount(termData) == 4) return INVALID_AMOUNT ;
	if (setMaxAmount(termData, MAX_AMOUNT) == 6) return INVALID_MAX_AMOUNT;
	if (isBelowMaxAmount(termData) == 5) return EXCEED_MAX_AMOUNT;
	return TERMINAL_OK;
}


