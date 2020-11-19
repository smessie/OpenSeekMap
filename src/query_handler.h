//
// Created by Ieben Smessaert on 11/11/2020.
//

#ifndef AD3_PROJECT_QUERY_HANDLER_H
#define AD3_PROJECT_QUERY_HANDLER_H

#include <stdbool.h>

#include "database.h"

typedef struct Query_t {
    char* value;
    struct Query_t* next;
} Query;

typedef struct QueryCollection_t {
    Query* head;
    Query* tail;
    int min_length;
    int max_length;
} QueryCollection;

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

typedef struct Match_t {
    Entry* value;
    int cost;
    struct Match_t* next;
} Match;

typedef struct TotalMatch_t {
    Match* head;
    Match* tail;
    Match* representative;
    double correctness;
    double synergy;
    int n;
    bool selected_as_best;
    bool parent_collection_freed;
    struct TotalMatch_t* next;
} TotalMatch;

typedef struct TotalMatchCollection_t {
    TotalMatch* head;
    TotalMatch* tail;
} TotalMatchCollection;

typedef struct BestMatches_t {
    TotalMatch* match_1;
    double value_1;

    TotalMatch* match_2;
    double value_2;

    TotalMatch* match_3;
    double value_3;

    TotalMatch* match_4;
    double value_4;

    TotalMatch* match_5;
    double value_5;
} BestMatches;

QueryBreakdownCollection* create_query_breakdown_collection(char* query);

void free_query_breakdown_collection(QueryBreakdownCollection* collection);

void free_query_breakdown(QueryBreakdown* breakdown);

void free_total_match(TotalMatch* total_match);

void free_total_match_collection(TotalMatchCollection* collection);

TotalMatchCollection* calculate_query_breakdown_total_matches(QueryBreakdown* breakdown, Database* database);

TotalMatch* duplicate_total_match(TotalMatch* source);

void add_total_match_to_collection(TotalMatchCollection* collection, TotalMatch* match);

void add_match_to_total_match(TotalMatch* total_match, Match* match);

void calculate_total_match_correctness(TotalMatch* total_match);

void calculate_all_total_matches_correctness(TotalMatchCollection* collection);

void free_best_matches(BestMatches* best_matches);

QueryCollection* read_queries();

void free_query_collection(QueryCollection* collection);

double get_max_cost(int query_length);

#endif //AD3_PROJECT_QUERY_HANDLER_H
