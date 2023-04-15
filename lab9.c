#include <stdio.h>
#include <stdlib.h>

struct RecordType {
    int id;
    char name;
    int order;
};

struct Node {
    struct RecordType record;
    struct Node *next;
};

struct HashType {
    struct Node **array;
    int size;
};

int hash(int x, int hashSize) { return x % hashSize; }

int parseData(char *inputFileName, struct RecordType **ppData) {
    FILE *inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile) {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData =
            (struct RecordType *)malloc(sizeof(struct RecordType) * dataSz);

        if (*ppData == NULL) {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i) {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

void printRecords(struct RecordType pData[], int dataSz) {
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i) {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

void displayRecordsInHash(struct HashType *pHashArray, int hashSz) {
    int i;
    struct Node *currentNode;

    for (i = 0; i < hashSz; ++i) {
        currentNode = pHashArray->array[i];
        if (currentNode != NULL) {
            printf("Index %d ->", i);
            while (currentNode != NULL) {
                printf(" %d, %c, %d ->", currentNode->record.id,
                       currentNode->record.name, currentNode->record.order);
                currentNode = currentNode->next;
            }
            printf(" NULL\n");
        }
    }
}

struct HashType *createHash(int size) {
    struct HashType *hashArray =
        (struct HashType *)malloc(sizeof(struct HashType));
    hashArray->array = (struct Node **)malloc(sizeof(struct Node *) * size);
    for (int i = 0; i < size; ++i) {
        hashArray->array[i] = NULL;
    }
    hashArray->size = size;
    return hashArray;
}

void insertToHash(struct HashType *hashArray, struct RecordType *record) {
    int index = hash(record->id, hashArray->size);
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->record = *record;
    newNode->next = NULL;

    if (hashArray->array[index] == NULL) {
        hashArray->array[index] = newNode;
    } else {
        struct Node *currentNode = hashArray->array[index];
        while (currentNode->next != NULL) {
            currentNode = currentNode->next;
        }
        currentNode->next = newNode;
    }
}

int main(void) {
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    int hashSize = recordSz * 2;
    struct HashType *hashArray = createHash(hashSize);
    for (int i = 0; i < recordSz; ++i) {
        insertToHash(hashArray, &pRecords[i]);
    }

    displayRecordsInHash(hashArray, hashSize);

    return 0;
}