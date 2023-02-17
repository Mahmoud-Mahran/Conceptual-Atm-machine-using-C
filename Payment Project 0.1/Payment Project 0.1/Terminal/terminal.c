#include "terminal.h"

void getTransactionDateTest(void);
void isCardExpiredTest(void);
void getTransactionAmountTest(void);
void isBelowMaxAmountTest(void);
void setMaxAmountTest(void);





EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	time_t t = time(NULL);
	struct tm timmy;
	localtime_s(&timmy, &t);
	printf("Input transaction date: ");
	fgets(user_input, sizeof(user_input), stdin);
	int temp = strlen(user_input);
	if (user_input[temp - 1] == '\n') user_input[temp - 1] = 0;
	if (strlen(user_input) != 10) {
		printf("using system date.\n");
		sprintf_s(termData->transactionDate, sizeof(termData->transactionDate),
			"%d%d/%d%d/%d",
			timmy.tm_mday / 10, timmy.tm_mday % 10,		//correcting day format to be always 2 digits
			(timmy.tm_mon + 1) / 10, (timmy.tm_mon + 1) % 10, //correcting month format to be always 2 digits
			timmy.tm_year + 1900);
		return WRONG_DATE;
	}
	//making sure that the string is formatted correctly with only numeric chars
	for (int i = 0; i < strlen(user_input); i++) {
		if (isdigit(user_input[i]) == 0 &&  i != 2 && i !=  5 ) {
			printf("using system date.\n");
			sprintf_s(termData->transactionDate, sizeof(termData->transactionDate),
				"%d%d/%d%d/%d",
				timmy.tm_mday / 10, timmy.tm_mday % 10,		//correcting day format to be always 2 digits
				(timmy.tm_mon + 1) / 10, (timmy.tm_mon + 1) % 10, //correcting month format to be always 2 digits
				timmy.tm_year + 1900);
			return WRONG_DATE;
		}
		if ((i == 2 || i == 5) && user_input[i] != 47) {
			printf("using system date.\n");
			sprintf_s(termData->transactionDate, sizeof(termData->transactionDate),
				"%d%d/%d%d/%d",
				timmy.tm_mday / 10, timmy.tm_mday % 10,		//correcting day format to be always 2 digits
				(timmy.tm_mon + 1) / 10, (timmy.tm_mon + 1) % 10, //correcting month format to be always 2 digits
				timmy.tm_year + 1900);
			return WRONG_DATE;
		}
	}
	strcpy_s(termData->transactionDate, sizeof(termData->transactionDate), user_input);
	return TERMINAL_OK;
}



void getTransactionDateTest(void) {
	ST_terminalData_t test;
	uint8_t tester_name[50];
	uint8_t expected_case[50];
	char result[30];
	printf("Enter your name:");
	fgets(tester_name, sizeof(tester_name), stdin);
	for (int i = 1; i < 6; i++) {
		printf("Enter expected result:");
		fgets(expected_case, sizeof(expected_case), stdin);
		switch (getTransactionDate(&test)) {
		case 0:
			strcpy_s(result, 30, "PASS");
			break;
		case 1:
			strcpy_s(result, 30, "FAIL");
			break;
		default:
			strcpy_s(result, 30, "undefined Error");
			break;
		}
		printf("Tester Name :%sFunction Name: getTransactionDate \nTest case %d:\nInput Data:%s\nExpected result : %sActual result: %s\n-----------------\n"
			, tester_name, i, user_input, expected_case, result);
	}

}



EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData) {
	char* tmp_pt;
	//getting the expiration and transaction dates as integers
	long exp_mon = strtol(cardData->cardExpirationDate, &tmp_pt, 10);
	long exp_year = strtol(&cardData->cardExpirationDate[3], &tmp_pt, 10);
	long term_mon = strtol(&termData->transactionDate[3], &tmp_pt, 10);
	long term_year = strtol(&termData->transactionDate[8], &tmp_pt, 10);
	//checking that the card isn't expired
	if (exp_year > term_year) {
		return TERMINAL_OK;
	}
	if (exp_year == term_year) {
		if (exp_mon > term_mon) {
			return TERMINAL_OK;
		}
	}
	return EXPIRED_CARD;
}




void isCardExpiredTest(void) {
	ST_terminalData_t test;
	ST_cardData_t test2;
	uint8_t tester_name[50];
	uint8_t expected_case[50];
	char result[30];
	printf("Enter your name:");
	fgets(tester_name, sizeof(tester_name), stdin);
	getTransactionDate(&test);
	for (int i = 1; i < 6; i++) {
		printf("Enter expected result:");
		fgets(expected_case, sizeof(expected_case), stdin);
		 if (getCardExpiryDate(&test2) == 0){
			switch (isCardExpired(&test2, &test)) {
			case 0:
				strcpy_s(result, 30, "PASS");
				break;
			case 1:
			case 2:
				strcpy_s(result, 30, "FAIL");
				break;
			default:
				strcpy_s(result, 30, "undefined Error");
				break;
			}
			printf("Tester Name :%sFunction Name: isCardExpired \nTest case %d:\nInput Data:(%s)&(%s)\nExpected result : %sActual result: %s\n-----------------\n"
				, tester_name, i, test.transactionDate, user_input, expected_case, result);
			continue;
		}
		 printf("Tester Name :%sFunction Name: isCardExpired \nTest case %d:\nInput Data:(%s)&(%s)\nExpected result : %sActual result: Wrong Expiry Date\n-----------------\n"
			 , tester_name, i, test.transactionDate, user_input, expected_case);
	}
	
}




EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) {
	char* tmp_pt;
	*user_input = 0;
	printf("Input transaction amount: ");
	fgets(user_input, sizeof(user_input), stdin);
	//checking user input to make sure it's numeric chars only
	for (int i = 0; i < strlen(user_input)-1; i++) {
		
		if (isdigit(user_input[i]) == 0 && user_input[i] != '.' ) {
			
			return INVALID_AMOUNT;
		}
	}
	//storing input after converting to float
	termData->transAmount = strtof(user_input, &tmp_pt);
	if (termData->transAmount <= 0) {
		return INVALID_AMOUNT;
	}
	return TERMINAL_OK;
}



void getTransactionAmountTest(void) {
	ST_terminalData_t test;
	uint8_t tester_name[50];
	uint8_t expected_case[50];
	char result[30];
	printf("Enter your name:");
	fgets(tester_name, sizeof(tester_name), stdin);
	for (int i = 1; i < 6; i++) {
		printf("Enter expected result:");
		fgets(expected_case, sizeof(expected_case), stdin);
		switch (getTransactionAmount(&test) ){
		case 0:
			strcpy_s(result, 30, "PASS");
			break;
		case 4:
			strcpy_s(result, 30, "FAIL");
			break;
		default:
			strcpy_s(result, 30, "undefined Error");
			break;
		}
		printf("Tester Name :%sFunction Name:getTransactionAmount \nTest case %d:\nInput Data:%sExpected result : %sActual result: %s\n-----------------\n",
			tester_name, i, user_input, expected_case, result);
	}
}



EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData) {
	if (termData->transAmount > termData->maxTransAmount) {
			return EXCEED_MAX_AMOUNT;
	}
	return TERMINAL_OK;
}


void isBelowMaxAmountTest(void) {
	ST_terminalData_t test;
	test.maxTransAmount = 20000;
	uint8_t tester_name[50];
	uint8_t expected_case[50];
	char result[30];
	printf("Enter your name:");
	fgets(tester_name, sizeof(tester_name), stdin);
	
	for (int i = 1; i < 6; i++) {
		printf("Enter expected result:");
		fgets(expected_case, sizeof(expected_case), stdin);
		if (getTransactionAmount(&test) == 0) {
			switch (isBelowMaxAmount(&test)) {
			case 0:
				strcpy_s(result, 30, "PASS");
				break;
			case 5:
				strcpy_s(result, 30, "Fail");
				break;
			default:
				strcpy_s(result, 30, "undefined Error");
				break;
			}
			printf("Tester Name :%s Function Name:isBelowMaxAmount \nTest case %d:\nInput Data:%f\nExpected result : %sActual result: %s\n-----------------\n",
				tester_name, i, test.transAmount, expected_case, result);
			continue;
		}
		strcpy_s(result, 30, "invalid_amount");
		printf("Tester Name :%sFunction Name:isBelowMaxAmount \nTest case %d:\nInput Data:%f\nExpected result : %sActual result: %s\n-----------------\n",
			tester_name, i, test.transAmount, expected_case, result);
		
	}
}



EN_terminalError_t setMaxAmount(ST_terminalData_t* termData, float maxAmount) {
	termData->maxTransAmount = 0;
	if (maxAmount <= 0) {
		return INVALID_MAX_AMOUNT;
	}
	termData->maxTransAmount = maxAmount;
	return TERMINAL_OK;
}


void setMaxAmountTest(void) {
	ST_terminalData_t test;
	uint8_t tester_name[50];
	uint8_t expected_case[50];
	char result[30];
	float m_a;
	printf("Enter your name:");
	fgets(tester_name, sizeof(tester_name), stdin);	
	printf("Enter expected result:");
	fgets(expected_case, sizeof(expected_case), stdin);
	for (int i = 1; i < 6; i++) {

		printf("Enter transaction max amount: ");
		scanf_s("%f", &m_a);
		switch (setMaxAmount(&test, m_a)) {
		case 0:
			strcpy_s(result, 30, "TERMINAL_OK-PASS");
			break;
		case 6:
			strcpy_s(result, 30, "INVALID_MAX_AMOUNT-FAIL");
			break;
		default:
			strcpy_s(result, 30, "undefined Error");
			break;
		}
		printf("Tester Name :%sFunction Name:setMaxAmount \nTest case %d:\nInput Data:%f\nExpected result : %sActual result: %s\n-----------------\n",
			tester_name, i, m_a, expected_case, result);
	}
}









