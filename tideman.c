#include <cs50.h>
#include <string.h>
#include <stdio.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
bool cyclic(int winner, int loser);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)

    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // To check the candidates array for the name of a valid cnadidate
    for (int j = 0, k = candidate_count; j < k ; j++)
    {
        // Updates the ranks if the name if found in the array
        if (!strcmp(candidates[j], name))
        {
            ranks[rank] = j;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // To go through the candidates array to updates the preferences
    for (int i = 0, k = candidate_count; i < k ; i++)
    {
        for (int j = i + 1; j < k; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // To record the pair of array for both the preferred
    for (int i = 0, k = candidate_count; i < k; i++)
    {
        for (int j = 0; j < k ; j++)
        {
            // check if one is preferred over the other
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // sort through the pair count array using selection sort
    for (int i = 0, k = pair_count; i < k - 1; i++)
    {
        int temp = 0;
        for (int j = i + 1; j < k; j++)
        {
            // the pairs are sorted according to decreasing winning strength
            if (preferences[pairs[j].winner][pairs[j].loser] > preferences[pairs[i].winner][pairs[i].loser])
            {
                temp = pairs[i].winner;
                pairs[i].winner = pairs[j].winner;
                pairs[j].winner = temp;

                temp = pairs[i].loser;
                pairs[i].loser = pairs[j].loser;
                pairs[j].loser = temp;
            }
        }
    }
    return;
}
// The recursive function cyclic for the locked pairs
bool cyclic(int winner, int loser)
{
    // if the winner and loser are locked the cycle is complete
    if (loser == winner)
    {
        return true;
    }
    for (int i = 0, k = candidate_count; i < k ; i++)
    {
        // If the loser [pair] is locked to another pair, the fuction returns true, to continue the cycle
        if (locked[loser][i] == true)
        {
            // The function continues to call it self until the case is reached
            if (cyclic(winner, i) == true)
            {
                return true;
            }
        }
    }
    return false;
}
// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // The cyclic function is used here to check the locked pair,
    for (int i = 0, k = pair_count; i < k ; i++)
    {
        locked[pairs[i].winner] [pairs[i].loser] = true;

        // If the locked pairs is true, the fuction comes into action
        if (cyclic(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TO print out the winner which just the source of the locked graph
    int source;
    for (int i = 0, k = candidate_count; i < k; i++)
    {
        source = 0;
        for (int j = 0 ; j < k; j++)
        {
            // If the pair locked is not the source, the function moves on to the next
            if (locked[j][i] == false)
            {
                source++;
            }
        }
        // If the source is found it print out
        if (source == candidate_count)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
}