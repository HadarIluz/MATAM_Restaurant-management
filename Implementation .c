#include "header.h"

/*Function name: Initialization
Input: Table *T, ListM* P
Output: void
Algorithm: Initialize the table array and size and head of the struct manger.*/
void Initialization(Table *T, ListM* P) {
	int i;
	for (i = 0; i < table; i++) //initialize the table array
	{
		T[i].head = NULL;
		T[i].bill = 0;
	}
	P->size = 0;
	P->head = NULL;
}

/*Function name: Error_Msg.
Inputs: char* msg
Output: void
Algorithm: Print message and exit progrem*/
void Error_Msg(char* msg){
	printf("\n%s", msg);
	exit(1);
}

/*Function name: PositiveNum.
Input: int num
Output: int
Algorithm: Check if the num is positive , if it is return 1 , else return 0.*/
int PositiveNum(int num) {
	if (num > 0)
		return 1;
	return 0;
}

/*Function name: CreateProduct.
Input: FILE* inM, ListM* P
Output: void
Algorithm: Function use dynamic alloction for temp in size of product`s struct, run until end of file and get data into a linke list add ech loop new node from the head of the list.
The function checks inputs vlidity and print appropiate message in case data is wrong.
At the and print to user if the data been saved successfully*/
void CreateProduct(FILE* inM, ListM* P) {
	char str[MAX];
	Product* temp;
	int flag = 0; //flag in order to know if was at lest one product that exist in the list but didn`t inset to menu.

	temp = (Product*)malloc(sizeof(Product)); //dynamic allocation to first Product in resturnd.
	if (temp == NULL)
		Error_Msg("Memoery alloction error.");

	//Get data from file and runs until reach to the end of the file.
	while (fscanf(inM,"%s %d %d %c", str,&temp->quantity, &temp->price, &temp->premium) != EOF)
	{
		if (PositiveNum(temp->price) == 1) {
			if (PositiveNum(temp->quantity) == 1) {
				P->size++;
				//Verify that is not the first scan and checks if the name of the product already exist.
				if (P->size != 1 && CheckIfNameExist(P, str) !=NULL) {
					printf("\nError! %s is already exist!\n", str);
					P->size--;
					flag = 1;
				}
				
				else { //if the name is not exist,we crate the name and start to enter into the menu.
					if (P->size == 1) { //in case this is the first scan
						P->head = temp;
						temp->next = NULL;
					}
					else { //in case is not the first "mana" which scan.
						temp->next = P->head;
						P->head = temp;
					}
					temp->ProductName = (char*)malloc(sizeof(char) * (strlen(str) + 1));
					if (temp->ProductName == NULL) {
						FreeList(P->head);
						Error_Msg("Memoery alloction error.");
					}
					else strcpy(temp->ProductName, str);
				}
				//Dynamic alloction for the next scan!!
				temp = (Product*)malloc(sizeof(Product));
				if (temp->ProductName == NULL)
				{
					FreeList(P->head);
					Error_Msg("Memoery alloction error.");
				}
			}
			else {
				printf("\nQuantity of prodact is wrong!\n");
			}
		}
		else {
			printf("\nPrice of prodact is wrong!\n");
		}
	}
	printf("\nAll the prodacts in the menu!!\n");
	if (flag) printf("Except for at least one product.\n");

	free(temp);
}


/*Function name: FreeList
Input:Product* head
Output: void
Algorithm: Gets a list and free all the nodes of the list
*/
void FreeList(Product* head) 
{
	Product* temp;
	while (head != NULL)
	{
		temp = head;
		head = head->next;
		free(temp->ProductName);
		free(temp);
	}

}



/*Function name: AddItem.
Input: ListM* P, char* ProdectName, int num
Output: void
Algorithm: Function add Quantity to exsisting prodact in case it exist else, print message and return to main.
The function save the adress of the node of the maching mana and update the Quantity value.*/
void AddItem(ListM* P, char *ProdectName, int num) {
	
	Product* temp;//save address
	if (PositiveNum(num) == 1) {
		if ((temp=CheckIfNameExist(P, ProdectName)) !=NULL){
			temp->quantity = ((temp->quantity) + num);
		}
		else {
			printf("\n%s is not exsit in the menu.\n", ProdectName);
			return;
		}
	}
	else {
		printf("\nQuantity is not valid, can not add this prodact!\n");
		return;
	}

	printf("\n%d manot of %s were added to the kitchen.\n", num, ProdectName);
}

/*Function name: CheckIfNameExists.
Input: ListM* P, char* str
Output: Product* (adress)
Algorithm: Check if string(ProdactName) is alredy exist and return the address of the current node and if not return NULL*/
Product* CheckIfNameExist(ListM* P, char* str) {
	Product* temp = P->head;
	while (temp != NULL) {
		if (strcmp(temp->ProductName, str) == 0)
			return temp; 
		temp = temp->next; //continue to the next node.
	}
	//in case there is`t mana in the same name, return NULL
	return NULL;   
}


/*Function name: free_tables
Input:Table* arr
Output: void
Algorithm:Funcio frees all the alloctions made in each table.
*/
void free_tables(Table* arr) { 
	int i=0;
	mana* temp;
	for (i = 0; i < table; i++) //free all tables
	{
			while (arr[i].head != NULL)
			{
				temp = arr[i].head;
				arr[i].head = temp->next;
				free(temp->ProductName);  
				free(temp);
			}
	}
}

/*Function name:OrderItem
Input: int table_num, char* ProductName, int quantity, Table* arr, ListM* PM
Output:void
Algorithm: Function attaches a dish from the kitchen to the specific table, adds the dish to the table by a new node to a linked list. Or updates the packet data on the table.
Additionally, the table's account is updated.
*/
void OrderItem(int table_num, char* ProductName, int quantity, Table* arr, ListM* PM) {

	mana* p_order, * temp_order, * order1;  //p_order= save the adrees of the node in the one waylist with the same productName.
											//temp_order= for dynamic alloction
	Product* temp = PM->head;				//in order to sech in the one-way list we crate.

	mana* loction_mana;			//save the adress of the node with the same name which alredy ordered to the table.
	int flag = 0;

	//verify validiy of table num.
	if (table_num > table || table_num < 1) {
		printf("\nThe table number is illegal.\n");
		return;
	}
	//verify validiy of table quantity.
	if (!PositiveNum(quantity)) {
		printf("\nThe number of the orders has illegal quantity.\n");
		return;
	}
	//Chack if the "mana exist in the kitchen.
	if ((p_order = CheckIfNameExist(PM, ProductName)) == NULL) {
		printf("\nError. The product %s not exist in the kitchen.\n", ProductName);
		return;
	}
	//check if the quantity exists in the kitchen
	if (quantity > p_order->quantity) {
		printf("\nThere are not enough manot of this product in the kitchen.\n");
		return;
	}

	//Chack if the "mana exist in the table.
	loction_mana = arr[table_num - 1].head;
	while ((loction_mana != NULL)&& (flag==0)) {
		if (strcmp(loction_mana->ProductName, ProductName) == 0) {
			loction_mana->quantity += quantity; //update the quantity of the mana in the table.(the rest of dishes`s parameters are the same).
			flag = 1;
		}
		loction_mana = loction_mana->next; //continue to the next node.
	}


	if(flag==0){ //is node exist create new node for new order 

		temp_order = (mana*)malloc(sizeof(mana));
		if (temp_order == NULL) {
			FreeList(PM->head); //free manu.
			free_tables(arr); //free all the tables .    
			Error_Msg("Memory alloction error.");
		}

		//in case it is the first node:
		if (arr[table_num - 1].head == NULL) { //flag in order
			//Initializations:
			temp_order->next = NULL;
			temp_order->prev = NULL;
			//link the temp_order to the list.
			arr[table_num - 1].head = temp_order;
		}
	//Handell a case is not the first node !!.
	//add a new node to the end of the list.
		else {
			order1 = arr[table_num - 1].head; //in order to run and find the last node!!
			while (order1->next != NULL) {
				order1 = order1->next;
			}
			//up to this point order1 have the last node which pouits of NULL.
			//we want to add to him the temporry node called: temp_order.
			order1->next = temp_order;
			temp_order->prev = order1;
			temp_order->next = NULL;
			
		}

		//Put data in the appropriate place.
		temp_order->price = p_order->price;
		temp_order->quantity = quantity;
		temp_order->premium = p_order->premium;

		temp_order->ProductName = (char*)malloc(sizeof(char) * (strlen(ProductName) + 1)); //dynamic allocation for a order`s name.
		if (temp_order->ProductName == NULL) {
			FreeList(PM->head);
			free_tables(arr); //free all the tables
			Error_Msg("Memory alloction error.");
		}
		else strcpy(temp_order->ProductName, ProductName);
	}

	arr[table_num - 1].bill += ((p_order->price) * quantity);

	//Update the quantity of product in the kitchen.
	p_order->quantity -= quantity;
	printf("\nTable number %d ordered: %d  |  %s  |  %d $ for one.\n", table_num, quantity,ProductName, arr[table_num - 1].head->price); //comment= i sent quantity and not a pointer in case the table order dish is already have.
	printf("The order is ready!! :)\n");
}



/*Function name: RemoveItem
Input: int table_num, char* ProductName, int quantity, Table* arr, ListM* P
Output: void
Algorithm:Function cancel dishes for a specific table, updates table`s data and bill. If case the order`s quantity is 0 (after the cancellation!!) the node of that packet is deleted from the linked list.
In addition, the function verifies parameter`s validiy.
*/
void RemoveItem(int table_num, char* ProductName, int quantity, Table* arr, ListM* P) {

	int found = 0; // flag indicate if mana exist in the table
	mana* order2cancel, *temp;
	mana* temp2;

	//verify validiy of table num.
	if (table_num > table || table_num < 1) {
		printf("\nTable number not exist in the restaurant.\n");
		return;
	}
	//verify validiy of table quantity.
	if (!PositiveNum(quantity)) {
		printf("\nThe number of the orders has illegal quantity.\n");
		return;
	}
	//Check if the table is an active table with orders.
	if (arr[table_num - 1].head == NULL) {
		printf("\nTable number %d not have any order.\n");
		return;
	}
	//Chack if the order to cancel exist in the kitchen.
	if ((CheckIfNameExist(P, ProductName)) == NULL) {
		printf("\nError. %s not exist in the kitchen.\n", ProductName);
		return;
	}

	order2cancel = arr[table_num - 1].head;
	//Check if the ProductName we want to cancel is exist in the requred table.	
	while ((order2cancel != NULL) && (found==0)) {

		//serch the ProductName
		if (strcmp(order2cancel->ProductName, ProductName) == 0) {
			found = 1;

			if (order2cancel->quantity < quantity) { //Verify validity of orders to cancel 
				printf("\nThis amount of orders to cancel is illega.\n");
				return;
			}
			else { //now order2cancel own the adress of the node with the mana we what to cancel in the table.
				
				order2cancel->quantity -= quantity;	//update the quantity of the orders			
				arr[table_num - 1].bill -= (quantity * (order2cancel->price ));//update the bill of the table

				if (order2cancel->quantity == 0) { //if the sum of orders in this mana is zero orders!!
					temp = arr[table_num - 1].head;
					if (order2cancel == temp) { //if order2cancel is the first node

						temp = order2cancel->next;
						if (temp != NULL) {
							temp->prev = NULL;  //handell a case that the order2cancel is not the only node.
							order2cancel->next = NULL;
							arr[table_num - 1].head = temp;
						}
						else arr[table_num - 1].head = NULL; //Initialization
					}
					else {//if order2cancel is NOT the first node
						if (order2cancel->next != NULL) { //if the node in the midlle.
							temp = order2cancel->prev;
							temp->next = order2cancel->next;
							order2cancel->prev = NULL;
							order2cancel->next = NULL;
							temp2 = temp->next;
							temp2->prev = temp;
							
						}
						else {// handlle the node to cancel is the last node in the list.
							temp = order2cancel->prev;
							order2cancel->prev = NULL;
							temp->next = NULL;
						}
					}
					free(order2cancel->ProductName);
					free(order2cancel);
				}
				
			}
			
		}else order2cancel = order2cancel->next;	
	}

	if (found == 0) {
		printf("\nThe order:  | %s |   not exist in table number %d.\n", ProductName, table_num);
		return;
	}
	else {
		printf("\nTable number %d returned %d dishes of %s.", table_num, quantity, ProductName);
		printf("\nThe updated bill is: %d", arr[table_num - 1].bill);
	}
}



/*Function name:RemoveTable
Input: int table_num, Table* arr
Output: void
Algorithm: Function performs "close" a table. Displays a report of all the details which ordered, a payment amount and indicates whether or not it is a premium table. 
(For the calculation of the table's account).
The function releases the linked list and verifies parameter`s validiy.
*/
void RemoveTable(int table_num, Table* arr) {

	mana* temp;
	int i, flag=0;
	float bill=0;
	//verify validiy of table num.
	if (table_num > table || table_num < 1) {
		printf("\nTable number not exist in the restaurant.\n");
		return;
	}
	//check if the table have orders
	if (arr[table_num-1].head == NULL) {
		printf("\nThe table has no orders\n");
		return;
	}
	//serch the mana in the table.
	temp = arr[table_num - 1].head;
	while (temp != NULL){ //check if the table ordered a primium dishes
		if (temp->premium == 'Y')
			flag = 1;
		temp = temp->next;
	}
	
	printf("\nTable %d orders:\n", table_num);
	while (arr[table_num-1].head != NULL) //print all the orderes and free the table
	{
		printf("%s | quantity: %d | %d $ for one | %c\n", arr[table_num-1].head->ProductName, arr[table_num-1].head->quantity, arr[table_num-1].head->price, arr[table_num-1].head->premium);
		temp = arr[table_num-1].head;
		arr[table_num - 1].head = arr[table_num-1].head->next;
		temp->prev = NULL;
		free(temp->ProductName);
		free(temp);
	}

	if (flag == 1){ //in case the table is premium

		bill = ((arr[table_num - 1].bill)+((arr[table_num - 1].bill) / 10.0));
		printf("\nTable number |%d| is a PREMIUM table, need to add extra 10.\nThe bill is: %.2f\n\n", table_num, bill);
	}
	else {
		printf("The bill is: %.2f\n\n", bill= arr[table_num - 1].bill);
	}

	arr[table_num - 1].bill = 0;
	arr[table_num - 1].head = NULL;
}

