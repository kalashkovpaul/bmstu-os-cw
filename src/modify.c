#include <stdio.h>

#define OLD_FILENAME "../protected"
#define NEW_FILENAME "../renamed"

int main() {
    int status;

    // Use the rename function
    status = rename(OLD_FILENAME, NEW_FILENAME);

    // Check if the rename was successful
    if (status == 0) {
        printf("File renamed successfully.\n");
    } else {
        printf("Unable to rename the file.\n");
    }

    return 0;
}