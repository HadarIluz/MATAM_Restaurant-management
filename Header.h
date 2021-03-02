#ifndef _main_function
#define _main_function

#define _CRT_SECURE_NO_WARNINGS

//Libraries:
#include<stdio.h>
#include<string.h>
#include<string.h>

//define:
#define  table  15  //The number of tables in the restaurant.
#define MAX 100		//For max chars in tempprery name before malloc. (in order to get name from file).

//Structures:
//One-Way list!!.->
typedef struct Product  //struct for each one of the "manot" in the file.
{
	char* ProductName;
	int price;
	int quantity;
	char premium;
	struct Product* next;
}Product;

typedef struct ListM //struct menager to struct Product
{
	Product* head;
	int size;
}ListM;


typedef struct Table
{
	struct mana* head;
	int bill;
}Table;

//For case 3: For Two-way list!!<->
typedef struct mana //Each table`s data.
{
	char* ProductName;
	int price;
	int quantity;
	char premium;
	struct mana* next;
	struct mana* prev;
} mana;

//Functions declaration:
void Initialization(Table* T, ListM* P);
void CreateProduct(FILE* inM, ListM* p);  //for case 1.
int PositiveNum(int num);		
void Error_Msg(char* msg);
void FreeList(Product* head);
Product* CheckIfNameExist(ListM* P, char* str); //used in cases 1,2
void AddItem(ListM* P, char* ProdectName, int num); //for case 2.
void OrderItem(int table_num, char* ProductName, int quantity, Table* arr, ListM* PM); //for case 3
void free_tables(Table* arr);
void RemoveItem(int table_num, char* ProductName, int quantity, Table* arr, ListM* PM); //for case 4
void RemoveTable(int table_num, Table* arr); //for case 5.



#endif