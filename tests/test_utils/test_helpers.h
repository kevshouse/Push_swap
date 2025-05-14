/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_helpers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin-anderson <kevin-anderson@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 14:32:12 by kevin-ander       #+#    #+#             */
/*   Updated: 2025/05/14 18:47:03 by kevin-ander      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_HELPERS_H
# define TEST_HELPERS_H

# include <criterion/criterion.h>
# include "../../includes/p_s_machine.h"
# include "../includes/p_s_machine.h"

/* Stack Verification Helpers */
void    assert_stack_contents(t_stack *s, int expected[], int size);
void    assert_stack_empty(t_stack *s);
void    assert_stack_size(t_stack *s, int expected_size);

/* Test Setup/Teardown Helpers */
t_stack *create_test_stack(int values[], int count);
void    destroy_test_stack(t_stack *s);

/* Debugging Helpers */
void    print_test_header(const char *test_name);
void    print_stack_contents(t_stack *s, const char *stack_name);

/* Error Case Helpers */
void    test_invalid_operations(t_stack *s);

/* Memory Helpers */
void    check_for_leaks(void);

#endif
