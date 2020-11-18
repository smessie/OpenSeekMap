#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "database.h"
#include "query_handler.h"
#include "algorithm.h"
#include "synergy.h"
#include "sort_util.h"
#include "strings/string_util.h"

int main(int argc, char** argv) {
    printf("Project AD3 ~ Welcome to OpenSeekMap!\n");

    // Validate function call
    if (argc != 2 && argc != 4) {
        printf("Usage: ./openseekmap <database file> [longitude] [latitude] < <query>\n");
        return EXIT_FAILURE;
    }

    // Read query's
    QueryCollection* qc = read_queries();

    // Load database
    char database_name[64];
    strcpy(database_name, argv[1]);
    printf("Loading database '%s'...\n", database_name);
    Database* database = load_database(database_name, qc->max_length);

    // Database is loaded, so report to user by sending question mark.
    printf("?\n");

    // Get optional longitude and latitude
    double longitude;
    double latitude;
    if (argc == 4) {
        longitude = strtod(argv[2], NULL);
        latitude = strtod(argv[3], NULL);
        printf("Database: %s; Logitude: %f; Latitude: %f\n", database_name, longitude, latitude);
    }

    // Process query's
    Query* query = qc->head;
    while (query != NULL) {
        BestMatches* best_matches = (BestMatches*) calloc(1, sizeof(BestMatches));
        QueryBreakdownCollection* qbc = create_query_breakdown_collection(query->value);

        // For each of the breakdowns, we look for total matches.
        QueryBreakdown* breakdown = qbc->head;
        while (breakdown != NULL) {
            // Each QueryBreakdown has a collection of total matches.
            TotalMatchCollection* tmc = calculate_query_breakdown_total_matches(breakdown, database);
            calculate_all_total_matches_correctness(tmc);
            calculate_all_total_matches_synergy(tmc);

            if (argc == 4) {
                calculate_best_matches_geo(best_matches, tmc, latitude, longitude);
            } else {
                calculate_best_matches(best_matches, tmc);
            }

            free_total_match_collection(tmc);
            breakdown = breakdown->next;
        }

        // The 5 best matches are now in `best_matches`, ready to print out to the user.
        print_result(best_matches->match_1);
        print_result(best_matches->match_2);
        print_result(best_matches->match_3);
        print_result(best_matches->match_4);
        print_result(best_matches->match_5);

        printf("?\n");

        // Free all stuff we do not need anymore and move on to the next query.
        free_query_breakdown_collection(qbc);
        free_best_matches(best_matches);
        query = query->next;
    }

    // End program
    free_query_collection(qc);
    free_database(database);
    fclose(stdin);
    fclose(stdout);
    fclose(stderr);
    return EXIT_SUCCESS;
}
