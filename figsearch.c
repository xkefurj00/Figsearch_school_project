#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FILEPATH "obrazek.txt"

struct Image
{
   int number_of_lines;  // variable that stores number of lines
   int number_of_columns; // variable that stores number of columns
   int **colors; // field of bitmap colors
};


int bitmap_ctor(struct Image *image){

image->colors = (int**)malloc(image->number_of_lines*sizeof(int*));
// Allocation of memory for rows

if (image->colors==NULL){

    fprintf(stderr,"Memory allocation failed!");
    return 1;

}// Checks if malloc was successfull

for (int row = 0; row < image->number_of_lines; row++)
{
    image->colors[row] = (int*)malloc(image->number_of_columns*sizeof(int));

    if (image->colors[row]==NULL){

    fprintf(stderr,"Memory allocation failed!");
    return 1;

    }// Checks if malloc was successfull

}// Allocation of memory for columns

return 0;

}



void bitmap_dtor(struct Image *image){

for (int row = 0; row < image->number_of_lines; row++)
{
    free(image->colors[row]);
}// Free memory for columns


    free(image->colors); 
    // Free memory for rows
    image->colors=NULL;
    image->number_of_lines=0;
    image->number_of_columns=0;

}

int load_file(struct Image *image, FILE *bitmap){



fscanf(bitmap,"%d %d", &image->number_of_lines, &image->number_of_columns);
// Loads first two numbers which are size of image

if (bitmap_ctor(image) !=0){
    return 1;
}


for (int row = 0; row < image->number_of_lines; row++){

    for (int column = 0; column < image->number_of_columns; column++)
    {
        fscanf(bitmap,"%d", &image->colors[row][column]);
    }
    
}


return 0;
}



void print_help(){




}

void find_hline(struct Image *image){

int longest1 = 0;
int longest2 = 0;
int row_number;
int column_number_start;
int column_number_end;


for (int row = 0; row < image->number_of_lines; row++)
{
    for (int column = 0; column < image->number_of_columns; column++)
    {
        if (image->colors[row][column]==1)
        {
            longest1++;
        }

        if ((image->colors[row][column]==0)  || (column == image->number_of_columns - 1))
        {
                if (longest1 > longest2)
                {
                    longest2 = longest1;
                    row_number = row;
                    column_number_start = column - longest2 + 1;
                    column_number_end = column;
                }
            longest1 = 0;
        } 
        
        

    }

}

fprintf(stdout,"%d %d %d %d", row_number, column_number_start ,
row_number, column_number_end);

}

void find_vline(struct Image *image){
    
int longest1 = 0;
int longest2 = 0;
int column_number;
int row_number_start;
int row_number_end;


for (int column = 0; column < image->number_of_columns; column++)
{
    for (int row = 0; row < image->number_of_lines; row++)
    {
        if (image->colors[row][column]==1)
        {
            longest1++;
        }

        if ((image->colors[row][column]==0)  || (row == image->number_of_lines - 1))
        {
                if (longest1 > longest2)
                {
                    longest2 = longest1;
                    column_number = column;
                    row_number_start = row - longest2 + 1;
                    row_number_end = row;
                }
            longest1 = 0;
        } 
        
        

    }

}

fprintf(stdout,"%d %d %d %d",  row_number_start , column_number, row_number_end, column_number);


}

void find_square(){
    
}

void use_function(struct Image *image, char *function){

if (strcmp(function, "hline")==0)
{
   find_hline(image);
}
if (strcmp(function, "vline")==0)
{
   find_vline(image);
}

}



int main(int argc, char *argv[]){

struct Image image;


FILE *bitmap=fopen(argv[2], "r");
if (bitmap == NULL)
{
    fprintf(stderr, "Error: Cant open file");
    return 1;
}





if (load_file(&image, bitmap) != 0)
{
    fclose(bitmap);
    return 1;
}

if (argc==3)
{
    use_function(&image , argv[1]);    
}

fclose(bitmap);

bitmap_dtor(&image);


return 0;

}