#include <stdio.h>
#include <stdlib.h>
#define B 10
//struct linked list
struct node{
    int data;
    struct node *next;
};
//Prototypes for all functions
void insertNode(struct node **head, struct node **value);
void removeNode(struct node **head, struct node **value);
void lengthList(struct node **head);
void printList(struct node **head);
void deleteList(struct node **head);
void radixSort(struct node **head, struct node **list, int N, int *pass);
void initializeValues(struct node **list, int N, int high, int low, int *pass);
void initializeBuckets(struct node **head);
//Main Function
int main(int argc, char *argv[]){
    if (argc != 5){//Checks that correct number of arguments are given
        printf("Error: Incorrect Argument Input!\n");//prints error message if incorrect
        exit(0);//exits program
    }
    
    //Variable - for loop
    int i;
    //Takes seed argument puts it in variable and runs srandom with seed
    int seed;
    sscanf(argv[1],"%d",&seed);
    srandom(seed);
    //Takes number argument and puts it in variable
    int N;
    sscanf(argv[2],"%d",&N);
    //Takes low argument and puts it in variable
    int low;
    sscanf(argv[3],"%d",&low);
    //Takes high argument and puts it in variable
    int high;
    sscanf(argv[4],"%d",&high);
    //Creates an Array for the Bucket Heads
    struct node *headList[10];
    initializeBuckets(headList); //Calls initializeBuckets function create the heads
    
    int pass = 0;//Variable will hold number of passes
    struct node *listHead = NULL; //Creates List Head
    initializeValues(&listHead, N, high, low, &pass); //Calls initializeValues function to create the list

    radixSort(headList, &listHead, N, &pass);//Calls radixSort function to sort the list
    //Deletes all Buckets
    for (i=0; i<B; i++){
	deleteList(&headList[i]);
    }
    //Deletes the list
    deleteList(&listHead);
}
void insertNode(struct node **head, struct node **value){ //takes node & adds it to end of list value
    struct node *add;//Pointer
    struct node *new;//Pointer
    add = *head;
    new = *value;
    //Checks that next value isnt Null then moves to that value
    while (add->next != NULL){
        add = add->next;
    }
    new->next = NULL;//Makes next value NULL
    add->next = new;//Links new value to end of the given list
}
void removeNode(struct node **head, struct node **value){ //takes in list head sets head equal to next value, takes bucket sets bucket equal to next value
    struct node *removed;//Pointer
    removed = *head;
    removed->next = *value;//Makes given list skip to the next node
}
void lengthList(struct node **head){
    struct node *val;//Pointer
    val = *head;
    int count = 1;//Counts length
    //Checks that next value isnt NULL
    if(val->next == NULL){
    	count--;//Removes list head from count
    }
    //Checks that next value isnt NULL
    while(val->next != NULL){
        val = val->next;//Moves to next value
        count ++;//Increments counter
    }
    printf("List Length: %d\n", count);//Prints Length
}
void printList(struct node **head){ //Takes the Head goes through the list & prints out the values
    struct node *val;//Pointer
    val = *head;
    //Checks that next value isnt NULL
    if(val->next != NULL){
    	val = val->next;//Moves to next value
	//Checks that next value isnt NULL
    	while(val->next != NULL){
        	val = val->next;//Moves to next value
        	printf("%d,", val->data);//Prints Data
    	}
    	printf("%d", val->data);//Prints data
    }
    printf("\n");//Prints new line
}
void deleteList(struct node **head){ //Takes the Head goes through the list & deletes the values
    struct node *temp;//Pointer
    struct node *val;//Pointer
    val = *head;
    //Checks that next value isnt NULL
    while(val->next != NULL){
        temp = val;
        val = val->next;//Moves to next value
        temp->next = NULL;//Sets link next value to NULL
        free(temp);//Frees current value
    }
    free(val);//Frees last value
}
void radixSort(struct node **head, struct node **list, int N, int *pass){
    //Initialize Variables
    int i;
    int j;
    int k;
    int digit;
    //Pointers
    struct node *sort;
    struct node *prev;
    //Goes through the Passes
    for (i=0; i<*pass; i++){
	printf("Pass #: %d\n",(i+1)); //Prints Pass #
	//Goes through the list & adds to buckets
	for (j = 0; j < N; j++){
            sort = *list;
            sort = sort->next;//Moves to next value
            digit = sort->data;//Sets data
            //Finds the required digit
	    for(k = 0; k<i; k++){
                    digit /= B;
            }
	    digit %= B;
            
	    prev = sort;
	    //Checks if next value is NULL & either moves to next value or is NULL
	    if (sort->next == NULL){            
	        sort = NULL;//Last Value Reached
	    }else{
	    	sort = sort->next;
            }
	    //Calls insert & remove functions to add and remove node from lists
            insertNode(&head[digit], &prev);
	    removeNode(list, &sort);
        }
	//Goes through buckets & adds to list
        for (j = 0; j < B; j++){
	    //Prints Bucket #, length & content
            printf("Bucket: %d - ",j);
	    lengthList(&head[j]);
	    printList(&head[j]);

	    sort = head[j];
	    //Checks if next value is NULL & either moves to next value or is NULL
	    if (sort->next == NULL){
	        continue; //bucket list is empty
	    }else{
                sort = sort->next;
	    }
	    //Adds all values in current bucket to list
            while (sort ->next != NULL){
		prev = sort;                
		sort = sort->next;
		//Calls insert & remove functions to add and remove node from lists
                insertNode(list,&prev);        
		removeNode(&head[j],&sort);    
            }
	    //Last value has been reached
            insertNode(list,&sort);
	    sort = NULL;//end of list
	    removeNode(&head[j],&sort);
        }
	//Prints new list content
	printf("\nNew List:");
	printList(list);
    }
}
void initializeValues(struct node **list, int N, int high, int low, int *pass){
    //Variables
    int i = 0;
    int curVal = 0;
    //Adds numbers to list
    for (i=0; i<=N; i++){
	//List Head data will be 0
        if(i == 0){
        }else{//Other vallues will be added randomly
            curVal = (random()%((high+1)-low))+low; //generate random number from low to high (inclusive);
        }
        struct node *temp = (struct node *)malloc (sizeof(struct node)); //Allocates memory
        if (temp ==NULL){//Checks memory is allocated 
            printf("Error: Memory allocation failed!\n");
            exit(1);
        }
	//Adds values to node
        temp->data = curVal;
        temp->next = NULL;
	//Checks if list Head is NULL
        if (*list ==NULL){
            *list = temp;//List Head values added
        }
        else{
            struct node *add;//Pointer
            add = *list;
	    //Goes through list if next isnt NULL
            while(add->next != NULL){
                add = add->next;//Moves to next node
            }
            add->next = temp;//Adds new node at the end
        }
        
        //Find largest number
        if (curVal > *pass){
            *pass = curVal;
        }
    }
    //calculate passing
    for(i=0; *pass > 0; i++){
        *pass /= B;
    }
    *pass = i;
    //Prints list, length, contents, and amount of passings
    printf("Initialized");
    lengthList(list);
    printList(list);
    printf("This List will pass  %d time/s\n",i);
}
void initializeBuckets(struct node **head){
   //Variable
   int i;
   //Creates Bucekts from 0-9
   for (i=0; i<B; i++){
        struct node *temp = (struct node *)malloc (sizeof(struct node)); //Allocates memory
        if (temp ==NULL){//Checks memory is allocated
            printf("Error: Memory allocation failed!\n");
            exit(1);
        }
        temp->data = i; //Sets new value to temp
        temp->next = NULL; //Head of list has no link
        head[i] = temp;//Head of list added
        }
}
