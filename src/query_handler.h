//
// Created by Ieben Smessaert on 11/11/2020.
//

#ifndef AD3_PROJECT_QUERY_HANDLER_H
#define AD3_PROJECT_QUERY_HANDLER_H

#include <stdbool.h>

typedef struct QueryBreakdownPart_t {
    char* value;
    bool malloced_value;
    struct QueryBreakdownPart_t* next;
} QueryBreakdownPart;

typedef struct QueryBreakdown_t {
    QueryBreakdownPart* head;
    QueryBreakdownPart* tail;
    struct QueryBreakdown_t* next;
} QueryBreakdown;

typedef struct QueryBreakdownCollection_t {
    /**
     * Used to iterate over the collection.
     */
    QueryBreakdown* head;

    /**
     * Used to get the latest breakdown if we have to add a new breakdown to the collection.
     */
    QueryBreakdown* tail;
} QueryBreakdownCollection;

QueryBreakdownCollection* create_query_breakdown_collection(char* query);

void free_query_breakdown_collection(QueryBreakdownCollection* collection);

void free_query_breakdown(QueryBreakdown* breakdown);

#endif //AD3_PROJECT_QUERY_HANDLER_H
