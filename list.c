
// ============================================================================
// list.c -- YOUR WORK for Lab 1.
// ============================================================================

#include "list.h"

#include <stdlib.h>

TokenList list_create(void) {
    return (TokenList){.head = nullptr, .tail = nullptr, .length = 0};
}

void list_destroy(TokenList *list) {
    TokenNode *current = list->head;

    while (current != nullptr) {
        TokenNode *next = current->next;

        token_free(&current->token);
        free(current);

        current = next;
    }

    *list = list_create();
}

bool list_push_front(TokenList *list, Token token) {
    TokenNode *new_node = malloc(sizeof *new_node);

    if (new_node == nullptr) {
        token_free(&token);
        return false;
    }

    new_node->token = token;
    new_node->next  = list->head;

    list->head = new_node;

    if (list->tail == nullptr) {
        list->tail = new_node;
    }

    list->length++;

    return true;
}

bool list_push_back(TokenList *list, Token token) {
    TokenNode *new_node = malloc(sizeof *new_node);

    if (new_node == nullptr) {
        token_free(&token);
        return false;
    }

    new_node->token = token;
    new_node->next  = nullptr;

    if (list->tail == nullptr) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail       = new_node;
    }

    list->length++;

    return true;
}

bool list_insert_after(TokenList *list, TokenNode *node, Token token) {
    if (node == nullptr) {
        token_free(&token);
        return false;
    }

    TokenNode *new_node = malloc(sizeof *new_node);

    if (new_node == nullptr) {
        token_free(&token);
        return false;
    }

    new_node->token = token;
    new_node->next  = node->next;
    node->next      = new_node;

    if (list->tail == node) {
        list->tail = new_node;
    }

    list->length++;

    return true;
}

TokenNode *list_find_first(const TokenList *list, TokenKind kind) {
    TokenNode *current = list->head;

    while (current != nullptr) {
        if (current->token.kind == kind) {
            return current;
        }

        current = current->next;
    }

    return nullptr;
}

bool list_remove_first(TokenList *list, TokenKind kind) {
    TokenNode *current = list->head;
    TokenNode *prev    = nullptr;

    while (current != nullptr) {
        if (current->token.kind == kind) {
            if (prev == nullptr) {
                list->head = current->next;
            } else {
                prev->next = current->next;
            }

            if (list->tail == current) {
                list->tail = prev;
            }

            token_free(&current->token);
            free(current);

            list->length--;

            return true;
        }

        prev    = current;
        current = current->next;
    }

    return false;
}

size_t list_length(const TokenList *list) {
    return list->length;
}

void list_print(const TokenList *list, FILE *out) {
    TokenNode *current = list->head;

    while (current != nullptr) {
        fprintf(out, "  %-9s '%s' (%d:%d)\n", token_kind_name(current->token.kind),
                current->token.lexeme, current->token.line, current->token.col);

        current = current->next;
    }
}

void list_to_dot(const TokenList *list, FILE *out) {
    fprintf(out, "digraph tokens {\n");
    fprintf(out, "  rankdir=LR;\n");
    fprintf(out, "  node [shape=box];\n");

    TokenNode *current = list->head;
    size_t index       = 0;

    while (current != nullptr) {
        fprintf(out, "  n%zu [label=\"%s\\n%s\"];\n", index, token_kind_name(current->token.kind),
                current->token.lexeme);

        if (current->next != nullptr) {
            fprintf(out, "  n%zu -> n%zu;\n", index, index + 1);
        } else {
            fprintf(out, "  n%zu -> nnull;\n", index);
        }

        current = current->next;
        index++;
    }

    fprintf(out, "  nnull [label=\"NULL\", shape=plaintext];\n");
    fprintf(out, "}\n");
}

bool list_check_invariant(const TokenList *list) {
    size_t count       = 0;
    TokenNode *current = list->head;
    TokenNode *last    = nullptr;

    while (current != nullptr) {
        count++;
        last    = current;
        current = current->next;
    }

    if (count != list->length) {
        return false;
    }

    if (list->length == 0) {
        return list->head == nullptr && list->tail == nullptr;
    }

    return list->head != nullptr && list->tail == last && list->tail->next == nullptr;
}
