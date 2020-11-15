//
// Created by Ieben Smessaert on 11/11/2020.
//

#include "query_handler.h"

#include <stdlib.h>
#include <string.h>

#include <stdio.h>

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