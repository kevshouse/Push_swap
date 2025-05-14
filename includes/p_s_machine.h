/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_s_machine.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin-anderson <kevin-anderson@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:46:44 by kevin-ander       #+#    #+#             */
/*   Updated: 2025/05/14 20:54:56 by kevin-ander      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef P_S_MACHINE_H
# define P_S_MACHINE_H

# include <stdio.h>
# include <stdlib.h>
# include <limits.h>



/* ========================
 * DEBUG CONFIGURATION
 * ======================== */
# ifndef DEBUG          // Allow override via compiler flag
#  define DEBUG 0       // 0=disabled, 1=enabled
# endif

/* ========================
 * OPERATION LOGGING MACRO
 * ======================== */
# define LOG_OPERATION(op, a, b) \
    do { \
        if (DEBUG) { \
            printf("--- %s ---\n", op); \
            print_stacks(op, a, b); \
        } \
    } while (0)

/* ========================
 * STACK STRUCTURES
 * ======================== */
typedef struct s_node {
    int             data;
    struct s_node   *next;
    struct s_node   *prev;
}   t_node;

typedef struct s_stack {
    t_node  *head;
    t_node  *tail;
    int     size;
    int     *shunting_array;
}   t_stack;

/* ========================
 * FUNCTION DECLARATIONS
 * ======================== */
/* Stack Operations */
void    stack_init(t_stack *s);
void    stack_clear(t_stack *s);
void    push(t_stack *s, int data);
int     pop(t_stack *s);

/* Swap Operations */
int     sa(t_stack *a);
int     sb(t_stack *b);
int     ss(t_stack *a, t_stack *b);

/* Push Operations */
int     pa(t_stack *a, t_stack *b);
int     pb(t_stack *a, t_stack *b);

/* Rotate Operations */
int     ra(t_stack *a);
int     rb(t_stack *b);
int     rr(t_stack *a, t_stack *b);

/* Reverse Rotate Operations */
int     rra(t_stack *a);
int     rrb(t_stack *b);
int     rrr(t_stack *a, t_stack *b);

/* Debug/Printing */
void    print_stacks(const char *op_name, t_stack *a, t_stack *b);

#endif
