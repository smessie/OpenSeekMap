//
// Created by Ieben Smessaert on 11/11/2020.
//

#include "query_handler.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "algorithm.h"

QueryBreakdownCollection* create_query_breakdown_collection(char* query) {
    QueryBreakdownCollection* collection = (QueryBreakdownCollection*) malloc(sizeof(QueryBreakdownCollection));
    collection->head = NULL;
    collection->tail = NULL;

    // We need to malloc the query because strtok needs to be able to edit it.
    char* malloced_query = (char*) malloc((strlen(query) + 1) * sizeof(char));
    strcpy(malloced_query, query);

    char* token = strtok(malloced_query, " ");
    while (token != NULL) {
        // Do something with the part.
        if (collection->head == NULL) {
            collection->head = (QueryBreakdown*) malloc(sizeof(QueryBreakdown));
            collection->head->next = NULL;
            collection->head->head = (QueryBreakdownPart*) malloc(sizeof(QueryBreakdownPart));
            collection->head->head->value = (char*) malloc((strlen(token) + 1) * sizeof(char));
            strcpy(collection->head->head->value, token);
            collection->head->head->malloced_value = true;
            collection->head->head->next = NULL;
            collection->head->tail = collection->head->head;

            collection->tail = collection->head;
        } else {
            // Add token to each current breakdown by doubling amount of breakdowns.
            QueryBreakdown* new_breakdowns_head = NULL;
            QueryBreakdown* new_breakdowns_tail = NULL;

            QueryBreakdown* breakdown = collection->head;
            while (breakdown != NULL) {
                // Duplicate breakdown and edit latest part value.
                QueryBreakdown* duplicated_breakdown = (QueryBreakdown*) malloc(sizeof(QueryBreakdown));
                duplicated_breakdown->next = NULL;
                duplicated_breakdown->head = NULL;
                duplicated_breakdown->tail = NULL;

                QueryBreakdownPart* part_to_copy = breakdown->head;
                QueryBreakdownPart* copied_part = NULL;
                while (part_to_copy != NULL) {
                    QueryBreakdownPart* new_part = (QueryBreakdownPart*) malloc(sizeof(QueryBreakdownPart));
                    new_part->next = NULL;

                    // If next element is null, edit value so token is concatenated. Otherwise take over the value.
                    if (part_to_copy->next != NULL) {
                        new_part->value = part_to_copy->value;
                        new_part->malloced_value = false;
                    } else {
                        int length = strlen(part_to_copy->value) + strlen(token) + 2;
                        new_part->value = (char*) malloc(length * sizeof(char));
                        strcpy(new_part->value, part_to_copy->value);
                        strcat(new_part->value, " ");
                        strcat(new_part->value, token);
                        new_part->malloced_value = true;
                    }
                    if (copied_part != NULL) {
                        copied_part->next = new_part;
                    } else {
                        duplicated_breakdown->head = new_part;
                    }
                    copied_part = new_part;

                    part_to_copy = part_to_copy->next;
                }
                duplicated_breakdown->tail = copied_part;

                // Now add the new duplicated breakdown to the new breakdowns.
                if (new_breakdowns_head == NULL) {
                    new_breakdowns_head = duplicated_breakdown;
                } else {
                    new_breakdowns_tail->next = duplicated_breakdown;
                }
                new_breakdowns_tail = duplicated_breakdown;

                // Now edit the existing breakdown by adding a new part holding the current token.
                QueryBreakdownPart* new_part = (QueryBreakdownPart*) malloc(sizeof(QueryBreakdownPart));
                new_part->value = (char*) malloc((strlen(token) + 1) * sizeof(char));
                strcpy(new_part->value, token);
                new_part->malloced_value = true;
                new_part->next = NULL;
                breakdown->tail->next = new_part;
                breakdown->tail = new_part;

                // Go to next breakdown in our loop.
                breakdown = breakdown->next;
            }

            // All breakdowns are edited with the extra token, so extend our collection.
            collection->tail->next = new_breakdowns_head;
            collection->tail = new_breakdowns_tail;
        }

        token = strtok(NULL, " ");
    }
    free(malloced_query);

    return collection;
}

void free_query_breakdown_collection(QueryBreakdownCollection* collection) {
    if (collection == NULL) {
        return;
    }
    while (collection->head != NULL) {
        QueryBreakdown* toBeRemoved = collection->head;
        collection->head = toBeRemoved->next;
        free_query_breakdown(toBeRemoved);
    }
    free(collection);
}

void free_query_breakdown(QueryBreakdown* breakdown) {
    if (breakdown == NULL) {
        return;
    }
    while (breakdown->head != NULL) {
        QueryBreakdownPart* toBeRemoved = breakdown->head;
        breakdown->head = toBeRemoved->next;
        if (toBeRemoved->malloced_value == true) {
            free(toBeRemoved->value);
        }
        free(toBeRemoved);
    }
    free(breakdown);
}

void free_total_match(TotalMatch* total_match) {
    if (total_match == NULL) {
        return;
    }
    while (total_match->head != NULL) {
        Match* toBeRemoved = total_match->head;
        total_match->head = toBeRemoved->next;
        free(toBeRemoved);
    }
    free(total_match);
}

void free_total_match_collection(TotalMatchCollection* collection) {
    if (collection == NULL) {
        return;
    }
    while (collection->head != NULL) {
        TotalMatch* toBeRemoved = collection->head;
        collection->head = toBeRemoved->next;
        if (toBeRemoved->selected_as_best == true) {
            toBeRemoved->parent_collection_freed = true;
            toBeRemoved->next = NULL;
        } else {
            free_total_match(toBeRemoved);
        }
    }
    free(collection);
}

TotalMatchCollection* calculate_query_breakdown_total_matches(QueryBreakdown* breakdown, Database* database) {
    TotalMatchCollection* collection = (TotalMatchCollection*) malloc(sizeof(TotalMatchCollection));
    collection->head = NULL;
    collection->tail = NULL;

    // For a split search query, look up each part separately.
    QueryBreakdownPart* part = breakdown->head;
    while (part != NULL) {
        // We will create a new collection where every resulting match for this current part is added to the matches
        // in the old collection.
        TotalMatchCollection* new_collection = (TotalMatchCollection*) malloc(sizeof(TotalMatchCollection));
        new_collection->head = NULL;
        new_collection->tail = NULL;

        // Now find for each string/part the matching results in the database.
        Entry* entry = database->head;
        while (entry != NULL) {
            // Check if the database entry is a match by calculating its editing distance.
            int cost = shiftAND_errors(part->value, entry->normalized);
            if (cost != -1 && cost < 1 + strlen(part->value) / 3) {
                // If it is a match, add it as a new v_i to all currently existing total matches.
                if (collection->head == NULL) {
                    // If this match is for the first part, do not extend the current total matches (because there are none),
                    // but add totally new total match.
                    Match* new_match = (Match*) malloc(sizeof(Match));
                    new_match->value = entry;
                    new_match->cost = cost;
                    new_match->next = NULL;

                    TotalMatch* total_match = (TotalMatch*) calloc(1, sizeof(TotalMatch));
                    add_match_to_total_match(total_match, new_match);
                    add_total_match_to_collection(new_collection, total_match);
                } else {
                    TotalMatch* total_match = collection->head;
                    while (total_match != NULL) {
                        Match* new_match = (Match*) malloc(sizeof(Match));
                        new_match->value = entry;
                        new_match->cost = cost;
                        new_match->next = NULL;

                        TotalMatch* duplicated_total_match = duplicate_total_match(total_match);
                        add_match_to_total_match(duplicated_total_match, new_match);
                        add_total_match_to_collection(new_collection, duplicated_total_match);

                        total_match = total_match->next;
                    }
                }
            }

            entry = entry->next;
        }

        free_total_match_collection(collection);
        collection = new_collection;
        // If there is no total_match in new_collection, this means there were no matches for this QBP so this QB ends here.
        // Return an empty TMC.
        if (collection->head == NULL) {
            return collection;
        }

        part = part->next;
    }

    return collection;
}

TotalMatch* duplicate_total_match(TotalMatch* source) {
    TotalMatch* duplicate = (TotalMatch*) calloc(1, sizeof(TotalMatch));

    Match* match_to_copy = source->head;
    while (match_to_copy != NULL) {
        Match* copied_match = (Match*) malloc(sizeof(Match));
        copied_match->value = match_to_copy->value;
        copied_match->cost = match_to_copy->cost;
        copied_match->next = NULL;

        add_match_to_total_match(duplicate, copied_match);

        match_to_copy = match_to_copy->next;
    }
    return duplicate;
}

void add_total_match_to_collection(TotalMatchCollection* collection, TotalMatch* match) {
    if (collection->head == NULL) {
        collection->head = match;
    } else {
        collection->tail->next = match;
    }
    collection->tail = match;
}

void add_match_to_total_match(TotalMatch* total_match, Match* match) {
    if (total_match->head == NULL) {
        total_match->head = match;
    } else {
        total_match->tail->next = match;
    }
    total_match->tail = match;
    total_match->n++;
}

void calculate_total_match_correctness(TotalMatch* total_match) {
    int numerator = 0;
    int denominator = 0;
    Match* match = total_match->head;
    while (match != NULL) {
        int length_z = strlen(match->value->normalized);
        numerator += length_z - match->cost;
        denominator += length_z;

        match = match->next;
    }
    total_match->correctness = numerator / denominator;
}

void calculate_all_total_matches_correctness(TotalMatchCollection* collection) {
    TotalMatch* total_match = collection->head;
    while (total_match != NULL) {
        calculate_total_match_correctness(total_match);
        total_match = total_match->next;
    }
}

void free_best_matches(BestMatches* best_matches) {
    free_total_match(best_matches->match_1);
    free_total_match(best_matches->match_2);
    free_total_match(best_matches->match_3);
    free_total_match(best_matches->match_4);
    free_total_match(best_matches->match_5);
    free(best_matches);
}

QueryCollection* read_queries() {
    QueryCollection* collection = (QueryCollection*) malloc(sizeof(QueryCollection));
    collection->head = NULL;
    collection->tail = NULL;
    collection->min_length = -1;
    collection->max_length = -1;

    char str[100];
    while (scanf("%[^\n]%*c", str) == 1) {
        // Create the query.
        int length = strlen(str);
        Query* query = (Query*) malloc(sizeof(Query));
        query->value = (char*) malloc((length + 1) * sizeof(char));
        strcpy(query->value, str);
        query->next = NULL;

        // Update query length statistics.
        if (collection->min_length == -1 || length < collection->min_length) {
            collection->min_length = length;
        }
        if (collection->max_length == -1 || length > collection->max_length) {
            collection->max_length = length;
        }

        // Add query to the collection.
        if (collection->head == NULL) {
            collection->head = query;
        } else {
            collection->tail->next = query;
        }
        collection->tail = query;
    }
    return collection;
}

void free_query_collection(QueryCollection* collection) {
    if (collection == NULL) {
        return;
    }
    while (collection->head != NULL) {
        Query* toBeRemoved = collection->head;
        collection->head = toBeRemoved->next;
        free(toBeRemoved->value);
        free(toBeRemoved);
    }
    free(collection);
}