/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin-anderson <kevin-anderson@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 18:44:09 by kevin-ander       #+#    #+#             */
/*   Updated: 2025/05/14 18:51:59 by kevin-ander      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_helpers.h"

void assert_stack_contents(t_stack *s, int expected[], int size)
{
    t_node *current = s->head;
    int i = 0;
    
    while (i < size && current != NULL)
    {
        cr_assert_eq(current->data, expected[i],
                   "Value mismatch at position %d: expected %d, got %d",
                   i, expected[i], current->data);
        current = current->next;
        i++;
    }
    cr_assert_eq(i, size, "Stack size mismatch: expected %d, got %d", size, i);
    cr_assert_null(current, "Stack contains more elements than expected");
}

t_stack *create_test_stack(int values[], int count)
{
    t_stack *s = malloc(sizeof(t_stack));
    stack_init(s);
    
    for (int i = 0; i < count; i++)
        push(s, values[i]);
    
    return s;
}

void destroy_test_stack(t_stack *s)
{
    stack_clear(s);
    free(s);
}

void print_stack_contents(t_stack *s, const char *stack_name)
{
    printf("[%s] Size: %d\n", stack_name, s->size);
    printf("Contents: ");
    
    t_node *current = s->head;
    while (current)
    {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}