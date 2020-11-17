//
// Created by Ieben Smessaert on 28/10/2020.
//

#include "algorithm.test.h"

#include <stdlib.h>
#include <stdio.h>

#include "../src/algorithm.h"
#include "../src/bitwise_operators.h"
#include "../src/strings/string_util.h"

void run_algorithm_tests() {
    int passed = 0;
    int failed = 0;

    test_characteristic_vectors(&passed, &failed);
    test_characteristic_vectors_2(&passed, &failed);

    test_shiftAND(&passed, &failed);

    test_M(&passed, &failed);

    test_M_i(&passed, &failed);

    test_shiftAND_errors(&passed, &failed);

    printf("\nCompleted algorithm tests with %d passes and %d failures.\n", passed, failed);
}

void test_characteristic_vectors(int* passed, int* failed) {
    int size = 3;
    characteristic_vectors* cvs = calculate_characteristic_vectors("ana", size);
    int expected_a[] = {1, 0, 1};
    if (equal(C(cvs, 'a'), expected_a, size) == true) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("characteristic_vectors char 'a' test failed!");
    }

    int expected_n[] = {0, 1, 0};
    if (equal(C(cvs, 'n'), expected_n, size) == true) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("characteristic_vectors char 'n' test failed!");
    }

    int expected_x[] = {0, 0, 0};
    if (equal(C(cvs, 'x'), expected_x, size) == true) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("characteristic_vectors char 'x' test failed!");
    }
    free_characteristic_vectors(cvs);
}

void test_characteristic_vectors_2(int* passed, int* failed) {
    int size = 8;
    characteristic_vectors* cvs = calculate_characteristic_vectors("barbados", size);
    int expected_a[] = {0, 1, 0, 0, 1, 0, 0, 0};
    if (equal(C(cvs, 'a'), expected_a, size) == true) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("characteristic_vectors 2 char 'a' test failed!");
    }

    int expected_b[] = {1, 0, 0, 1, 0, 0, 0, 0};
    if (equal(C(cvs, 'b'), expected_b, size) == true) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("characteristic_vectors 2 char 'b' test failed!");
    }

    int expected_x[] = {0, 0, 0, 0, 0, 0, 0, 0};
    if (equal(C(cvs, 'x'), expected_x, size) == true) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("characteristic_vectors 2 char 'x' test failed!");
    }
    free_characteristic_vectors(cvs);
}

void test_shiftAND(int* passed, int* failed) {
    if (shiftAND("zoek", "mijn zoek string") == true) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("shiftAND test 1 failed!");
    }

    if (shiftAND("zoeck", "mijn zoek string") == false) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("shiftAND test 2 failed!");
    }

    if (shiftAND("zoeken", "iets_zoets_zoeken") == true) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("shiftAND test 3 failed!");
    }

    if (shiftAND("ans", "ananas") == false) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("shiftAND test 4 failed!");
    }
}

void test_M(int* passed, int* failed) {
    int size = 3;
    characteristic_vectors* cvs = calculate_characteristic_vectors("ana", size);
    M* matrix = calculate_M("ana", "ananas", cvs);
    int expected_ana[6][3] = {
            {1, 0, 0},
            {0, 1, 0},
            {0, 0, 1},
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0}
    };
    bitvector* testing_column_ana = matrix->head;
    for (int i = 0; i < matrix->n; i++) {
        if (equal(testing_column_ana->value, expected_ana[i], size) == false) {
            (*failed)++;
            perror("M z=ana, t=ananas test failed!");
            break;
        }
        testing_column_ana = testing_column_ana->next;
    }
    if (testing_column_ana == NULL) {
        (*passed)++;
    }
    free_M(matrix);
    free_characteristic_vectors(cvs);

    size = 4;
    cvs = calculate_characteristic_vectors("bond", size);
    matrix = calculate_M("bond", "bonbond", cvs);
    int expected_bond[7][4] = {
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
    };
    bitvector* testing_column_bond = matrix->head;
    for (int i = 0; i < matrix->n; i++) {
        if (equal(testing_column_bond->value, expected_bond[i], size) == false) {
            (*failed)++;
            perror("M z=bond, t=bonbond test failed!");
            break;
        }
        testing_column_bond = testing_column_bond->next;
    }
    if (testing_column_bond == NULL) {
        (*passed)++;
    }
    free_M(matrix);
    free_characteristic_vectors(cvs);
}

void test_M_i(int* passed, int* failed) {
    int size = 4;
    characteristic_vectors* cvs = calculate_characteristic_vectors("dank", size);
    M* m_0 = calculate_M("dank", "dansfeest", cvs);
    M* m_1 = calculate_M_i("dank", "dansfeest", cvs, m_0);
    int expected_dank[13][4] = {
            {1, 1, 0, 0}, /* d */
            {1, 1, 1, 0}, /* a */
            {0, 1, 1, 1}, /* n */
            {0, 0, 1, 1}, /* s */
            {0, 0, 0, 0}, /* f */
            {0, 0, 0, 0}, /* e */
            {0, 0, 0, 0}, /* e */
            {0, 0, 0, 0}, /* s */
            {0, 0, 0, 0}  /* t */
    };
    bitvector* testing_column_dank = m_1->head;
    for (int i = 0; i < m_1->n; i++) {
        if (equal(testing_column_dank->value, expected_dank[i], size) == false) {
            (*failed)++;
            perror("M z=dank, t=dansfeest test failed!");
            break;
        }
        testing_column_dank = testing_column_dank->next;
    }
    if (testing_column_dank == NULL) {
        (*passed)++;
    }
    free_M(m_0);
    free_M(m_1);
    free_characteristic_vectors(cvs);

    size = 6;
    cvs = calculate_characteristic_vectors("zoeven", size);
    m_0 = calculate_M("zoeven", "zoekken", cvs);
    m_1 = calculate_M_i("zoeven", "zoekken", cvs, m_0);
    M* m_2 = calculate_M_i("zoeven", "zoekken", cvs, m_1);
    int expected_zoeven[7][6] = {
            {1, 1, 1, 0, 0, 0}, /* z */
            {1, 1, 1, 1, 0, 0}, /* o */
            {1, 1, 1, 1, 1, 0}, /* e */
            {0, 1, 1, 1, 1, 0}, /* k */
            {0, 0, 1, 1, 1, 0}, /* k */
            {0, 0, 0, 0, 1, 0}, /* e */
            {0, 0, 0, 0, 0, 1}  /* n */
    };
    bitvector* testing_column_zoeven = m_2->head;
    for (int i = 0; i < m_2->n; i++) {
        if (equal(testing_column_zoeven->value, expected_zoeven[i], size) == false) {
            (*failed)++;
            perror("M z=zoeven, t=zoekken test failed!");
            break;
        }
        testing_column_zoeven = testing_column_zoeven->next;
    }
    if (testing_column_zoeven == NULL) {
        (*passed)++;
    }
    free_M(m_0);
    free_M(m_1);
    free_M(m_2);
    free_characteristic_vectors(cvs);

    size = 7;
    cvs = calculate_characteristic_vectors("werkeun", size);
    m_0 = calculate_M("werkeun", "gekreun", cvs);
    m_1 = calculate_M_i("werkeun", "gekreun", cvs, m_0);
    m_2 = calculate_M_i("werkeun", "gekreun", cvs, m_1);
    M* m_3 = calculate_M_i("werkeun", "gekreun", cvs, m_2);
    int expected_werkeun_2[7][7] = {
            {1, 1, 0, 0, 0, 0, 0}, /* g */
            {1, 1, 1, 0, 0, 0, 0}, /* e */
            {0, 1, 1, 1, 0, 0, 0}, /* k */
            {0, 0, 1, 0, 0, 0, 0}, /* r */
            {0, 0, 0, 0, 0, 0, 0}, /* e */
            {0, 0, 0, 0, 0, 0, 0}, /* u */
            {0, 0, 0, 0, 0, 0, 0}  /* n */
    };
    bitvector* testing_column_werkeun = m_2->head;
    for (int i = 0; i < m_2->n; i++) {
        if (equal(testing_column_werkeun->value, expected_werkeun_2[i], size) == false) {
            (*failed)++;
            perror("M z=werkeun, t=gekreun test (e=2) failed!");
            break;
        }
        testing_column_werkeun = testing_column_werkeun->next;
    }
    if (testing_column_werkeun == NULL) {
        (*passed)++;
    }
    int expected_werkeun_3[7][7] = {
            {1, 1, 1, 0, 0, 0, 0}, /* g */
            {1, 1, 1, 1, 0, 0, 0}, /* e */
            {1, 1, 1, 1, 1, 0, 0}, /* k */
            {0, 1, 1, 1, 1, 0, 0}, /* r */
            {0, 0, 1, 1, 1, 0, 0}, /* e */
            {0, 0, 0, 0, 0, 1, 0}, /* u */
            {0, 0, 0, 0, 0, 0, 1}  /* n */
    };
    testing_column_werkeun = m_3->head;
    for (int i = 0; i < m_2->n; i++) {
        if (equal(testing_column_werkeun->value, expected_werkeun_3[i], size) == false) {
            (*failed)++;
            perror("M z=werkeun, t=gekreun test (e=3) failed!");
            break;
        }
        testing_column_werkeun = testing_column_werkeun->next;
    }
    if (testing_column_werkeun == NULL) {
        (*passed)++;
    }
    free_M(m_0);
    free_M(m_1);
    free_M(m_2);
    free_M(m_3);
    free_characteristic_vectors(cvs);

    size = 3;
    cvs = calculate_characteristic_vectors("ans", size);
    m_0 = calculate_M("ans", "anan", cvs);
    m_1 = calculate_M_i("ans", "anan", cvs, m_0);
    m_2 = calculate_M_i("ans", "anan", cvs, m_1);
    int expected_ans[4][3] = {
            {1, 1, 1}, /* a */
            {1, 1, 1}, /* n */
            {1, 1, 1}, /* a */
            {0, 1, 1} /* n */
    };
    bitvector* testing_column_ans = m_2->head;
    for (int i = 0; i < m_2->n; i++) {
        if (equal(testing_column_ans->value, expected_ans[i], size) == false) {
            (*failed)++;
            perror("M z=ans, t=anan test failed!");
            break;
        }
        testing_column_ans = testing_column_ans->next;
    }
    if (testing_column_ans == NULL) {
        (*passed)++;
    }
    free_M(m_0);
    free_M(m_1);
    free_M(m_2);
    free_characteristic_vectors(cvs);
}

void test_shiftAND_errors(int* passed, int* failed) {
    if (shiftAND_errors("zoek", "zoek") == 0) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("shiftAND_errors test 1 failed!");
    }

    if (shiftAND_errors("zoeck", "zoek") == 1) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("shiftAND_errors test 2 failed!");
    }

    if (shiftAND_errors("werken", "gekreun") == -1) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("shiftAND_errors test 3 failed!");
    }

    if (shiftAND_errors("werkeun", "gekreun") == 3) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("shiftAND_errors test 4 failed!");
    }

    if (shiftAND_errors("ans", "anan") == 2) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("shiftAND_errors test 5 failed!");
    }

    if (shiftAND_errors("zoeven", "zoekken") == 2) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("shiftAND_errors test 6 failed!");
    }
}