#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct gameItem
{
    char itemName[10];
    float itemCurrentItemPrice;
    float itemStartingPrice;
    unsigned int itemWiggleRoom;
};

struct gameCustomer
{
    char customerName[10];
    float customerWiggleRoom; //should be a randomly generated number
    bool customerRecentlyDoneDeal;
};

struct gameItem item[10] =
{
    {"Banana", 5, 5, 2},
    {"Music CD", 10, 10, 5},
    {"Book", 25, 25, 10},
    {"Jacket", 50, 50, 20},
    {"Headphones", 150, 150, 20},
    {"TV", 400, 400, 20},
    {"Smartphone", 0, 1200, 0},
    //special items
    {"Snake Oil", 65, 65, 150},
    {"Weed", 12, 12, 300},
    {"Crypto Wallet", 150, 150, 500}
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


void initCustomers()
{
        for (int i = 0; i < 10; i++)
        {
            customer[i].customerWiggleRoom = rand() % 100 + 1;
        }
}

int main()
{
    return 0;
}