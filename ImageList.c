#include <stdlib.h>
#include <assert.h>
#include "ImageList.h"

/* Create a new image list */
ILIST *CreateImageList(void)
{
	ILIST *list = (ILIST *)malloc(sizeof(ILIST));
	assert(list != NULL);
    	list->Length = 0;       
    	list->First = NULL;     
    	list->Last = NULL;     
    	return list;
}

/* Delete an image list (and all entries) */
void DeleteImageList(ILIST *list)
{
	assert(list != NULL);
    	IENTRY *current = list->First;
    	while (current) {
        	IENTRY *next = current->Next;
        	if (current->image) {
       			free(current->image);
		}
        free(current);
        current = next;
    }
    free(list);
}

void AppendRGBImage(ILIST *list, Image *RGBimage)
{
	assert(list != NULL);
	assert(RGBimage != NULL);
    	IENTRY *entry = (IENTRY *)malloc(sizeof(IENTRY));
    	entry->image = RGBimage;
	entry->Next = NULL;
        entry->Prev = list->Last;
    	if (list->Last != NULL) {
        	list->Last->Next = entry;
    	} else {
        	list->First = entry;
    	}
    	list->Last = entry;
    	list->Length++;
}

/* Insert a YUV image to the image list at the end */
void AppendYUVImage(ILIST *list, YUVImage *YUVimage)
{
	assert(list != NULL);
	assert(YUVimage != NULL);
    	IENTRY *entry = (IENTRY *)malloc(sizeof(IENTRY));
    	entry->image = YUVimage;
    	entry->ImageIsYUV = 1;
	entry->Next = NULL;
	entry->Prev = list->Last;
    	if (list->Last) {
        	list->Last->Next = entry;
    	} 
	else {
        	list->First = entry;
    	}
    	list->Last = entry;
    	list->Length++;
}

/* Crop an image list */
void CropImageList(ILIST *list, unsigned int start, unsigned int end)
{
	assert(list != NULL);
    	assert(list->Length > 0);
    	assert(start <= end);
    	assert(end < list->Length);	
    	IENTRY *current = list->First;
    	unsigned int index = 0;
    	while (current != NULL && index < start) {
        	IENTRY *next = current->Next;
        	if (current->image != NULL) {
			free(current->image);
		}
		free(current);
        	current = next;
        	index++;
    	}
    	list->First = current;
    	if (current != NULL) {
        	current->Prev = NULL;
    	}
    	while (current != NULL && index < end) {
        	current = current->Next;
        	index++;
    	}
    	list->Last = current;
    	if (current != NULL) {
        	current->Next = NULL;
    	}
    	list->Length = end - start + 1;
}

/* Fast forward an image list */
void FastImageList(ILIST *list, unsigned int factor)
{
	assert(list != NULL);
    	assert(list->Length > 0);
    	assert(factor >= 1);

    	IENTRY *current = list->First;
    	IENTRY *newFirst = NULL;
    	IENTRY *newLast = NULL;
    	unsigned int count = 0;
    	unsigned int newLength = 0;

    	while (current != NULL) {
    		if (count % factor == 0) {
       			if (newFirst == NULL) {
                		newFirst = current;
            		}
            	newLast = current;
            	newLength++;
        } else {
            	IENTRY *next = current->Next;
            	if (current->image != NULL) {
			free(current->image);
		}
		free(current);
            	current = next;
            	count++;
            	continue;
        }
        	current = current->Next;
        	count++;
    	}

    	list->First = newFirst;
    	list->Last = newLast;
    	list->Length = newLength;

    	if (newFirst != NULL) {
        	newFirst->Prev = NULL;
    	}
    	if (newLast != NULL) {
        	newLast->Next = NULL;
    	}
}

/* Reverse an image list */
void ReverseImageList(ILIST *list)
{
	assert(list != NULL);
    	assert(list->Length > 0);

    	IENTRY *current = list->First;
    	IENTRY *temp = NULL;
    	while (current != NULL) {
        	temp = current->Prev;
        	current->Prev = current->Next;
        	current->Next = temp;
        	current = current->Prev;
    	}
    	if (temp != NULL) {
        	list->First = temp->Prev;
    	}
    	list->Last = list->First;
}

/* Insert a RGB image to the image list in the front */
void PrependRGBImage(ILIST *list, Image *RGBimage)
{
	assert(list != NULL);
    	assert(RGBimage != NULL);
    	IENTRY *newEntry = (IENTRY *)malloc(sizeof(IENTRY));
    	assert(newEntry != NULL);
    	newEntry->image = RGBimage;
    	newEntry->Next = list->First;
    	newEntry->Prev = NULL;
    	if (list->First != NULL) {
        	list->First->Prev = newEntry;
    	} 
	else {
        	list->Last = newEntry;
    	}
    	list->First = newEntry;
    	list->Length++;
}
