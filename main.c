#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Rule {
    char left[10];
    char right[50];
    struct Rule *next;
} Rule;

typedef struct Grammar {
    char terminals[10];
    char non_terminals[10];
    char axiom[10];
    int num_rules;
    Rule *rules;
} Grammar;

Grammar *read_grammar_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        exit(1);
    }

    Grammar *grammar = (Grammar *)malloc(sizeof(Grammar));

    fgets(grammar->terminals, sizeof(grammar->terminals), file);
    fgets(grammar->non_terminals, sizeof(grammar->non_terminals), file);
    fgets(grammar->axiom, sizeof(grammar->axiom), file);

    fscanf(file, "%d\n", &(grammar->num_rules));

    Rule *current_rule = NULL;
    for (int i = 0; i < grammar->num_rules; i++) {
        Rule *new_rule = (Rule *)malloc(sizeof(Rule));
        fgets(new_rule->left, sizeof(new_rule->left), file);
        fgets(new_rule->right, sizeof(new_rule->right), file);
        new_rule->next = NULL;

        if (current_rule == NULL) {
            grammar->rules = new_rule;
        } else {
            current_rule->next = new_rule;
        }
        current_rule = new_rule;
    }

    fclose(file);
    return grammar;
}

void display_rules(Grammar *grammar) {
    Rule *current_rule = grammar->rules;
    int i = 1;
    while (current_rule != NULL) {
        printf("\n Rule %d: %s -> %s \n", i, current_rule->left, current_rule->right);
        current_rule = current_rule->next;
        i++;
    }
}

int get_rule_type(const char *terminals, const char *non_terminals, const char *right) {
    int type = 3;
    for (int i = 0; i < strlen(right); i++) {
        if (strchr(terminals, right[i])) {
            type = 2;
        } else if (strchr(non_terminals, right[i])) {
            type = 1;
        } else if (right[i] == '£') {
            return 4;
        }
    }
    return type;
}

void display_rule_types(Grammar *grammar) {
    Rule *current_rule = grammar->rules;
    int i = 1;
    while (current_rule != NULL) {
        int type = get_rule_type(grammar->terminals, grammar->non_terminals, current_rule->right);
        printf("Rule %d type: %d\n", i, type);
        current_rule = current_rule->next;
        i++;
    }
}

int get_grammar_type(Grammar *grammar) {
    int max_type = 0;
    Rule *current_rule = grammar->rules;
    while (current_rule != NULL) {
        int type = get_rule_type(grammar->terminals, grammar->non_terminals, current_rule->right);
        if (type > max_type) {
            max_type = type;
        }
        current_rule = current_rule->next;
    }
    return max_type;
}

void display_grammar_type(Grammar *grammar) {
    int type = get_grammar_type(grammar);
    printf("Grammar type: %d\n", type);
}

int main() {
    const char *filename = "Grammar.txt";
    Grammar *grammar = read_grammar_from_file(filename);

    printf("Grammar Rules:\n");
    display_rules(grammar);

    printf("\nRule Types:\n");
    display_rule_types(grammar);

    printf("\nGrammar Type:\n");
    display_grammar_type(grammar);

    // Free memory
    Rule *current_rule = grammar->rules;
    while (current_rule != NULL) {
        Rule *next_rule = current_rule->next;
        free(current_rule);
        current_rule = next_rule;
    }
    free(grammar);

    return 0;
}
