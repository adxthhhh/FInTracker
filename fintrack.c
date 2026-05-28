#include<stdio.h>
#include<time.h>

#define MAX_TRANSACTIONS 1000 // Defines our memory limit

// We use 'typedef' so we can just type 'Transaction' later instead of 'struct Transaction'
typedef struct {
    char date[11];        // Format: YYYY-MM-DD (10 chars + 1 hidden null terminator '\0')
    char time[6];         // Format: HH:MM (5 chars + 1 hidden null terminator '\0')
    char description[50]; // e.g., "Groceries"
    float amount;         // e.g., 500.50
    char type;            // 'C' for Credit (Income), 'D' for Debit (Expense)
} Transaction;

int main() {

    // 1. Declare our memory array (The Ledger)
    Transaction ledger[MAX_TRANSACTIONS]; 
    
    // 2. Keep track of how many items are actually in the ledger
    int current_count = 0; 


    int choice; // Declaration of the choice value
    
    // Dummy variables just so our printf statements compile for now
    float income = 0.0;
    float expense = 0.0;
    float balance = 0.0;

    do {
        // --- UI ---
        printf("\nWelcome to FinTrack\n");
        printf("------------------------\n");
        printf("|\tMonthly\t\t|\n");
        printf("------------------------\n");
        printf(" Income  : %.1f \n", income);
        printf(" Expense : %.1f \n", expense);
        printf(" Balance : %.1f \n", balance);
        printf("------------------------\n");
        
        printf("\n \tMenu\t \n");
        printf("1. Add New Transaction \n");
        printf("2. Show Full Balance sheet \n");
        printf("3. Show Full Dashboard \n");
        printf("4. Settings \n");
        printf("5. Exit \n\n");
        
        printf("Your input (choice): ");
        scanf("%d", &choice);

        // --- Logic ---
        switch(choice) {
            case 1:
                printf("\n>>> Adding new Transaction <<<\n");
                
                // 1. Grab the raw system time
                time_t t = time(NULL);
                
                // 2. Convert it to local time format
                struct tm tm = *localtime(&t);

                // 3. Format and save the Date (YYYY-MM-DD) directly into our ledger array
                strftime(ledger[current_count].date, sizeof(ledger[current_count].date), "%Y-%m-%d", &tm);
                
                // 4. Format and save the Time (HH:MM) directly into our ledger array
                strftime(ledger[current_count].time, sizeof(ledger[current_count].time), "%H:%M", &tm);

                // Show the user the auto-filled data
                printf("Date auto-filled: %s\n", ledger[current_count].date);
                printf("Time auto-filled: %s\n", ledger[current_count].time);

                printf("Transaction Description:  ");
                scanf("%s",ledger[current_count].description);
               
                printf("\n Enter the amount:  ");
                scanf("%f",&ledger[current_count].amount);
 
                printf("\n Credit/Debit (Enter C or D):  ");
                scanf(" %c",&ledger[current_count].type);
                
                // Update Dashboard Totals
                if (ledger[current_count].type == 'C' || ledger[current_count].type == 'c') {
                     income = income + ledger[current_count].amount;
                } else if (ledger[current_count].type == 'D' || ledger[current_count].type == 'd') {
                     expense = expense + ledger[current_count].amount;
                }
                
                // Calculate balance
                balance = income - expense;
                  
                //FILE saving logic
                //Open a file in append mode ( Creates the file if it does not exist )
                
                FILE *file = fopen("fintrack.csv","a");
                
                //Checking if file opened successfully
                if(file == NULL){
                    printf("Error : Could not open database file! \n");
                }else{
                //Write the data separated by comma
                fprintf(file,"%s,%s,%s,%.2f,%c \n",
                        ledger[current_count].date,
                        ledger[current_count].time,
                        ledger[current_count].description,
                        ledger[current_count].amount,
                        ledger[current_count].type);
                        
                fclose(file);
                
                printf("Transactions successfully saved to fintrack.csv! \n");
                }      
              
                // Increment the counter so the next transaction moves to the next slot!
                current_count++; 

                break; // Stops it from bleeding into case 2

            case 2:
                printf("\n>>> Opening Balance sheet <<<\n");

                // 1. The 'if' statement: Check if empty
                if (current_count == 0) {
                    printf("No transactions found! Please add some first.\n");
                    break; 
                }

                // Print a nice header for our table
                printf("\nDate       | Time  | Description     | Amount   | Type\n");
                printf("--------------------------------------------------------\n");

                // 2. The 'for' loop: Notice how 'int i = 0' is declared right here!
                for (int i = 0; i < current_count; i++) {
                    
                    // 3. Print the data pointing to row [i]
                    printf("%s | %s | %-15s | %8.2f | %c\n", 
                           ledger[i].date, 
                           ledger[i].time, 
                           ledger[i].description, 
                           ledger[i].amount, 
                           ledger[i].type);
                }
                
                printf("--------------------------------------------------------\n");
                break;

            case 3:
                printf("\n>>> Opening Dashboard <<<\n");
                break;
            case 4:
                printf("\n>>> Opening settings <<<\n");
                break;
            case 5:
                printf("\n>>> Exiting FinTrack... Goodbye! <<<\n");
                break;
            default:
                // Triggers if they type 6, 99, etc.
                printf("\n>>> Enter correct option! <<<\n"); 
                break;
        }

    } while (choice != 5);

    return 0;
}
