#include "server.h"
ST_accountsDB_t accountsDB[255] =   {{200000.0, RUNNING,	"8989374615436851"}
									,{1000000.0, RUNNING,	"5807007076043875"}
									,{1000.0, RUNNING,		"6661444837595252"}
									,{100.0, BLOCKED,		"0354381764364978"}
									,{100000.0, BLOCKED,	"7552816017388337"} };
ST_transaction_t transactionDB[255] = { 0 };
void isValidAccountTest(void);
void isBlockedAccountTest(void);
void isAmountAvailableTest(void);
void saveTransactionTest(void);
void listSavedTransactionsTest(void);
void recieveTransactionDataTest(void);




EN_transState_t recieveTransactionData(ST_transaction_t* transData) {
	ST_accountsDB_t ref = { 0 };
	if (isValidAccount(&transData->cardHolderData, &ref) == 3) return FRAUD_CARD;
	if (isBlockedAccount(&ref) == 5) return DECLINED_STOLEN_CARD;
	if (isAmountAvailable(&transData->terminalData,  &ref) == 4) return DECLINED_INSUFFECIENT_FUND;
	transData->transState = APPROVED;
	if (saveTransaction(transData) == 1) return INTERNAL_SERVER_ERROR;
	for (int i = 0; i <= 255; i++) {
		if (strcmp(accountsDB[i].primaryAccountNumber, ref.primaryAccountNumber) == 0) {
			printf("Previous balance: %f\n", accountsDB[i].balance);
			accountsDB[i].balance -= transData->terminalData.transAmount;
			printf("New balance: %f\n", accountsDB[i].balance);
			break;
		}
	}
	
	return transData->transState;
}


void recieveTransactionDataTest(void) {
	ST_transaction_t transData;
	uint8_t tester_name[50];
	uint8_t expected_case[50];
	char result[30];
	printf("Enter your name:\n");
	fgets(tester_name, sizeof(tester_name), stdin);

	for (int i = 1; i < 6; i++) {
		getCardExpiryDate(&transData.cardHolderData);
		getCardHolderName(&transData.cardHolderData);
		getCardPAN(&transData.cardHolderData);
		setMaxAmount(&transData.terminalData, 20000.0);
		getTransactionDate(&transData.terminalData);
		getTransactionAmount(&transData.terminalData);
		printf("Enter expected result:\n");
		fgets(expected_case, sizeof(expected_case), stdin);
		switch (recieveTransactionData(&transData)) {
		case 0:
			strcpy_s(result, 30, "APPROVED");
			break;
		case 1:
			strcpy_s(result, 30, "DECLINED_INSUFFECIENT_FUND");
			break;
		case 2:
			strcpy_s(result, 30, "DECLINED_STOLEN_CARD");
			break;
		case 3:
			strcpy_s(result, 30, "FRAUD_CARD");
			break;
		case 4:
			strcpy_s(result, 30, "INTERNAL_SERVER_ERROR");
			break;
		default:
			printf("undefined Error");
			break;
		}
		printf("Tester Name :%sFunction Name: recieveTransactionData \nTest case %d:\nExpected result : %sActual result: %s\n-----------------\n"
			, tester_name, i, expected_case, result);

	}

	
}





EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence) {
	for (int i = 0; i <= 255; i++) {
		if (strcmp(accountsDB[i].primaryAccountNumber , cardData->primaryAccountNumber) == 0 ) {
			*accountRefrence = accountsDB[i];
			return SERVER_OK;
		}
	}
	accountRefrence = NULL;
	return ACCOUNT_NOT_FOUND;
}

void isValidAccountTest(void) {
	ST_cardData_t test;
	ST_accountsDB_t accountRefrence;
	uint8_t tester_name[50];
	uint8_t expected_case[50];
	char result[30];
	printf("Enter your name:\n");
	fgets(tester_name, sizeof(tester_name), stdin);
	
	for (int i = 1; i < 6; i++) {
		printf("Enter expected result:\n");
		fgets(expected_case, sizeof(expected_case), stdin);
		if (getCardPAN(&test) == 0) {
			switch (isValidAccount(&test, &accountRefrence)) {
			case 0:
				strcpy_s(result, 30, "SERVER_OK");
				break;
			case 3:
				strcpy_s(result, 30, "ACCOUNT_NOT_FOUND");
				break;
			default:
				printf("undefined Error");
				break;
			}
			printf("Tester Name :%sFunction Name: isValidAccount \nTest case %d:\nInput Data:%s\nExpected result : %sActual result: %s\n-----------------\n"
				, tester_name, i, user_input, expected_case, result);
		}
		else
		{
			strcpy_s(result, 30, "FAIL: WRONG PAN");
			printf("Tester Name :%sFunction Name: isValidAccount \nTest case %d:\nInput Data:%s\nExpected result : %sActual result: %s\n-----------------\n"
				, tester_name, i, user_input, expected_case, result);
		}

	}

}

EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence) {
	if (accountRefrence->state == RUNNING) {
		return SERVER_OK;
	}
	return BLOCKED_ACCOUNT;
}


void isBlockedAccountTest(void) {
	ST_cardData_t test;
	ST_accountsDB_t accountRefrence;
	uint8_t tester_name[50];
	uint8_t expected_case[50];
	char result[50];
	printf("Enter your name:\n");
	fgets(tester_name, sizeof(tester_name), stdin);
	for (int i = 1; i < 6; i++) {
		printf("Enter expected result:\n");
		fgets(expected_case, sizeof(expected_case), stdin);
		if (getCardPAN(&test) == 0 && isValidAccount(&test, &accountRefrence) == 0) {
			switch (isBlockedAccount(&accountRefrence)) {
			case 0:
				strcpy_s(result, 30, "SERVER_OK");
				break;
			case 5:
				strcpy_s(result, 30, "BLOCKED_ACCOUNT");
				break;
			default:
				printf("undefined Error");
				break;
			}
			printf("Tester Name :%sFunction Name: isBlockedAccount \nTest case %d:\nInput Data:%s\nExpected result : %sActual result: %s\n-----------------\n"
				, tester_name, i, user_input, expected_case, result);
		}
		else
		{
			strcpy_s(result, sizeof(result), "FAIL: WRONG PAN OR INVALID ACCOUNT");
			printf("Tester Name :%sFunction Name: isBlockedAccount \nTest case %d:\nInput Data:%s\nExpected result : %sActual result: %s\n-----------------\n"
				, tester_name, i, user_input, expected_case, result);
		}
	}
}



EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence) {
	if (termData->transAmount > accountRefrence->balance) {
		return LOW_BALANCE;
	}
	return SERVER_OK;
}


void isAmountAvailableTest(void) {
	ST_terminalData_t termData;
	ST_accountsDB_t accountRefrence = accountsDB[2];
	uint8_t tester_name[50];
	uint8_t expected_case[50];
	char result[30];
	printf("Enter your name:\n");
	fgets(tester_name, sizeof(tester_name), stdin);
	for (int i = 1; i < 6; i++) {
		printf("Enter expected result:\n");
		fgets(expected_case, sizeof(expected_case), stdin);
		if (getTransactionAmount(&termData) == 0) {
			switch (isAmountAvailable(&termData, &accountRefrence)) {
			case 0:
				strcpy_s(result, 30, "SERVER_OK");
				break;
			case 4:
				strcpy_s(result, 30, "LOW_BALANCE");
				break;
			default:
				printf("undefined Error");
				break;
			}
			printf("Tester Name :%sFunction Name: isAmountAvailable \nTest case %d:\nInput Data:%sExpected result : %sActual result: %s\n-----------------\n"
				, tester_name, i, user_input, expected_case, result);
		}
		else {
			strcpy_s(result, 30, "FAIL: INVALID_AMOUNT");
			printf("Tester Name :%sFunction Name: isAmountAvailable \nTest case %d:\nInput Data:%sExpected result : %sActual result: %s\n-----------------\n"
				, tester_name, i, user_input, expected_case, result);
		}

	}
}


EN_serverError_t saveTransaction(ST_transaction_t* transData) {
	for (int i = 0; i < 255; i++) {
		if (transactionDB[i].transactionSequenceNumber == 0) {
			transactionDB[i].cardHolderData = transData->cardHolderData;
			transactionDB[i].terminalData = transData->terminalData;
			if (i == 0) { transactionDB[i].transactionSequenceNumber = 404; }
			if (i > 0) { transactionDB[i].transactionSequenceNumber = transactionDB[i - 1].transactionSequenceNumber + 1;}
			transactionDB[i].transState = transData->transState;
			listSavedTransactions();

			return SERVER_OK;
		}
	  }
	return SAVING_FAILED;
}



void saveTransactionTest(void) {
	ST_transaction_t transData;
	uint8_t tester_name[50];
	uint8_t expected_case[50];
	char result[30];
	printf("Enter your name:\n");
	fgets(tester_name, sizeof(tester_name), stdin);
	for (int j = 0; j < 253; j++) {transactionDB[j].transactionSequenceNumber = 5;}
	for (int i = 0; i < 3; i++) {
		printf("Enter expected result:\n");
		fgets(expected_case, sizeof(expected_case), stdin);
		getCardExpiryDate(&transData.cardHolderData);
		getCardHolderName(&transData.cardHolderData);
		getCardPAN(&transData.cardHolderData);
		setMaxAmount(&transData.terminalData, 20000.0);
		getTransactionDate(&transData.terminalData);
		getTransactionAmount(&transData.terminalData);
		transData.transState = i;
		switch (saveTransaction(&transData)) {
			case 0:
				strcpy_s(result, 30, "SERVER_OK");
				break;
			case 1:
				strcpy_s(result, 30, "INTERNAL_SERVER_ERROR");
				break;
			default:
				printf("undefined Error");
				break;
			}
			printf("Tester Name :%sFunction Name: saveTransaction \nTest case %d:\nExpected result : %sActual result: %s\n-----------------\n"
				, tester_name, i+1, expected_case, result);


	}
}



void listSavedTransactions(void) {
	char trans[5][30] = { {"APPROVED"}, {"DECLINED_INSUFFECIENT_FUND"}, {"DECLINED_STOLEN_CARD"}, {"FRAUD_CARD"}, {"INTERNAL_SERVER_ERROR"}};

	for (int i = 0; i <= 255; i++) {
		if (transactionDB[i].transState < 0 || transactionDB[i].transState > 4) {
			break;
		}
		if (transactionDB[i].transactionSequenceNumber == 0) {
			break;
		}
		printf("###################################\n");
		printf("Transaction Sequence Number: %d\n",transactionDB[i].transactionSequenceNumber);
		printf("Transaction Date: %s\n", &transactionDB[i].terminalData.transactionDate);
		printf("Transaction Amount: %f\n", transactionDB[i].terminalData.transAmount);
		printf("Transaction State: %s\n", trans[transactionDB[i].transState]);
		printf("Terminal Max Amount: %f\n", transactionDB[i].terminalData.maxTransAmount);
		printf("Cardholder Name: %s\n", transactionDB[i].cardHolderData.cardHolderName);
		printf("PAN: %s\n", transactionDB[i].cardHolderData.primaryAccountNumber);
		printf("Card Expiration Date: %s\n", transactionDB[i].cardHolderData.cardExpirationDate);
		printf("###################################\n");

	}

}



void listSavedTransactionsTest(void) {
	uint8_t tester_name[50];
	uint8_t expected_case[50];
	char result[30];
	ST_transaction_t transData,transData1,transData2;
	strcpy_s(transData.cardHolderData.cardExpirationDate, 6, "05/25");
	strcpy_s(transData.cardHolderData.cardHolderName, 25, "Mahmoud Ahmed Mohamed");
	strcpy_s(transData.cardHolderData.primaryAccountNumber, 20, "5847596847589658");
	transData.terminalData.maxTransAmount = 20000;
	strcpy_s(transData.terminalData.transactionDate, 11, "16/01/2023");
	transData.terminalData.transAmount = 1500;
	transData.transState = 0;
	transData.transactionSequenceNumber = 69;

	strcpy_s(transData1.cardHolderData.cardExpirationDate, 6, "05/21");
	strcpy_s(transData1.cardHolderData.cardHolderName, 25, "Mahmoud Ahmed ");
	strcpy_s(transData1.cardHolderData.primaryAccountNumber, 20, "584759");
	transData1.terminalData.maxTransAmount = 20000;
	strcpy_s(transData1.terminalData.transactionDate, 11, "17/01/2023");
	transData1.terminalData.transAmount = 200;
	transData1.transState = 3;
	transData1.transactionSequenceNumber = 420;

	strcpy_s(transData2.cardHolderData.cardExpirationDate, 6, "05/23");
	strcpy_s(transData2.cardHolderData.cardHolderName, 25, "Mahmoud  ");
	strcpy_s(transData2.cardHolderData.primaryAccountNumber, 20, "584");
	transData2.terminalData.maxTransAmount = 20000;
	strcpy_s(transData2.terminalData.transactionDate, 11, "18/01/2023");
	transData2.terminalData.transAmount = 5000;
	transData2.transState = 1;
	transData2.transactionSequenceNumber = 42069;


	transactionDB[0] = transData;
	transactionDB[1] = transData1;
	transactionDB[2] = transData2;

	printf("Enter your name:\n");
	fgets(tester_name, sizeof(tester_name), stdin);
	printf("Enter expected result:\n");
	fgets(expected_case, sizeof(expected_case), stdin);
	listSavedTransactions();
	strcpy_s(result, 30, "Server Ok");
	printf("Tester Name :%sFunction Name:  listSavedTransactions \nTest case:\nExpected result : %sActual result: %s\n-----------------\n"
			, tester_name, expected_case, result);
}


