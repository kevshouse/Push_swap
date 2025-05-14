/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_s_machine.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin-anderson <kevin-anderson@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:46:44 by kevin-ander       #+#    #+#             */
/*   Updated: 2025/05/14 21:09:42 by kevin-ander      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "p_s_machine.h"

void stack_init(t_stack *s)
{
    if (!s) return;
    s->head = NULL;
    s->tail = NULL;
    s->size = 0;
    s->shunting_array = malloc(2 * sizeof(int));
}

void stack_clear(t_stack *s)
{
    if (!s) return;
    while (s->head)
    {
        t_node *tmp = s->head;
        s->head = s->head->next;
        free(tmp);
    }
    free(s->shunting_array);
    stack_init(s);
}

void push(t_stack *s, int data)
{
    if (!s) return;

    t_node *new_node = malloc(sizeof(t_node));
    if (!new_node) return;

    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = NULL;

    if (s->head == NULL) {
        s->head = new_node;
        s->tail = new_node;
    } else {
        new_node->prev = s->tail;
        s->tail->next = new_node;
        s->tail = new_node;
    }
    s->size++;
}

int pop(t_stack *s)
{
    if (!s || !s->head) {
        if (DEBUG) printf("Stack is empty!\n");
        return -1;
    }

    int data = s->tail->data;
    if (s->head == s->tail) {
        free(s->tail);
        s->head = NULL;
        s->tail = NULL;
    } else {
        t_node *prev_node = s->tail->prev;
        prev_node->next = NULL;
        free(s->tail);
        s->tail = prev_node;
    }
    s->size--;
    return data;
}

void print_stacks(const char *op_name, t_stack *a, t_stack *b)
{
    if (!DEBUG) return;

    printf("\n--- %s ---\n", op_name ? op_name : "NULL_OPERATION");

    if (!a) printf("Stack A: NULL\n");
    else {
        printf("Stack A [%d]: ", a->size);
        for (t_node *curr = a->head; curr; curr = curr->next)
            printf("%d ", curr->data);
        printf("\n");
    }

    if (!b) printf("Stack B: NULL\n");
    else {
        printf("Stack B [%d]: ", b->size);
        for (t_node *curr = b->head; curr; curr = curr->next)
            printf("%d ", curr->data);
        printf("\n");
    }
}

/* Operation Implementations */
int swap_top_with_shunting(t_stack *s)
{
    if (!s || s->size < 2) {
        if (DEBUG) printf("Not enough elements to swap!\n");
        return 1;
    }

    s->shunting_array[0] = pop(s);
    s->shunting_array[1] = pop(s);
    push(s, s->shunting_array[0]);
    push(s, s->shunting_array[1]);
    return 0;
}

int sa(t_stack *a) { return a ? swap_top_with_shunting(a) : 1; }
int sb(t_stack *b) { return b ? swap_top_with_shunting(b) : 1; }
int ss(t_stack *a, t_stack *b) { return (a ? sa(a) : 1) | (b ? sb(b) : 1); }

int push_to(t_stack *dest, t_stack *src)
{
    if (!dest || !src || src->size == 0) {
        if (DEBUG) printf("Cannot push - invalid stacks!\n");
        return 1;
    }
    push(dest, pop(src));
    return 0;
}

int pa(t_stack *a, t_stack *b) { return push_to(a, b); }
int pb(t_stack *a, t_stack *b) { return push_to(b, a); }

int rotate(t_stack *s)
{
    if (!s || s->size < 2) {
        if (DEBUG) printf("Not enough elements to rotate!\n");
        return 1;
    }

    t_node *first = s->head;
    s->head = first->next;
    s->head->prev = NULL;

    first->next = NULL;
    first->prev = s->tail;
    s->tail->next = first;
    s->tail = first;

    return 0;
}

int ra(t_stack *a) { return a ? rotate(a) : 1; }
int rb(t_stack *b) { return b ? rotate(b) : 1; }
int rr(t_stack *a, t_stack *b) { return (a ? ra(a) : 1) | (b ? rb(b) : 1); }

int reverse_rotate(t_stack *s)
{
    if (!s || s->size < 2) {
        if (DEBUG) printf("Not enough elements to reverse rotate!\n");
        return 1;
    }

    t_node *last = s->tail;
    s->tail = last->prev;
    s->tail->next = NULL;

    last->prev = NULL;
    last->next = s->head;
    s->head->prev = last;
    s->head = last;

    return 0;
}

int rra(t_stack *a) { return a ? reverse_rotate(a) : 1; }
int rrb(t_stack *b) { return b ? reverse_rotate(b) : 1; }
int rrr(t_stack *a, t_stack *b) { return (a ? rra(a) : 1) | (b ? rrb(b) : 1); }
