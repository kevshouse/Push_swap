/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_stack_ops.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin-anderson <kevin-anderson@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 18:13:13 by kevin-ander       #+#    #+#             */
/*   Updated: 2025/05/14 19:06:11 by kevin-ander      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <criterion/criterion.h>
#include <limits.h>
#include "../../p_s_machine.h"
#include "test_utils/test_helpers.h"

TestSuite(stack_operations, .init = NULL, .fini = NULL);

/* -- Helper Functions -- */
static void init_stack(t_stack *s, int values[], int count)
{
    stack_init(s);
    for (int i = 0; i < count; i++) {
        push(s, values[i]);
    }
}

/* -- Test Cases -- */

/* Test 1: Stack initialization */
Test(stack_operations, stack_init_works)
{
    t_stack s;
    stack_init(&s);
    cr_assert_null(s.head);
    cr_assert_null(s.tail);
    cr_assert_eq(s.size, 0);
    stack_clear(&s);
}

/* Test 2: Basic push/pop operations */
Test(stack_operations, push_pop_operations)
{
    t_stack s;
    int values[] = {10, 20, 30};

    init_stack(&s, values, 3);
    cr_assert_eq(s.size, 3);
    cr_assert_eq(pop(&s), 30);
    cr_assert_eq(pop(&s), 20);
    cr_assert_eq(pop(&s), 10);
    cr_assert_eq(pop(&s), -1);
    stack_clear(&s);
}

/* Test 3: Swap operation */
Test(stack_operations, swap_operations)
{
    t_stack s;
    int values[] = {1, 2};

    init_stack(&s, values, 2);
    cr_assert_eq(sa(&s), 0);
    cr_assert_eq(s.head->data, 2);
    cr_assert_eq(s.tail->data, 1);
    stack_clear(&s);
}

/* Test 4: Edge cases with empty stack */
Test(stack_operations, edge_cases)
{
    t_stack s;
    stack_init(&s);
    cr_assert_eq(sa(&s), 1);
    cr_assert_eq(pop(&s), -1);
    stack_clear(&s);
}

/* Test 5: Single element operations */
Test(stack_operations, single_element_operations)
{
    t_stack s;
    stack_init(&s);
    push(&s, 42);

    cr_assert_eq(sa(&s), 1);
    cr_assert_eq(pop(&s), 42);
    cr_assert_eq(pop(&s), -1);
    stack_clear(&s);
}

/* Test 6: Null stack safety */
Test(stack_operations, null_stack_safety)
{
    t_stack valid;
    stack_init(&valid);

    cr_assert_eq(sa(NULL), 1);
    cr_assert_eq(sb(NULL), 1);
    cr_assert_eq(ss(NULL, NULL), 1);
    cr_assert_eq(pa(NULL, NULL), 1);
    cr_assert_eq(pb(NULL, NULL), 1);
    cr_assert_eq(ra(NULL), 1);
    cr_assert_eq(rb(NULL), 1);
    cr_assert_eq(rra(NULL), 1);
    cr_assert_eq(rrb(NULL), 1);
    cr_assert_eq(pop(NULL), -1);

    cr_assert_eq(ss(&valid, NULL), 1);
    cr_assert_eq(ss(NULL, &valid), 1);
    cr_assert_eq(pa(&valid, NULL), 1);
    cr_assert_eq(pb(NULL, &valid), 1);

    stack_clear(&valid);
}

/* Test 7: Empty stack torture test */
Test(stack_operations, empty_stack_torture)
{
    t_stack empty;
    stack_init(&empty);

    cr_assert_eq(sa(&empty), 1);
    cr_assert_eq(ra(&empty), 1);
    cr_assert_eq(rra(&empty), 1);
    cr_assert_eq(pop(&empty), -1);

    cr_assert_null(empty.head);
    cr_assert_null(empty.tail);
    cr_assert_eq(empty.size, 0);

    stack_clear(&empty);
}

/* Test 8: Large stack performance */
Test(stack_operations, large_stack_performance, .timeout = 2.0)
{
    t_stack s;
    stack_init(&s);

    for (int i = 0; i < 10000; i++) {
        push(&s, i);
    }
    cr_assert_eq(sa(&s), 0);
    stack_clear(&s);
}

/* Test 9: Mass push/pop operations */
Test(stack_operations, mass_push_pop, .timeout = 5.0)
{
    t_stack s;
    stack_init(&s);
    const size_t count = 100000;

    for (size_t i = 0; i < count; i++) {
        push(&s, (int)i);
        cr_assert_eq(s.size, i + 1);
    }

    for (size_t i = count; i > 0; i--) {
        cr_assert_eq(pop(&s), (int)(i - 1));
    }
    cr_assert_eq(s.size, 0);
    stack_clear(&s);
}

/* Test 10: Interleaved large operations */
Test(stack_operations, interleaved_large_ops, .timeout = 3.0)
{
    t_stack a, b;
    stack_init(&a);
    stack_init(&b);

    for (int i = 0; i < 5000; i++) {
        push(i % 2 ? &a : &b, i);
        if (i % 100 == 0) {
            ss(&a, &b);
            rr(&a, &b);
            rrr(&a, &b);
        }
    }

    while (a.size > 0) {
        pb(&a, &b);
    }
    cr_assert_eq(a.size, 0);
    cr_assert_eq(b.size, 5000);
    stack_clear(&a);
    stack_clear(&b);
}

/* Test 11: Alternating operations */
Test(stack_operations, alternating_operations)
{
    t_stack a, b;
    stack_init(&a);
    stack_init(&b);

    for (size_t i = 0; i < 500; i++) {
        push(&a, (int)i);
        sa(&a);
        pb(&a, &b);
        rb(&b);
        pa(&a, &b);
        rra(&a);

        if (i % 50 == 0) {
            size_t total_size = a.size + b.size;
            cr_assert(total_size == i + 1,
                    "Expected total size %zu, got %zu",
                    i + 1, total_size);
        }
    }
    stack_clear(&a);
    stack_clear(&b);
}

/* Test 12: Extreme value swapping */
Test(stack_operations, extreme_swapping)
{
    t_stack s;
    stack_init(&s);

    for (int i = 0; i < 1000; i++) {
        push(&s, i % 2 == 0 ? INT_MAX : INT_MIN);
    }

    for (int i = 0; i < 100; i++) {
        cr_assert_eq(ss(&s, &s), 0);
    }
    cr_assert(s.head->data == INT_MIN || s.head->data == INT_MAX);
    stack_clear(&s);
}

/* Test 13: Duplicate values handling */
Test(stack_operations, duplicate_values)
{
    t_stack s;
    stack_init(&s);

    for (int i = 0; i < 100; i++) {
        push(&s, 42);
    }
    cr_assert_eq(sa(&s), 0);
    cr_assert_eq(s.head->data, 42);
    cr_assert_eq(ra(&s), 0);
    cr_assert_eq(s.tail->data, 42);
    stack_clear(&s);
}
