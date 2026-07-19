#ifndef CANDIDATE_H
#define CANDIDATE_H

#define MAX_CANDIDATES 10

typedef struct
{
    int id;
    char word[64];
    int freq;
} Candidate;

#endif