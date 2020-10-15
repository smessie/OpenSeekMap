#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
   printf("Project AD3 ~ Welcome to OpenSeekMap!\n");

   // Validate function call
   if (argc != 2 && argc != 4) {
       printf("Usage: ./openseekmap <database file> [longitude] [latitude] < <query>\n");
       return EXIT_FAILURE;
   }

   // Load database
   char database[64];
   strcpy(database, argv[1]);
   printf("Loading database '%s'...\n", database);

   // Get optional longitude and latitude
   double longitude;
   double latitude;
   if (argc == 4) {
       longitude = strtod(argv[2], NULL);
       latitude = strtod(argv[3], NULL);
       printf("Database: %s; Logitude: %f; Latitude: %f\n", database, longitude, latitude);
   }

   // Read query's
   char str[100];
   while (scanf("%[^\n]%*c", str) == 1) {
       printf("You entered: %s\n", str);
   }
   return EXIT_SUCCESS;
}
