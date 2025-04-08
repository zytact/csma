#include <stdio.h>
#include "one_persistent.h"
#include "non_persistent.h"
#include "p_persistent.h"

int main()
{
    int choice;
    float p_value;

    printf("Select CSMA Implementation:\n");
    printf("1. One Persistent CSMA\n");
    printf("2. Non-Persistent CSMA\n");
    printf("3. P-Persistent CSMA\n");
    printf("Enter your choice (1-3): ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        printf("Running One Persistent CSMA\n");
        one_persistent();
        break;
    case 2:
        printf("Running Non-Persistent CSMA\n");
        non_persistent();
        break;
    case 3:
        printf("Enter p value (0.0-1.0): ");
        scanf("%f", &p_value);
        printf("Running P-Persistent CSMA with p = %.2f\n", p_value);
        p_persistent(p_value);
        break;
    default:
        printf("Invalid choice! Exiting...\n");
        break;
    }

    return 0;
}