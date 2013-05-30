/* Università degli Studi di Trento
 * A.A. 2012/2013 - Corso di Laurea in Informatica
 * Sistemi operativi 1
 * Progetto 2: Threads
 *
 * Autori:
 * Mattia Larentis 151889
 * Andrea Panizza 151887
 * Michele Pittoni 152549
 */

#ifndef LOG_H
#define LOG_H

void log_init();
void log_post(char * message, char * ident);
void log_close();

#endif
