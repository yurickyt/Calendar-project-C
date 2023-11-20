/*Yurick Tchakountio 117991758 ytchakou*/
#include "calendar.h"
#include "event.h"
#include "my_memory_checker_216.h"
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

/* Initializing a calendar and setting the name, compare function, free info
 * function to the specified paramenter. Initializing the number of days
 * according to the specified amount.*/
int init_calendar(const char *name, int days,
                  int (*comp_func)(const void *ptr1, const void *ptr2),
                  void (*free_info_func)(void *ptr), Calendar **calendar) {
    if (calendar == NULL || name == NULL || days < 1) {
        return FAILURE;
    }
    *calendar = malloc(sizeof(Calendar));
    (*calendar)->name = malloc(strlen(name) + 1);
    (*calendar)->events = calloc(days, sizeof(Event *));

    if (calendar == NULL || (*calendar)->name == NULL ||
        (*calendar)->events == NULL) {
        return FAILURE;
    }

    strcpy((*calendar)->name, name);
    (*calendar)->comp_func = comp_func;
    (*calendar)->free_info_func = free_info_func;
    (*calendar)->total_events = 0;
    (*calendar)->days = days;

    return SUCCESS;
}

/* Prints part of the calendar or all of the calendar based on the print_all
 * parameter*/
int print_calendar(Calendar *calendar, FILE *output_stream, int print_all) {
    int i;
    Event *temp;
    if (calendar == NULL || output_stream == NULL) {
        return FAILURE;
    }
    if (print_all) {
        fprintf(output_stream, "Calendar's Name: \"%s\"\n", calendar->name);
        fprintf(output_stream, "Days: %d\n", calendar->days);
        fprintf(output_stream, "Total Events: %d\n\n", calendar->total_events);
    }
    if (calendar->total_events == 0) {
        fprintf(output_stream, "**** Events ****\n\n");
    } else {
        fprintf(output_stream, "**** Events ****\n");
    }
    if (calendar->total_events > 0) {
        for (i = 1; i <= calendar->days; i++) {
            fprintf(output_stream, "Day %d\n", i);
            temp = calendar->events[i - 1];
            while (temp) {
                fprintf(output_stream, "Event's Name: \"%s\", ", temp->name);
                fprintf(output_stream, "Start_time: %d, ", temp->start_time);
                fprintf(output_stream, "Duration: %d\n",
                        temp->duration_minutes);
                temp = temp->next;
            }
        }
    }

    return SUCCESS;
}

/* Adds an event based on the specified parameters to the calendar */
int add_event(Calendar *calendar, const char *name, int start_time,
              int duration_minutes, void *info, int day) {
    Event *node, *temp, *prev, *curr;
    int i;

    if (calendar && name && (start_time <= 2400 && start_time >= 0) &&
        duration_minutes > 0 && (day > 0 && day <= calendar->days)) {

        for (i = 0; i < calendar->days; i++) {
            curr = calendar->events[i];
            while (curr) {
                if (strcmp(curr->name, name) == 0) {
                    return FAILURE;
                }
                curr = curr->next;
            }
        }
        node = malloc(sizeof(Event));
        node->name = malloc(strlen(name) + 1);

        strcpy(node->name, name);
        node->start_time = start_time;
        node->duration_minutes = duration_minutes;
        node->info = info;

        temp = calendar->events[day - 1];

        if (temp == NULL) {
            calendar->events[day - 1] = node;
            node->next = NULL;
        } else if (temp != NULL) {
            if (calendar->comp_func(temp, node) > 0) {
                calendar->events[day - 1] = node;
                node->next = temp;
            } else {
                prev = temp;
                temp = temp->next;
                while (temp && calendar->comp_func(temp, node) <= 0) {
                    prev = temp;
                    temp = temp->next;
                }
                node->next = temp;
                prev->next = node;
            }
        }
        calendar->total_events += 1;
        return SUCCESS;
    }
    return FAILURE;
}

/* Finds an event in the whole calendar that matches name of the specified
 * paremeter */
int find_event(Calendar *calendar, const char *name, Event **event) {
    int i;
    if (calendar == NULL || name == NULL || event == NULL) {
        return FAILURE;
    }

    if (calendar->total_events > 0) {
        for (i = 0; i < calendar->days; i++) {
            if (find_event_in_day(calendar, name, i + 1, event) == SUCCESS) {
                return SUCCESS;
            }
        }
    }
    return FAILURE;
}

/* Finds an event in a specified day that matches the specified name */
int find_event_in_day(Calendar *calendar, const char *name, int day,
                      Event **event) {
    Event *curr;
    if (calendar == NULL || name == NULL || event == NULL || day < 1 ||
        day > calendar->days) {
        return FAILURE;
    }
    curr = calendar->events[day - 1];
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            *event = curr;
            return SUCCESS;
        }
        curr = curr->next;
    }
    return FAILURE;
}

/* Removes an event with a specified name from the calendar */
int remove_event(Calendar *calendar, const char *name) {
    Event *curr, *prev;
    int i;
    if (calendar == NULL || name == NULL) {
        return FAILURE;
    }

    if (calendar->total_events > 0) {
        for (i = 0; i < calendar->days; i++) {
            curr = calendar->events[i];
            if (curr != NULL) {
                if (strstr(curr->name, name) != NULL) {
                    calendar->events[i] = curr->next;
                    curr->next = NULL;
                    free(curr->name);
                    if (calendar->free_info_func && curr->info) {
                        calendar->free_info_func(curr->info);
                    }
                    free(curr);
                    calendar->total_events -= 1;
                    return SUCCESS;
                }
                prev = curr;
                curr = curr->next;
                while (curr) {
                    if (strcmp(curr->name, name) == 0) {
                        prev->next = curr->next;
                        curr->next = NULL;
                        free(curr->name);
                        if (calendar->free_info_func && curr->info) {
                            calendar->free_info_func(curr->info);
                        }
                        free(curr);
                        calendar->total_events -= 1;
                        return SUCCESS;
                    }
                    prev = curr;
                    curr = curr->next;
                }
            }
        }
    }
    return FAILURE;
}

/* Finds and returns the info from an event with a specified name */
void *get_event_info(Calendar *calendar, const char *name) {
    Event *curr;
    int i;
    if (calendar->total_events > 0) {
        for (i = 0; i < calendar->days; i++) {
            curr = calendar->events[i];
            while (curr) {
                if (strcmp(curr->name, name) == 0) {
                    return curr->info;
                }
                curr = curr->next;
            }
        }
    }
    return NULL;
}

/* Clears and frees all the eevents in a calendar*/
int clear_calendar(Calendar *calendar) {
    int i;
    if (calendar == NULL) {
        return FAILURE;
    }
    if (calendar->total_events > 0) {
        for (i = 0; i < calendar->days; i++) {
            clear_day(calendar, i);
        }
    }
    return SUCCESS;
}

/* Clears and frees all the events in a specified day*/
int clear_day(Calendar *calendar, int day) {
    Event *curr, *prev;
    if (calendar == NULL || day < 0 || day > calendar->days) {
        return FAILURE;
    }
    curr = calendar->events[day];
    while (curr) {
        prev = curr;
        curr = curr->next;
        free(prev->name);
        if (calendar->free_info_func && prev->info) {
            calendar->free_info_func(prev->info);
        }
        free(prev);
        calendar->total_events -= 1;
    }
    calendar->events[day] = NULL;
    return SUCCESS;
}

/* Clears and frees all events in a calendar as well as the calendar itself*/
int destroy_calendar(Calendar *calendar) {
    if (calendar == NULL) {
        return FAILURE;
    }
    free(calendar->name);
    clear_calendar(calendar);
    free(calendar->events);
    free(calendar);

    return SUCCESS;
}