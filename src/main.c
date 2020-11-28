#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>

#include <time.h>

#include "database.h"
#include "query_handler.h"
#include "algorithm.h"
#include "synergy.h"
#include "sort_util.h"
#include "strings/string_util.h"

int main(int argc, char** argv) {
    clock_t t = clock();
    // Validate function call
    if (argc < 2 || argc > 6) {
        print_usage();
        return EXIT_FAILURE;
    }

    // Handle algorithm options.
    int argument_boost = 0;
    int time_flag = 0;
    int algorithm = 0;
    int c;
    while (1) {
        struct option long_options[] = {
                {"algorithm", required_argument, 0, 'a'},
                {"time", no_argument, &time_flag, 1},
                {0, 0, 0, 0}
        };
        int option_index = 0;
        c = getopt_long(argc, argv, "a:", long_options, &option_index);

        if (c == -1) {
            break;
        }
        switch (c) {
            case 0:
                break;
            case 'a':
                if (isdigit(*optarg)) {
                    algorithm = atoi(optarg);
                    argument_boost = 2;
                } else {
                    print_usage();
                    return EXIT_FAILURE;
                }
                break;
            default:
                abort();
        }
    }
    if (algorithm < 0 || algorithm > 2) {
        print_usage();
        return EXIT_FAILURE;
    }
    if (time_flag == 1) {
        argument_boost++;
    }

    // Read query's
    QueryCollection* qc = read_queries();

    // Load database
    char database_name[64];
    strcpy(database_name, argv[1 + argument_boost]);
    Database* database = load_database(database_name, qc->max_length);

    // Database is loaded, so report to user by sending question mark.
    printf("?\n");

    // Get optional longitude and latitude
    double longitude;
    double latitude;
    if (argc == 4 + argument_boost) {
        latitude = strtod(argv[2 + argument_boost], NULL);
        longitude = strtod(argv[3 + argument_boost], NULL);
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
            TotalMatchCollection* tmc = calculate_query_breakdown_total_matches(breakdown, database, algorithm);
            calculate_all_total_matches_correctness(tmc);
            calculate_all_total_matches_synergy(tmc);

            if (argc == 4 + argument_boost) {
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

    t = clock() - t;
    if (time_flag == 1) {
        printf("%f\n", (double) t / CLOCKS_PER_SEC);
    }

    fclose(stdin);
    fclose(stdout);
    fclose(stderr);
    return EXIT_SUCCESS;
}
