#include "header.h"

void main()
{
	
	Table table_arr[table];  
	ListM list;
	FILE *inM, * inI; //for each file.

	int flag1 = 0; //In order to verify if the first action is '1'.
	int bool = 0, temp_Quantity, table_num;
	char action, zero, temp_ProdectName[MAX];

	Initialization(&table_arr, &list);

	inM = fopen("Manot.txt", "r");
	if (inM == NULL){
		Error_Msg("Can not opens file.");
	}

	inI = fopen("Instructions.txt", "r");
	if (inI == NULL){
		fclose(inM); //in case inI not open close "Manot file".
		Error_Msg("Can not opens file.");
	}
	/*------------Read actions form file---------*/

	while (fscanf(inI, "%c ", &action) != EOF) { //run as long as there are actions in "Instructions" file
		
		if (!flag1) {
			if (action == '1') flag1 = 1;
			else {
				printf("The first action must be '1', the file is not valid.");
				exit(1);
			}
		}
		printf("\n  Menu actions: \n1)\tCreate a menu.\n2)\tAdding an inventory to an existing dish.\n3)\tOrder dishes to table.\n4)\tCancellation of ordering items.\n5)\tClosing a table.");
		printf("\nEnter '0' to scan the action from file: ");
		rewind(stdin); //in order to clean buffer.
		scanf("%1c", &zero, 1);
		while (zero != '0') { //run until user ser 0 to acknowledge the action.
			printf("\nYou nust press '0' .");
			rewind(stdin); //in order to clean buffer.
			scanf("%1c", &zero, 1);
		}

		//printf("In order to scan the action, press '0'.")
		switch (action)
		{
		case '1': {

			if (bool == 1) { //in case function 1 been chose befoe need to do free and gets new data.
				FreeList(&list);
			}
			CreateProduct(inM, &list);
			bool = 1;
		}
				break;
		case '2': {
			fscanf(inI, "%s %d ", &temp_ProdectName, &temp_Quantity);
			AddItem(&list, temp_ProdectName, temp_Quantity);
		}
				break;
		case '3': {
			fscanf(inI, "%d %s %d ", &table_num, temp_ProdectName, &temp_Quantity);
			OrderItem(table_num, temp_ProdectName, temp_Quantity, table_arr, &list);

		}
				break;
				//
		case '4': {
			fscanf(inI, "%d %s %d ", &table_num, temp_ProdectName, &temp_Quantity);
			RemoveItem(table_num, temp_ProdectName, temp_Quantity, table_arr, &list);
		}
				break;
		case '5': {
			fscanf(inI, "%d ", &table_num);
			RemoveTable(table_num, table_arr);
		}
				break;

		default: {
			printf("Action %c is wrong.", action);
		}

		}
	}
	printf("\nThere are no more actions in the input file");
	
	fclose(inM);
	fclose(inI);
	
	free_tables(table_arr); //free all table.
	FreeList(list.head); //free the products list.

}


