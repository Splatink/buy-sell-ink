#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

//values below control random item/customer selections, change for debugging (illegal items only 7-9, default 0-9)
#define ITEM_LOWER_BOUND 0
#define ITEM_UPPER_BOUND 9
#define CUSTOMER_LOWER_BOUND 0
#define CUSTOMER_UPPER_BOUND 9

//values below control the police random chances, out of a 100
#define POLICE_SHOOT_SUCCESS 20
#define POLICE_DECLINE_SUCCESS 50

//values below control the police fines, and when they will get suspicous or attempt to arrest the player
#define POLICE_FINE_NORMAL 400
#define POLICE_FINE_INCREASE 100
#define POLICE_WARNING 1 //warn at X amount of illegal items sold
#define POLICE_ARREST 3 //attempt arrest at X amount of illegal items sold

//values below control how the player starts the game, default is 100 gold and 1 banana
#define PLAYER_STARTING_MONEY 100

struct gameItem
{
    char itemName[10];
    int itemCurrentItemPrice;
    int itemStartingPrice;
    int itemWiggleRoom;
    bool itemInInventory;
};

struct gameCustomer
{
    char customerName[10];
    int customerWiggleRoom; //should be a randomly generated number
    bool customerRecentlyDoneDeal;
};

struct gamePlayer
{
    char playerName[10];
    int playerScore;
    int playerMoney;
    int totalItemsSold;
    int illegalItemsSold;
    bool warrantForArrest;
    bool gameForceEnd;
    bool playerNotified;
    bool gameKilled;
};

struct gameItem item[10] =
{
    {"Banana", 5, 5, 2, 1},
    {"Music CD", 10, 10, 5, 0},
    {"Book", 25, 25, 10, 0},
    {"Jacket", 50, 50, 20, 0},
    {"Headphones", 150, 150, 20, 0},
    {"TV", 400, 400, 20, 0},
    {"Smartphone", 0, 1200, 0, 0},
    //special items
    {"Ink", 65, 65, 35, 0},
    {"Weed", 12, 12, 60, 0},
    {"Crypto", 150, 100, 500, 0}
};

struct gameCustomer customer[10] =
{
                {"George", 0, false},
                {"Mandy", 0, false},
                {"Teo", 0,false},
                {"Benji", 0, false},
                {"Xavier", 0, false},
                {"Joey", 0, false},
                {"Kurt", 0, false},
                {"Armando", 0, false},
                {"Nick", 0, false},
                {"Shelly", 0, false}
};

struct gamePlayer player;

void initCustomers()
{
        srand((unsigned int) time(NULL));
        for (int i = 0; i < 10; i++)
        {
            customer[i].customerWiggleRoom = rand() % 100 + 1;
        }
}

void debugInfo()
{
    for (int i = 0; i < 10; i++)
    {
        printf("%s\n", customer[i].customerName);
        printf("%d\n", customer[i].customerWiggleRoom);
        printf("%d\n", customer[i].customerRecentlyDoneDeal);
    }
    for (int i = 0; i < 10; i++)
    {
        printf("%s\n", item[i].itemName);
        printf("%d\n", item[i].itemCurrentItemPrice);
        printf("%d\n", item[i].itemStartingPrice);
        printf("%d\n", item[i].itemWiggleRoom);
    }
}

void gameIntro()
{
    printf("Welcome to a game about buying and selling items.\n");
    printf("It's simple, buy items and sell them to your customers for a profit.\n");
    printf("The game ends after your chosen amount of turns and you get a score!\n");
    printf("Please enter your player name (Max 10 characters)\n");
    scanf("%s", &player.playerName);
    player.playerMoney = PLAYER_STARTING_MONEY;
    printf("You start with %d gold and 1 %s\n", player.playerMoney, item->itemName);
}

void printStats()
{
    printf("Name: %s\nGold: %d\n", player.playerName, player.playerMoney);
}

void printInventory()
{
    for (int i = 0; i < 10; i++)
    {
        if (item[i].itemInInventory)
        {
            printf(item[i].itemName);
            printf("\n");
        }
    }
}

void printItems()
{
    printf("Current list of items:\n");
    int busnPrice;
    for (int i = 0; i < 10; i++)
    {
        busnPrice = item[i].itemStartingPrice / 1.5;
        printf("N: %d I: %s P: %d Gold\n", i, item[i].itemName, busnPrice); 
    }
}

bool buyItem(int itemNumber)
{
    int busnItemPrice = item[itemNumber].itemStartingPrice / 1.5;
    if (!item[itemNumber].itemInInventory && (player.playerMoney >= busnItemPrice))
    {
        item[itemNumber].itemInInventory = true;
        player.playerMoney -= busnItemPrice;
        return true;
    }
    else
    {
        return false;
    }
}

void buyItems()
{
    printf("What item would you like to buy? Type X to exit\n");
    printItems();
    while (true)
    {
        int selection = getchar();
        if (selection == 'X' || selection == 'x')
        {
            return;
        }
        if (selection >= '0' && selection <= '9')
        {
            int index = selection - '0';
            if(buyItem(index))
            {
                printf("Item bought\n");
            }
            else
            {
                printf("You already have this item or you can't afford it\n");
            }
        }
    }   
}

bool checkForItem(int itemPicked)
{
    if (item[itemPicked].itemInInventory == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void policeAccept()
{
    printf("You were arrested by the police and issued a %d gold fine\n");
    player.playerMoney -= POLICE_FINE_NORMAL;
    player.illegalItemsSold = 0;
}

void policeDecline()
{
    int randomness = rand() % 100 + 1;
    if (randomness > POLICE_DECLINE_SUCCESS)
    {
            printf("You have been arrested by the police and have been fined %d gold, and an additional %d gold for resisting arrest\n", POLICE_FINE_NORMAL, POLICE_FINE_INCREASE);
            player.playerMoney -= (POLICE_FINE_NORMAL + POLICE_FINE_INCREASE);
            player.illegalItemsSold = 0;
            return;
    }
    else
    {
        printf("Alright then, since we have no evidence we will get a warrant\n");
        player.warrantForArrest = true;
        return;
    }
}

void policeShoot()
{
    int randomness = rand() % 100 + 1;
    if (randomness > POLICE_SHOOT_SUCCESS)
    {
            printf("You were shot and killed by the police\n");
            player.gameForceEnd = true;
            player.gameKilled = true;
            return;
    }
    else
    {
        printf("You shot and killed the police, you escaped this time\n");
        player.illegalItemsSold = 0;
        return;
    }

}

void policeUI()
{
    printf("A: Accept\nD: Decline\nS: Shoot Police\n");
    while (true)
    {
        int selection = getchar();
        switch (selection)
        {
            case 'A':
                policeAccept();
                return;
            case 'D':
                policeDecline();
                return;
            case 'S':
                policeShoot();
                return;
        }
    }

}

void warrantUI()
{
    printf("A: Accept\nS: Shoot Police\n");
    while (true)
    {
        int selection = getchar();
        switch (selection)
        {
            case 'A':
                policeAccept();
                return;
            case 'S':
                policeShoot();
                return;
        }
    }
}

void checkPolice()
{
    if (player.warrantForArrest)
    {
        printf("Police: %s, WE HAVE A WARRANT FOR YOUR ARREST. PUT YOUR HANDS BEHIND YOUR BACK AND INTERLOCK YOUR FINGERS\n", player.playerName);
        warrantUI();
        return;
    }
    if (player.illegalItemsSold == POLICE_WARNING && player.playerNotified == false)
    {
        printf("You have sold an illegal item, if you keep this up the police might start asking questions\n");
        player.playerNotified = true;
    }
    else if (player.illegalItemsSold >= POLICE_ARREST)
    {
        printf("Police: You seem to be selling contraband, you are under arrest!\n");
        policeUI();
    }
}

void checkMoney()
{
    if (player.playerMoney <= 0)
    {
        printf("You have no money, you lose unless you can sell your inventory\n");
        player.playerMoney = 0;
        player.gameForceEnd = true;
    }
    else
    {
        player.gameForceEnd = false;
    }
}

int gameUI(int itemPicked)
{
    printf("A: Accept\nN: Negotiate\nD: Decline\nS: Print Stats\nI: Print Inventory\nB: Buy Items\n");
    while (true)
    {
        int selection = getchar();
        switch (selection)
        {
            case 'A':
                if (checkForItem(itemPicked))
                { 
                    return 1;
                }
                else
                {
                    printf("You don't have this item, buy it with B\n");
                    break;
                }
            case 'N':
                if (checkForItem(itemPicked))
                {
                    return 2;
                }
                else
                {
                    printf("You don't have this item, buy it with B\n");
                    break;
                }
            case 'D':
                return 3;
            case 'S':
                printStats();
                break;
            case 'I':
                printInventory();
                break;
            case 'B':
                buyItems();
                printf("A: Accept\nN: Negotiate\nD: Decline\nS: Print Stats\nI: Print Inventory\nB: Buy Items\n");
                break;
        }
    }
}

bool checkCustomerWiggle(int potNewItemPrice, int customerPicked, int itemPicked)
{
    int randomness = rand() % 100 + 1;
    float wiggleness = (float)randomness / (float)customer[customerPicked].customerWiggleRoom;
    float negotionness = ((float)potNewItemPrice - (float)item[itemPicked].itemCurrentItemPrice) / (float)item[itemPicked].itemWiggleRoom;
    if ((potNewItemPrice - item[itemPicked].itemCurrentItemPrice) <= 0)
    {
        item[itemPicked].itemCurrentItemPrice = potNewItemPrice;
        return true;
    }
    else if (wiggleness <= 1 && negotionness <= 1)
    {
        item[itemPicked].itemCurrentItemPrice = potNewItemPrice;
        return true;
    }
    else
    {
        return false;
    }
}

void sellItem(int itemPicked)
{
    player.playerMoney += item[itemPicked].itemCurrentItemPrice;
    player.playerScore++;
    item[itemPicked].itemInInventory = false;
    player.totalItemsSold++;
    printf("Sold!\n\n");
    if (itemPicked >= 7)
    {
        player.illegalItemsSold++;
    }
}

void negotiateItem(int itemPicked, int customerPicked)
{
    int potNewItemPrice;
    printf("%s: Aight, what are we talking?\n", customer[customerPicked].customerName);
    printf("Enter desired item price:\n");
    scanf("%d", &potNewItemPrice);
    if (checkCustomerWiggle(potNewItemPrice, customerPicked, itemPicked))
    {
        player.playerMoney += item[itemPicked].itemCurrentItemPrice;
        player.playerScore += 2;
        item[itemPicked].itemInInventory = false;
        player.totalItemsSold++;
        printf("Sold!\n\n");
        if (itemPicked >= 7)
        {
        printf("You have sold an illegal item. Keep it up and the police might start asking questions");
        player.illegalItemsSold++;
        }
    }
    else
    {
        printf("%s: Nah, think I'll pass\n\n", customer[customerPicked].customerName);
        if(player.playerScore != 0)
        {
            player.playerScore -= 1;
        }
    }
}

void gameTurn()
{
    checkPolice();
    if (player.gameKilled)
    {
        return;
    }
    checkMoney();
    int itemPicked = rand() % (ITEM_UPPER_BOUND - ITEM_LOWER_BOUND + 1) + ITEM_LOWER_BOUND;
    int customerPicked = rand() % (CUSTOMER_UPPER_BOUND - CUSTOMER_LOWER_BOUND + 1) + CUSTOMER_LOWER_BOUND;
    item[itemPicked].itemCurrentItemPrice = item[itemPicked].itemStartingPrice; //reset item prices for negotiation; stupid solution
    printf("%s: I need %s, willing to pay %d gold for it.\n", customer[customerPicked].customerName, item[itemPicked].itemName, item[itemPicked].itemCurrentItemPrice);
    int selection = gameUI(itemPicked);
    switch (selection)
    {
        case 1:
            sellItem(itemPicked);
            break;
        case 2:
            negotiateItem(itemPicked, customerPicked);
            break;
        case 3:
            printf("%s: Alright then, maybe some other time\n\n", customer[customerPicked].customerName);
            break;
    }
}

int turnSelector()
{
    printf("Please select amount of turns\n");
    int turns;
    scanf("%d", &turns);
    return turns;
}

void printExtStats()
{
    printf("Name: %s\nGold: %d\nScore: %d\nTotal Items Sold: %d\n", player.playerName, player.playerMoney, player.playerScore, player.totalItemsSold);
}

void gameEnd(int turns)
{
    printf("Game End!\n");
    printf("Total turns played: %d\n", turns);
    printf("Player Stats:\n");
    printExtStats();
}

int main()
{
    initCustomers();
    gameIntro();
    int turns;
    int numOfTurns = turnSelector();
    for (turns = 0; turns < numOfTurns; turns++)
    {
        if (player.gameForceEnd)
        {
            break;
        }
        else
        {
            gameTurn();
        }
    }
    gameEnd(turns);
    return 0;
}