// Implements a dictionary's functionality
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 26

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[N];

// count the number of words
int num_words = 0;

// represents the first element in a linked list
node *head = NULL;

// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a';
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (!file)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {
        // provide memory for a word
        node *new_node = malloc(sizeof(node));
        // ran out of memory
        if (!new_node)
        {
            unload();
            free(new_node);
            return false;
        }
        // put word into the new node
        strcpy(new_node->word, word);
        // put new node into start of linked list
        new_node->next = head;
        // reassign pointer to head of list
        head = new_node;
        // hash the word into hash table
        int index = hash(word);
        hashtable[index] = head;
        // received null pointer for new word
        if (!hashtable[index])
        {
            unload();
            free(new_node);
            fclose(file);
            return false;
        }
        num_words++;
    }

    // Close dictionary
    fclose(file);
    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    if (&load)
    {
        return num_words;
    }
    else
    {
        unload();
        return 0;
    }
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // calc length of input word
    int wlength = strlen(word);
    // make a copy of the word since its a read only const
    char word_copy[wlength + 1];
    // convert word copy to lowercase
    for (int i = 0; i < wlength; i++)
    {
        word_copy[i] = word[i];
    }
    // add trailing 0 to end string
    word_copy[wlength] = '\0';
    // traverse each linked list from the head
    node *cursor = head;
    while (cursor != NULL)
    {
        // compare dictionary with current word
        if (strcasecmp(word_copy, cursor->word) == 0)
        {
            return true;
        }
        else // check next node
        {
            cursor = cursor->next;
        }
    }
    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // pointer to traverse linked list
    node *cursor = head;
    // free nodes in linked list with a valid pointer, from end to start
    while (cursor != NULL)
    {
        node *temp = cursor;
        cursor = cursor->next;
        free(temp);
    }
    return true;
}
