/**
 * @file a4q1.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-12-03
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define PAGE_SIZE 256
#define NUM_PAGES 16
#define NUM_FRAMES 8

// Typedef PageEntry
typedef struct
{
    int frameIndex;
    bool isPresent;
    bool useBit;
} PageTableEntry;

PageTableEntry pageTable[NUM_PAGES];
int frameSlots[NUM_FRAMES];
int clockIndex = 0;
int pageFaults = 0;
int replacements = 0;

// Generate Random Addresses
void generateRandomAddresses(int addressList[], int count)
{
    for (int i = 0; i < count; i++)
    {
        addressList[i] = rand() % (NUM_PAGES * PAGE_SIZE);
    }
}

// Handles page fault
void handlePageFault(int pageNum)
{
    // Increment
    pageFaults++;

    while (1)
    {
        int currentPage = frameSlots[clockIndex];
        if (currentPage == -1 || !pageTable[currentPage].useBit)
        {
            if (currentPage != -1)
            {
                pageTable[currentPage].isPresent = false;
                pageTable[currentPage].useBit = false;
                replacements++;
                printf("Replaced page %d from frame %d\n", currentPage, clockIndex);
            }

            frameSlots[clockIndex] = pageNum;
            pageTable[pageNum].frameIndex = clockIndex;
            pageTable[pageNum].isPresent = true;
            pageTable[pageNum].useBit = true;

            clockIndex = (clockIndex + 1) % NUM_FRAMES;
            return;
        }
        else
        {
            pageTable[currentPage].useBit = false;
            clockIndex = (clockIndex + 1) % NUM_FRAMES;
        }
    }
}

int translateLogicalToPhysical(int logicalAddress)
{
    int pageNum = logicalAddress / PAGE_SIZE;
    int offset = logicalAddress % PAGE_SIZE;

    if (!pageTable[pageNum].isPresent)
    {
        printf("Page fault for page %d\n", pageNum);
        handlePageFault(pageNum);
    }

    pageTable[pageNum].useBit = true;
    int frameNum = pageTable[pageNum].frameIndex;
    return frameNum * PAGE_SIZE + offset;
}

int main(int argc, char *argv[])
{
    // Take input, make sure it's there
    if (argc != 2)
    {
        printf("Usage: %s 50\n", argv[0]);
        return 1;
    }

    int numAddresses = atoi(argv[1]);
    if (numAddresses <= 0)
    {
        printf("Number of virtual addresses must be positive!!!\n");
        return 1;
    }

    srand(time(NULL));

    for (int i = 0; i < NUM_PAGES; i++)
    {
        pageTable[i].frameIndex = -1;
        pageTable[i].isPresent = false;
        pageTable[i].useBit = false;
    }

    for (int i = 0; i < NUM_FRAMES; i++)
    {
        frameSlots[i] = -1;
    }

    int *virtualAddresses = (int *)malloc(numAddresses * sizeof(int));
    if (!virtualAddresses)
    {
        printf("Memory allocation failed!\n");
        return 1;
    }

    generateRandomAddresses(virtualAddresses, numAddresses);

    for (int i = 0; i < numAddresses; i++)
    {
        int logical = virtualAddresses[i];
        int physical = translateLogicalToPhysical(logical);
        printf("Logical Address: %d to Physical Address: %d\n", logical, physical);
    }

    printf("\nTotal Page Faults: %d\n", pageFaults);
    printf("Total Replacements: %d\n", replacements);

    free(virtualAddresses);
    return 0;
}
